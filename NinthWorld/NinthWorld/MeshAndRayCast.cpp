#include "stdafx.h"
#include"MeshAndRayCast.h"

//-------------------------------------【GetMeshInformation()函数】------------------------------------
// Desc:得到网格对象各个三角形面的信息
//-----------------------------------------------------------------------------------------------------
void GetMeshInformation(const Ogre::MeshPtr mesh,
	size_t &vertex_count, Ogre::Vector3* &vertices,
	size_t &index_count, unsigned long* &indices,
	const Ogre::Vector3 &position /* = Ogre::Vector3::ZERO */,
	const Ogre::Quaternion &orient /* = Ogre::Quaternion::IDENTITY */,
	const Ogre::Vector3 &scale /* = Ogre::Vector3::UNIT_SCALE */)
{

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	// 计算顶点数据与索引数据的大小
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// 只需要添加the shared vertices一次
		if (submesh->useSharedVertices)
		{
			if (!added_shared)
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}
		// Add the indices 索引
		index_count += submesh->indexData->indexCount;
	}

	// Allocate space for the vertices and indices
	// 分配内存
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];

	added_shared = false;

	// 添加具体的数据
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
		{
			if (submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			// 位置索引
			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			// 位置数据
			Ogre::HardwareVertexBufferSharedPtr vbuf =
				vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex =
				static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			// as second argument. So make it float, to avoid trouble when Ogre::Real will
			// be comiled/typedefed as double:
			//Ogre::Real* pReal;
			float* pReal;

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				// 添加顶点数据
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

		if (use32bitindexes)
		{
			for (size_t k = 0; k < numTris * 3; ++k)
			{
				// 添加索引数据
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for (size_t k = 0; k < numTris * 3; ++k)
			{
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
					static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
}

//-----------------------------------------【raycast()函数】-------------------------------------------
// Desc:用射线相交检测射中的网格对象，遍历其每个三角面，来取得最近相交的精确的三角面信息
//-----------------------------------------------------------------------------------------------------
bool raycast(Ogre::RaySceneQuery* raySceneQuery, const Ogre::Ray &ray, Ogre::Vector3 &result,
	Ogre::MovableObject* &target, float &closest_distance, const Ogre::uint32 queryMask)
{
	target = NULL;
	// 测试射线是否有效
	if (raySceneQuery != NULL)
	{
		// 创建一个射线查询
		raySceneQuery->setRay(ray);
		raySceneQuery->setSortByDistance(true);//查询结果按距离排序
		raySceneQuery->setQueryMask(queryMask);//设置掩码
		// 执行查询
		if (raySceneQuery->execute().size() <= 0)
		{
			return (false);
		}
	}
	else
	{
		//LOG_ERROR << "Cannot raycast without RaySceneQuery instance" << ENDLOG;
		return (false);
	}
	// 注意哦，到这里我们已经得到一系列按照包围盒检测到的模型了.
	// 我们要找到第一个相交的物体.
	// 这就意味着我们不必去检测后面的物体了，这样大大节省了时间
	// 但是很遗憾，我们不得不遍历每一个物体的三角面，听起来是多么痛苦，必须得忍
	// 
	//初始化最小距离为-1
	closest_distance = -1.0f;
	Ogre::Vector3 closest_result;
	Ogre::RaySceneQueryResult &query_result = raySceneQuery->getLastResults();//取回刚才查询的结果，因为之前并没有保存

	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		// 如果下一个碰撞物体比这个还远，当然要无视啦        
		if ((closest_distance >= 0.0f) &&
			(closest_distance < query_result[qr_idx].distance))
		{
			break;
		}
		// 我们只关心碰撞的东西是个物体
		if ((query_result[qr_idx].movable != NULL) &&
			(query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
			// 取得被碰撞的物体
			Ogre::MovableObject *pentity = static_cast<Ogre::MovableObject*>(query_result[qr_idx].movable);
			// 顶点是顶点，索引是索引，不着急，往下看
			size_t vertex_count;
			size_t index_count;
			Ogre::Vector3 *vertices;
			unsigned long *indices;
			// 下面的函数得到模型的详细信息
			GetMeshInformation(((Ogre::Entity*)pentity)->getMesh(), vertex_count, vertices, index_count, indices,
				pentity->getParentNode()->_getDerivedPosition(),
				pentity->getParentNode()->_getDerivedOrientation(),
				pentity->getParentNode()->_getDerivedScale());
			// 再次注意了，下面求每一个三角面的交点，同样记录最近点
			bool new_closest_found = false;
			for (size_t i = 0; i < index_count; i += 3)
			{
				// 下面的函数求一条射线与三角面的交点，返回一个pair，《是否相交，距离交点距离》
				std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
					vertices[indices[i + 1]], vertices[indices[i + 2]], true, false);//知道索引干嘛用的了吧，索引乃顶点之索引也
				// 如果碰撞，检查是否是当前最小距离
				if (hit.first)
				{
					if ((closest_distance < 0.0f) ||
						(hit.second < closest_distance))
					{
						// 如果是则更新
						closest_distance = hit.second;
						new_closest_found = true;
					}
				}
			}
			// 释放刚才申请的内存，这种东西当然是在写申请的时候就要成对编写的了，正如同时写下{}
			delete[] vertices;
			delete[] indices;
			//如果找到新的点，不要忘了更新相应信息
			if (new_closest_found)
			{
				target = pentity;
				closest_result = ray.getPoint(closest_distance);//最近点的计算，简单的线性方程
			}
		}
	}
	// 返回结果
	if (closest_distance >= 0.0f)
	{
		// 成功了
		result = closest_result;
		return (true);
	}
	else
	{
		//失败了
		return (false);
	}
}