/*------------------------------------------------------------------
【头文件】：Dragon.h		Created by Beyond Ray,2014年4月
（描述）：一个封装了雷龙卡雷苟斯AI所有行为的类的实现
--------------------------------------------------------------------*/
#include "stdafx.h"
#include"Dragon.h"
#include"time.h"
#include"Sword.h"
//静态成员初始化
int Dragon::ms_iDDieNum = 0;

//-------------------------------------【Dragon()函数】----------------------------------------
// Desc:构造函数
//---------------------------------------------------------------------------------------------
Dragon::Dragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, Dragon_Object &dragon, ControlModel* model, NinthCEGUI* ninthCEGUI) :
m_SceneMgr(sceneMgr),
m_SoundMgr(soundMgr),
m_MeshName(meshName),
m_NinthCEGUI(ninthCEGUI),
m_Dragon(dragon),
m_bRunFlag(false),
m_Model(model),
m_AttackNode(model->getMainNode()),
m_bDead(false),
m_bBigger(false),
m_bSmaller(false),
m_Beam(NULL),
m_fNewOrbitTime(0.0f),
m_iScreenChannel(INVALID_SOUND_CHANNEL),
m_iThunderStormChannel(INVALID_SOUND_CHANNEL),
m_iAureolaChannel(INVALID_SOUND_CHANNEL),
m_iBiggerChannel(INVALID_SOUND_CHANNEL),
m_iHarmObjectChannel(INVALID_SOUND_CHANNEL),
m_iJetChannel(INVALID_SOUND_CHANNEL)
{
	//创建射线场景查询
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
	//Dragon对象初始化
	DragonInitialise(dragon);
}

Dragon::Dragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, ControlModel* model, NinthCEGUI* ninthCEGUI) :
m_SceneMgr(sceneMgr),
m_SoundMgr(soundMgr),
m_MeshName(meshName),
m_NinthCEGUI(ninthCEGUI),
m_bRunFlag(false),
m_Model(model),
m_AttackNode(m_Model->getMainNode()),
m_bDead(false),
m_bBigger(false),
m_bSmaller(false),
m_Beam(NULL),
m_fNewOrbitTime(0.0f),
m_iScreenChannel(INVALID_SOUND_CHANNEL),
m_iThunderStormChannel(INVALID_SOUND_CHANNEL),
m_iAureolaChannel(INVALID_SOUND_CHANNEL),
m_iBiggerChannel(INVALID_SOUND_CHANNEL),
m_iHarmObjectChannel(INVALID_SOUND_CHANNEL),
m_iJetChannel(INVALID_SOUND_CHANNEL)
{
	//创建射线场景查询
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
}

//-------------------------------------【~Dragon()函数】---------------------------------------
// Desc:析构函数
//---------------------------------------------------------------------------------------------
Dragon::~Dragon()
{
	m_SceneMgr->destroyQuery(m_RaySceneQuery);
	ms_iDDieNum = 0;
}

