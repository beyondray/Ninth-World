/*------------------------------------------------------------------
【头文件】：Sword.h		Created by Beyond Ray,2014年4月
（描述）：一个封装了剑体AI所有行为的类的实现
--------------------------------------------------------------------*/
#include "stdafx.h"
#include"Sword.h"
#include"time.h"
#include "Dragon.h"
//静态成员初始化
int Sword::ms_iDieNum = 0;
int Sword::ms_iRotToCoreNum = 0;
int Sword::ms_iLaunchNum = 0;
int Sword::ms_iDiv3LaySuc = 0;
int Sword::ms_iDivSucNum = 0;
int Sword::ms_iRKeySusCount = 0;
int Sword::ms_iRevSwordFromHC = 0;
bool Sword::ms_bNinthSwordShoot = false;
bool Sword::ms_bDivine3LayDown = false;
bool Sword::ms_bDivine4Start = false;
bool Sword::ms_bDivine3Ready = false;
bool Sword::ms_bCoreHaved = false;
bool Sword::ms_bHCorehaved = false;
bool Sword::ms_bPuppetStop = false;
bool Sword::ms_bHelSwordAchieved = false;
bool Sword::ms_bHCLaunchState = false;
bool Sword::ms_bHelSwordState = false;
bool Sword::ms_bHelSwordManual = true;
Ogre::Real Sword::ms_Divine4WaitTime = 0.0f;
Ogre::Real Sword::ms_Divine3Scale = SWORD_DIVINE2_SCALE;
Ogre::SceneNode* Sword::ms_SwordCore = NULL;
Ogre::SceneNode* Sword::ms_HelicalCore = NULL;
Ogre::SceneNode* Sword::ms_HelSCenCore = NULL;
int Sword::ms_iSDefenseChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDefenseScreamChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDiv1_TrasChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDiv1_RotChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDiv1_ShootChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDiv3_RotChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDiv3_ThunderChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDiv3_ShootChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iDiv3_SThunderChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iHelSword_SThunderCannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iHelSword_RotChannel1 = INVALID_SOUND_CHANNEL;
int Sword::ms_iHelSword_RotChannel2 = INVALID_SOUND_CHANNEL;
int Sword::ms_iHelSword_FengDunChannel = INVALID_SOUND_CHANNEL;
int Sword::ms_iRotateDenSucNum = 0;
Ogre::Real Sword::ms_DenfenseTime = SWORD_DEFENSE_COOL_TIME;

//------------------------------------【RadFabs()函数】---------------------------------------
// Desc:弧度绝对值
//--------------------------------------------------------------------------------------------
Ogre::Radian RadFabs(Ogre::Radian rad)
{
	if (rad < Ogre::Radian(0.0f))
		return -rad;
	else
		return rad;
}
//-------------------------------------【Sword()函数】----------------------------------------
// Desc:构造函数
//---------------------------------------------------------------------------------------------
Sword::Sword(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Sword_Object &sword, ControlModel* model, NinthCEGUI* ninthCEGUI):
m_SceneMgr(sceneMgr),
m_SoundMgr(soundMgr),
m_NinthCEGUI(ninthCEGUI),
m_Sword(sword),
m_Model(model),
m_AttackNode(model->getMainNode()),
m_AttackPos(Ogre::Vector3(0, 0, 0)),
m_bArriveAttackPos(false),
m_bDenFirstAttack(true),
m_bAttackState(false),
m_bGetAttackPos(false),
m_bDead(false),
m_bLaunchState(false),
m_iRotToCoreIndex(-1),
m_iLaunchIndex(-1),
m_bSwordMoved(false),
m_bRotateDenfense(false),
m_bDivineSword123(false),
m_bHelicalSword(false),
m_bDiv3LaySuc(false),
m_bSpace(false),
m_bDenSkCoolPer(false),
m_bHelSSCoolPer(false),
m_iDivineSword(1),
m_iHelicIndex(0),
m_Divine1RotateTime(0.0f),
m_PuppleSwordShootTime(0.0f),
m_HelSwordKTime(0.0f),
m_iThroughtDestChannel(INVALID_SOUND_CHANNEL),
m_iHowlChannel(INVALID_SOUND_CHANNEL),
m_iHomingChannel(INVALID_SOUND_CHANNEL),
m_iSwordShootChannel1(INVALID_SOUND_CHANNEL),
m_iSwordShootChannel2(INVALID_SOUND_CHANNEL),
m_iIkuSochannel(INVALID_SOUND_CHANNEL),
m_iDiv2Channel(INVALID_SOUND_CHANNEL),
m_iDiv4_ShootChannel(INVALID_SOUND_CHANNEL),
m_iDiv4_LThunderChannel(INVALID_SOUND_CHANNEL),
m_iShootSwordChannel(INVALID_SOUND_CHANNEL)
{
	//创建射线场景查询
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
	//创建区域场景查询
	m_AABBSceneQuery = m_SceneMgr->createAABBQuery(Ogre::AxisAlignedBox());
	SwordInitialise(sword);
}

Sword::Sword(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, ControlModel* model, NinthCEGUI* ninthCEGUI):
m_SceneMgr(sceneMgr),
m_SoundMgr(soundMgr),
m_NinthCEGUI(ninthCEGUI),
m_Model(model),
m_AttackNode(m_Model->getMainNode()),
m_AttackPos(Ogre::Vector3(0, 0, 0)),
m_bArriveAttackPos(false),
m_bDenFirstAttack(true),
m_bAttackState(false),
m_bGetAttackPos(false),
m_bDead(false),
m_bLaunchState(false),
m_iRotToCoreIndex(-1),
m_iLaunchIndex(-1),
m_bSwordMoved(false),
m_bRotateDenfense(false),
m_bDivineSword123(false),
m_bHelicalSword(false),
m_bDiv3LaySuc(false),
m_bSpace(false),
m_bDenSkCoolPer(false),
m_bHelSSCoolPer(false),
m_iDivineSword(1),
m_iHelicIndex(0),
m_Divine1RotateTime(0.0f),
m_PuppleSwordShootTime(0.0f),
m_HelSwordKTime(0.0f),
m_iThroughtDestChannel(INVALID_SOUND_CHANNEL),
m_iHowlChannel(INVALID_SOUND_CHANNEL),
m_iHomingChannel(INVALID_SOUND_CHANNEL),
m_iSwordShootChannel1(INVALID_SOUND_CHANNEL),
m_iSwordShootChannel2(INVALID_SOUND_CHANNEL),
m_iIkuSochannel(INVALID_SOUND_CHANNEL),
m_iDiv2Channel(INVALID_SOUND_CHANNEL),
m_iDiv4_ShootChannel(INVALID_SOUND_CHANNEL),
m_iDiv4_LThunderChannel(INVALID_SOUND_CHANNEL),
m_iShootSwordChannel(INVALID_SOUND_CHANNEL)
{
	//创建射线场景查询
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
	//创建区域场景查询
	m_AABBSceneQuery = m_SceneMgr->createAABBQuery(Ogre::AxisAlignedBox());
}

//-------------------------------------【~Sword()函数】---------------------------------------
// Desc:析构函数
//---------------------------------------------------------------------------------------------
Sword::~Sword()
{
	m_SceneMgr->destroyQuery(m_RaySceneQuery);
	m_SceneMgr->destroyQuery(m_AABBSceneQuery);
	Sword::ms_iDieNum = 0;
	Sword::ms_iRotToCoreNum = 0;
	Sword::ms_iLaunchNum = 0;
	Sword::ms_iDiv3LaySuc = 0;
	Sword::ms_iDivSucNum = 0;
	Sword::ms_iRKeySusCount = 0;
	Sword::ms_iRevSwordFromHC = 0;
	Sword::ms_bNinthSwordShoot = false;
	Sword::ms_bDivine3LayDown = false;
	Sword::ms_bDivine4Start = false;
	Sword::ms_bDivine3Ready = false;
	Sword::ms_bCoreHaved = false;
	Sword::ms_bHCorehaved = false;
	Sword::ms_bPuppetStop = false;
	Sword::ms_bHelSwordAchieved = false;
	Sword::ms_bHCLaunchState = false;
	Sword::ms_bHelSwordState = false;
	Sword::ms_bHelSwordManual = true;
	Sword::ms_Divine4WaitTime = 0.0f;
	Sword::ms_Divine3Scale = SWORD_DIVINE2_SCALE;
	Sword::ms_SwordCore = NULL;
	Sword::ms_HelicalCore = NULL;
	Sword::ms_HelSCenCore = NULL;
	Sword::ms_iSDefenseChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDefenseScreamChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDiv1_TrasChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDiv1_RotChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDiv1_ShootChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDiv3_RotChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDiv3_ThunderChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDiv3_ShootChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iDiv3_SThunderChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iHelSword_SThunderCannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iHelSword_RotChannel1 = INVALID_SOUND_CHANNEL;
	Sword::ms_iHelSword_RotChannel2 = INVALID_SOUND_CHANNEL;
	Sword::ms_iHelSword_FengDunChannel = INVALID_SOUND_CHANNEL;
	Sword::ms_iRotateDenSucNum = 0;
	Sword::ms_DenfenseTime = SWORD_DEFENSE_COOL_TIME;
}

