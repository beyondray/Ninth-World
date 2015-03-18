/*------------------------------------------------------------------
��ͷ�ļ�����Dragon.h		Created by Beyond Ray,2014��4��
����������һ����װ���������׹�˹AI������Ϊ�����ʵ��
--------------------------------------------------------------------*/
#include "stdafx.h"
#include"Dragon.h"
#include"time.h"
#include"Sword.h"
//��̬��Ա��ʼ��
int Dragon::ms_iDDieNum = 0;

//-------------------------------------��Dragon()������----------------------------------------
// Desc:���캯��
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
	//�������߳�����ѯ
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
	//Dragon�����ʼ��
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
	//�������߳�����ѯ
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	m_RaySceneQuery->setSortByDistance(true);
}

//-------------------------------------��~Dragon()������---------------------------------------
// Desc:��������
//---------------------------------------------------------------------------------------------
Dragon::~Dragon()
{
	m_SceneMgr->destroyQuery(m_RaySceneQuery);
	ms_iDDieNum = 0;
}

//---------------------------------��DragonInitialise()������----------------------------------
// Desc:����AI�������Գ�ʼ��
//---------------------------------------------------------------------------------------------
void Dragon::DragonInitialise(Dragon_Object &dragon)
{
	//���������ʼ��
	m_iDieSd = m_SoundMgr->CreateSound(Ogre::String("(��)������Ч.wav"));
	m_iScreenSd = m_SoundMgr->CreateSound(Ogre::String("(��)����.wav"));
	m_iThunderStormSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(��)�ױ�.mp3"));
	m_iAureolaSd = m_SoundMgr->CreateSound(Ogre::String("(��)ǿ��.wav"));
	m_iBiggerSd = m_SoundMgr->CreateSound(Ogre::String("(��)bigger���.mp3"));
	m_iHarmObjectSd = m_SoundMgr->CreateSound(Ogre::String("(��)�˺�Ů��1.wav"));
	m_iJetSd = m_SoundMgr->CreateSound(Ogre::String("(��)�������.wav"));

	//�����������Գ�ʼ��
	m_Dragon = dragon;
	m_HP = dragon.m_MaxHP;

	//�����������Ԫ��
	Ogre::Vector3 ax, ay, az;
	ax = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	ay = Ogre::Vector3(randFloat(), randFloat(), randFloat());
	az = ax.crossProduct(ay);
	ay = az.crossProduct(ax);
	ax.normalise(); ay.normalise(); az.normalise();
	m_Orbit.FromAxes(ax, ay, az);

	//��������AI��ʵ����󣬲���ӵ������ڵ�
	m_DraEnt = m_SceneMgr->createEntity(m_MeshName);
	m_DragonNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode(m_Dragon.m_Position);
	m_DragonNode->attachObject(m_DraEnt);
	m_DragonNode->setScale(m_Dragon.m_Scale);
	m_DraEnt->setQueryFlags(DRAGON_MASK);

	//���������䴩Ŀ��ʱ����ӵ�������Ч
	m_RedSmoke = m_SceneMgr->createParticleSystem("DragonRedSmoke" +
		Ogre::StringConverter::toString(m_Dragon.m_Index), "Examples/DSmoke");
	m_DragonNode->attachObject(m_RedSmoke);
	m_RedSmoke->setEmitting(false);
	m_RedSmoke->setKeepParticlesInLocalSpace(true);

	//��ӹ�����������ʼ��Ϊ������״̬
	ChooseBeam(m_Beam, m_Dragon.m_JetIndex);
	m_DragonNode->attachObject(m_Beam);
	m_Beam->setEmitting(false);

	//������ʱ�Ŀ���������ӳ�ʼ��
	m_Aureola = m_SceneMgr->createParticleSystem("Blood" + 
		Ogre::StringConverter::toString(m_Dragon.m_Index), "Beyond/Aureola");

	m_DragonNode->attachObject(m_Aureola);
	m_Aureola->setEmitting(false);

	//��ӷ��ж���������Ϊ����
	if (m_Dragon.m_FlyIndex == 0)
		m_Fly = m_DraEnt->getAnimationState("fly1");
	else if (m_Dragon.m_FlyIndex == 1)
		m_Fly = m_DraEnt->getAnimationState("fly2");
	else
		m_Fly = m_DraEnt->getAnimationState("fly3");
	m_Fly->setEnabled(true);

	//�����������,Ĭ��Ϊ�ر�
	m_Die = m_DraEnt->getAnimationState("fly3");
	m_Die->setEnabled(true);
}