//---------------------------------【DragonInitialise()函数】----------------------------------
// Desc:雷龙AI个体属性初始化
//---------------------------------------------------------------------------------------------
void Dragon::DragonInitialise(Dragon_Object &dragon)
{
	//相关声音初始化
	m_iDieSd = m_SoundMgr->CreateSound(Ogre::String("(龙)死亡声效.wav"));
	m_iScreenSd = m_SoundMgr->CreateSound(Ogre::String("(龙)凄鸣.wav"));
	m_iThunderStormSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(龙)雷暴.mp3"));
	m_iAureolaSd = m_SoundMgr->CreateSound(Ogre::String("(龙)强风.wav"));
	m_iBiggerSd = m_SoundMgr->CreateSound(Ogre::String("(龙)bigger变大.mp3"));
	m_iHarmObjectSd = m_SoundMgr->CreateSound(Ogre::String("(龙)伤害女孩1.wav"));
	m_iJetSd = m_SoundMgr->CreateSound(Ogre::String("(龙)光束射击.wav"));

	//雷龙基本属性初始化
	m_Dragon = dragon;
	m_HP = dragon.m_MaxHP;

	//创建随机的四元数
	Ogre::Vector3 ax, ay, az;
	ax = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	ay = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	az = ax.crossProduct(ay);
	ay = az.crossProduct(ax);
	ax.normalise(); ay.normalise(); az.normalise();
	m_Orbit.FromAxes(ax, ay, az);

	//创建雷龙AI的实体对象，并添加到场景节点
	m_DraEnt = m_SceneMgr->createEntity(m_MeshName);
	m_DragonNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(m_Dragon.m_Position);
	m_DragonNode->attachObject(m_DraEnt);
	m_DragonNode->setScale(m_Dragon.m_Scale);
	m_DraEnt->setQueryFlags(DRAGON_MASK);

	//创建剑灵射穿目标时所添加的粒子特效
	m_RedSmoke = m_SceneMgr->createParticleSystem("DragonRedSmoke" +
		Ogre::StringConverter::toString(m_Dragon.m_Index), "Examples/DSmoke");
	m_DragonNode->attachObject(m_RedSmoke);
	m_RedSmoke->setEmitting(false);
	m_RedSmoke->setKeepParticlesInLocalSpace(true);

	//添加攻击光束，初始化为不攻击状态
	ChooseBeam(m_Beam, m_Dragon.m_JetIndex);
	m_DragonNode->attachObject(m_Beam);
	m_Beam->setEmitting(false);

	//被击中时的狂卷旋风粒子初始化
	m_Aureola = m_SceneMgr->createParticleSystem("Blood" + 
		Ogre::StringConverter::toString(m_Dragon.m_Index), "Beyond/Aureola");

	m_DragonNode->attachObject(m_Aureola);
	m_Aureola->setEmitting(false);

	//添加飞行动画，并设为开启
	if (m_Dragon.m_FlyIndex == 0)
		m_Fly = m_DraEnt->getAnimationState("fly1");
	else if (m_Dragon.m_FlyIndex == 1)
		m_Fly = m_DraEnt->getAnimationState("fly2");
	else
		m_Fly = m_DraEnt->getAnimationState("fly3");
	m_Fly->setEnabled(true);

	//添加死亡动画,默认为关闭
	m_Die = m_DraEnt->getAnimationState("fly3");
	m_Die->setEnabled(true);
}

//-------------------------------------【RotateAndAttack()函数】-----------------------------------
// Desc:使得雷龙AI绕着攻击目标在某一个轨道上旋转(roateRate在0.0f~1.0f之间)
//-------------------------------------------------------------------------------------------------
void Dragon::RotateAndAttack(const Ogre::FrameEvent & evt, float roateRate /* = 1.0f */, 
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	static Ogre::Vector3 randVec, x, y, z;
	static Ogre::Quaternion qua;

	//先将雷龙AI移动至目标点，便于四元数的面向旋转
	m_DragonNode->translate(m_Direcion, relativeTo);

	//计算旋转后的四元数，并将其设为新的旋转朝向
	qua = m_DragonNode->getOrientation() * m_Orbit;
	qua = m_DragonNode->getOrientation() + roateRate * evt.timeSinceLastFrame * (qua - m_DragonNode->getOrientation());
	m_DragonNode->setOrientation(qua);

	//根据雷龙AI新的正面朝向将其按原距离平移回
	m_DragonNode->getOrientation().ToAxes(x, y, z);
	x.normalise();
	m_DragonNode->translate(-x * m_Distance, relativeTo);
}

//-------------------------------------【RotateAndGo()函数】---------------------------------------
// Desc:调转面向一个方向，在决定动作是否分解后，朝着目标方向前进(始终背对Ogre::Vector3::UNIT_Y)
//-------------------------------------------------------------------------------------------------
void Dragon::RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	const Ogre::Vector3 & dirRotate, Ogre::Vector3 dirGo, bool rotateResolve /* = true */, 
	float roatePre /* = 30.0f */, bool toSkyOriSet /* = true */,
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//设置雷龙AI的朝向
	Ogre::Vector3 src = m_DragonNode->getOrientation()* orientation;

	//雷龙AI的朝向到目标面向方向的旋转
	Ogre::Quaternion quat = src.getRotationTo(dirRotate);
	if (rotateResolve)
	{
		static Ogre::Radian rad;
		static Ogre::Vector3 axis;
		quat.ToAngleAxis(rad, axis);
		if (rad >= Ogre::Radian(0.01f))
			m_DragonNode->rotate(axis, Ogre::Radian(Ogre::Math::PI / roatePre), relativeTo);
	}
	else
	{
		m_DragonNode->rotate(quat, relativeTo);
	}
	if (toSkyOriSet)
	{
		//将雷龙AI再次旋转，使得雷龙AI始终背对着天空
		src = m_DragonNode->getOrientation()* orientation;
		Ogre::Vector3 vecZ = src.crossProduct(Ogre::Vector3::UNIT_Y);
		src = m_DragonNode->getOrientation()* (orientation.crossProduct(Ogre::Vector3::UNIT_Y));
		quat = src.getRotationTo(vecZ);
		m_DragonNode->rotate(quat, relativeTo);
	}
	//到目标方向的前进
	dirGo.normalise();
	m_DragonNode->translate(dirGo * m_Dragon.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
}

