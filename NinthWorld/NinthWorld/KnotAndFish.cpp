/*------------------------------------------------------------------
��Cpp�ļ�����KnotAndFish.cpp       Created by Beyond Ray,2014��4��
����������һ����װ��Ŧ������ת���Ƶ���Ķ�̬������ʵ��
--------------------------------------------------------------------*/
#include "stdafx.h"
#include"KnotAndFish.h"

//-----------------------------------------��initialise()������------------------------------------------
// Desc:��ʼ��Ŧ��������������������
//-------------------------------------------------------------------------------------------------------
void KnotAndFish::initialise(Ogre::Vector3 vec)
{
	//�����Ч��ʼ��
	m_iRainSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(Ŧ��)����.mp3"));
	m_iBlisterSd = m_SoundMgr->CreateSound(Ogre::String("(Ŧ��)ˮ����.wav"));
	m_iReCoverSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(Ŧ��)Ѫ���ָ�.wav"));

	//----------------------------------------------------------------
	// ����һ��Բ��, ����һ��͸����ˮ����, ������õ�����ָ��λ��
	mPosition = vec;

	Ogre::Entity* knot = m_SceneMgr->createEntity("Knot", "knot.mesh");
	knot->setMaterialName("Examples/WaterStream");
	mKnotNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(mPosition);
	mKnotNode->attachObject(knot);
	knot->setQueryFlags(KNOT_MASK);

	// ��Բ���м����һ������
	Ogre::Entity *sphere = m_SceneMgr->createEntity("sphere", "sphere.mesh");
	Ogre::SceneNode *sphereNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(mPosition);
	sphereNode->attachObject(sphere);
	sphereNode->setScale(0.1f, 0.1f, 0.1f);
	sphere->setQueryFlags(SPHERE_MASK);

	// ���������㣬�����������ζ�����
	Ogre::Entity *fish_0 = m_SceneMgr->createEntity("Fish0", "fish.mesh");
	mFishSwim0 = fish_0->getAnimationState("swim");
	mFishSwim0->setEnabled(true);

	Ogre::Entity *fish_1 = m_SceneMgr->createEntity("Fish1", "fish.mesh");
	mFishSwim1 = fish_1->getAnimationState("swim");
	mFishSwim1->setEnabled(true);

	Ogre::Entity *fish_2 = m_SceneMgr->createEntity("Fish2", "fish.mesh");
	mFishSwim2 = fish_2->getAnimationState("swim");
	mFishSwim2->setEnabled(true);

	// �������������ڵ㣬������Щ��󶨵������ڵ�
	mFishNode0 = m_SceneMgr->getRootSceneNode()->createChildSceneNode();
	mFishNode0->attachObject(fish_0);
	mFishNode0->setScale(4, 4, 4);
	mFishNode0->setPosition(mPosition);
	mFishNode0->attachObject(m_SceneMgr->createParticleSystem("Smoke0", "Examples/Smoke0"));

	mFishNode1 = m_SceneMgr->getRootSceneNode()->createChildSceneNode();
	mFishNode1->attachObject(fish_1);
	mFishNode1->setScale(4, 4, 4);
	mFishNode1->setPosition(mPosition);
	mFishNode1->attachObject(m_SceneMgr->createParticleSystem("Smoke1", "Examples/Smoke1"));

	mFishNode2 = m_SceneMgr->getRootSceneNode()->createChildSceneNode();
	mFishNode2->attachObject(fish_2);
	mFishNode2->setScale(4, 4, 4);
	mFishNode2->setPosition(mPosition);
	mFishNode2->attachObject(m_SceneMgr->createParticleSystem("Smoke2", "Examples/Smoke2"));

	//���������Ч
	m_Rain = m_SceneMgr->createParticleSystem("Rain", "Beyond/Rain");
	mKnotNode->attachObject(m_Rain);
	m_Rain->setEmitting(false);
}