//-------------------------------------��RotateAndAttack()������-----------------------------------
// Desc:ʹ������AI���Ź���Ŀ����ĳһ���������ת(roateRate��0.0f~1.0f֮��)
//-------------------------------------------------------------------------------------------------
void Dragon::RotateAndAttack(const Ogre::FrameEvent & evt, float roateRate /* = 1.0f */, 
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	static Ogre::Vector3 randVec, x, y, z;
	static Ogre::Quaternion qua;

	//�Ƚ�����AI�ƶ���Ŀ��㣬������Ԫ����������ת
	m_DragonNode->translate(m_Direcion, relativeTo);

	//������ת�����Ԫ������������Ϊ�µ���ת����
	qua = m_DragonNode->getOrientation() * m_Orbit;
	qua = m_DragonNode->getOrientation() + roateRate * evt.timeSinceLastFrame * (qua - m_DragonNode->getOrientation());
	m_DragonNode->setOrientation(qua);

	//��������AI�µ����泯���䰴ԭ����ƽ�ƻ�
	m_DragonNode->getOrientation().ToAxes(x, y, z);
	x.normalise();
	m_DragonNode->translate(-x * m_Distance, relativeTo);
}

//-------------------------------------��RotateAndGo()������---------------------------------------
// Desc:��ת����һ�������ھ��������Ƿ�ֽ�󣬳���Ŀ�귽��ǰ��(ʼ�ձ���Ogre::Vector3::UNIT_Y)
//-------------------------------------------------------------------------------------------------
void Dragon::RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	const Ogre::Vector3 & dirRotate, Ogre::Vector3 dirGo, bool rotateResolve /* = true */, 
	float roatePre /* = 30.0f */, bool toSkyOriSet /* = true */,
	Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//��������AI�ĳ���
	Ogre::Vector3 src = m_DragonNode->getOrientation()* orientation;

	//����AI�ĳ���Ŀ�����������ת
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
		//������AI�ٴ���ת��ʹ������AIʼ�ձ��������
		src = m_DragonNode->getOrientation()* orientation;
		Ogre::Vector3 vecZ = src.crossProduct(Ogre::Vector3::UNIT_Y);
		src = m_DragonNode->getOrientation()* (orientation.crossProduct(Ogre::Vector3::UNIT_Y));
		quat = src.getRotationTo(vecZ);
		m_DragonNode->rotate(quat, relativeTo);
	}
	//��Ŀ�귽���ǰ��
	dirGo.normalise();
	m_DragonNode->translate(dirGo * m_Dragon.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
}