//-------------------------------------【RotateAndDieRotate()函数】---------------------------------------
// Desc:调转面向一个方向，在决定动作是否分解后，朝着目标方向前进(始终背对Ogre::Vector3::UNIT_Y)
//--------------------------------------------------------------------------------------------------------
void Dragon::RoateAndDieRotate(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 30.0f */,
	float dieRoateSpeed /* = 10.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//设置雷龙AI的朝向
	Ogre::Vector3 src = m_DragonNode->getOrientation()* orientation;

	//雷龙AI的朝向到目标面向方向的旋转
	Ogre::Quaternion quat = src.getRotationTo(dirRotate);
	if (rotateResolve)
	{
		static Ogre::Radian rad;
		static Ogre::Vector3 axis;
		quat.ToAngleAxis(rad, axis);
		if (rad >= Ogre::Radian(Ogre::Math::PI / 30.0f))
			m_DragonNode->rotate(axis, Ogre::Radian(Ogre::Math::PI / rotatePre), relativeTo);
		else
		{
			//死亡时做惯性旋转
			m_DragonNode->pitch(Ogre::Radian(dieRoateSpeed * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
		}
	}
	else
	{
		m_DragonNode->rotate(quat, relativeTo);
	}

	//到目标方向的前进
	Ogre::Vector3 dir = dirRotate;
	dir.normalise();
	m_DragonNode->translate(dir * m_Dragon.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
}
//----------------------------------【frameRenderingQueued()函数】-----------------------------
// Desc:在渲染队列中更新雷龙AI每一帧的行为
//---------------------------------------------------------------------------------------------
bool Dragon::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	//相关声音的更新
	m_SoundMgr->FrameStarted(m_Model->getMainNode(), evt.timeSinceLastFrame);

	//死亡计数显示
	DieNumChangeShow();

	//累计时间，到达一定时间时产生新轨道
	//*
	m_fNewOrbitTime += evt.timeSinceLastFrame;
	if (m_fNewOrbitTime >= 6.0f)
	{
		Ogre::Vector3 ax, ay, az;
		ax = Ogre::Vector3(randFloat(), randFloat(), randFloat());
		ay = Ogre::Vector3(randFloat(), randFloat(), randFloat());
		az = ax.crossProduct(ay);
		ay = az.crossProduct(ax);
		ax.normalise(); ay.normalise(); az.normalise();
		m_Orbit.FromAxes(ax, ay, az);
		m_fNewOrbitTime = 0.0f;
	}
	//*/
	//死亡检测
	if (!m_bDead)
	{
		//对玩家的攻击检测
		if (this->JecCollDec())
		{
			m_Model->cutModelHP(5 * m_Dragon.m_Damage * evt.timeSinceLastFrame);
			//伤害到对方时，对方发出声音
			m_SoundMgr->PlaySound(m_iHarmObjectSd, m_Model->getMainNode(), &m_iHarmObjectChannel, 0.5f);
			if (m_Model->getModelHP() <= 0.0f)
			{
				m_Model->setModelHP(0.0f);
				m_Model->setDieFlag(true);
			}
		}
		//被剑灵攻击穿透的检测
		BeThroughCollDec(evt);
		if (m_bBigger)
		{
			if (ScaleChange(evt, 3.0f, 3.5f, true))
				m_bBigger = false;
			//播放体积变大的声音
			m_SoundMgr->PlaySound(m_iBiggerSd, m_Model->getMainNode(), &m_iBiggerChannel);
		}
		
		//被玩家攻击的检测
		if ((this->BeJetCollDec() && m_Model->getLMouseDown()))
		{
			m_HP -= 5 * (MODEL_DAMAGE - m_Dragon.m_Defense) * evt.timeSinceLastFrame;
			m_Aureola->setEmitting(true);
			//播放凄鸣声和雷暴声,还有风声
			m_SoundMgr->PlaySound(m_iScreenSd, m_DragonNode, &m_iScreenChannel);
			m_SoundMgr->PlaySound(m_iThunderStormSd, m_AttackNode, &m_iThunderStormChannel);
			m_SoundMgr->PlaySound(m_iAureolaSd, m_DragonNode, &m_iAureolaChannel);
		}
		else
		{
			if (m_iThunderStormChannel != INVALID_SOUND_CHANNEL)
				m_SoundMgr->StopSound(&m_iThunderStormChannel);		
		}
		//血量为0的死亡检测
		if (m_HP <= 0.0f)
		{
			m_bBigger = false;
			m_bSmaller = true;
			m_bDead = true;
			m_Beam->setEmitting(false);
			m_RedSmoke->setEmitting(false);
			ms_iDDieNum++;
			m_iDieChannel = INVALID_SOUND_CHANNEL;
			//播放死亡声效，停止雷暴声效
			m_SoundMgr->PlaySound(m_iDieSd, m_Model->getMainNode(), &m_iDieChannel, 0.5f);
			m_SoundMgr->StopSound(&m_iThunderStormChannel);
			//转换粒子光束
			m_SceneMgr->destroyParticleSystem(m_Beam->getName());
			m_Beam = NULL;
			ChooseBeam(m_Beam, static_cast<int>((rand() / (RAND_MAX + 1)) * 8));
			m_DragonNode->attachObject(m_Beam);
			m_Beam->setEmitting(false);
			return true;
		}
	}
	else
	{
		//对于被剑灵击中的雷龙AI先进行不断的缩小
		if (m_bSmaller)
		{
			if (ScaleChange(evt, 1.0f, 3.5f, false))
				m_bSmaller = false;
		}
		//添加雷龙AI的死亡动画帧
		m_Die->addTime( evt.timeSinceLastFrame);

		RoateAndDieRotate(evt, Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Y, true, 30.0f, 10.0f, Ogre::Node::TS_PARENT);

		//向目标方向靠近移动
		static Ogre::Real downDistance = 0.0f;
		downDistance += 2 * m_Dragon.m_FlySpeed * evt.timeSinceLastFrame;

		if (downDistance >= DRAGON_DOWN_NEWLIFE_DIS)
		{
			setFollowRandPos(m_AttackNode->getPosition(), 300.0f);
			downDistance = 0.0f;
			m_bDead = false;
			m_HP = m_Dragon.m_MaxHP;
			m_Aureola->setEmitting(false);
			m_SoundMgr->StopSound(&m_iAureolaChannel);
		}
		return true;
	}

	//-----------------------------------------【未死亡时执行】---------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//添加雷龙AI的飞行动画帧
	m_bRunFlag ? m_Fly->addTime(1.6f * evt.timeSinceLastFrame) : m_Fly->addTime(1.2f * evt.timeSinceLastFrame);

	//得到雷龙AI到攻击目标的方向
	m_Direcion = m_AttackNode->getPosition() - m_Dragon.m_Position;

	//得到雷龙AI与目标之间的距离
	m_Distance = m_Direcion.length();

	//-----------------------【雷龙AI朝目标进行前进和攻击及行为的检测】-----------------------
	//---------------------------------【目标追踪】-------------------------------
	if (m_Distance >= DRAGON_CIRCLE_DISTANCE)
	{
		//逃跑标志设置
		m_bRunFlag = false;
		RotateAndGo(evt, Ogre::Vector3::UNIT_X, m_Direcion, m_Direcion, false, 0.0f, true, Ogre::Node::TS_PARENT);
	}
	else if (m_Distance < DRAGON_CIRCLE_DISTANCE && m_Distance >= DRAGON_RUNAWAY_DISTANCE)
	{
		//--------------【处于强势进攻状态时，才进行周旋攻击】--------------------
		if (!m_bRunFlag)
		{
			if (fabs(m_Direcion.x) > 50.0f || fabs(m_Direcion.z) > 50.0f)
			{
				RotateAndAttack(evt, 0.4f, Ogre::Node::TS_PARENT); //周旋攻击
			}
			else  //直线前进出击
			{
				RotateAndGo(evt, Ogre::Vector3::UNIT_X, m_Direcion, m_Direcion, false, 0.0f, false, Ogre::Node::TS_PARENT);
			}
				
			//粒子攻击判断
			if (m_Distance <= DRAGON_ATTACK_DISTANCE)
			{
				m_Beam->setEmitting(true);
				m_SoundMgr->PlaySound(m_iJetSd, m_DragonNode, &m_iJetChannel);
			}
			else
				m_Beam->setEmitting(false);
		}
		//-------------【若处于逃跑成功后，缓冲调转攻击状态，则执行下面这段代码】-------------
		else if (m_Distance < DRAGON_CIRCLE_DISTANCE && m_Distance >= DRAGON_BACK_ATTACK_DIS && m_bRunFlag)
		{
			RotateAndGo(evt, Ogre::Vector3::UNIT_X, m_Direcion, -m_Direcion, true, 20.0f, true, Ogre::Node::TS_PARENT);
		}
		//---------------【若处于逃跑状态，远离攻击目标（调转动作未完成则继续执行）】------------------
		else if (m_Distance < DRAGON_BACK_ATTACK_DIS && m_Distance >= DRAGON_RUNAWAY_DISTANCE && m_bRunFlag)
		{
			RotateAndGo(evt, Ogre::Vector3::UNIT_X, -m_Direcion, -m_Direcion, true, 60.0f, true, Ogre::Node::TS_PARENT);
		}	// if_else_结束	
	}
	//-------------------------------【被追上时，试图调头逃跑】------------------------------------------
	else    
	{
		//逃跑标志设置
		m_bRunFlag = true;
		RotateAndGo(evt, Ogre::Vector3::UNIT_X, -m_Direcion, -m_Direcion, true,  60.0f, true, Ogre::Node::TS_PARENT);
		m_Beam->setEmitting(false);
	}
	//重新计算雷龙AI的位置
	m_Dragon.m_Position = m_DragonNode->getPosition();

	return true;
}

