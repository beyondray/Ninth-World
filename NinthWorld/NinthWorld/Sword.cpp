/*------------------------------------------------------------------
��ͷ�ļ�����Sword.h		Created by Beyond Ray,2014��4��
����������һ����װ�˽���AI������Ϊ�����ʵ��
--------------------------------------------------------------------*/
#include "stdafx.h"
#include"Sword.h"
#include"time.h"
#include "Dragon.h"
//��̬��Ա��ʼ��
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

//------------------------------------��RadFabs()������---------------------------------------
// Desc:���Ⱦ���ֵ
//--------------------------------------------------------------------------------------------
Ogre::Radian RadFabs(Ogre::Radian rad)
{
	if (rad < Ogre::Radian(0.0f))
		return -rad;
	else
		return rad;
}
//-------------------------------------��Sword()������----------------------------------------
// Desc:���캯��
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
	//�������߳�����ѯ
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
	//�������򳡾���ѯ
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
	//�������߳�����ѯ
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
	//�������򳡾���ѯ
	m_AABBSceneQuery = m_SceneMgr->createAABBQuery(Ogre::AxisAlignedBox());
}

//-------------------------------------��~Sword()������---------------------------------------
// Desc:��������
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

//---------------------------------��SwordInitialise()������-----------------------------------
// Desc:����AI�������Գ�ʼ��
//---------------------------------------------------------------------------------------------
void Sword::SwordInitialise(const Sword_Object & sword)
{
	//��������ĳ�ʼ��
	m_iThroughtDestSd = m_SoundMgr->CreateSound(Ogre::String("(��)a����-��-����.wav"));
	m_iHowlSd = m_SoundMgr->CreateSound(Ogre::String("(��)�����Х.mp3"));
	m_iSDefenseSd = m_SoundMgr->CreateSound(Ogre::String("(��)ˮ֮ӿ����.wav"));
	m_iDefenseScreamSd = m_SoundMgr->CreateSound(Ogre::String("(��)ˮ����.mp3"));
	m_iHomingSd = m_SoundMgr->CreateSound(Ogre::String("(��)�����λ.wav"));
	m_iSwordShootSd1 = m_SoundMgr->CreateSound(Ogre::String("(��)���鷢��-�ű�.snd"));
	m_iSwordShootSd2 = m_SoundMgr->CreateSound(Ogre::String("(��)���鷢��-��Х.snd"));
	m_iIkuSoSd = m_SoundMgr->CreateSound(Ogre::String("(��)������-���߷緶.ogg"));
	m_iShootSwordSd = m_SoundMgr->CreateSound(Ogre::String("(��)���û���.wav"));
	//���¾Ž������Ч��ʼ��
	m_iDiv1_TrasSd = m_SoundMgr->CreateSound(Ogre::String("(��)���¾Ž�1��ƽ��.wav"));
	m_iDiv1_RotSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(��)���¾Ž�1����ת.snd"));
	m_iDiv1_ShootSd = m_SoundMgr->CreateSound(Ogre::String("(��)���¾Ž�1�����.wav"));
	m_iDiv2Sd = m_SoundMgr->CreateSound(Ogre::String("(��)���¾Ž�2��.wav"));
	m_iDiv3_RotSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(��)���¾Ž�3����ת.wav"));
	m_iDiv3_SThunderSd = m_SoundMgr->CreateSound(Ogre::String("(��)���¾Ž�3��������.wav"));
	m_iDiv3_ThunderSd = m_SoundMgr->CreateSound(Ogre::String("(��)���¾Ž�3������.wav"));
	ms_iDiv3_ShootSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(��)���¾Ž�3�����.snd"));
	//�����������Ч��ʼ��
	m_iHelSword_FengDunSd = m_SoundMgr->CreateSound(Ogre::String("(��)����������｣.mp3"));
	m_iHelSword_SThunderSd = m_iDiv3_SThunderSd;
	m_iHelSword_RotSd1 = m_iDiv1_RotSd;
	m_iHelSword_RotSd2 = m_iDiv3_RotSd;

	//����������Գ�ʼ��
	m_Sword = sword;
	m_HP = sword.m_MaxHP;

	//�����������Ԫ��
	Ogre::Vector3 ax, ay, az;
	ax = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	ay = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	az = ax.crossProduct(ay);
	ay = az.crossProduct(ax);
	ax.normalise(); ay.normalise(); az.normalise();
	m_Orbit.FromAxes(ax, ay, az);

	//��ʼ������,Ϊ�����λ��׼��
	if (!ms_bCoreHaved)
	{
		ms_SwordCore = m_AttackNode->createChildSceneNode();
		ms_bCoreHaved = true;
	}

	//��ʼ�������ˣ�Ϊ���������ܷ�����׼��
	if (!ms_bHCorehaved)
	{
		ms_HelSCenCore = m_AttackNode->createChildSceneNode();
		ms_HelicalCore = ms_HelSCenCore->createChildSceneNode("HelicalCore");
		ms_bHCorehaved = true;
	}

	//��������AI��ʵ����󣬲���ӵ������ڵ�
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
//---------------------------------��PuppetSwordInitialise()������----------------------------------
// Desc:����AI�������Գ�ʼ��
//---------------------------------------------------------------------------------------------
void Sword::PuppetSwordInitialise(const Sword_Object & sword)
{
	//�򽣹��������Ч��ʼ��
	m_iDiv4_ShootSd = m_SoundMgr->CreateSound(Ogre::String("(��)�򽣹���-�������.wav"));
	m_iDiv4_LThunderSd = m_SoundMgr->CreateSound(Ogre::String("(��)�򽣹���-��������.wav"));

	//����������Գ�ʼ��
	m_Sword = sword;
	m_HP = sword.m_MaxHP;
	//��������AI��ʵ����󣬲���ӵ������ڵ�
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

//-------------------------------------��RotateAndGo()������---------------------------------------
// Desc:��ת����һ�������ھ��������Ƿ�ֽ�󣬳���Ŀ�귽��ǰ��
//-------------------------------------------------------------------------------------------------
void Sword::RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	Ogre::Vector3 destPos, bool rotateResolve /* = true */, float rotatePre /* = 1.0f */,
	bool rotateItself /* = true */, float rotateRate /* = 15.0f */, 
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//�ù���Ŀ��������ǰ���ķ���
	static Ogre::Vector3 dirGo;
	static Ogre::Quaternion quat;
	static Ogre::Radian rad;
	if (!m_bGetAttackPos)
	{
		//������ǰ������
		dirGo = m_AttackNode->getPosition() - m_Sword.m_Position;

		//���ý���AI�ĳ���
		Ogre::Vector3 src = m_SwordNode->getOrientation()* orientation;

		//����AI�ĳ���Ŀ�����������ת
		quat = src.getRotationTo(dirGo);			
	}

	if (rotateResolve)
	{
		static Ogre::Vector3 axis;
		quat.ToAngleAxis(rad, axis);
		
		if (rad >= Ogre::Radian(Ogre::Math::PI / rotatePre * evt.timeSinceLastFrame))
		{
			m_SwordNode->rotate(axis, Ogre::Radian(Ogre::Math::PI / rotatePre * evt.timeSinceLastFrame), relativeTo);
	
			//������AI�ٴ���ת��ʹ�ý���AIʼ�ձ��������
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
			//�Ƚ�ʣ��ĽǶ���ת��
			m_SwordNode->rotate(axis, rad, relativeTo);
			//�õ�����AI������Ŀ��ķ���
			m_Direction = m_AttackNode->getPosition() - m_SwordNode->getPosition();
			//��λ����������
			m_Distance = m_Direction.normalise();
			//���ݽ������������¼���Ŀ�깥����
			if (m_Sword.m_bOneAttack)
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_STOP_DISTANCE);
			else
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_ATTACK_DISTANCE);
			//�����Ѽ�¼��Ŀ�깥����
			m_bGetAttackPos = true;
		}
	}
	else
	{
		if (!m_bGetAttackPos)
		{
			m_SwordNode->rotate(quat, relativeTo);
			//�õ�����AI������Ŀ��ķ���
			m_Direction = m_AttackNode->getPosition() - m_Sword.m_Position;
			//�õ�����AI��Ŀ��֮��ľ���
			m_Distance = m_Direction.length();
			//��λ����������
			m_Direction.normalise();
			//���ݽ������������¼���Ŀ�깥����
			if (m_Sword.m_bOneAttack)
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_STOP_DISTANCE);
			else
				m_AttackPos = m_Sword.m_Position + m_Direction * (m_Distance + SWORD_ATTACK_DISTANCE);
			m_bGetAttackPos = true;
		}
	}

	//������ת
	if (rotateItself)
		m_SwordNode->roll(Ogre::Radian(rotateRate *Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
	m_SwordNode->translate(m_Direction * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
	//���Ž�����Х��
	m_SoundMgr->PlaySound(m_iHowlSd, m_SwordNode, &m_iHowlChannel, 0.25f);
}

//-------------------------------------��RotateAndStop()������---------------------------------------
// Desc:��ĳ�������תһ������ͣ��
//---------------------------------------------------------------------------------------------------
bool Sword::RotateAndStop(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 1.0f */,
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//���ý���AI�ĳ���
	Ogre::Vector3 src = m_SwordNode->getOrientation()* orientation;

	//����AI�ĳ���Ŀ�����������ת
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

//-----------------------------------��RotateAndDenfense()������-----------------------------------
// Desc:���������Ž�������ĳһ���ض��Ĺ�����ϵ���ת(roateRate��0.0f~1.0f֮��)
//-------------------------------------------------------------------------------------------------
void Sword::RotateAndDenfense(const Ogre::FrameEvent & evt, float rotateRate /* = 10.0f */,
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	static Ogre::Vector3 randVec, x, y, z;
	static Ogre::Quaternion qua;

	//�Ƚ�����AI�ƶ�����ˣ����ڽ���������ת
	m_SwordNode->setPosition(Ogre::Vector3(0, 0, 0));

	//��X����ת,ʹ�����������н�
	m_SwordNode->pitch(Ogre::Radian(- rotateRate * Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);

	//���ݽ���AI�µĽ��泯���䰴ԭ����ƽ�ƻ�
	m_SwordNode->translate(Ogre::Vector3::UNIT_Y * 1.20f * SWORD_HOMING_DISTANCE, Ogre::Node::TS_LOCAL);
}

//----------------------------------��DieToHoming()������----------------------------------------------
// Desc:�����������Ϊ����, ��λ��ɱ¾�߰�����ľ���λ֮һ��������������ת
//-----------------------------------------------------------------------------------------------------
void Sword::DieToHoming(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
	const Ogre::Vector3 & dirRotate,bool rotateResolve /* = true */, float rotatePre /* = 30.0f */,
	float ownRotateRate /* = 1.0f */,Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	static Ogre::Real distance;
	//�����ʶΪ-1��ʾ��λ����,ֱ���˳�
	if (m_iRotToCoreIndex == -1) 
		return;
	//��λ����
	distance = m_SwordNode->getPosition().length();
	static Ogre::Vector3 dirGo;
	if (distance < SWORD_HOMING_DISTANCE)
	{
		RotateAndStop(evt, orientation, dirRotate, rotateResolve, rotatePre, relativeTo); 
		//��ѡ���Ե�������λ
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
//-----------------------------------��keyPressed()������---------------------------------------
// Desc:���ý���AI��Ϊ��������ؼ��̰��²���
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
		//״̬���������ĸı�
		m_Model->setLaunchIndex(m_iRotToCoreIndex - 1);
		m_iRotToCoreIndex = -1;
		ms_iRotToCoreNum--;
		m_bLaunchState = true;
		ms_iLaunchNum++;
		m_Sword.m_FlySpeed = m_Sword.m_FlySpeed;
		//�����λ�ü���С�ĸı�
		ms_SwordCore->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
		m_SceneMgr->getRootSceneNode()->addChild(m_SwordNode);
		m_SwordNode->resetToInitialState();
		m_SwordNode->setScale(m_Sword.m_Scale);
		m_SwordNode->setPosition(m_AttackNode->getPosition());
		m_SwordNode->setOrientation(m_AttackNode->getOrientation());
		//���Ž������������
		m_SoundMgr->PlaySound(m_iSwordShootSd1, NULL, &m_iSwordShootChannel1);
		m_SoundMgr->PlaySound(m_iSwordShootSd2, NULL, &m_iSwordShootChannel2);
		//��ʾ��������
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

		//�Ƴ����˽ڵ㣬���������ڵ�,�����е���
		ms_SwordCore->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
		ms_HelicalCore->addChild(m_SwordNode);
		m_SwordNode->setScale(m_Sword.m_Scale * 0.7f);
		m_SwordNode->setPosition(0, 0, 0);
		//״̬���޸�
		m_iRotToCoreIndex = -1;
		m_bHelicalSword = true;
		m_iHelicIndex = ++ms_iRKeySusCount;
		m_SwordEnt->setQueryFlags(SWORD_JIANLING_MASK);
		//λ�ñ仯
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
		//���ŷ���������｣��Ч
		if (ms_iHelSword_FengDunChannel == INVALID_SOUND_CHANNEL)
		{
			m_SoundMgr->PlaySound(m_iHelSword_FengDunSd, m_AttackNode, &ms_iHelSword_FengDunChannel);
			m_NinthCEGUI->getSkillShadeW()[3]->show();
			m_NinthCEGUI->getSkillCoolCalW()[3]->show();
			//���ļ���Ȩ��
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
			//���á����¾Ž����򽣹��ڡ��о�̬������ֵ
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
			//�������������
			m_SwordNode->setOrientation(m_Orbit);
			m_SwordNode->setScale(0.6f * m_Sword.m_Scale);
			//ʹ����ĸ�Y��ʼ������Ŀ���
			Ogre::Vector3 src = m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Y;
			Ogre::Quaternion quat1 = src.getRotationTo(-m_SwordNode->getPosition());
			m_SwordNode->rotate(quat1, Ogre::Node::TS_PARENT);
			m_Model->getMeiZhiEntity()->setQueryFlags(INVICIBLE_MASK);
			m_Model->getYunTianHeEntity()->setQueryFlags(INVICIBLE_MASK);
			m_Model->getMainEntity()->setQueryFlags(INVICIBLE_MASK);
			//���Ž�����������
			if (ms_iSDefenseChannel == INVALID_SOUND_CHANNEL)
				m_SoundMgr->PlaySound(m_iSDefenseSd, m_AttackNode, &ms_iSDefenseChannel);
			if (ms_iDefenseScreamChannel == INVALID_SOUND_CHANNEL)
			{
				m_SoundMgr->PlaySound(m_iDefenseScreamSd, m_AttackNode, &ms_iDefenseScreamChannel);
			}
			//������ȴȨ�޸��裬���������£�����״̬����
			if (m_iRotToCoreIndex == 0)
			{
				m_bDenSkCoolPer = true;
				m_NinthCEGUI->getSkillShadeW()[4]->show();
				m_NinthCEGUI->getSkillCoolCalW()[4]->show();
				//������������״̬ͼ����ʾ
				m_NinthCEGUI->showSkillState(1, 2);
			}
				
			//����ʱ���ʼ��
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
//---------------------------------��mousePressed()������--------------------------------------
// Desc:�������µļ�λ����
//---------------------------------------------------------------------------------------------
bool Sword::mousePressed(const OIS::MouseEvent & arg, const OIS::MouseButtonID id)
{
	return true;
}
//----------------------------------��mouseMoved()������---------------------------------------
// Desc:����������������
//---------------------------------------------------------------------------------------------
bool Sword::mouseMoved(const OIS::MouseEvent & arg)
{
	//����������
	if (m_bSwordMoved && ms_bHCLaunchState && ms_bHelSwordState)
	{
		if (m_iHelicIndex == 3) //�ɵ����ѽ������������˵�ƽ��
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
	//��ͨ���巢��
	if (m_bSwordMoved && m_bLaunchState)
	{
		//������ת
		Ogre::Vector3 zVec = m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		Ogre::Vector3 yVec = m_AttackNode->getOrientation() * Ogre::Vector3::UNIT_Y;
		Ogre::Vector3 xVec = (yVec).crossProduct(zVec);
		Ogre::Quaternion quat;
		quat.FromAngleAxis(Ogre::Radian(arg.state.Y.rel * SWORD_ROTATE_SPEED), xVec);
		m_SwordNode->rotate(quat, Ogre::Node::TS_PARENT);
		//������ת
		zVec = m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		yVec = zVec.crossProduct(xVec);
		quat.FromAngleAxis(Ogre::Radian(-arg.state.X.rel * SWORD_ROTATE_SPEED), yVec);
		m_SwordNode->rotate(quat, Ogre::Node::TS_PARENT);
	}
	return true;
}
//----------------------------------��frameRenderingQueued()������-----------------------------
// Desc:����Ⱦ�����и��½���AIÿһ֡����Ϊ
//---------------------------------------------------------------------------------------------
bool Sword::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	//��������ĸ���
	m_SoundMgr->FrameStarted(m_AttackNode, evt.timeSinceLastFrame);

	//����һ��������Ľ�������ֵ
	m_iLaunchIndex = m_Model->getLaunchIndex();

	//�õ������Ŀ��λ�ü��͹���Ŀ��ľ���
	if (!m_bDead || m_bLaunchState)
	{
		m_Sword.m_Position = m_SwordNode->getPosition();
		m_Distance = (m_Sword.m_Position - m_AttackNode->getPosition()).length();
		//����Ŀ��̫Զֱ�ӷ���
		if (!m_bAttackState && m_Distance >= 3 * SWORD_DEFENSE_DISTANCE) return true;
	}

	//����������ʾ
	DieNumChangeShow();

	//�������
	if (!m_bDead)
	{		
		//�������
		if (this->ThroughCollDec())
		{
			m_Model->cutModelHP(5 * m_Sword.m_Damage * evt.timeSinceLastFrame);
			if (m_Model->getModelHP() <= 0.0f)
			{
				m_Model->setModelHP(0.0f);
				m_Model->setDieFlag(true);
			}
		}
		//���������
		if (this->BeJetCollDec() && m_Model->getLMouseDown())
		{
			m_HP -= 5 * (MODEL_DAMAGE - m_Sword.m_Defense) * evt.timeSinceLastFrame;
			//���Ž��û���
			m_SoundMgr->PlaySound(m_iShootSwordSd, m_SwordNode, &m_iShootSwordChannel);
			if (m_HP <= 0.0f)
			{
				//����״̬�ı�
				m_HP = m_Sword.m_MaxHP;
				m_bDead = true;
				m_bLaunchState = false;
				m_bDenFirstAttack = true;
				m_bArriveAttackPos = false;
				m_bGetAttackPos = false;
				m_bAttackState = false;
				ms_iDieNum++;
				//ֹͣ���û���
				m_SoundMgr->StopSound(&m_iShootSwordChannel);
				//�Ž���δ��ʱ�Ž����λ
				if (ms_iRotToCoreNum < 9)
				{
					m_SwordEnt->setQueryFlags(SWORD_JIANLING_MASK);
					ms_iRotToCoreNum++;
					//��λ�ɹ�,����ԭ��������
					m_SceneMgr->getRootSceneNode()->removeChild("BeyondSword" + Ogre::StringConverter::toString(m_Sword.m_ObjectIndex));
					ms_SwordCore->addChild(m_SwordNode);
					m_SwordNode->resetToInitialState();
					m_SwordNode->setScale(0.4f * m_Sword.m_Scale);
					m_iRotToCoreIndex = ms_iRotToCoreNum - 1;	   //������λ��������еĺ���һ��
					m_Model->setLaunchIndex(ms_iRotToCoreNum - 1); //������Ľ���ָ�����һ��
					m_SoundMgr->PlaySound(m_iHomingSd, m_SwordNode, &m_iHomingChannel);
					if (ms_iRotToCoreNum == 9)
					{
						m_SoundMgr->PlaySound(m_iIkuSoSd, m_Model->getMainNode(), &m_iIkuSochannel);
						//ˢ�¼���Ϊ�ɼ�����ʾΪ����״̬
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
		
		//����������ȴ��ʱ������
		if (m_bDenSkCoolPer)
		{
			//�ۼƷ�����������������ʱ��
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
		//������ϵ���������ͷ�
		if (m_bRotateDenfense)
		{
			//�ƽ�����ת����
			RotateAndDenfense(evt, 2.7f, Ogre::Node::TS_PARENT);
			if (ms_DenfenseTime >= SWORD_MAX_DENFENSE_TIME)
			{
				m_bRotateDenfense = false;
				m_SwordNode->resetToInitialState();
				m_SwordNode->setScale(0.4f * m_Sword.m_Scale);
				m_Model->getMeiZhiEntity()->setQueryFlags(MAINMODEL_MASK);
				m_Model->getYunTianHeEntity()->setQueryFlags(MAINMODEL_MASK);
				m_Model->getMainEntity()->setQueryFlags(MAINMODEL_MASK);
				//ֹͣ����������������
				m_SoundMgr->StopSound(&ms_iSDefenseChannel);
				ms_iDefenseScreamChannel = INVALID_SOUND_CHANNEL;
				//���ؼ��ܷ���״̬��ʾ
				m_NinthCEGUI->hideSkillState(1);
			}
			return true;
		}

		//�����¾Ž�, �򽣹���"���ܷ���
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
			//�жϡ����¾Ž�,�򽣹��ڡ������ı�־
			if (m_iDivineSword == 4) 
			{
				m_bDivineSword123 = false;
				m_iDivineSword = 1;
				m_bDiv3LaySuc = false;
				m_Model->setLaunchIndex(-1);
			}
			return true;
		}

		//���������ܷ���
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
		
		//�����ɹ�ʱ��ִ��֮��Ĵ���...

		if (!m_bLaunchState)
		{
			//�����λ
			DieToHoming(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::NEGATIVE_UNIT_Y, true, 2.0f, 2.0f);
			//������ת
			ms_SwordCore->yaw(Ogre::Radian(2.0f * evt.timeSinceLastFrame / ms_iRotToCoreNum), Ogre::Node::TS_LOCAL);
		}
		else
		{
			//���ϵ���ǰǰ��,�����м�����ת
			m_SwordNode->translate( m_Sword.m_FlySpeed * Ogre::Vector3::NEGATIVE_UNIT_Z * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			m_SwordNode->roll(Ogre::Radian(50.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
			//�뿪�����Ч�������ʱִ�����ж���
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

	//-----------------------------------------��δ����ʱִ�С�---------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//��һ�ι������ѵ��﹥����ʱ�Ż��ٴν��м��㹥��Ŀ��
	if (m_bDenFirstAttack || m_bArriveAttackPos)
	{
		//�õ�����AI������Ŀ��ķ���
		Ogre::Vector3 direction = m_AttackNode->getPosition() - m_SwordNode->getPosition();
		//�õ�����AI��Ŀ��֮��ľ���
		m_Distance = direction.length();
		//---------------------------------��������Ŀ�������������---------------------------------
		if (m_Distance <= SWORD_DEFENSE_DISTANCE)
		{
			//���ڹ���״̬
			m_bAttackState = true;
			//������������������������������������ĵ�һ�ι�������
			if (m_bDenFirstAttack)
				m_bDenFirstAttack = false;
		}
		//---------���ڷ�������֮�⣬�޷����ֿ��ܣ�1��Ŀ��δ����, 2:��������Ŀ�������������---------
		else
		{
			//�������׷���ὣ�����������˹���״̬�������ɷ�������
			if (m_Sword.m_bNoStopAttack && (!m_bDenFirstAttack))
			{
				m_bAttackState = true;
			}
			else //����׷���ὣ����ԭ�ش���
			{
				RotateAndStop(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::NEGATIVE_UNIT_Y, true, 5.0f, Ogre::Node::TS_PARENT);
				//�˺����ٴ������������Ŀ��ܣ���������������һ�ι�����Ϊtrue
				m_bDenFirstAttack = true;
				//�����䴦�ڷǹ���״̬
				m_bAttackState = false;
			}
		}
		//����ֻ��һ��
		if (m_bArriveAttackPos)
			m_bArriveAttackPos = false;
	}

	//���ڹ���״̬ʱ�����Ƿ񵽴﹥��Ŀ��λ���ж�
	if (m_bAttackState)
	{
		//���﹥��Ŀ����򽫱�־��Ϊtrue,���������������
		if (( m_SwordNode->getPosition().x <= m_AttackPos.x + 100.0f
			&&m_SwordNode->getPosition().x >= m_AttackPos.x - 100.0f
			&&m_SwordNode->getPosition().y <= m_AttackPos.y + 100.0f
			&&m_SwordNode->getPosition().y >= m_AttackPos.y - 100.0f
			&&m_SwordNode->getPosition().z <= m_AttackPos.z + 50.0f
			&&m_SwordNode->getPosition().z >= m_AttackPos.z - 50.0f)
			&& m_bGetAttackPos)
		{
			m_bArriveAttackPos = true;
			//����Ŀ�깥����ʱ����
			m_bGetAttackPos = false;
		}
		else
		{	
			//���������������ƶ�
			RotateAndGo(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, m_AttackPos,true, 1.0f, m_Sword.m_bAttackRotate, 15.0f);
		}
	}

	return true;
}

//----------------------------------��PuppetFrameRenderingQueued()������-------------------------------
// Desc:����Ϊ�ض��Ŀ��ܽ���ٿ�ʱ�����µ�ÿһ֡��Ϊ
//-----------------------------------------------------------------------------------------------------
bool Sword::PuppetFrameRenderingQueued(const Ogre::FrameEvent & evt)
{
	//������Դ��������Ϣ
	m_SoundMgr->FrameStarted(m_AttackNode, evt.timeSinceLastFrame);

	static int iShootSwordIndex = 100;
	static Ogre::Real distance;
	//�����Ӧ�����Ŀ��ܽ����ʹ�䲻�ɼ�,��������
	if (iShootSwordIndex != m_Sword.m_ObjectIndex && iShootSwordIndex + 1 != m_Sword.m_ObjectIndex
		&& iShootSwordIndex + 2 != m_Sword.m_ObjectIndex && iShootSwordIndex + 3 != m_Sword.m_ObjectIndex)
		return false;
	if (!ms_bPuppetStop)
	{
		m_SwordNode->setVisible(true);
		m_SwordEnt->setQueryFlags(SWORD_JIANLING_MASK);
		m_SwordNode->translate(2.2f * Ogre::Vector3::NEGATIVE_UNIT_Z * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		//�������ʱ�Ľ�����
		m_SoundMgr->PlaySound(m_iDiv4_ShootSd, m_SwordNode, &m_iDiv4_ShootChannel);
		//�жϼ�����Ϊֹͣ�ı�־
		if (m_SwordNode->getPosition().y <= -64.0f)
		{
			//���Ų�����������
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
	else //��ȫ�����ܽ��弤����ʱ,ʹȫ�������ɼ�
	{
		m_SwordNode->setVisible(false);	
		//����ѯ��־��Ϊ��ͨ����
		m_SwordEnt->setQueryFlags(SWORD_NORMAL_MASK);
		iShootSwordIndex += 1;
		if (iShootSwordIndex == 100 + DIVINE_SWORD_NUM)
		{
			iShootSwordIndex = 100;
			m_Model->setDivine4State(false);	//ֹͣ�������֡��Ϊ��ִ��
			m_Model->setSmallerState(true);     //���ý�ɫ��С״̬
		}
	}
	return true;
}
//--------------------------------------��setPosition()������------------------------------------------
// Desc:���ý���AI��λ��
//-----------------------------------------------------------------------------------------------------
void Sword::setPosition(Ogre::Vector3 positon)
{
	m_Sword.m_Position = positon;
	m_SwordNode->setPosition(m_Sword.m_Position);
}

//-------------------------------------��setRandPosition()������---------------------------------------
// Desc:���ý���AI�����λ��
//-----------------------------------------------------------------------------------------------------
void Sword::setRandPosition(Ogre::Real range /* = 2000 */)
{
	m_Sword.m_Position = Ogre::Vector3(
		Ogre::Real(rand() % (int)range) - range / 2,
		Ogre::Real(rand() % (int)range) - range / 2,
		Ogre::Real(rand() % (int)range) - range / 2);
	m_SwordNode->setPosition(m_Sword.m_Position);
}
//----------------------------------------��setRandCylinerPos()������-----------------------------------
// Desc:����Բ��������Ľ���AI���λ��
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
//----------------------------------------��setFollowRandCylinerPos()������-----------------------------------
// Desc:���û���ĳĿ���Բ��������Ľ���AI���λ��
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

//----------------------------------------��setFollowRandPos()������------------------------------------
// Desc:���ý���AI����ĳһ������λ��
//-----------------------------------------------------------------------------------------------------
void Sword::setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range /* = 2000 */)
{
	m_Sword.m_Position = Ogre::Vector3(
		position.x + Ogre::Real(rand() % (int)range) - range / 2,
		position.y + Ogre::Real(rand() % (int)range) - range / 2,
		position.z + Ogre::Real(rand() % (int)range) - range / 2);
	m_SwordNode->setPosition(m_Sword.m_Position);
}
//-----------------------------------------��BeJetCollDec()������---------------------------------------
// Desc:�Խ���AI�ı����乥��������ײ���
//------------------------------------------------------------------------------------------------------
bool Sword::BeJetCollDec()
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

//-----------------------------------------��ThroughCollDec()������-------------------------------------
// Desc:�Խ���AI�Ĵ�͸����������ײ���
//------------------------------------------------------------------------------------------------------

bool Sword::ThroughCollDec()
{
	static Ogre::Ray ray;
	static Ogre::Vector3 srcPos;
	m_Distance = (m_AttackNode->getPosition() - m_SwordNode->getPosition()).length();
	if (m_bDead || (m_Distance >= 100.0f) || m_Model->getDieFlag()) return false;
	//����Դ������
	srcPos = m_SwordNode->getPosition();
	ray = Ogre::Ray(srcPos, m_SwordNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z);

	m_RaySceneQuery->setRay(ray);
	Ogre::RaySceneQueryResult & result = m_RaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator iter = result.begin();

	//���߼��Ͱ�Χ����ײ���
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

//------------------------------��ChangePosToWorld()������------------------------------
// Desc:�����ڳ������ڵ������ת��Ϊ���������(ǰ�᣺�������ڵ�pitch��PI/2)
//--------------------------------------------------------------------------------------
inline Ogre::Vector3 Sword::ChangePosToWorld(const Ogre::Vector3 &pos) const
{
	Ogre::Vector3 src(pos.x, -pos.z, pos.y);
	return src;
}

//------------------------------��DivineSword1()������-----------------------------------
// Desc:����Ž��鷢��֮��ǿ��--------���¾Ž�,�򽣹���(��һ��)�Ž���һ
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
		//����ƽ�ƺ�Х��
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
			//���Ž�����ת��
			m_SoundMgr->PlaySound(m_iDiv1_RotSd, m_Model->getMainNode(), &ms_iDiv1_RotChannel);
		}
		else
		{
			Ogre::Vector3 src = m_SwordNode->getOrientation() * orientation;
			dirGo = destPos - m_SwordNode->getPosition();
			//����AI�ĳ���Ŀ�����������ת
			quat = src.getRotationTo(dirGo);
			m_SwordNode->rotate(quat, relativeTo);				

			//������ת
			if (rotateItself)
				m_SwordNode->roll(Ogre::Radian(Ogre::Math::PI * rotateRate * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
			//��Ŀ���ǰ��
			dirGo.normalise();
			m_SwordNode->translate(dirGo * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
			//ֹͣ��ת����Ч,�����������Ч
			m_SoundMgr->StopSound(&ms_iDiv1_RotChannel);
			m_SoundMgr->PlaySound(m_iDiv1_ShootSd, m_Model->getMainNode(), &ms_iDiv1_ShootChannel);
			//�����һ�׶��ٽ��
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
//------------------------------��DivineSword1()������-----------------------------------
// Desc:����Ž��鷢��֮��ǿ��--------���¾Ž�,�򽣹���(�ڶ���)֮�Ž�����
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
	if (!bOrientated)//�ѵ�������ʹ��ɼ�
	{
		Ogre::Vector3 src = m_SwordNode->getOrientation() * orientation;
		Ogre::Quaternion quat = src.getRotationTo(dirGo);
		m_SwordNode->rotate(quat, relativeTo);
		m_SwordNode->setVisible(true);
		bOrientated = true;
	}		//����ʼ�����
	else
	{
		m_SwordNode->translate(dirGo * m_Sword.m_FlySpeed * shootSpeedRate * evt.timeSinceLastFrame, relativeTo);
		//��������������
		m_SoundMgr->PlaySound(m_iDiv2Sd, m_SwordNode, &m_iDiv2Channel);
		//���������ж�
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
//------------------------------��DivineSword3()������-----------------------------------
// Desc:����Ž��鷢��֮��ǿ��--------���¾Ž�,�򽣹���(������)֮�Ž��ֻ�
//---------------------------------------------------------------------------------------
bool Sword::DivineSword3(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 1.0f */,
	bool rotateItself /* = true */, float ownRotateRate /* = 1.0f */, float coreRotateRate /* = 1.0f */, 
	float flyRate /* = 1.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//��ÿ��������������ת
	m_SwordNode->roll(Ogre::Radian(ownRotateRate * Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
	
	if (!ms_bDivine3LayDown)
	{
		//�Ÿ��������ת������
		if (!m_bDiv3LaySuc)
		{
			if (RotateAndStop(evt, orientation, dirRotate, rotateResolve, rotatePre, relativeTo))
			{
				ms_iDiv3LaySuc++;
				m_bDiv3LaySuc = true;
			}
		}
		//���Ŷ��ݵ�������
		m_SoundMgr->PlaySound(m_iDiv3_SThunderSd, m_Model->getMainNode(), &ms_iDiv3_SThunderChannel);
		//�Ÿ�����ȫ��������
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
			//���Ž�����ת��,ͬʱ����������
			m_SoundMgr->PlaySound(m_iDiv3_RotSd, m_Model->getMainNode(), &ms_iDiv3_RotChannel);
			m_SoundMgr->PlaySound(m_iDiv3_ThunderSd, m_Model->getMainNode(), &ms_iDiv3_ThunderChannel);
		}
		else
		{
			if (!ms_bDivine4Start)	//���ĶΡ��򽣹��ڡ�δ����ʱֻ��������ת�ĵȴ�״̬,���ۼƵȴ�ʱ��׼������
			{
				m_NinthCEGUI->getSkillShadeW()[6]->hide();
				m_NinthCEGUI->getSkillCoolCalW()[6]->hide();
				ms_bDivine3Ready = true;
				ms_SwordCore->yaw(Ogre::Radian(coreRotateRate * Ogre::Math::PI * evt.timeSinceLastFrame / ms_iRotToCoreNum), Ogre::Node::TS_LOCAL);
				ms_Divine4WaitTime += evt.timeSinceLastFrame / ms_iRotToCoreNum;
				//�����ȴ�ʱ��,ֱ�ӷ������򽣹��ڡ�����������������Ž���
				if (ms_Divine4WaitTime >= SWORD_DIVINE4_WAIT_TIME)
				{
					ms_Divine4WaitTime = 0.0f;
					ms_bDivine4Start = true;
					//ֹͣ���Ž�����ת��
					m_SoundMgr->StopSound(&ms_iDiv3_RotChannel);
				}
			}
			else
			{
				m_SwordNode->translate(-orientation * flyRate * m_Sword.m_FlySpeed * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
				//���Ž��������
				m_SoundMgr->PlaySound(ms_iDiv3_ShootSd, m_Model->getMainNode(), &ms_iDiv3_ShootChannel);
				//���������ж�
				if (m_SwordNode->getPosition().length() >= SWORD_DIVINE3_SWEEP_DIS)
				{
					//ֹͣ���Ž�����ת��,�������������
					m_SoundMgr->StopSound(&ms_iDiv3_RotChannel);
					m_SoundMgr->StopSound(&ms_iDiv3_ShootChannel);
					m_SoundMgr->StopSound(&ms_iDiv3_ThunderChannel);
					//���������õ����統�г�Ϊ����,���Ҹı���������
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

//--------------------------------------��HelicalSword()������-------------------------------------------
// Desc:����������(ƽ�ƣ���ת������)
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

//---------------------------------��HelicalSwordFrameRendering()������----------------------------------
// Desc:���������ܷ���ʱÿһ֡����Ϊ
//-------------------------------------------------------------------------------------------------------
bool Sword::HelicalSwordFrameRendering(const Ogre::FrameEvent & evt)
{
	
		//�ۻ��ָ�
		if (ms_iRKeySusCount == 3 && m_iHelicIndex == 3)
		{
			ms_iRotToCoreNum -= 3;
			m_Model->setLaunchIndex(ms_iRotToCoreNum - 1);
			ms_iRKeySusCount = 0;
			ms_bHelSwordAchieved = false;
			ms_bHelSwordState = true;
		}

		if (ms_iRKeySusCount == 0) //�ۼ��ѻָ��ɹ�
		{
			Ogre::Vector3 dirGo;
			//ѡ��ת������
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
				//���������ܷ�����
				if (HelicalSword(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, dirGo, true, 2.0f, 1.0f, Ogre::Node::TS_PARENT))
				{
					if (m_iHelicIndex == 3)
					{
						ms_bHCLaunchState = true;
						//��������ת������Ч
						if (ms_bHelSwordManual)
							m_SoundMgr->PlaySound(m_iHelSword_RotSd1, ms_HelicalCore, &ms_iHelSword_RotChannel1);
						else
							m_SoundMgr->PlaySound(m_iHelSword_RotSd2, ms_HelicalCore, &ms_iHelSword_RotChannel2);
					}
				}
			}
			else  //������������
			{
				if (m_iHelicIndex == 3)
				{
					//���ϵ���ǰǰ��,�����м�����ת
					static float fHelSwordTime = 0.0f;
					fHelSwordTime += evt.timeSinceLastFrame;
					if (!ms_bHelSwordManual)
						ms_HelSCenCore->yaw(Ogre::Radian(6.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
					if (ms_bHelSwordManual)
					{
						ms_HelicalCore->yaw(Ogre::Radian(15.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
						//��������ת������Ч
						if (ms_iDiv1_RotChannel == INVALID_SOUND_CHANNEL)
						{
							m_SoundMgr->StopSound(&ms_iHelSword_RotChannel2);
							m_SoundMgr->PlaySound(m_iHelSword_RotSd1, ms_HelicalCore, &ms_iHelSword_RotChannel1);
						}
					}
					else
					{
						ms_HelicalCore->yaw(Ogre::Radian(5.0f * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
						//��������ת������Ч
						if (ms_iDiv3_RotChannel == INVALID_SOUND_CHANNEL)
						{
							m_SoundMgr->StopSound(&ms_iHelSword_RotChannel1);
							m_SoundMgr->PlaySound(m_iHelSword_RotSd2, ms_HelicalCore, &ms_iHelSword_RotChannel2);
						}
					}
					//�뿪�����Ч�������ʱִ�����ж���
					if (fHelSwordTime >= SWORD_HELSWORD_KEEP_TIME)
					{
						ms_bHelSwordAchieved = true;
						fHelSwordTime = 0.0f;
					}
				}
				//�������������ʱ�趨����λ�ü�״̬��
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
					//�ۼ��Ƴ�������˵Ľ��������ﵽ��ʱ��������������״̬
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