//-------------------------------------��RotateAndDieRotate()������---------------------------------------
// Desc:��ת����һ�������ھ��������Ƿ�ֽ�󣬳���Ŀ�귽��ǰ��(ʼ�ձ���Ogre::Vector3::UNIT_Y)
//--------------------------------------------------------------------------------------------------------
void Dragon::RoateAndDieRotate(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 30.0f */,
	float dieRoateSpeed /* = 10.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//��������AI�ĳ���
	Ogre::Vector3 src = m_DragonNode->getOrientation()* orientation;

	//����AI�ĳ���Ŀ�����������ת
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
			//����ʱ��������ת
			m_DragonNode->pitch(Ogre::Radian(dieRoateSpeed * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
		}
	}
	else
	{
		m_DragonNode->rotate(quat, relativeTo);
	}

	//��Ŀ�귽���ǰ��
	Ogre::Vector3 dir = dirRotate;
	dir.normalise();
	m_DragonNode->translate(dir * m_Dragon.m_FlySpeed * evt.timeSinceLastFrame, relativeTo);
}
//----------------------------------��frameRenderingQueued()������-----------------------------
// Desc:����Ⱦ�����и�������AIÿһ֡����Ϊ
//---------------------------------------------------------------------------------------------
bool Dragon::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	//��������ĸ���
	m_SoundMgr->FrameStarted(m_Model->getMainNode(), evt.timeSinceLastFrame);

	//����������ʾ
	DieNumChangeShow();

	//�ۼ�ʱ�䣬����һ��ʱ��ʱ�����¹��
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
	//�������
	if (!m_bDead)
	{
		//����ҵĹ������
		if (this->JecCollDec())
		{
			m_Model->cutModelHP(5 * m_Dragon.m_Damage * evt.timeSinceLastFrame);
			//�˺����Է�ʱ���Է���������
			m_SoundMgr->PlaySound(m_iHarmObjectSd, m_Model->getMainNode(), &m_iHarmObjectChannel, 0.5f);
			if (m_Model->getModelHP() <= 0.0f)
			{
				m_Model->setModelHP(0.0f);
				m_Model->setDieFlag(true);
			}
		}
		//�����鹥����͸�ļ��
		BeThroughCollDec(evt);
		if (m_bBigger)
		{
			if (ScaleChange(evt, 3.0f, 3.5f, true))
				m_bBigger = false;
			//���������������
			m_SoundMgr->PlaySound(m_iBiggerSd, m_Model->getMainNode(), &m_iBiggerChannel);
		}
		
		//����ҹ����ļ��
		if ((this->BeJetCollDec() && m_Model->getLMouseDown()))
		{
			m_HP -= 5 * (MODEL_DAMAGE - m_Dragon.m_Defense) * evt.timeSinceLastFrame;
			m_Aureola->setEmitting(true);
			//�������������ױ���,���з���
			m_SoundMgr->PlaySound(m_iScreenSd, m_DragonNode, &m_iScreenChannel);
			m_SoundMgr->PlaySound(m_iThunderStormSd, m_AttackNode, &m_iThunderStormChannel);
			m_SoundMgr->PlaySound(m_iAureolaSd, m_DragonNode, &m_iAureolaChannel);
		}
		else
		{
			if (m_iThunderStormChannel != INVALID_SOUND_CHANNEL)
				m_SoundMgr->StopSound(&m_iThunderStormChannel);		
		}
		//Ѫ��Ϊ0���������
		if (m_HP <= 0.0f)
		{
			m_bBigger = false;
			m_bSmaller = true;
			m_bDead = true;
			m_Beam->setEmitting(false);
			m_RedSmoke->setEmitting(false);
			ms_iDDieNum++;
			m_iDieChannel = INVALID_SOUND_CHANNEL;
			//����������Ч��ֹͣ�ױ���Ч
			m_SoundMgr->PlaySound(m_iDieSd, m_Model->getMainNode(), &m_iDieChannel, 0.5f);
			m_SoundMgr->StopSound(&m_iThunderStormChannel);
			//ת�����ӹ���
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
		//���ڱ�������е�����AI�Ƚ��в��ϵ���С
		if (m_bSmaller)
		{
			if (ScaleChange(evt, 1.0f, 3.5f, false))
				m_bSmaller = false;
		}
		//�������AI����������֡
		m_Die->addTime( evt.timeSinceLastFrame);

		RoateAndDieRotate(evt, Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Y, true, 30.0f, 10.0f, Ogre::Node::TS_PARENT);

		//��Ŀ�귽�򿿽��ƶ�
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

	//-----------------------------------------��δ����ʱִ�С�---------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//�������AI�ķ��ж���֡
	m_bRunFlag ? m_Fly->addTime(1.6f * evt.timeSinceLastFrame) : m_Fly->addTime(1.2f * evt.timeSinceLastFrame);

	//�õ�����AI������Ŀ��ķ���
	m_Direcion = m_AttackNode->getPosition() - m_Dragon.m_Position;

	//�õ�����AI��Ŀ��֮��ľ���
	m_Distance = m_Direcion.length();

	//-----------------------������AI��Ŀ�����ǰ���͹�������Ϊ�ļ�⡿-----------------------
	//---------------------------------��Ŀ��׷�١�-------------------------------
	if (m_Distance >= DRAGON_CIRCLE_DISTANCE)
	{
		//���ܱ�־����
		m_bRunFlag = false;
		RotateAndGo(evt, Ogre::Vector3::UNIT_X, m_Direcion, m_Direcion, false, 0.0f, true, Ogre::Node::TS_PARENT);
	}
	else if (m_Distance < DRAGON_CIRCLE_DISTANCE && m_Distance >= DRAGON_RUNAWAY_DISTANCE)
	{
		//--------------������ǿ�ƽ���״̬ʱ���Ž�������������--------------------
		if (!m_bRunFlag)
		{
			if (fabs(m_Direcion.x) > 50.0f || fabs(m_Direcion.z) > 50.0f)
			{
				RotateAndAttack(evt, 0.4f, Ogre::Node::TS_PARENT); //��������
			}
			else  //ֱ��ǰ������
			{
				RotateAndGo(evt, Ogre::Vector3::UNIT_X, m_Direcion, m_Direcion, false, 0.0f, false, Ogre::Node::TS_PARENT);
			}
				
			//���ӹ����ж�
			if (m_Distance <= DRAGON_ATTACK_DISTANCE)
			{
				m_Beam->setEmitting(true);
				m_SoundMgr->PlaySound(m_iJetSd, m_DragonNode, &m_iJetChannel);
			}
			else
				m_Beam->setEmitting(false);
		}
		//-------------�����������ܳɹ��󣬻����ת����״̬����ִ��������δ��롿-------------
		else if (m_Distance < DRAGON_CIRCLE_DISTANCE && m_Distance >= DRAGON_BACK_ATTACK_DIS && m_bRunFlag)
		{
			RotateAndGo(evt, Ogre::Vector3::UNIT_X, m_Direcion, -m_Direcion, true, 20.0f, true, Ogre::Node::TS_PARENT);
		}
		//---------------������������״̬��Զ�빥��Ŀ�꣨��ת����δ��������ִ�У���------------------
		else if (m_Distance < DRAGON_BACK_ATTACK_DIS && m_Distance >= DRAGON_RUNAWAY_DISTANCE && m_bRunFlag)
		{
			RotateAndGo(evt, Ogre::Vector3::UNIT_X, -m_Direcion, -m_Direcion, true, 60.0f, true, Ogre::Node::TS_PARENT);
		}	// if_else_����	
	}
	//-------------------------------����׷��ʱ����ͼ��ͷ���ܡ�------------------------------------------
	else    
	{
		//���ܱ�־����
		m_bRunFlag = true;
		RotateAndGo(evt, Ogre::Vector3::UNIT_X, -m_Direcion, -m_Direcion, true,  60.0f, true, Ogre::Node::TS_PARENT);
		m_Beam->setEmitting(false);
	}
	//���¼�������AI��λ��
	m_Dragon.m_Position = m_DragonNode->getPosition();

	return true;
}

//-----------------------------------------��setPosition()������---------------------------------------
// Desc:��������AI��λ��
//-----------------------------------------------------------------------------------------------------
void Dragon::setPosition(Ogre::Vector3 positon)
{
	m_Dragon.m_Position = positon;
}

//----------------------------------------��setRandPosition()������------------------------------------
// Desc:��������AI�����λ��
//-----------------------------------------------------------------------------------------------------
void Dragon::setRandPosition(Ogre::Real range /* = 2000 */)
{
	m_Dragon.m_Position = Ogre::Vector3(
		rand() % (int)range - range/2, 
		rand() % (int)range - range/2, 
		rand() % (int)range - range/2);
	m_DragonNode->setPosition(m_Dragon.m_Position);
}

