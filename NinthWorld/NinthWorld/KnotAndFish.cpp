/*------------------------------------------------------------------
【Cpp文件】：KnotAndFish.cpp       Created by Beyond Ray,2014年4月
（描述）：一个封装了纽环和旋转环绕的鱼的动态场景的实现
--------------------------------------------------------------------*/
#include "stdafx.h"
#include"KnotAndFish.h"

//-----------------------------------------【initialise()函数】------------------------------------------
// Desc:初始化纽环网格和三条带烟雾的鱼
//-------------------------------------------------------------------------------------------------------
void KnotAndFish::initialise(Ogre::Vector3 vec)
{
	//相关音效初始化
	m_iRainSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(纽环)雨声.mp3"));
	m_iBlisterSd = m_SoundMgr->CreateSound(Ogre::String("(纽环)水泡声.wav"));
	m_iReCoverSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(纽环)血量恢复.wav"));

	//----------------------------------------------------------------
	// 创建一个圆环, 给它一个透明的水纹理, 将其放置到场景指定位置
	mPosition = vec;

	Ogre::Entity* knot = m_SceneMgr->createEntity("Knot", "knot.mesh");
	knot->setMaterialName("Examples/WaterStream");
	mKnotNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(mPosition);
	mKnotNode->attachObject(knot);
	knot->setQueryFlags(KNOT_MASK);

	// 在圆环中间添加一颗珠子
	Ogre::Entity *sphere = m_SceneMgr->createEntity("sphere", "sphere.mesh");
	Ogre::SceneNode *sphereNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(mPosition);
	sphereNode->attachObject(sphere);
	sphereNode->setScale(0.1f, 0.1f, 0.1f);
	sphere->setQueryFlags(SPHERE_MASK);

	// 创建三条鱼，并赋予它们游动动画
	Ogre::Entity *fish_0 = m_SceneMgr->createEntity("Fish0", "fish.mesh");
	mFishSwim0 = fish_0->getAnimationState("swim");
	mFishSwim0->setEnabled(true);

	Ogre::Entity *fish_1 = m_SceneMgr->createEntity("Fish1", "fish.mesh");
	mFishSwim1 = fish_1->getAnimationState("swim");
	mFishSwim1->setEnabled(true);

	Ogre::Entity *fish_2 = m_SceneMgr->createEntity("Fish2", "fish.mesh");
	mFishSwim2 = fish_2->getAnimationState("swim");
	mFishSwim2->setEnabled(true);

	// 创建三个场景节点，并将这些鱼绑定到场景节点
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

	//雨的粒子特效
	m_Rain = m_SceneMgr->createParticleSystem("Rain", "Beyond/Rain");
	mKnotNode->attachObject(m_Rain);
	m_Rain->setEmitting(false);
}