//-----------------------------------------【setPosition()函数】---------------------------------------
// Desc:设置雷龙AI的位置
//-----------------------------------------------------------------------------------------------------
void Dragon::setPosition(Ogre::Vector3 positon)
{
	m_Dragon.m_Position = positon;
}

//----------------------------------------【setRandPosition()函数】------------------------------------
// Desc:设置雷龙AI的随机位置
//-----------------------------------------------------------------------------------------------------
void Dragon::setRandPosition(Ogre::Real range /* = 2000 */)
{
	m_Dragon.m_Position = Ogre::Vector3(
		rand() % (int)range - range/2, 
		rand() % (int)range - range/2, 
		rand() % (int)range - range/2);
	m_DragonNode->setPosition(m_Dragon.m_Position);
}

//----------------------------------------【setFollowRandPos()函数】------------------------------------
// Desc:设置雷龙AI基于某一点的随机位置
//-----------------------------------------------------------------------------------------------------
void Dragon::setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range /* = 2000 */)
{
	m_Dragon.m_Position = Ogre::Vector3(
		position.x + rand() % (int)range - range / 2,
		position.y + rand() % (int)range - range / 2,
		position.z + rand() % (int)range - range / 2);
	m_DragonNode->setPosition(m_Dragon.m_Position);
}
//-----------------------------------------【BeJetCollDec()函数】---------------------------------------
// Desc:对雷龙AI的被喷射攻击进行碰撞检测
//------------------------------------------------------------------------------------------------------
bool Dragon::BeJetCollDec()
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
		if (iter->movable->getQueryFlags() == DRAGON_MASK)
		{
			if (iter->movable->getParentSceneNode() == m_DragonNode)
			{
				return true;
			}
		}
	}
	return false;
}