//----------------------------------------��setFollowRandPos()������------------------------------------
// Desc:��������AI����ĳһ������λ��
//-----------------------------------------------------------------------------------------------------
void Dragon::setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range /* = 2000 */)
{
	m_Dragon.m_Position = Ogre::Vector3(
		position.x + rand() % (int)range - range / 2,
		position.y + rand() % (int)range - range / 2,
		position.z + rand() % (int)range - range / 2);
	m_DragonNode->setPosition(m_Dragon.m_Position);
}
//-----------------------------------------��BeJetCollDec()������---------------------------------------
// Desc:������AI�ı����乥��������ײ���
//------------------------------------------------------------------------------------------------------
bool Dragon::BeJetCollDec()
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

//-----------------------------------------��JetCollDec()������-----------------------------------------
// Desc:������AI�����乥��������ײ���
//------------------------------------------------------------------------------------------------------
bool Dragon::JecCollDec()
{
	static Ogre::Ray ray;
	static Ogre::Vector3 srcPos;
	if (m_bRunFlag || m_bDead || (m_Distance > DRAGON_ATTACK_DISTANCE) || m_Model->getDieFlag()) return false;
	//����Դ�����߷���
	srcPos = m_DragonNode->getPosition();
	ray = Ogre::Ray(srcPos, m_DragonNode->getOrientation() * Ogre::Vector3::UNIT_X);
	
	m_RaySceneQuery->setRay(ray);
	Ogre::RaySceneQueryResult & result = m_RaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator iter = result.begin();

	//���㹥��Ŀ��λ�õ�x,y,z�����������
	Ogre::Vector3 destPos = m_AttackNode->getPosition();
	
	//���߼����˺����ӻ��м��
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
//------------------------------��ChangePosToWorld()������------------------------------
// Desc:�����ڳ������ڵ������ת��Ϊ���������(ǰ�᣺�������ڵ�pitch��PI/2)
//--------------------------------------------------------------------------------------
inline Ogre::Vector3 Dragon::ChangePosToWorld(const Ogre::Vector3 &pos) const
{
	Ogre::Vector3 src(pos.x, -pos.z, pos.y);
	return src;
}
//-----------------------------------------��BeThroughCollDec()������-----------------------------------
// Desc:�Խ���AI�Ĵ�͸�������б�������ײ���
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
//--------------------------------------��ScaleChange()������-------------------------------------
// Desc:�Ա�����С���в��ϷŴ����С(biggerΪtrue��Ŵ�,falseΪ��С)
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
//---------------------------------��ChooseBeam()������------------------------------
// Desc:ѡ�����ӹ�������Ŵ�0��7
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