void KnotAndFish::initialise(float x, float y, float z)
{
	initialise(Ogre::Vector3(x, y, z));
}
//--------------------------------------【frameRenderingQueued()函数】---------------------------------
// Desc:进行三条鱼的动画帧更新
//-----------------------------------------------------------------------------------------------------
bool KnotAndFish::frameRenderingQueued(const FrameEvent & evt)
{
	//更新声源及听众位置
	m_SoundMgr->FrameStarted(m_AttackNode, evt.timeSinceLastFrame);

	//对下雨技能进行累计时间
	m_fKeepRainTime += evt.timeSinceLastFrame;
	
	if (m_bRaining)
	{
		//雨穿过主角的包围盒检测
		if (m_Rain->getBoundingBox().contains(m_AttackNode->getPosition()))
		{
			//每秒加50滴血
			m_Model->setModelHP(m_Model->getModelHP() + evt.timeSinceLastFrame * REC_BLOOD_PER_SECOND);
			//播放恢复血量的音乐
			if (m_iReCoverChannel == INVALID_SOUND_CHANNEL)
				m_SoundMgr->PlaySound(m_iReCoverSd, m_AttackNode, &m_iReCoverChannel);
			//渲染回复生命的粒子特效
			m_Model->setRecBloodPar(true);
			//显示回复生命状态
			if (!m_bRecBloodState)
			{
				m_NinthCEGUI->showSkillState(2, 3);
				m_bRecBloodState = true;
			}
		}
		else
		{
			//关闭生命回复特效
			m_Model->setRecBloodPar(false);
			//停止播放恢复血量的音乐
			if (m_iReCoverChannel != INVALID_SOUND_CHANNEL)
			{
				m_SoundMgr->StopSound(&m_iReCoverChannel);
			}
			//关闭回复声明生命显示
			if (m_bRecBloodState)
			{
				m_NinthCEGUI->hideSkillState(2);
				m_bRecBloodState = false;
			}
		}
	}
	//雨的检测与状态改变
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
		//隐藏生命回复状态
		if (m_bRecBloodState)
		{
			m_NinthCEGUI->hideSkillState(2);
			m_bRecBloodState = false;
		}
		m_SoundMgr->StopSound(&m_iRainChannel);
		//关闭生命回复特效
		m_Model->setRecBloodPar(false);
		//停止播放恢复血量的音乐
		if (m_iReCoverChannel != INVALID_SOUND_CHANNEL)
		{
			m_SoundMgr->StopSound(&m_iReCoverChannel);
		}
		//冷却计时
		m_NinthCEGUI->getSkillCoolCalW()[7]->setText(Ogre::StringConverter::toString((int)(RAIN_COOL_TIME - m_fKeepRainTime)));
	}
	else
	{
		//冷却计时
		m_NinthCEGUI->getSkillCoolCalW()[7]->setText(Ogre::StringConverter::toString((int)(RAIN_COOL_TIME - m_fKeepRainTime)));
	}
	
	//-------------------------------------------------------------------------
	Real theta = m_Root->getTimer()->getMilliseconds() / 1000.0f;

	// r为半径，x, y, z为三维空间位置，为圆环轨迹相对于世界原点的位置

	//-------------------mfishNode0--------------------
	Ogre::Real r = 28 * (2 + Math::Sin(theta * 3 / 2 + 0.2f));
	Ogre::Real x = r * Math::Cos(theta);
	Ogre::Real y = r * Math::Sin(theta);
	Ogre::Real z = 60 * Math::Cos(theta * 3 / 2 + 0.2f);

	Vector3 lastPos = mFishNode0->getPosition();   // 保存鱼上一次的位置
	mFishNode0->setPosition(x + mPosition.x, y + mPosition.y, z + mPosition.z);  // 设置鱼新的位置

	// 设置鱼改变的方向
	mFishNode0->setDirection(mFishNode0->getPosition() - lastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X);

	mFishSwim0->addTime(evt.timeSinceLastFrame * 5);   // 更新鱼的游动动画

	//-------------------mfishNode1--------------------
	theta += 2 * Ogre::Math::PI / 3;
	x = r * Math::Cos(theta);
	y = r * Math::Sin(theta);
	z = 60 * Math::Cos(theta * 3 / 2 + 0.2f);

	lastPos = mFishNode1->getPosition();   // 保存鱼上一次的位置
	mFishNode1->setPosition(x + mPosition.x, y + mPosition.y, z + mPosition.z); // 设置鱼新的位置

	// 设置鱼改变的方向
	mFishNode1->setDirection(mFishNode1->getPosition() - lastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X);

	mFishSwim1->addTime(evt.timeSinceLastFrame * 5);   // 更新鱼的游动动画

	//-------------------mfishNode2--------------------
	theta += 2 * Ogre::Math::PI / 3;
	x = r * Math::Cos(theta);
	y = r * Math::Sin(theta);
	z = 60 * Math::Cos(theta * 3 / 2 + 0.2f);

	lastPos = mFishNode2->getPosition();   // 保存鱼上一次的位置
	mFishNode2->setPosition(x + mPosition.x, y + mPosition.y, z + mPosition.z);  // 设置鱼新的位置

	// 设置鱼改变的方向
	mFishNode2->setDirection(mFishNode2->getPosition() - lastPos, Node::TS_PARENT, Vector3::NEGATIVE_UNIT_X);

	mFishSwim2->addTime(evt.timeSinceLastFrame * 5);   // 更新鱼的游动动画
	//----------------------------------------------------------------------------
	return true;
}
//------------------------------------【BeJetCollDec()函数】----------------------------------
// Desc:检测是否有物体穿过
//------------------------------------------------------------------------------------------------
bool KnotAndFish::BeJetCollDec(const FrameEvent & evt)
{
	static Ogre::Ray ray;
	static Ogre::Vector3 srcPos;
	//计算源及射线
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
				//雨的技能遮罩
				m_NinthCEGUI->getSkillShadeW()[7]->show();
				m_NinthCEGUI->getSkillCoolCalW()[7]->show();
			}
			return true;
		}
	}
	return false;
}