//---------------------------------【SwordInitialise()函数】-----------------------------------
// Desc:剑体AI个体属性初始化
//---------------------------------------------------------------------------------------------
void Sword::SwordInitialise(const Sword_Object & sword)
{
	//相关声音的初始化
	m_iThroughtDestSd = m_SoundMgr->CreateSound(Ogre::String("(剑)a攻击-剑-刺中.wav"));
	m_iHowlSd = m_SoundMgr->CreateSound(Ogre::String("(剑)剑体呼啸.mp3"));
	m_iSDefenseSd = m_SoundMgr->CreateSound(Ogre::String("(剑)水之涌动声.wav"));
	m_iDefenseScreamSd = m_SoundMgr->CreateSound(Ogre::String("(剑)水牢术.mp3"));
	m_iHomingSd = m_SoundMgr->CreateSound(Ogre::String("(剑)剑灵归位.wav"));
	m_iSwordShootSd1 = m_SoundMgr->CreateSound(Ogre::String("(剑)剑灵发射-磁暴.snd"));
	m_iSwordShootSd2 = m_SoundMgr->CreateSound(Ogre::String("(剑)剑灵发射-呼啸.snd"));
	m_iIkuSoSd = m_SoundMgr->CreateSound(Ogre::String("(剑)画中音-王者风范.ogg"));
	m_iShootSwordSd = m_SoundMgr->CreateSound(Ogre::String("(剑)剑敲击声.wav"));
	//独孤九剑相关音效初始化
	m_iDiv1_TrasSd = m_SoundMgr->CreateSound(Ogre::String("(剑)独孤九剑1段平移.wav"));
	m_iDiv1_RotSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(剑)独孤九剑1段旋转.snd"));
	m_iDiv1_ShootSd = m_SoundMgr->CreateSound(Ogre::String("(剑)独孤九剑1段射出.wav"));
	m_iDiv2Sd = m_SoundMgr->CreateSound(Ogre::String("(剑)独孤九剑2段.wav"));
	m_iDiv3_RotSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(剑)独孤九剑3段旋转.wav"));
	m_iDiv3_SThunderSd = m_SoundMgr->CreateSound(Ogre::String("(剑)独孤九剑3短暂雷鸣.wav"));
	m_iDiv3_ThunderSd = m_SoundMgr->CreateSound(Ogre::String("(剑)独孤九剑3段雷声.wav"));
	ms_iDiv3_ShootSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(剑)独孤九剑3段射出.snd"));
	//螺旋剑相关音效初始化
	m_iHelSword_FengDunSd = m_SoundMgr->CreateSound(Ogre::String("(剑)风遁螺旋手里剑.mp3"));
	m_iHelSword_SThunderSd = m_iDiv3_SThunderSd;
	m_iHelSword_RotSd1 = m_iDiv1_RotSd;
	m_iHelSword_RotSd2 = m_iDiv3_RotSd;

	//剑体基本属性初始化
	m_Sword = sword;
	m_HP = sword.m_MaxHP;

	//创建随机的四元数
	Ogre::Vector3 ax, ay, az;
	ax = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	ay = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	az = ax.crossProduct(ay);
	ay = az.crossProduct(ax);
	ax.normalise(); ay.normalise(); az.normalise();
	m_Orbit.FromAxes(ax, ay, az);

	//初始化剑核,为剑灵归位作准备
	if (!ms_bCoreHaved)
	{
		ms_SwordCore = m_AttackNode->createChildSceneNode();
		ms_bCoreHaved = true;
	}

	//初始化螺旋核，为螺旋剑技能发动作准备
	if (!ms_bHCorehaved)
	{
		ms_HelSCenCore = m_AttackNode->createChildSceneNode();
		ms_HelicalCore = ms_HelSCenCore->createChildSceneNode("HelicalCore");
		ms_bHCorehaved = true;
	}

	//创建剑体AI的实体对象，并添加到场景节点
	switch (m_Sword.m_CategoryIndex)
	{
	case 0:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword.MESH.mesh");
		break;
	case 1:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword2.MESH.mesh");
		break;
	case 2:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword3.MESH.mesh");
		break;
	case 3:
		m_SwordEnt = m_SceneMgr->createEntity("SpinousSword.MESH.mesh");
		break;
	case 4:
		m_SwordEnt = m_SceneMgr->createEntity("PurpleSword.MESH.mesh");
		break;
	case 5:
		m_SwordEnt = m_SceneMgr->createEntity("BigSword.MESH.mesh");
		break;
	case 6:
		m_SwordEnt = m_SceneMgr->createEntity("GreenSword.MESH.mesh");
		break;
	case 7:
		m_SwordEnt = m_SceneMgr->createEntity("GraySword.MESH.mesh");
		break;
	case 8:
		m_SwordEnt = m_SceneMgr->createEntity("BlackWhiteSword.MESH.mesh");
		break;
	case 9:
		m_SwordEnt = m_SceneMgr->createEntity("AxeSword.MESH.mesh");
		break;
	case 10:
		m_SwordEnt = m_SceneMgr->createEntity("GoldenSword.MESH.mesh");
		break;
	case 11:
		m_SwordEnt = m_SceneMgr->createEntity("JunSword.MESH.mesh");
		break;
	case 12:
		m_SwordEnt = m_SceneMgr->createEntity("EightDiagramsSword.MESH.mesh");
		break;
	case 13:
		m_SwordEnt = m_SceneMgr->createEntity("GoodSword.MESH.mesh");
		break;
	case 14:
		m_SwordEnt = m_SceneMgr->createEntity("HelicalSword.MESH.mesh");
		break;
	default:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword.MESH.mesh");	
	}
	m_SwordNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
	m_SwordNode->setPosition(m_Sword.m_Position);
	m_SwordNode->attachObject(m_SwordEnt);
	m_SwordNode->setScale(m_Sword.m_Scale);
	m_SwordNode->pitch(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
	m_SwordEnt->setQueryFlags(SWORD_NORMAL_MASK);

}
//---------------------------------【PuppetSwordInitialise()函数】----------------------------------
// Desc:剑体AI个体属性初始化
//---------------------------------------------------------------------------------------------
void Sword::PuppetSwordInitialise(const Sword_Object & sword)
{
	//万剑归宗相关音效初始化
	m_iDiv4_ShootSd = m_SoundMgr->CreateSound(Ogre::String("(剑)万剑归宗-射击剑鸣.wav"));
	m_iDiv4_LThunderSd = m_SoundMgr->CreateSound(Ogre::String("(剑)万剑归宗-残留雷鸣.wav"));

	//剑体基本属性初始化
	m_Sword = sword;
	m_HP = sword.m_MaxHP;
	//创建剑体AI的实体对象，并添加到场景节点
	switch (m_Sword.m_CategoryIndex)
	{
	case 0:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword.MESH.mesh");
		break;
	case 1:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword2.MESH.mesh");
		break;
	case 2:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword3.MESH.mesh");
		break;
	case 3:
		m_SwordEnt = m_SceneMgr->createEntity("SpinousSword.MESH.mesh");
		break;
	case 4:
		m_SwordEnt = m_SceneMgr->createEntity("PurpleSword.MESH.mesh");
		break;
	case 5:
		m_SwordEnt = m_SceneMgr->createEntity("BigSword.MESH.mesh");
		break;
	case 6:
		m_SwordEnt = m_SceneMgr->createEntity("GreenSword.MESH.mesh");
		break;
	case 7:
		m_SwordEnt = m_SceneMgr->createEntity("GraySword.MESH.mesh");
		break;
	case 8:
		m_SwordEnt = m_SceneMgr->createEntity("BlackWhiteSword.MESH.mesh");
		break;
	case 9:
		m_SwordEnt = m_SceneMgr->createEntity("AxeSword.MESH.mesh");
		break;
	case 10:
		m_SwordEnt = m_SceneMgr->createEntity("GoldenSword.MESH.mesh");
		break;
	case 11:
		m_SwordEnt = m_SceneMgr->createEntity("JunSword.MESH.mesh");
		break;
	case 12:
		m_SwordEnt = m_SceneMgr->createEntity("EightDiagramsSword.MESH.mesh");
		break;
	case 13:
		m_SwordEnt = m_SceneMgr->createEntity("GoodSword.MESH.mesh");
		break;
	case 14:
		m_SwordEnt = m_SceneMgr->createEntity("HelicalSword.MESH.mesh");
		break;
	default:
		m_SwordEnt = m_SceneMgr->createEntity("RedSword.MESH.mesh");
	}
	m_SwordNode = m_AttackNode->createChildSceneNode("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
	m_SwordNode->setPosition(m_Sword.m_Position);
	m_SwordNode->attachObject(m_SwordEnt);
	m_SwordNode->setScale(m_Sword.m_Scale);
	m_SwordEnt->setQueryFlags(SWORD_PUPPET_MASK);
}

//-------------------------------------【RotateAndGo()函数】---------------------------------------
// Desc:调转面向一个方向，在决定动作是否分解后，朝着目标方向前进
//-------------------------------------------------------------------------------------------------
void Sword::RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	Ogre::Vector3 destPos, bool rotateResolve /* = true */, float rotatePre /* = 1.0f */,
	bool rotateItself /* = true */, float rotateRate /* = 15.0f */, 
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//用攻击目标点计算欲前进的方向
	static Ogre::Vector3 dirGo;
	static Ogre::Quaternion quat;
	static Ogre::Radian rad;
	if (!m_bGetAttackPos)
	{
		//计算欲前进方向
		dirGo = m_AttackNode->getPosition() - m_Sword.m_Position;

		//设置剑体AI的朝向
		Ogre::Vector3 src = m_SwordNode->getOrientation()* orientation;

		//剑体AI的朝向到目标面向方向的旋转
		quat = src.getRotationTo(dirGo);			
	}

	if (rotateResolve)
	{
		static Ogre::Vector3 axis;
		quat.ToAngleAxis(rad, axis);
		
		if (rad >= Ogre::Radian(Ogre::Math::PI / rotatePre * evt.timeSinceLastFrame))
		{
			m_SwordNode->rotate(axis, Ogre::Radian(Ogre::Math::PI / rotatePre * evt.timeSinceLastFrame), relativeTo);
	
			//将剑体AI再次旋转，使得剑体AI始终背对着天空
			Ogre::Vector3 src = m_SwordNode->getOrientation()* orientation;
			Ogre::Vector3 vecZ = src.crossProduct(Ogre::Vector3::UNIT_Y);
			src = m_SwordNode->getOrientation()* (orientation.crossProduct(Ogre::Vector3::UNIT_Y));
			quat = src.getRotationTo(vecZ);
			m_SwordNode->rotate(quat, relativeTo);
			m_SwordNode->translate(0.8f * orientation * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			return;
		}
		if (rad < Ogre::Radian(Ogre::Math::PI / rotatePre * evt.timeSinceLastFrame) && (!m_bGetAttackPos))
		{
			//先将剩余的角度旋转完
			m_SwordNode->rotate(axis, rad, relativeTo);
			//得到剑体AI到攻击目标的方向
			m_Direction = m_AttackNode->getPosition() - m_SwordNode->getPosition();
			//单位化方向向量
			m_Distance = m_Direction.normalise();
			//根据剑体类型来重新计算目标攻击点
			if (m_Sword.m_bOneAttack)
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_STOP_DISTANCE);
			else
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_ATTACK_DISTANCE);
			//表明已记录下目标攻击点
			m_bGetAttackPos = true;
		}
	}
	else
	{
		if (!m_bGetAttackPos)
		{
			m_SwordNode->rotate(quat, relativeTo);
			//得到剑体AI到攻击目标的方向
			m_Direction = m_AttackNode->getPosition() - m_Sword.m_Position;
			//得到剑体AI与目标之间的距离
			m_Distance = m_Direction.length();
			//单位化方向向量
			m_Direction.normalise();
			//根据剑体类型来重新计算目标攻击点
			if (m_Sword.m_bOneAttack)
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_STOP_DISTANCE);
			else
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_ATTACK_DISTANCE);
			m_bGetAttackPos = true;
		}
	}

	//剑体旋转
	if (rotateItself)
		m_SwordNode->roll(Ogre::Radian(rotateRate *Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
	m_SwordNode->translate(m_Direction * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
	//播放剑鸣呼啸声
	m_SoundMgr->PlaySound(m_iHowlSd, m_SwordNode, &m_iHowlChannel, 0.25f);
}

//-------------------------------------【RotateAndStop()函数】---------------------------------------
// Desc:朝某个方向调转一个面向并停下
//---------------------------------------------------------------------------------------------------
bool Sword::RotateAndStop(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 1.0f */,
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//设置剑体AI的朝向
	Ogre::Vector3 src = m_SwordNode->getOrientation()* orientation;

	//剑体AI的朝向到目标面向方向的旋转
	Ogre::Quaternion quat = src.getRotationTo(dirRotate);
	if (rotateResolve)
	{
		Ogre::Radian rad;
		Ogre::Vector3 axis;
		quat.ToAngleAxis(rad, axis);
		if (rad >= Ogre::Radian(Ogre::Math::PI / rotatePre * evt.timeSinceLastFrame))
		{
			m_SwordNode->rotate(axis, Ogre::Radian(Ogre::Math::PI / rotatePre * evt.timeSinceLastFrame), relativeTo);
			return false;
		}
		else
		{
			m_SwordNode->rotate(quat, relativeTo);
		}
	}
	else
	{
		m_SwordNode->rotate(quat, relativeTo);
	}
	return true;
}

//-----------------------------------【RotateAndDenfense()函数】-----------------------------------
// Desc:将剑灵绕着剑核向着某一个特定的轨道不断的旋转(roateRate在0.0f~1.0f之间)
//-------------------------------------------------------------------------------------------------
void Sword::RotateAndDenfense(const Ogre::FrameEvent & evt, float rotateRate /* = 10.0f */,
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	static Ogre::Vector3 randVec, x, y, z;
	static Ogre::Quaternion qua;

	//先将剑灵AI移动至检核，便于接下来的旋转
	m_SwordNode->setPosition(Ogre::Vector3(0, 0, 0));

	//绕X轴旋转,使得其绕正向行进
	m_SwordNode->pitch(Ogre::Radian(- rotateRate * Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);

	//根据剑灵AI新的剑面朝向将其按原距离平移回
	m_SwordNode->translate(Ogre::Vector3::UNIT_Y * 1.20f * SWORD_HOMING_DISTANCE, Ogre::Node::TS_LOCAL);
}

//----------------------------------【DieToHoming()函数】----------------------------------------------
// Desc:剑体死亡后成为剑灵, 归位到杀戮者按排序的九灵位之一，并且自身不断旋转
//-----------------------------------------------------------------------------------------------------
void Sword::DieToHoming(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
	const Ogre::Vector3 & dirRotate,bool rotateResolve /* = true */, float rotatePre /* = 30.0f */,
	float ownRotateRate /* = 1.0f */,Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	static Ogre::Real distance;
	//剑灵标识为-1表示灵位已满,直接退出
	if (m_iRotToCoreIndex == -1) 
		return;
	//归位计算
	distance = m_SwordNode->getPosition().length();
	static Ogre::Vector3 dirGo;
	if (distance < SWORD_HOMING_DISTANCE)
	{
		RotateAndStop(evt, orientation, dirRotate, rotateResolve, rotatePre, relativeTo); 
		//有选择性的移向剑灵位
		switch (m_iRotToCoreIndex)
		{
		case 0:
		case 1:
		case 2:
			dirGo = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(m_iRotToCoreIndex * 120.0f)),
				0, Ogre::Math::Sin(Ogre::Degree(m_iRotToCoreIndex * 120.0f)));
			break;
		case 3:
		case 4:
		case 5:
			dirGo = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(40.0f + (m_iRotToCoreIndex - 3) * 120.0f)),
				0, Ogre::Math::Sin(Ogre::Degree(40.0f + (m_iRotToCoreIndex - 3) * 120.0f)));
			break;
		case 6:
		case 7:
		case 8:
			dirGo = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(80.0f + (m_iRotToCoreIndex - 6) * 120.0f)),
				0, Ogre::Math::Sin(Ogre::Degree(80.0f + (m_iRotToCoreIndex - 6) * 120.0f)));
			break;
		}
		m_SwordNode->translate(10 * dirGo * evt.timeSinceLastFrame, relativeTo);
	}
	else
	{
		m_SwordNode->roll(Ogre::Radian(ownRotateRate * Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
	}
}
//-----------------------------------【keyPressed()函数】---------------------------------------
// Desc:设置剑体AI成为剑灵后的相关键盘按下操作
//----------------------------------------------------------------------------------------------
bool Sword::keyPressed(const OIS::KeyEvent & arg)
{
	static Ogre::FrameEvent evt;
	static Ogre::Vector3 dirGo;
	switch (arg.key)
	{
	case OIS::KC_SPACE:
		if (!m_bDead || (ms_iRotToCoreNum == 0) || (m_iLaunchIndex != m_iRotToCoreIndex) || m_bRotateDenfense || m_bDivineSword123)
			return true;
		//状态及属性量的改变
		m_Model->setLaunchIndex(m_iRotToCoreIndex - 1);
		m_iRotToCoreIndex = -1;
		ms_iRotToCoreNum--;
		m_bLaunchState = true;
		ms_iLaunchNum++;
		m_Sword.m_FlySpeed = m_Sword.m_FlySpeed;
		//剑灵的位置及大小的改变
		ms_SwordCore->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
		m_SceneMgr->getRootSceneNode()->addChild(m_SwordNode);
		m_SwordNode->resetToInitialState();
		m_SwordNode->setScale(m_Sword.m_Scale);
		m_SwordNode->setPosition(m_AttackNode->getPosition());
		m_SwordNode->setOrientation(m_AttackNode->getOrientation());
		//播放剑灵射出的声音
		m_SoundMgr->PlaySound(m_iSwordShootSd1, NULL, &m_iSwordShootChannel1);
		m_SoundMgr->PlaySound(m_iSwordShootSd2, NULL, &m_iSwordShootChannel2);
		//显示技能遮罩
		m_NinthCEGUI->getSkillShadeW()[2]->show();
		m_bSpace = true;
		break;

	case OIS::KC_LSHIFT:
		if (!m_bDead)
			return true;
		m_bSwordMoved = true;
		break;

	case OIS::KC_C:
		if (ms_iRotToCoreNum < 3 || !m_bDead || (m_iRotToCoreIndex <= (ms_iRotToCoreNum - 4)) || ms_bHelSwordState 
			|| m_bRotateDenfense || m_bDivineSword123 || m_Model->getDivine4State())
			return true;

		//移出剑核节点，捆绑到螺旋节点,并进行调整
		ms_SwordCore->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
		ms_HelicalCore->addChild(m_SwordNode);
		m_SwordNode->setScale(m_Sword.m_Scale * 0.7f);
		m_SwordNode->setPosition(0, 0, 0);
		//状态量修改
		m_iRotToCoreIndex = -1;
		m_bHelicalSword = true;
		m_iHelicIndex = ++ms_iRKeySusCount;
		m_SwordEnt->setQueryFlags(SWORD_JIANLING_MASK);
		//位置变化
		switch (m_iHelicIndex)
		{
		case 1:
			dirGo = Vector3(Ogre::Math::Cos(0.0f), 0.0f, Ogre::Math::Sin(0.0f));
			break;
		case 2:
			dirGo = Vector3(Ogre::Math::Cos(2 * Ogre::Math::PI / 3), 0.0f, Ogre::Math::Sin(2 * Ogre::Math::PI / 3));
			break;
		case 3:
			dirGo = Vector3(Ogre::Math::Cos(4 * Ogre::Math::PI / 3), 0.0f, Ogre::Math::Sin(4 * Ogre::Math::PI / 3));
			break;
		}
		m_SwordNode->translate(dirGo * 23, Ogre::Node::TS_PARENT);
		RotateAndStop(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::NEGATIVE_UNIT_Y, false, 0.0f, Ogre::Node::TS_PARENT);	
		//播放风遁螺旋手里剑音效
		if (ms_iHelSword_FengDunChannel == INVALID_SOUND_CHANNEL)
		{
			m_SoundMgr->PlaySound(m_iHelSword_FengDunSd, m_AttackNode, &ms_iHelSword_FengDunChannel);
			m_NinthCEGUI->getSkillShadeW()[3]->show();
			m_NinthCEGUI->getSkillCoolCalW()[3]->show();
			//更改技能权限
			m_bHelSSCoolPer = true;
		}
			
		break;

	case OIS::KC_F :
		if (m_bDead && m_Model->getDivineTime() >= SWORD_DIVINE_COOL_TIME && (ms_iRotToCoreNum == 9)
			&& !m_bLaunchState && !m_bDivineSword123 && !m_bRotateDenfense)
		{
			ms_iDivSucNum++;
			if (ms_iDivSucNum == 9)
			{
				m_Model->setDivineTime(0.0f);
				ms_iDivSucNum = 0;
				m_NinthCEGUI->getSkillShadeW()[5]->show();
				m_NinthCEGUI->getSkillCoolCalW()[5]->show();
				m_Model->setDivSkillPer(true);
			}
			m_bDivineSword123 = true;
			//重置“独孤九剑，万剑归宗“中静态变量的值
			ms_iDiv3LaySuc = 0;
			ms_bNinthSwordShoot = false;
			ms_bDivine3LayDown = false;
			ms_bDivine4Start = false;
			ms_bDivine3Ready = false;
			ms_bPuppetStop = false;
			ms_Divine3Scale = SWORD_DIVINE2_SCALE;
		}
		break;

	case OIS::KC_X:
		if (m_bDead && ms_DenfenseTime >= SWORD_DEFENSE_COOL_TIME && (ms_iRotToCoreNum == 9) && !m_bLaunchState)
		{
			ms_iRotateDenSucNum++;
			m_bRotateDenfense = true;
			//设置随机的面向
			m_SwordNode->setOrientation(m_Orbit);
			m_SwordNode->setScale(0.6f * m_Sword.m_Scale);
			//使剑灵的负Y轴始终面向目标点
			Ogre::Vector3 src = m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Y;
			Ogre::Quaternion quat1 = src.getRotationTo(-m_SwordNode->getPosition());
			m_SwordNode->rotate(quat1, Ogre::Node::TS_PARENT);
			m_Model->getMeiZhiEntity()->setQueryFlags(INVICIBLE_MASK);
			m_Model->getYunTianHeEntity()->setQueryFlags(INVICIBLE_MASK);
			m_Model->getMainEntity()->setQueryFlags(INVICIBLE_MASK);
			//播放剑道防御声音
			if (ms_iSDefenseChannel == INVALID_SOUND_CHANNEL)
				m_SoundMgr->PlaySound(m_iSDefenseSd, m_AttackNode, &ms_iSDefenseChannel);
			if (ms_iDefenseScreamChannel == INVALID_SOUND_CHANNEL)
			{
				m_SoundMgr->PlaySound(m_iDefenseScreamSd, m_AttackNode, &ms_iDefenseScreamChannel);
			}
			//技能冷却权限赋予，技能栏更新，技能状态更新
			if (m_iRotToCoreIndex == 0)
			{
				m_bDenSkCoolPer = true;
				m_NinthCEGUI->getSkillShadeW()[4]->show();
				m_NinthCEGUI->getSkillCoolCalW()[4]->show();
				//剑道防御技能状态图标显示
				m_NinthCEGUI->showSkillState(1, 2);
			}
				
			//技能时间初始化
			if (ms_iRotateDenSucNum == 9)
				ms_DenfenseTime = 0.0f;
		}
		break;
	}
	return true;
}

bool Sword::keyReleased(const OIS::KeyEvent & arg)
{
	switch (arg.key)
	{
	case OIS::KC_LSHIFT:
		m_bSwordMoved = false;
		break;
	case OIS::KC_SPACE:
		if (m_bSpace)
		{
			if (ms_iRotToCoreNum!= 0)
				m_NinthCEGUI->getSkillShadeW()[2]->hide();
			m_bSpace = false;
		}
	}
	return true;
}
//---------------------------------【mousePressed()函数】--------------------------------------
// Desc:鼠标键按下的键位操作
//---------------------------------------------------------------------------------------------
bool Sword::mousePressed(const OIS::MouseEvent & arg, const OIS::MouseButtonID id)
{
	return true;
}
//----------------------------------【mouseMoved()函数】---------------------------------------
// Desc:设置射出后剑灵的移向
//---------------------------------------------------------------------------------------------
bool Sword::mouseMoved(const OIS::MouseEvent & arg)
{
	//螺旋剑技能
	if (m_bSwordMoved && ms_bHCLaunchState && ms_bHelSwordState)
	{
		if (m_iHelicIndex == 3) //由第三把剑来决定螺旋核的平移
		{
			Ogre::Vector3 dir;
			if (ms_bHelSwordManual)
			{
				dir = Vector3(0.3f * arg.state.X.rel, 0.08f * arg.state.Z.rel, 0.6f *arg.state.Y.rel);
				ms_HelSCenCore->translate(dir, Ogre::Node::TS_PARENT);
			}
			else
			{
				dir = Vector3(0, 0.02f * arg.state.Z.rel, 0.2f * arg.state.X.rel);
				ms_HelicalCore->translate(dir, Ogre::Node::TS_PARENT);
			}	
		}
	}
	//普通剑体发射
	if (m_bSwordMoved && m_bLaunchState)
	{
		//上下旋转
		Ogre::Vector3 zVec = m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		Ogre::Vector3 yVec = m_AttackNode->getOrientation() * Ogre::Vector3::UNIT_Y;
		Ogre::Vector3 xVec = (yVec).crossProduct(zVec);
		Ogre::Quaternion quat;
		quat.FromAngleAxis(Ogre::Radian(arg.state.Y.rel * SWORD_ROTATE_SPEED), xVec);
		m_SwordNode->rotate(quat, Ogre::Node::TS_PARENT);
		//左右旋转
		zVec = m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		yVec = zVec.crossProduct(xVec);
		quat.FromAngleAxis(Ogre::Radian(-arg.state.X.rel * SWORD_ROTATE_SPEED), yVec);
		m_SwordNode->rotate(quat, Ogre::Node::TS_PARENT);
	}
	return true;
}
//----------------------------------【frameRenderingQueued()函数】-----------------------------
// Desc:在渲染队列中更新剑体AI每一帧的行为
//---------------------------------------------------------------------------------------------
bool Sword::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	//相关声音的更新
	m_SoundMgr->FrameStarted(m_AttackNode, evt.timeSinceLastFrame);

	//保存一个欲发射的剑灵索引值
	m_iLaunchIndex = m_Model->getLaunchIndex();

	//得到剑体的目标位置及和攻击目标的距离
	if (!m_bDead || m_bLaunchState)
	{
		m_Sword.m_Position = m_SwordNode->getPosition();
		m_Distance = (m_Sword.m_Position - m_AttackNode->getPosition()).length();
		//距离目标太远直接返回
		if (!m_bAttackState && m_Distance >= 3 * SWORD_DEFENSE_DISTANCE) return true;
	}

	//死亡计数显示
	DieNumChangeShow();

	//死亡检测
	if (!m_bDead)
	{		
		//攻击检测
		if (this->ThroughCollDec())
		{
			m_Model->cutModelHP(5 * m_Sword.m_Damage * evt.timeSinceLastFrame);
			if (m_Model->getModelHP() <= 0.0f)
			{
				m_Model->setModelHP(0.0f);
				m_Model->setDieFlag(true);
			}
		}
		//被攻击检测
		if (this->BeJetCollDec() && m_Model->getLMouseDown())
		{
			m_HP -= 5 * (MODEL_DAMAGE - m_Sword.m_Defense) * evt.timeSinceLastFrame;
			//播放剑敲击声
			m_SoundMgr->PlaySound(m_iShootSwordSd, m_SwordNode, &m_iShootSwordChannel);
			if (m_HP <= 0.0f)
			{
				//剑体状态改变
				m_HP = m_Sword.m_MaxHP;
				m_bDead = true;
				m_bLaunchState = false;
				m_bDenFirstAttack = true;
				m_bArriveAttackPos = false;
				m_bGetAttackPos = false;
				m_bAttackState = false;
				ms_iDieNum++;
				//停止剑敲击声
				m_SoundMgr->StopSound(&m_iShootSwordChannel);
				//九剑灵未满时才剑灵归位
				if (ms_iRotToCoreNum < 9)
				{
					m_SwordEnt->setQueryFlags(SWORD_JIANLING_MASK);
					ms_iRotToCoreNum++;
					//归位成功,脱离原来的世界
					m_SceneMgr->getRootSceneNode()->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
					ms_SwordCore->addChild(m_SwordNode);
					m_SwordNode->resetToInitialState();
					m_SwordNode->setScale(0.4f * m_Sword.m_Scale);
					m_iRotToCoreIndex = ms_iRotToCoreNum - 1;	   //将剑灵位添加至已有的后面一个
					m_Model->setLaunchIndex(ms_iRotToCoreNum - 1); //欲发射的剑灵指向最后一个
					m_SoundMgr->PlaySound(m_iHomingSd, m_SwordNode, &m_iHomingChannel);
					if (ms_iRotToCoreNum == 9)
					{
						m_SoundMgr->PlaySound(m_iIkuSoSd, m_Model->getMainNode(), &m_iIkuSochannel);
						//刷新技能为可见，表示为可用状态
						m_NinthCEGUI->getSkillShadeW()[4]->hide();
						m_NinthCEGUI->getSkillShadeW()[5]->hide();
					}
					else if (ms_iRotToCoreNum == 3)
					{
						m_NinthCEGUI->getSkillShadeW()[3]->hide();
					}
					else if (ms_iRotToCoreNum == 1)
					{
						m_NinthCEGUI->getSkillShadeW()[2]->hide();
					}
				}
				else
				{
					m_bDead = false;
					setFollowRandPos(m_AttackNode->getPosition(), 2000);
				}
				return true;
			}
		}
	}
	else
	{
		
		//防御技能冷却计时及更新
		if (m_bDenSkCoolPer)
		{
			//累计防御开启后所经过的时间
			ms_DenfenseTime += evt.timeSinceLastFrame;
			if (ms_DenfenseTime <= SWORD_DEFENSE_COOL_TIME)
			{

				Ogre::Real cooTime = SWORD_DEFENSE_COOL_TIME - ms_DenfenseTime;
				m_NinthCEGUI->getSkillCoolCalW()[4]->setText(Ogre::StringConverter::toString(
					cooTime < 10.0f ? (int)(cooTime * 1000) / 1000.0f : (int)(cooTime * 100) / 100.0f));
			}
			else
			{
				ms_iRotateDenSucNum = 0;
				if (ms_iRotToCoreNum == 9)
				{
					m_NinthCEGUI->getSkillShadeW()[4]->hide();
					
				}
				else
				{
					m_NinthCEGUI->getSkillShadeW()[4]->show();
				}
				m_NinthCEGUI->getSkillCoolCalW()[4]->hide();
				m_bDenSkCoolPer = false;
			}
		}
		//剑灵体系防御技能释放
		if (m_bRotateDenfense)
		{
			//绕剑核旋转防御
			RotateAndDenfense(evt, 2.7f, Ogre::Node::TS_PARENT);
			if (ms_DenfenseTime >= SWORD_MAX_DENFENSE_TIME)
			{
				m_bRotateDenfense = false;
				m_SwordNode->resetToInitialState();
				m_SwordNode->setScale(0.4f * m_Sword.m_Scale);
				m_Model->getMeiZhiEntity()->setQueryFlags(MAINMODEL_MASK);
				m_Model->getYunTianHeEntity()->setQueryFlags(MAINMODEL_MASK);
				m_Model->getMainEntity()->setQueryFlags(MAINMODEL_MASK);
				//停止剑道防御声音播放
				m_SoundMgr->StopSound(&ms_iSDefenseChannel);
				ms_iDefenseScreamChannel = INVALID_SOUND_CHANNEL;
				//隐藏技能防御状态显示
				m_NinthCEGUI->hideSkillState(1);
			}
			return true;
		}

		//“独孤九剑, 万剑归宗"技能发动
		if (m_bDivineSword123)
		{
			switch (m_iDivineSword)
			{
			case 1:
				if (DivineSword1(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3(0, SWORD_RISE_MAX_HEIGHT, 0),
					2.0f, m_Sword.m_bAttackRotate, 15.0f, Ogre::Node::TS_PARENT))
					m_iDivineSword++;
				break;
			case 2:
				if (DivineSword2(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, 2.0f, Ogre::Node::TS_PARENT))
					m_iDivineSword++;
				break;
			case 3:		
				if (ms_bNinthSwordShoot)
				{
					Ogre::Vector3 dirGo = -m_SwordNode->getPosition();
					dirGo.y = 0.0f;
					if (DivineSword3(evt, Ogre::Vector3::UNIT_Z, dirGo, true, 4.0f, true, 3.0f,
						2.0f, 2.0f, Ogre::Node::TS_PARENT))
						m_iDivineSword++;
				}
				break;
			}
			//判断“独孤九剑,万剑归宗“结束的标志
			if (m_iDivineSword == 4) 
			{
				m_bDivineSword123 = false;
				m_iDivineSword = 1;
				m_bDiv3LaySuc = false;
				m_Model->setLaunchIndex(-1);
			}
			return true;
		}

		//螺旋剑技能发动
		if (m_bHelicalSword)
		{
			if (m_bHelSSCoolPer)
			{
				m_HelSwordKTime += evt.timeSinceLastFrame;
				Ogre::Real cooTime = SWORD_HELSWORD_KEEP_TIME - m_HelSwordKTime + 1.0f;
				if (cooTime >= 0.0f)
				{
					m_NinthCEGUI->getSkillCoolCalW()[3]->setText(Ogre::StringConverter::toString(
						cooTime < 10.0f ? (int)(cooTime * 1000) / 1000.0f : (int)(cooTime * 100) / 100.0f));
				}
				else
				{
					m_NinthCEGUI->getSkillCoolCalW()[3]->hide();
					m_bHelSSCoolPer = false;
				}
			}
			HelicalSwordFrameRendering(evt);
			return true;
		}
		
		//发动成功时不执行之后的代码...

		if (!m_bLaunchState)
		{
			//剑灵归位
			DieToHoming(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::NEGATIVE_UNIT_Y, true, 2.0f, 2.0f);
			//剑核旋转
			ms_SwordCore->yaw(Ogre::Radian(2.0f * evt.timeSinceLastFrame / ms_iRotToCoreNum), Ogre::Node::TS_LOCAL);
		}
		else
		{
			//不断的向前前进,并进行加速旋转
			m_SwordNode->translate( m_Sword.m_FlySpeed * Ogre::Vector3::NEGATIVE_UNIT_Z * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			m_SwordNode->roll(Ogre::Radian(50.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
			//离开最大有效发射距离时执行下列动作
			if (m_Distance >= SWORD_LAUNCH_VALID_DIS)
			{
				m_Sword.m_FlySpeed = m_Sword.m_FlySpeed;
				m_bDead = false;
				m_bLaunchState = false;
				m_SwordNode->setScale(m_Sword.m_Scale);
				m_SwordEnt->setQueryFlags(SWORD_NORMAL_MASK);
			}
			
		}
		return true;

	}

	//-----------------------------------------【未死亡时执行】---------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//第一次攻击或已到达攻击点时才会再次进行计算攻击目标
	if (m_bDenFirstAttack || m_bArriveAttackPos)
	{
		//得到剑体AI到攻击目标的方向
		Ogre::Vector3 direction = m_AttackNode->getPosition() - m_SwordNode->getPosition();
		//得到剑体AI与目标之间的距离
		m_Distance = direction.length();
		//---------------------------------【发现有目标侵入防御领域】---------------------------------
		if (m_Distance <= SWORD_DEFENSE_DISTANCE)
		{
			//处于攻击状态
			m_bAttackState = true;
			//欲发动攻击，则表明攻击后不是侵入防御领域的第一次攻击趋势
			if (m_bDenFirstAttack)
				m_bDenFirstAttack = false;
		}
		//---------【在防御领域之外，无非两种可能，1：目标未侵入, 2:攻击后与目标脱离防御领域】---------
		else
		{
			//如果是穷追不舍剑并且已陷入了攻击状态，则依旧发动攻击
			if (m_Sword.m_bNoStopAttack && (!m_bDenFirstAttack))
			{
				m_bAttackState = true;
			}
			else //非穷追不舍剑，则原地待命
			{
				RotateAndStop(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::NEGATIVE_UNIT_Y, true, 5.0f, Ogre::Node::TS_PARENT);
				//此后有再次侵入防御领域的可能，将防御领域欲第一次攻击记为true
				m_bDenFirstAttack = true;
				//表明其处于非攻击状态
				m_bAttackState = false;
			}
		}
		//到达只有一次
		if (m_bArriveAttackPos)
			m_bArriveAttackPos = false;
	}

	//处于攻击状态时进行是否到达攻击目标位置判断
	if (m_bAttackState)
	{
		//到达攻击目标点则将标志记为true,否则继续发动攻击
		if (( m_SwordNode->getPosition().x <= m_AttackPos.x + 100.0f
			&&m_SwordNode->getPosition().x >= m_AttackPos.x - 100.0f
			&&m_SwordNode->getPosition().y <= m_AttackPos.y + 100.0f
			&&m_SwordNode->getPosition().y >= m_AttackPos.y - 100.0f
			&&m_SwordNode->getPosition().z <= m_AttackPos.z + 50.0f
			&&m_SwordNode->getPosition().z >= m_AttackPos.z - 50.0f)
			&& m_bGetAttackPos)
		{
			m_bArriveAttackPos = true;
			//到达目标攻击点时重置
			m_bGetAttackPos = false;
		}
		else
		{	
			//真正发动攻击的移动
			RotateAndGo(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, m_AttackPos,true, 1.0f, m_Sword.m_bAttackRotate, 15.0f);
		}
	}

	return true;
}

//----------------------------------【PuppetFrameRenderingQueued()函数】-------------------------------
// Desc:仅作为特定的傀儡剑体操控时所更新的每一帧行为
//-----------------------------------------------------------------------------------------------------
bool Sword::PuppetFrameRenderingQueued(const Ogre::FrameEvent & evt)
{
	//更新声源及听众信息
	m_SoundMgr->FrameStarted(m_AttackNode, evt.timeSinceLastFrame);

	static int iShootSwordIndex = 100;
	static Ogre::Real distance;
	//激射对应索引的傀儡剑体或使其不可见,其他返回
	if (iShootSwordIndex != m_Sword.m_ObjectIndex && iShootSwordIndex + 1 != m_Sword.m_ObjectIndex
		&& iShootSwordIndex + 2 != m_Sword.m_ObjectIndex && iShootSwordIndex + 3 != m_Sword.m_ObjectIndex)
		return false;
	if (!ms_bPuppetStop)
	{
		m_SwordNode->setVisible(true);
		m_SwordEnt->setQueryFlags(SWORD_JIANLING_MASK);
		m_SwordNode->translate(2.2f * Ogre::Vector3::NEGATIVE_UNIT_Z * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		//播放射出时的剑鸣声
		m_SoundMgr->PlaySound(m_iDiv4_ShootSd, m_SwordNode, &m_iDiv4_ShootChannel);
		//判断激射行为停止的标志
		if (m_SwordNode->getPosition().y <= -64.0f)
		{
			//播放残留的雷鸣声
			m_SoundMgr->PlaySound(m_iDiv4_LThunderSd, m_SwordNode, &m_iDiv4_LThunderChannel);
			if (m_Sword.m_ObjectIndex % 4 == 0)
			{
				iShootSwordIndex += 4;
			}
			if (iShootSwordIndex == 100 + DIVINE_SWORD_NUM)
			{
				iShootSwordIndex = 100;
				ms_bPuppetStop = true;
			}
		}
	}
	else //当全部傀儡剑体激射完时,使全部都不可见
	{
		m_SwordNode->setVisible(false);	
		//将查询标志改为普通剑体
		m_SwordEnt->setQueryFlags(SWORD_NORMAL_MASK);
		iShootSwordIndex += 1;
		if (iShootSwordIndex == 100 + DIVINE_SWORD_NUM)
		{
			iShootSwordIndex = 100;
			m_Model->setDivine4State(false);	//停止剑体傀儡帧行为的执行
			m_Model->setSmallerState(true);     //设置角色变小状态
		}
	}
	return true;
}
//--------------------------------------【setPosition()函数】------------------------------------------
// Desc:设置剑体AI的位置
//-----------------------------------------------------------------------------------------------------
void Sword::setPosition(Ogre::Vector3 positon)
{
	m_Sword.m_Position = positon;
	m_SwordNode->setPosition(m_Sword.m_Position);
}

//-------------------------------------【setRandPosition()函数】---------------------------------------
// Desc:设置剑体AI的随机位置
//-----------------------------------------------------------------------------------------------------
void Sword::setRandPosition(Ogre::Real range /* = 2000 */)
{
	m_Sword.m_Position = Ogre::Vector3(
		Ogre::Real(rand() % (int)range) - range / 2,
		Ogre::Real(rand() % (int)range) - range / 2,
		Ogre::Real(rand() % (int)range) - range / 2);
	m_SwordNode->setPosition(m_Sword.m_Position);
}
//----------------------------------------【setRandCylinerPos()函数】-----------------------------------
// Desc:设置圆柱体领域的剑体AI随机位置
//------------------------------------------------------------------------------------------------------
void Sword::setRandCylinderPos(Ogre::Real rad, Ogre::Real height)
{
	Ogre::Real xyRadius = Ogre::Real(rand() % (int)rad);
	m_Sword.m_Position = Ogre::Vector3(
		rad * Ogre::Math::Cos(Ogre::Real(rand())),
		Ogre::Real(rand() % (int)height) - height / 2,
		rad * Ogre::Math::Sin(Ogre::Real(rand()))
		);
	m_SwordNode->setPosition(m_Sword.m_Position);
}
//----------------------------------------【setFollowRandCylinerPos()函数】-----------------------------------
// Desc:设置基于某目标的圆柱体领域的剑体AI随机位置
//------------------------------------------------------------------------------------------------------
void Sword::setFollowRandCylinderPos(const Ogre::Vector3 &position, Ogre::Real rad, Ogre::Real height)
{
	Ogre::Real xyRadius = Ogre::Real(rand() % (int)rad);
	m_Sword.m_Position = Ogre::Vector3(
		position.x + rad * Ogre::Math::Cos(Ogre::Real(rand())),
		position.y + Ogre::Real(rand() % (int)height) - height / 2,
		position.z + rad * Ogre::Math::Sin(Ogre::Real(rand()))
		);
	m_SwordNode->setPosition(m_Sword.m_Position);
}

//----------------------------------------【setFollowRandPos()函数】------------------------------------
// Desc:设置剑体AI基于某一点的随机位置
//-----------------------------------------------------------------------------------------------------
void Sword::setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range /* = 2000 */)
{
	m_Sword.m_Position = Ogre::Vector3(
		position.x + Ogre::Real(rand() % (int)range) - range / 2,
		position.y + Ogre::Real(rand() % (int)range) - range / 2,
		position.z + Ogre::Real(rand() % (int)range) - range / 2);
	m_SwordNode->setPosition(m_Sword.m_Position);
}
//-----------------------------------------【BeJetCollDec()函数】---------------------------------------
// Desc:对剑体AI的被喷射攻击进行碰撞检测
//------------------------------------------------------------------------------------------------------
bool Sword::BeJetCollDec()
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
		if (iter->movable->getQueryFlags() == SWORD_NORMAL_MASK)
		{
			if (iter->movable->getParentSceneNode() == m_SwordNode)
			{
				return true;
			}
		}
	}
	return false;
}

//-----------------------------------------【ThroughCollDec()函数】-------------------------------------
// Desc:对剑体AI的穿透攻击进行碰撞检测
//------------------------------------------------------------------------------------------------------

bool Sword::ThroughCollDec()
{
	static Ogre::Ray ray;
	static Ogre::Vector3 srcPos;
	m_Distance = (m_AttackNode->getPosition() - m_SwordNode->getPosition()).length();
	if (m_bDead || (m_Distance >= 100.0f) || m_Model->getDieFlag()) return false;
	//计算源及射线
	srcPos = m_SwordNode->getPosition();
	ray = Ogre::Ray(srcPos, m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z);

	m_RaySceneQuery->setRay(ray);
	Ogre::RaySceneQueryResult & result = m_RaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator iter = result.begin();

	//射线检测和包围盒碰撞检测
	for (; iter != result.end(); iter++)
	{
		if (iter->movable->getQueryFlags() == MAINMODEL_MASK)
		{
			if (m_SwordEnt->getBoundingBox().intersects(m_Model->getMainEntity()->getBoundingBox()))
			{
				m_SoundMgr->PlaySound(m_iThroughtDestSd, m_SwordNode, &m_iThroughtDestChannel);
				m_SoundMgr->setVolume(0.8f, &m_iThroughtDestChannel);
				return true;
			}
		}
	}
	
	return false;
}

//------------------------------【ChangePosToWorld()函数】------------------------------
// Desc:将基于场景根节点的坐标转换为基于世界的(前提：场景根节点pitch了PI/2)
//--------------------------------------------------------------------------------------
inline Ogre::Vector3 Sword::ChangePosToWorld(const Ogre::Vector3 &pos) const
{
	Ogre::Vector3 src(pos.x, -pos.z, pos.y);
	return src;
}

//------------------------------【DivineSword1()函数】-----------------------------------
// Desc:集齐九剑灵发动之超强技--------独孤九剑,万剑归宗(第一段)九剑归一
//---------------------------------------------------------------------------------------
bool Sword::DivineSword1(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
	Ogre::Vector3 destPos, float ownCenterRotateRate  /* =2.0f */,  bool rotateItself /* = true */,
	float rotateRate /* = 1.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	Ogre::Real distance;
	distance = m_SwordNode->getPosition().length();

	if (distance < 1.7f * SWORD_HOMING_DISTANCE)
	{
		static Ogre::Vector3 dirGo;
		switch (m_iRotToCoreIndex)
		{
		case 0:
		case 1:
		case 2:
			dirGo = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(m_iRotToCoreIndex * 120.0f)),
				0, Ogre::Math::Sin(Ogre::Degree(m_iRotToCoreIndex * 120.0f)));
			break;
		case 3:
		case 4:
		case 5:
			dirGo = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(40.0f + (m_iRotToCoreIndex - 3) * 120.0f)),
				0, Ogre::Math::Sin(Ogre::Degree(40.0f + (m_iRotToCoreIndex - 3) * 120.0f)));
			break;
		case 6:
		case 7:
		case 8:
			dirGo = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(80.0f + (m_iRotToCoreIndex - 6) * 120.0f)),
				0, Ogre::Math::Sin(Ogre::Degree(80.0f + (m_iRotToCoreIndex - 6) * 120.0f)));
			break;
		}
		m_SwordNode->translate(30 * dirGo * evt.timeSinceLastFrame, relativeTo);
		//播放平移呼啸声
		m_SoundMgr->PlaySound(m_iDiv1_TrasSd, m_Model->getMainNode(), &ms_iDiv1_TrasChannel);
	}
	else
	{
		static Ogre::Quaternion quat;
		static Ogre::Vector3 dirGo;
		static Ogre::Radian rad;
		if (m_Divine1RotateTime <= 2.0f)
		{
			Ogre::Quaternion quat;
			quat.FromAngleAxis(Ogre::Radian(ownCenterRotateRate * Ogre::Math::PI * evt.timeSinceLastFrame / ms_iRotToCoreNum), m_SwordNode->getPosition());
			m_SwordNode->rotate(quat, relativeTo);
			m_Divine1RotateTime += evt.timeSinceLastFrame;
			ms_SwordCore->yaw(Ogre::Radian(0.11f * Ogre::Math::PI * evt.timeSinceLastFrame ), Ogre::Node::TS_LOCAL);
			//播放剑灵旋转声
			m_SoundMgr->PlaySound(m_iDiv1_RotSd, m_Model->getMainNode(), &ms_iDiv1_RotChannel);
		}
		else
		{
			Ogre::Vector3 src = m_SwordNode->getOrientation() * orientation;
			dirGo = destPos - m_SwordNode->getPosition();
			//剑体AI的朝向到目标面向方向的旋转
			quat = src.getRotationTo(dirGo);
			m_SwordNode->rotate(quat, relativeTo);				

			//剑体旋转
			if (rotateItself)
				m_SwordNode->roll(Ogre::Radian(Ogre::Math::PI * rotateRate * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
			//朝目标点前进
			dirGo.normalise();
			m_SwordNode->translate(dirGo * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
			//停止旋转的音效,播放射出的音效
			m_SoundMgr->StopSound(&ms_iDiv1_RotChannel);
			m_SoundMgr->PlaySound(m_iDiv1_ShootSd, m_Model->getMainNode(), &ms_iDiv1_ShootChannel);
			//到达第一阶段临界点
			if (m_SwordNode->getPosition().positionEquals(destPos, 3.0f))
			{
				m_SwordNode->setPosition(2 * destPos);
				m_SwordNode->setScale(SWORD_DIVINE2_SCALE, SWORD_DIVINE2_SCALE, SWORD_DIVINE2_SCALE);
				m_SwordNode->setVisible(false);
				m_Divine1RotateTime = 0.0f;
				return true;
			}
		}
	}
	return false;
}
//------------------------------【DivineSword1()函数】-----------------------------------
// Desc:集齐九剑灵发动之超强技--------独孤九剑,万剑归宗(第二段)之九剑归宗
//---------------------------------------------------------------------------------------
bool Sword::DivineSword2(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		float shootSpeedRate /* = 3.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	static int iShootSwordIndex;
	if (iShootSwordIndex != m_iRotToCoreIndex)
		return false;
	static Ogre::Vector3 dirGo;
	static Ogre::Real distance;
	dirGo = Ogre::Vector3(SWORD_SHOOT_DEVIATE_DIS * Ogre::Math::Cos(Ogre::Degree(iShootSwordIndex * 40.0f)),
		-2 * SWORD_RISE_MAX_HEIGHT, SWORD_SHOOT_DEVIATE_DIS * Ogre::Math::Sin(Ogre::Degree(iShootSwordIndex * 40.0f)));
	distance = dirGo.normalise();
	static bool bOrientated = false;
	if (!bOrientated)//已调整朝向并使其可见
	{
		Ogre::Vector3 src = m_SwordNode->getOrientation() * orientation;
		Ogre::Quaternion quat = src.getRotationTo(dirGo);
		m_SwordNode->rotate(quat, relativeTo);
		m_SwordNode->setVisible(true);
		bOrientated = true;
	}		//否则开始狂暴射击
	else
	{
		m_SwordNode->translate(dirGo * m_Sword.m_FlySpeed * shootSpeedRate * evt.timeSinceLastFrame, relativeTo);
		//播放雷鸣爆射声
		m_SoundMgr->PlaySound(m_iDiv2Sd, m_SwordNode, &m_iDiv2Channel);
		//超出距离判断
		Ogre::Vector3 pos = m_SwordNode->getPosition();
		if (pos.y <= 0.0f)
		{
			m_SwordNode->setPosition(pos.x, 0, pos.z);
			bOrientated = false;
			iShootSwordIndex++;
			if (iShootSwordIndex == 9)
			{
				iShootSwordIndex = 0;
				ms_bNinthSwordShoot = true;
			}
			return true;
		}
	}
	return false;
}
//------------------------------【DivineSword3()函数】-----------------------------------
// Desc:集齐九剑灵发动之超强技--------独孤九剑,万剑归宗(第三段)之九剑轮回
//---------------------------------------------------------------------------------------
bool Sword::DivineSword3(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 1.0f */,
	bool rotateItself /* = true */, float ownRotateRate /* = 1.0f */, float coreRotateRate /* = 1.0f */, 
	float flyRate /* = 1.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//让每柄剑灵绕自身旋转
	m_SwordNode->roll(Ogre::Radian(ownRotateRate * Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
	
	if (!ms_bDivine3LayDown)
	{
		//九根剑灵边旋转边躺下
		if (!m_bDiv3LaySuc)
		{
			if (RotateAndStop(evt, orientation, dirRotate, rotateResolve, rotatePre, relativeTo))
			{
				ms_iDiv3LaySuc++;
				m_bDiv3LaySuc = true;
			}
		}
		//播放短暂的雷鸣声
		m_SoundMgr->PlaySound(m_iDiv3_SThunderSd, m_Model->getMainNode(), &ms_iDiv3_SThunderChannel);
		//九根剑灵全部已躺下
		if (ms_iDiv3LaySuc == 9)
			ms_bDivine3LayDown = true;
	}
	else
	{
		if (ms_Divine3Scale <= 2 * SWORD_DIVINE2_SCALE)
		{
			m_SwordNode->setScale(ms_Divine3Scale += evt.timeSinceLastFrame / ms_iRotToCoreNum,
				ms_Divine3Scale += evt.timeSinceLastFrame / ms_iRotToCoreNum,
				ms_Divine3Scale += evt.timeSinceLastFrame / ms_iRotToCoreNum);
			ms_SwordCore->yaw(Ogre::Radian(coreRotateRate * Ogre::Math::PI * evt.timeSinceLastFrame / ms_iRotToCoreNum), Ogre::Node::TS_LOCAL);
			//播放剑灵旋转声,同时播放雷鸣声
			m_SoundMgr->PlaySound(m_iDiv3_RotSd, m_Model->getMainNode(), &ms_iDiv3_RotChannel);
			m_SoundMgr->PlaySound(m_iDiv3_ThunderSd, m_Model->getMainNode(), &ms_iDiv3_ThunderChannel);
		}
		else
		{
			if (!ms_bDivine4Start)	//第四段“万剑归宗”未开启时只做不断旋转的等待状态,并累计等待时长准备发动
			{
				m_NinthCEGUI->getSkillShadeW()[6]->hide();
				m_NinthCEGUI->getSkillCoolCalW()[6]->hide();
				ms_bDivine3Ready = true;
				ms_SwordCore->yaw(Ogre::Radian(coreRotateRate * Ogre::Math::PI * evt.timeSinceLastFrame / ms_iRotToCoreNum), Ogre::Node::TS_LOCAL);
				ms_Divine4WaitTime += evt.timeSinceLastFrame / ms_iRotToCoreNum;
				//超过等待时间,直接发动“万剑归宗”连续技，并且射出九剑灵
				if (ms_Divine4WaitTime >= SWORD_DIVINE4_WAIT_TIME)
				{
					ms_Divine4WaitTime = 0.0f;
					ms_bDivine4Start = true;
					//停止播放剑灵旋转声
					m_SoundMgr->StopSound(&ms_iDiv3_RotChannel);
				}
			}
			else
			{
				m_SwordNode->translate(-orientation * flyRate * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
				//播放剑灵射出声
				m_SoundMgr->PlaySound(ms_iDiv3_ShootSd, m_Model->getMainNode(), &ms_iDiv3_ShootChannel);
				//超出距离判断
				if (m_SwordNode->getPosition().length() >= SWORD_DIVINE3_SWEEP_DIS)
				{
					//停止播放剑灵旋转声,射出声和雷鸣声
					m_SoundMgr->StopSound(&ms_iDiv3_RotChannel);
					m_SoundMgr->StopSound(&ms_iDiv3_ShootChannel);
					m_SoundMgr->StopSound(&ms_iDiv3_ThunderChannel);
					//将剑灵重置到世界当中成为剑体,并且改变其属性量
					Ogre::Vector3 worldPos = m_SwordNode->_getDerivedPosition();
					ms_SwordCore->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
					m_SceneMgr->getRootSceneNode()->addChild(m_SwordNode);
					m_SwordNode->setScale(m_Sword.m_Scale);
					m_SwordNode->setPosition(worldPos);
					m_SwordEnt->setQueryFlags(SWORD_NORMAL_MASK);
					m_bDead = false;
					//m_bSeal = true;
					m_iRotToCoreIndex = -1;
					ms_iRotToCoreNum--;
					return true;
				}
			}
		}
	}
	return false;
}

//--------------------------------------【HelicalSword()函数】-------------------------------------------
// Desc:螺旋剑技能(平移，旋转，发动)
//-------------------------------------------------------------------------------------------------------
bool Sword::HelicalSword(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 0.3f */,
	float ownRotateRate /* = 1.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	m_SoundMgr->PlaySound(m_iHelSword_SThunderSd, m_Model->getMainNode(), &ms_iHelSword_SThunderCannel);
	if(RotateAndStop(evt, orientation, dirRotate, rotateResolve, rotatePre, relativeTo))
		return true;
	return false;
}

//---------------------------------【HelicalSwordFrameRendering()函数】----------------------------------
// Desc:螺旋剑技能发动时每一帧的行为
//-------------------------------------------------------------------------------------------------------
bool Sword::HelicalSwordFrameRendering(const Ogre::FrameEvent & evt)
{
	
		//累积恢复
		if (ms_iRKeySusCount == 3 && m_iHelicIndex == 3)
		{
			ms_iRotToCoreNum -= 3;
			m_Model->setLaunchIndex(ms_iRotToCoreNum - 1);
			ms_iRKeySusCount = 0;
			ms_bHelSwordAchieved = false;
			ms_bHelSwordState = true;
		}

		if (ms_iRKeySusCount == 0) //累计已恢复成功
		{
			Ogre::Vector3 dirGo;
			//选择转动方向
			switch (m_iHelicIndex)
			{
			case 1:
				dirGo = Vector3(Ogre::Math::Cos(0.0f), 0.0f, Ogre::Math::Sin(0.0f));
				break;
			case 2:
				dirGo = Vector3(Ogre::Math::Cos(2 * Ogre::Math::PI / 3), 0.0f, Ogre::Math::Sin(2 * Ogre::Math::PI / 3));
				break;
			case 3:
				dirGo = Vector3(Ogre::Math::Cos(4 * Ogre::Math::PI / 3), 0.0f, Ogre::Math::Sin(4 * Ogre::Math::PI / 3));
				break;
			}

			if (!ms_bHCLaunchState)
			{
				//螺旋剑技能发动中
				if (HelicalSword(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, dirGo, true, 2.0f, 1.0f, Ogre::Node::TS_PARENT))
				{
					if (m_iHelicIndex == 3)
					{
						ms_bHCLaunchState = true;
						//播放螺旋转动的声效
						if (ms_bHelSwordManual)
							m_SoundMgr->PlaySound(m_iHelSword_RotSd1, ms_HelicalCore, &ms_iHelSword_RotChannel1);
						else
							m_SoundMgr->PlaySound(m_iHelSword_RotSd2, ms_HelicalCore, &ms_iHelSword_RotChannel2);
					}
				}
			}
			else  //螺旋剑发射中
			{
				if (m_iHelicIndex == 3)
				{
					//不断的向前前进,并进行加速旋转
					static float fHelSwordTime = 0.0f;
					fHelSwordTime += evt.timeSinceLastFrame;
					if (!ms_bHelSwordManual)
						ms_HelSCenCore->yaw(Ogre::Radian(6.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
					if (ms_bHelSwordManual)
					{
						ms_HelicalCore->yaw(Ogre::Radian(15.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
						//播放螺旋转动的声效
						if (ms_iDiv1_RotChannel == INVALID_SOUND_CHANNEL)
						{
							m_SoundMgr->StopSound(&ms_iHelSword_RotChannel2);
							m_SoundMgr->PlaySound(m_iHelSword_RotSd1, ms_HelicalCore, &ms_iHelSword_RotChannel1);
						}
					}
					else
					{
						ms_HelicalCore->yaw(Ogre::Radian(5.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
						//播放螺旋转动的声效
						if (ms_iDiv3_RotChannel == INVALID_SOUND_CHANNEL)
						{
							m_SoundMgr->StopSound(&ms_iHelSword_RotChannel1);
							m_SoundMgr->PlaySound(m_iHelSword_RotSd2, ms_HelicalCore, &ms_iHelSword_RotChannel2);
						}
					}
					//离开最大有效发射距离时执行下列动作
					if (fHelSwordTime >= SWORD_HELSWORD_KEEP_TIME)
					{
						ms_bHelSwordAchieved = true;
						fHelSwordTime = 0.0f;
					}
				}
				//螺旋剑技能完成时设定剑体位置及状态量
				if (ms_bHelSwordAchieved)
				{
					m_bDead = false;
					m_bHelicalSword = false;
					m_iHelicIndex = 0;
					Ogre::Vector3 worldPos = m_SwordNode->_getDerivedPosition();
					ms_HelicalCore->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
					m_SceneMgr->getRootSceneNode()->addChild(m_SwordNode);
					m_SwordNode->setPosition(worldPos + 100 * dirGo);
					m_SwordNode->setScale(m_Sword.m_Scale);
					m_SwordEnt->setQueryFlags(SWORD_NORMAL_MASK);
					//累计移除螺旋检核的剑体数，达到三时重置螺旋剑技能状态
					ms_iRevSwordFromHC++;
					if (ms_iRevSwordFromHC == 3)
					{
						m_SoundMgr->StopSound(&ms_iHelSword_RotChannel1);
						m_SoundMgr->StopSound(&ms_iHelSword_RotChannel2);
						ms_iHelSword_FengDunChannel = INVALID_SOUND_CHANNEL;
						ms_bHelSwordState = false;
						ms_iRevSwordFromHC = 0;
						ms_HelicalCore->setPosition(0, 0, 0);
						ms_HelSCenCore->setPosition(0, 0, 0);
						ms_bHCLaunchState = false;
						m_HelSwordKTime = 0.0f;
						if (ms_iRotToCoreNum >= 3)
						{
							m_NinthCEGUI->getSkillShadeW()[3]->hide();
							m_NinthCEGUI->getSkillCoolCalW()[3]->hide();
						}
					}
				}
			}
		}
		return true;
}