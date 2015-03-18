#ifndef H_MeshAndRayCast
#define H_MeshAndRayCast

//-------------------------------------【GetMeshInformation()函数】------------------------------------
// Desc:得到网格对象各个三角形面的信息
//-----------------------------------------------------------------------------------------------------
void GetMeshInformation(const Ogre::MeshPtr mesh,
	size_t &vertex_count, Ogre::Vector3* &vertices,
	size_t &index_count, unsigned long* &indices,
	const Ogre::Vector3 &position /* = Ogre::Vector3::ZERO */,
	const Ogre::Quaternion &orient /* = Ogre::Quaternion::IDENTITY */,
	const Ogre::Vector3 &scale /* = Ogre::Vector3::UNIT_SCALE */);

//-----------------------------------------【raycast()函数】-------------------------------------------
// Desc:用射线相交检测射中的网格对象，遍历其每个三角面，来取得最近相交的精确的三角面信息
//-----------------------------------------------------------------------------------------------------
bool raycast(Ogre::RaySceneQuery* raySceneQuery, const Ogre::Ray &ray, Ogre::Vector3 &result,
	Ogre::MovableObject* &target, float &closest_distance, const Ogre::uint32 queryMask);

#endif