void KnotAndFish::initialise(float x, float y, float z)
{
	initialise(Ogre::Vector3(x, y, z));
}
//--------------------------------------��frameRenderingQueued()������---------------------------------
// Desc:����������Ķ���֡����
//-----------------------------------------------------------------------------------------------------
bool KnotAndFish::frameRenderingQueued(const FrameEvent & evt)
{
	//������Դ������λ��
	m_SoundMgr->FrameStarted(m_AttackNode, evt.timeSinceLastFrame);

	//�����꼼�ܽ����ۼ�ʱ��
	m_fKeepRainTime += evt.timeSinceLastFrame;
	
	if (m_bRaining)
	{
		//�괩�����ǵİ�Χ�м��
		if (m_Rain->getBoundingBox().contains(m_AttackNode->getPosition()))
		{
			//ÿ���50��Ѫ
			m_Model->setModelHP(m_Model->getModelHP() + evt.timeSinceLastFrame * REC_BLOOD_PER_SECOND);
			//���Żָ�Ѫ��������
			if (m_iReCoverChannel == INVALID_SOUND_CHANNEL)
				m_SoundMgr->PlaySound(m_iReCoverSd, m_AttackNode, &m_iReCoverChannel);
			//��Ⱦ�ظ�������������Ч
			m_Model->setRecBloodPar(true);
			//��ʾ�ظ�����״̬
			if (!m_bRecBloodState)
			{
				m_NinthCEGUI->showSkillState(2, 3);
				m_bRecBloodState = true;
			}
		}
		else
		{
			//�ر������ظ���Ч
			m_Model->setRecBloodPar(false);
			//ֹͣ���Żָ�Ѫ��������
			if (m_iReCoverChannel != INVALID_SOUND_CHANNEL)
			{
				m_SoundMgr->StopSound(&m_iReCoverChannel);
			}
			//�رջظ�����������ʾ
			if (m_bRecBloodState)
			{
				m_NinthCEGUI->hideSkillState(2);
				m_bRecBloodState = false;
			}
		}
	}
	//��ļ����״̬�ı�
	if (m_fKeepRainTime >= RAIN_COOL_TIME)
	{
		if (m_bRainSkillPer)
		{
			m_NinthCEGUI->getSkillShadeW()[7]->hide();
			m_NinthCEGUI->getSkillCoolCalW()[7]->hide();
			m_bRainSkillPer = false;
		}
		BeJetCollDec(evt);
	}
	else if (m_fKeepRainTime >= KEEP_RAIN_MAX_TIME && m_bRaining)
	{
		m_Rain->setEmitting(false);
		m_bRaining = false;
		//���������ظ�״̬
		if (m_bRecBloodState)
		{
			m_NinthCEGUI->hideSkillState(2);
			m_bRecBloodState = false;
		}
		m_SoundMgr->StopSound(&m_iRainChannel);
		//�ر������ظ���Ч
		m_Model->setRecBloodPar(false);
		//ֹͣ���Żָ�Ѫ��������
		if (m_iReCoverChannel != INVALID_SOUND_CHANNEL)
		{
			m_SoundMgr->StopSound(&m_iReCoverChannel);
		}
		//��ȴ��ʱ
		m_NinthCEGUI->getSkillCoolCalW()[7]->setText(Ogre::StringConverter::toString((int)(RAIN_COOL_TIME - m_fKeepRainTime)));
	}
	else
	{
		//��ȴ��ʱ
		m_NinthCEGUI->getSkillCoolCalW()[7]->setText(Ogre::StringConverter::toString((int)(RAIN_COOL_TIME - m_fKeepRainTime)));
	}
	
	//-------------------------------------------------------------------------
	Real theta = m_Root->getTimer()->getMilliseconds() / 1000.0f;

	// rΪ�뾶��x, y, zΪ��ά�ռ�λ�ã�ΪԲ���켣���������ԭ���λ��

	//-------------------mfishNode0--------------------
	Ogre::Real r = 28 * (2 + Math::Sin(theta * 3 / 2 + 0.2f));
	Ogre::Real x = r * Math::Cos(theta);
	Ogre::Real y = r * Math::Sin(theta);
	Ogre::Real z = 60 * Math::Cos(theta * 3 / 2 + 0.2f);

	Vector3 lastPos = mFishNode0->getPosition();   // ��������һ�ε�λ��
	mFishNode0->setPosition(x + mPosition.x, y + mPosition.y, z + mPosition.z);  // �������µ�λ��

	// ������ı�ķ���
	mFishNode0->setDirection(mFishNode0->getPosition() - lastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X);

	mFishSwim0->addTime(evt.timeSinceLastFrame * 5);   // ��������ζ�����

	//-------------------mfishNode1--------------------
	theta += 2 * Ogre::Math::PI / 3;
	x = r * Math::Cos(theta);
	y = r * Math::Sin(theta);
	z = 60 * Math::Cos(theta * 3 / 2 + 0.2f);

	lastPos = mFishNode1->getPosition();   // ��������һ�ε�λ��
	mFishNode1->setPosition(x + mPosition.x, y + mPosition.y, z + mPosition.z); // �������µ�λ��

	// ������ı�ķ���
	mFishNode1->setDirection(mFishNode1->getPosition() - lastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X);

	mFishSwim1->addTime(evt.timeSinceLastFrame * 5);   // ��������ζ�����

	//-------------------mfishNode2--------------------
	theta += 2 * Ogre::Math::PI / 3;
	x = r * Math::Cos(theta);
	y = r * Math::Sin(theta);
	z = 60 * Math::Cos(theta * 3 / 2 + 0.2f);

	lastPos = mFishNode2->getPosition();   // ��������һ�ε�λ��
	mFishNode2->setPosition(x + mPosition.x, y + mPosition.y, z + mPosition.z);  // �������µ�λ��

	// ������ı�ķ���
	mFishNode2->setDirection(mFishNode2->getPosition() - lastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X);

	mFishSwim2->addTime(evt.timeSinceLastFrame * 5);   // ��������ζ�����
	//----------------------------------------------------------------------------
	return true;
}
//------------------------------------��BeJetCollDec()������----------------------------------
// Desc:����Ƿ������崩��
//------------------------------------------------------------------------------------------------
bool KnotAndFish::BeJetCollDec(const FrameEvent & evt)
{
	static Ogre::Ray ray;
	static Ogre::Vector3 srcPos;
	//����Դ������
	srcPos = m_AttackNode->getPosition();
	ray = Ogre::Ray(srcPos, m_AttackNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z);

	m_RaySceneQuery->setRay(ray);
	Ogre::RaySceneQueryResult & result = m_RaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator iter = result.begin();
	for (; iter != result.end(); iter++)
	{
		if (iter->movable->getQueryFlags() == SPHERE_MASK)
		{
			if (m_Model->getLMouseDown())
			{
				m_Rain->setEmitting(true);
				m_bRaining = true;
				m_bRainSkillPer = true;
				m_fKeepRainTime = 0.0f;
				m_SoundMgr->PlaySound(m_iBlisterSd, m_AttackNode, &m_iBlisterChannel);
				m_SoundMgr->PlaySound(m_iRainSd, m_AttackNode, &m_iRainChannel);
				//��ļ�������
				m_NinthCEGUI->getSkillShadeW()[7]->show();
				m_NinthCEGUI->getSkillCoolCalW()[7]->show();
			}
			return true;
		}
	}
	return false;
}
