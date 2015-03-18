#ifndef H_MeshAndRayCast
#define H_MeshAndRayCast

//-------------------------------------��GetMeshInformation()������------------------------------------
// Desc:�õ����������������������Ϣ
//-----------------------------------------------------------------------------------------------------
void GetMeshInformation(const Ogre::MeshPtr mesh,
	size_t &vertex_count, Ogre::Vector3* &vertices,
	size_t &index_count, unsigned long* &indices,
	const Ogre::Vector3 &position /* = Ogre::Vector3::ZERO */,
	const Ogre::Quaternion &orient /* = Ogre::Quaternion::IDENTITY */,
	const Ogre::Vector3 &scale /* = Ogre::Vector3::UNIT_SCALE */);

//-----------------------------------------��raycast()������-------------------------------------------
// Desc:�������ཻ������е�������󣬱�����ÿ�������棬��ȡ������ཻ�ľ�ȷ����������Ϣ
//-----------------------------------------------------------------------------------------------------
bool raycast(Ogre::RaySceneQuery* raySceneQuery, const Ogre::Ray &ray, Ogre::Vector3 &result,
	Ogre::MovableObject* &target, float &closest_distance, const Ogre::uint32 queryMask);

#endif