//-----------------------------------------【JetCollDec()函数】-----------------------------------------
// Desc:对雷龙AI的喷射攻击进行碰撞检测
//------------------------------------------------------------------------------------------------------
bool Dragon::JecCollDec()
{
	static Ogre::Ray ray;
	static Ogre::Vector3 srcPos;
	if (m_bRunFlag || m_bDead || (m_Distance > DRAGON_ATTACK_DISTANCE) || m_Model->getDieFlag()) return false;
	//计算源及射线方向
	srcPos = m_DragonNode->getPosition();
	ray = Ogre::Ray(srcPos, m_DragonNode->getOrientation() * Ogre::Vector3::UNIT_X);
	
	m_RaySceneQuery->setRay(ray);
	Ogre::RaySceneQueryResult & result = m_RaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator iter = result.begin();

	//计算攻击目标位置的x,y,z世界坐标分量
	Ogre::Vector3 destPos = m_AttackNode->getPosition();
	
	//射线检测和伤害粒子击中检测
	for (; iter != result.end(); iter++)
	{
		if (iter->movable->getQueryFlags() == MAINMODEL_MASK)
		{
			
			for (size_t i = 0; i < m_Beam->getNumParticles(); i += 50)
			{
				Ogre::Vector3 position = m_Beam->getParticle(i)->position;
				if (position.positionEquals(destPos, 10.0f))
					return true;
			}
			
		}
	}
	
	return false;
}
//------------------------------【ChangePosToWorld()函数】------------------------------
// Desc:将基于场景根节点的坐标转换为基于世界的(前提：场景根节点pitch了PI/2)
//--------------------------------------------------------------------------------------
inline Ogre::Vector3 Dragon::ChangePosToWorld(const Ogre::Vector3 &pos) const
{
	Ogre::Vector3 src(pos.x, -pos.z, pos.y);
	return src;
}
//-----------------------------------------【BeThroughCollDec()函数】-----------------------------------
// Desc:对剑体AI的穿透攻击进行被击中碰撞检测
//------------------------------------------------------------------------------------------------------
bool Dragon::BeThroughCollDec(const Ogre::FrameEvent & evt)
{
	m_SphereSceneQuery = m_SceneMgr->createSphereQuery(Ogre::Sphere(m_DragonNode->getPosition(), 12 * m_DragonNode->getScale().x));
	Ogre::SceneQueryResult & result = m_SphereSceneQuery->execute();
	std::list<Ogre::MovableObject*>::iterator iter = result.movables.begin();
	bool have = false;
	for (; iter != result.movables.end(); iter++)
	{
		Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(*iter);
		if (pObject)
		{
			Ogre::uint32 mask = pObject->getQueryFlags();
			if (mask == SWORD_JIANLING_MASK)
			{
				m_HP -= 10 * (MODEL_DAMAGE - m_Dragon.m_Defense) * evt.timeSinceLastFrame;
				m_bBigger = true;
				have = true;
			}
			else if (mask == SWORD_PUPPET_MASK)
			{
				m_HP -= 10 * (10 * MODEL_DAMAGE - m_Dragon.m_Defense) * evt.timeSinceLastFrame;
				m_bBigger = true;
				have = true;
			}
		}
	}
	if (have)
		m_RedSmoke->setEmitting(true);
	return have;
}
//--------------------------------------【ScaleChange()函数】-------------------------------------
// Desc:对比例大小进行不断放大或缩小(bigger为true则放大,false为缩小)
//------------------------------------------------------------------------------------------------
bool Dragon::ScaleChange(const Ogre::FrameEvent &evt, float limitScale,
	float changeRate /* = 3.0f */, bool bigger /* = true */)
{
	Ogre::Vector3 scale = m_DragonNode->getScale();
	if (bigger)
	{
		if (scale.x <= limitScale)
		{
			m_DragonNode->setScale(
				scale.x + changeRate * evt.timeSinceLastFrame,
				scale.y + changeRate * evt.timeSinceLastFrame,
				scale.z + changeRate * evt.timeSinceLastFrame);
			return false;
		}
	}
	else
	{
		if (scale.x >= limitScale)
		{
			m_DragonNode->setScale(
				scale.x - changeRate * evt.timeSinceLastFrame,
				scale.y - changeRate * evt.timeSinceLastFrame,
				scale.z - changeRate * evt.timeSinceLastFrame);
			return false;
		}
	}
	return true;
}
//---------------------------------【ChooseBeam()函数】------------------------------
// Desc:选择粒子光束，编号从0到7
//-----------------------------------------------------------------------------------
void Dragon::ChooseBeam(Ogre::ParticleSystem*& beam, int index)
{
	if (beam != NULL)
		return;

	switch (index)
	{
	case 0:
		beam = m_SceneMgr->createParticleSystem("RedBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/RedFlare");
		break;
	case 1:
		beam = m_SceneMgr->createParticleSystem("RedBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/RedAureola");
		break;
	case 2:
		beam = m_SceneMgr->createParticleSystem("BlueBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/BlueFlare");
		break;
	case 3:
		beam = m_SceneMgr->createParticleSystem("BlueBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/BlueAureola");
		break;
	case 4:
		beam = m_SceneMgr->createParticleSystem("GreenBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/GreenFlare");
		break;
	case 5:
		beam = m_SceneMgr->createParticleSystem("GreenBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/GreenAureola");
		break;
	case 6:
		beam = m_SceneMgr->createParticleSystem("YellowBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/YellowFlare");
		break;
	case 7:
		beam = m_SceneMgr->createParticleSystem("YellowBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/YellowAureola");
		break;
	default:
		beam = m_SceneMgr->createParticleSystem("RedBeam" +
			Ogre::StringConverter::toString(m_Dragon.m_Index), "Dragon/RedAureola");
	}
}