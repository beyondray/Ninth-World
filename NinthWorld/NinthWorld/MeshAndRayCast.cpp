#include "stdafx.h"
#include"MeshAndRayCast.h"

//-------------------------------------��GetMeshInformation()������------------------------------------
// Desc:�õ����������������������Ϣ
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

	// ���㶥���������������ݵĴ�С
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// ֻ��Ҫ���the shared verticesһ��
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
		// Add the indices ����
		index_count += submesh->indexData->indexCount;
	}

	// Allocate space for the vertices and indices
	// �����ڴ�
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];

	added_shared = false;

	// ��Ӿ��������
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

			// λ������
			const Ogre::VertexElement* posElem =
				vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			// λ������
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
				// ��Ӷ�������
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
				// �����������
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

//-----------------------------------------��raycast()������-------------------------------------------
// Desc:�������ཻ������е�������󣬱�����ÿ�������棬��ȡ������ཻ�ľ�ȷ����������Ϣ
//-----------------------------------------------------------------------------------------------------
bool raycast(Ogre::RaySceneQuery* raySceneQuery, const Ogre::Ray &ray, Ogre::Vector3 &result,
	Ogre::MovableObject* &target, float &closest_distance, const Ogre::uint32 queryMask)
{
	target = NULL;
	// ���������Ƿ���Ч
	if (raySceneQuery != NULL)
	{
		// ����һ�����߲�ѯ
		raySceneQuery->setRay(ray);
		raySceneQuery->setSortByDistance(true);//��ѯ�������������
		raySceneQuery->setQueryMask(queryMask);//��������
		// ִ�в�ѯ
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
	// ע��Ŷ�������������Ѿ��õ�һϵ�а��հ�Χ�м�⵽��ģ����.
	// ����Ҫ�ҵ���һ���ཻ������.
	// �����ζ�����ǲ���ȥ������������ˣ���������ʡ��ʱ��
	// ���Ǻ��ź������ǲ��ò�����ÿһ������������棬�������Ƕ�ôʹ�࣬�������
	// 
	//��ʼ����С����Ϊ-1
	closest_distance = -1.0f;
	Ogre::Vector3 closest_result;
	Ogre::RaySceneQueryResult &query_result = raySceneQuery->getLastResults();//ȡ�ظղŲ�ѯ�Ľ������Ϊ֮ǰ��û�б���

	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		// �����һ����ײ����������Զ����ȻҪ������        
		if ((closest_distance >= 0.0f) &&
			(closest_distance < query_result[qr_idx].distance))
		{
			break;
		}
		// ����ֻ������ײ�Ķ����Ǹ�����
		if ((query_result[qr_idx].movable != NULL) &&
			(query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
			// ȡ�ñ���ײ������
			Ogre::MovableObject *pentity = static_cast<Ogre::MovableObject*>(query_result[qr_idx].movable);
			// �����Ƕ��㣬���������������ż������¿�
			size_t vertex_count;
			size_t index_count;
			Ogre::Vector3 *vertices;
			unsigned long *indices;
			// ����ĺ����õ�ģ�͵���ϸ��Ϣ
			GetMeshInformation(((Ogre::Entity*)pentity)->getMesh(), vertex_count, vertices, index_count, indices,
				pentity->getParentNode()->_getDerivedPosition(),
				pentity->getParentNode()->_getDerivedOrientation(),
				pentity->getParentNode()->_getDerivedScale());
			// �ٴ�ע���ˣ�������ÿһ��������Ľ��㣬ͬ����¼�����
			bool new_closest_found = false;
			for (size_t i = 0; i < index_count; i += 3)
			{
				// ����ĺ�����һ��������������Ľ��㣬����һ��pair�����Ƿ��ཻ�����뽻����롷
				std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
					vertices[indices[i + 1]], vertices[indices[i + 2]], true, false);//֪�����������õ��˰ɣ������˶���֮����Ҳ
				// �����ײ������Ƿ��ǵ�ǰ��С����
				if (hit.first)
				{
					if ((closest_distance < 0.0f) ||
						(hit.second < closest_distance))
					{
						// ����������
						closest_distance = hit.second;
						new_closest_found = true;
					}
				}
			}
			// �ͷŸղ�������ڴ棬���ֶ�����Ȼ����д�����ʱ���Ҫ�ɶԱ�д���ˣ�����ͬʱд��{}
			delete[] vertices;
			delete[] indices;
			//����ҵ��µĵ㣬��Ҫ���˸�����Ӧ��Ϣ
			if (new_closest_found)
			{
				target = pentity;
				closest_result = ray.getPoint(closest_distance);//�����ļ��㣬�򵥵����Է���
			}
		}
	}
	// ���ؽ��
	if (closest_distance >= 0.0f)
	{
		// �ɹ���
		result = closest_result;
		return (true);
	}
	else
	{
		//ʧ����
		return (false);
	}
}