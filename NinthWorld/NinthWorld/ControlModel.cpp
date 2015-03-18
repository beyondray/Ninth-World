/*----------------------------------------------------------------
【Cpp文件】：ControlModel.cpp   Created by Beyond Ray,2014年4月
（描述）：一个封装了主模型基本控制系统的类
------------------------------------------------------------------*/
#include "stdafx.h"
#include"ControlModel.h"
#include"GhostdomDragon.h"
#include"Dragon.h"
#include"Sword.h"
//--------------------------------------【ControlModel()函数】------------------------------------------
// Desc:构造函数
//------------------------------------------------------------------------------------------------------
ControlModel::ControlModel(Ogre::Root* root, Ogre::SceneManager* sceneMgr,SoundManager* soundMgr, 
	Ogre::Camera* camera, NinthCEGUI* ninthCEGUI, Ogre::String charName):
m_Root(root),
m_SceneMgr(sceneMgr),
m_SoundMgr(soundMgr),
m_CharName(charName),
m_iShootSd(INVALID_SOUND_INDEX),
m_iShootChannel(INVALID_SOUND_CHANNEL),
m_Camera(camera),
m_CamNode(NULL),
m_Direction(Ogre::Vector3::ZERO), 
m_Model(NULL),
m_ModelNode(NULL),
m_bBeginSoundPlayed(false),
m_bDieSoundPlayed(false),
m_bDieScoreWShow(false),
m_bMRight(false),
m_bMLeft(false),
m_DownDistance(0.0f),
m_rotate(0.0f),
m_lMouseTime(0.0f),
m_FlyFlag(1),
m_CycleIndex(0),
m_ConvInterval(0),
m_RLUpdateIndex(-1),
m_WKeyInterval(KEYDOWN_INTERVAL),
m_HelicTime(HELIC_COOL_TIME),
m_HP(0),
m_rotateSum(0.0f),
m_iPreKillNum(0),
m_fIntervalTime(0.0f),
m_iUnStopKillNum(0),
m_bDead(false),
m_bDieShow(false),
m_bHelicState(false),
m_bSwordMoved(false),
m_bDivineSword4(false),
m_bLGlide(false),
m_bRGlide(false),
m_bBigger(false),
m_bSmaller(false),
m_bHelSkillPer(false),
m_bChangeExistScore(false),
m_bDivSkillPer(false),
m_bJetFirePer(false),
m_limitRad(Ogre::Radian(0.0f)),
m_ChangeSpeed(MOVE_SPEED),
m_LaunchIndex(-1),
m_DivineTime(SWORD_DIVINE_COOL_TIME),
m_NinthCEGUI(ninthCEGUI),
m_iHeartBeatChannel(INVALID_SOUND_CHANNEL),
m_iGameBeginChannel(INVALID_SOUND_CHANNEL),
m_iGameOverChannel(INVALID_SOUND_CHANNEL),
m_iHelicChannel(INVALID_SOUND_CHANNEL),
m_iFanWindsChannel(INVALID_SOUND_CHANNEL),
m_iGlideChannel(INVALID_SOUND_CHANNEL),
m_iKillChannel(INVALID_SOUND_CHANNEL),
m_iClickChannel(INVALID_SOUND_CHANNEL)
{
	m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
	//杀戮声道初始化
	m_iFirstBloodChannel = m_iDoubleKillChannel = m_iKillingSpreeChannel = m_iTripleKillChannel =
		m_iDominatingChannel = m_iUltraKillChannel = m_iUnstoppableChannel = m_iMegaKillChannel =
		m_iWickedSickChannel = m_iMonsterKillChannel = m_iRampageChannel = m_iGodLikeChannel =
		m_iHolyShitChannel = m_iOwnageChannel = m_iKillChannel;
	m_NinthCEGUI->getOkAgainW()->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ControlModel::FileAndRankLUpdate, this));
}
//--------------------------------------【getScore()函数】----------------------------------------------
// Desc:取得已获得的分数
//------------------------------------------------------------------------------------------------------
int ControlModel::getScore() const
{
	return Dragon::ms_iDDieNum + GhostdomDragon::ms_iGDDieNum + Sword::ms_iDieNum;
}

//--------------------------------------【~ControlModel()函数】-----------------------------------------
// Desc:析构函数
//------------------------------------------------------------------------------------------------------
ControlModel::~ControlModel()
{
	m_SceneMgr->destroyQuery(m_RaySceneQuery);

	for (size_t i = 0; i < DIVINE_SWORD_NUM; i++)
	{
		delete m_DivineSword[i];
	}

	m_NinthCEGUI->getOkAgainW()->removeAllEvents();

}

//--------------------------------------【ModelInitialise()函数】---------------------------------------
// Desc:主模型初始化
//------------------------------------------------------------------------------------------------------
void ControlModel::ModelInitialise(const Ogre::String &entityName, const Ogre::String &meshName,
	const Ogre::Vector3 &position, Ogre::Real HPValue)
{
	//-----相关声音初始化------
	m_iShootSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(主)m魔法-火-重.wav"));
	m_iHeartBeatSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(主)ME-心跳.wav"));
	m_iGameBeginSd = m_SoundMgr->CreateSound(Ogre::String("(主)game begin.ogg"));
	m_iGameOverSd = m_SoundMgr->CreateSound(Ogre::String("(主)game over.ogg"));
	m_iHelicSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(主)土龙卷.wav"));
	m_iFanWingsSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(主)煽动翅膀.mp3"));
	m_iGlideSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(主)滑翔声.wav"));
	m_iClickSd = m_SoundMgr->CreateSound(Ogre::String("(N)Click.wav"));
	//创建不死连杀声效
	m_iFirstBloodSd = m_SoundMgr->CreateSound(Ogre::String("(主)FirstBlood.mp3"));                //第一滴血
	m_iKillingSpreeSd = m_SoundMgr->CreateSound(Ogre::String("(主)KillingSpree.mp3"));            //狂欢的杀戮
	m_iDominatingSd = m_SoundMgr->CreateSound(Ogre::String("(主)Dominating.mp3"));                //主控局面
	m_iMegaKillSd = m_SoundMgr->CreateSound(Ogre::String("(主)MegaKill.mp3"));                    //可怕的杀戮
	m_iUnstoppableSd = m_SoundMgr->CreateSound(Ogre::String("(主)Unstoppable.mp3"));              //不可停止
	m_iWickedSickSd = m_SoundMgr->CreateSound(Ogre::String("(主)WickedSick.mp3"));                //变态杀戮
	m_iMonsterKillSd = m_SoundMgr->CreateSound(Ogre::String("(主)MonsterKill.mp3"));              //暴走的杀戮
	m_iGodLikeSd = m_SoundMgr->CreateSound(Ogre::String("(主)GodLike.mp3"));                     //像上帝一般的
	m_iHolyShitSd = m_SoundMgr->CreateSound(Ogre::String("(主)HolyShit.mp3"));                     //神圣的
	//创建时间连杀声效
	m_iDoubleKillSd = m_SoundMgr->CreateSound(Ogre::String("(主)DoubleKill.mp3"));                //双杀
	m_iTripleKillSd = m_SoundMgr->CreateSound(Ogre::String("(主)TripleKill.mp3"));                //三杀
	m_iUltraKillSd = m_SoundMgr->CreateSound(Ogre::String("(主)UltraKill.mp3"));                  //极端的连续杀戮
	m_iRampageSd = m_SoundMgr->CreateSound(Ogre::String("(主)Rampage.mp3"));                      //连杀暴怒
	m_iOwnageSd = m_SoundMgr->CreateSound(Ogre::String("(主)Ownage.mp3"));                         //连杀暴走
	
	//血量赋值
	m_HP = HPValue;

	//创建控制模型和摄像机的主节点
	m_MainNode = m_SceneMgr->getRootSceneNode()->createChildSceneNode();
	m_MainNode->setPosition(position);

	//用模型节点来初始化坐骑模型
	m_ModelNode = m_MainNode->createChildSceneNode();
	m_Model = m_SceneMgr->createEntity(entityName, meshName);
	m_ModelNode->yaw(Ogre::Radian(Ogre::Math::PI / 2), Ogre::Node::TS_LOCAL);
	m_ModelNode->attachObject(m_Model);
	m_Model->setQueryFlags(MAINMODEL_MASK);

	//用模型节点来初始化坐骑上的人物模型
	m_MeiZhi = m_SceneMgr->createEntity("meizhi.MESH.mesh");
	m_YunTianHe = m_SceneMgr->createEntity("yuntianheAppend.MESH.mesh");
	m_Model->attachObjectToBone("Bone_DragonKalecgos_118", m_MeiZhi);
	m_Model->attachObjectToBone("Bone_DragonKalecgos_118", m_YunTianHe);
	m_MeiZhi->setQueryFlags(MAINMODEL_MASK);
	m_YunTianHe->setQueryFlags(MAINMODEL_MASK);

	//绑定摄像机节点
	m_CamNode = m_MainNode->createChildSceneNode("CameraNode");
	m_CamNode->attachObject(m_Camera);

	//绑定监视器
	m_MonitorNode = m_MainNode->createChildSceneNode("MonitorNode");
	m_Monitor = m_SceneMgr->createCamera("Monitor");
	m_Monitor->setPosition(0, 60, -100);
	m_Monitor->lookAt(0, 10, 0);
	m_Monitor->setNearClipDistance(5);
	m_MonitorNode->attachObject(m_Monitor);

	//创建监视器图像
	CreateMonitor();

	//飞行动画
	m_Fly1 = m_Model->getAnimationState("fly1");
	m_Fly1to2 = m_Model->getAnimationState("fly1to2");
	m_Fly2 = m_Model->getAnimationState("fly2");
	m_Fly2to3 = m_Model->getAnimationState("fly2to3");
	m_Fly3 = m_Model->getAnimationState("fly3");
	m_Flying = m_Fly1;

	//创建红色光束粒子系统,究极龙卷风暴,生命恢复特效
	m_RedBeam = m_SceneMgr->createParticleSystem("RedBeam", "Beyond/RedBeam");
	m_ModelNode->attachObject(m_RedBeam);
	//m_Model->attachObjectToBone("Bone_DragonKalecgos_118", m_RedBeam);
	m_RedBeam->setEmitting(false);

	m_Aureola = m_SceneMgr->createParticleSystem("HelicAureola", "Model/Aureola");
	m_ModelNode->attachObject(m_Aureola);
	m_Aureola->setEmitting(false);

	m_RecBlood = m_SceneMgr->createParticleSystem("RecBlood", "Beyond/GreenyNimbus");
	m_ModelNode->attachObject(m_RecBlood);
	m_RecBlood->setEmitting(false);
	m_RecBlood->setKeepParticlesInLocalSpace(true);

	//将所有飞行动画设为开启状态
	m_Fly1->setEnabled(true);
	
	//关闭所有飞行动画的循环状态
	m_Fly1->setLoop(false);
	m_Fly2->setLoop(false);
	m_Fly3->setLoop(false);
	m_Fly1to2->setLoop(false);
	m_Fly2to3->setLoop(false);
	
	//动态数组内存分配，创建非常多的万剑对象,为“万剑归宗”作准备
	
	Sword_Object sword;
	sword.m_Scale = Ogre::Vector3(2, 2, 2);
	for (size_t i = 0; i < DIVINE_SWORD_NUM; i++)
	{
		sword.m_ObjectIndex = 100 + i;
		sword.m_CategoryIndex = i % 16;
		sword.m_Damage = 100.0f;
		sword.m_Defense = 100.0f;
		sword.m_MaxHP = 200.0f;
		sword.m_FlySpeed = 800.0f;
		sword.m_bAttackRotate = true;
		sword.m_bNoStopAttack = false;
		sword.m_bOneAttack = true;
		m_DivineSword[i] = new Sword(m_SceneMgr, m_SoundMgr, this, m_NinthCEGUI);
		m_DivineSword[i]->PuppetSwordInitialise(sword);
		m_DivineSword[i]->getSwordNode()->setVisible(false);
	}

}

//-----------------------------------【frameRenderingQueued()函数】-------------------------------------
// Desc:每帧画面的操作更新
//------------------------------------------------------------------------------------------------------
bool ControlModel::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	//播放游戏开始时的声音
	if (!m_bBeginSoundPlayed)
	{
		m_SoundMgr->PlaySound(m_iGameBeginSd, m_MainNode, &m_iGameBeginChannel);
		m_SoundMgr->PlaySound(m_iFanWingsSd, m_MainNode, &m_iFanWindsChannel, 0.13f);
		m_bBeginSoundPlayed = true;
	}

	//更新声音通道
	m_SoundMgr->FrameStarted(m_MainNode, evt.timeSinceLastFrame);
	
	int killNumSum = (Dragon::ms_iDDieNum + GhostdomDragon::ms_iGDDieNum);
	int killNum = killNumSum - m_iPreKillNum;
	if (killNum!=0)
	{
		m_iPreKillNum = killNumSum;
	}
	//不死连续杀戮的声音(十连杀)
	if (m_iPreKillNum <= 10)
		PlayUnDeadKillSound(killNumSum);
	//十连杀之后,播放短时间内连续击杀音效(六连杀)
	else
		PlayUnStopKillSound(killNum, evt);
	if (m_bDivSkillPer)
	{
		//累计“独孤九剑,万剑归宗"技能开启后所经过的时间
		m_DivineTime += evt.timeSinceLastFrame;
		//“独孤九剑,万剑归宗"冷却计时及技能更新
		if (m_DivineTime <= SWORD_DIVINE_COOL_TIME)
		{
			Ogre::Real coolTime = SWORD_DIVINE_COOL_TIME - m_DivineTime;

			m_NinthCEGUI->getSkillCoolCalW()[5]->setText(Ogre::StringConverter::toString(
				coolTime < 10.0f ? (int)(coolTime * 100) / 100.0f : (int)(coolTime * 10) / 10.0f));
			if (Sword::ms_bDivine4Start)
			{
				m_NinthCEGUI->getSkillCoolCalW()[6]->setText(Ogre::StringConverter::toString(
					coolTime < 10.0f ? (int)(coolTime * 100) / 100.0f : (int)(coolTime * 10) / 10.0f));
			}
		}
		else
		{
			if (Sword::ms_iRotToCoreNum == 9)
			{
				m_NinthCEGUI->getSkillShadeW()[5]->hide();
			}
			m_NinthCEGUI->getSkillCoolCalW()[5]->hide();
			m_NinthCEGUI->getSkillCoolCalW()[6]->hide();
			m_bDivSkillPer = false;
		}
	}
	if (m_bDivineSword4)	//傀儡剑体开始激射
	{
		for (size_t i = 0; i < DIVINE_SWORD_NUM; i++)
			m_DivineSword[i]->PuppetFrameRenderingQueued(evt);
		if (m_bBigger)
		{
			if (ScaleChange(evt, 25.0f, 10.0f, true))
				m_bBigger = false;
		}
	}

	if (m_bSmaller)
	{
		if (ScaleChange(evt, 1.0f, 8.0f, false))
			m_bSmaller = false;
	}

	//得到摄像机相对于起始点的旋转轴和旋转角度
	static Ogre::Quaternion quat;
	static Ogre::Radian rad;
	static Ogre::Vector3 axis, direction;
	quat = m_CamNode->getOrientation();
	quat.ToAngleAxis(rad, axis);

	//当鼠标右键释放，并按下了W,A,S,D其中一个键时，重新转回面向
	
	if (!m_bMRight && m_Direction != Ogre::Vector3::ZERO && rad >= Ogre::Radian(Ogre::Math::PI / 2 * evt.timeSinceLastFrame))
		m_CamNode->rotate(axis, -Ogre::Radian(Ogre::Math::PI / 2 * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
	
	//W.A.S.D
	direction = m_Direction;

	if (m_bHelicState)		//究极螺旋形态
	{		
		if (m_ChangeSpeed <= MOVE_HELIC_LIMITSPEED)
			m_ChangeSpeed += MOVE_HELIC_ACCEL * evt.timeSinceLastFrame;
		m_ModelNode->pitch(Ogre::Radian(40 * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
		m_MainNode->translate(direction * m_ChangeSpeed * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

		if (m_HelicTime >= HELIC_MAX_TIME)
		{
			m_Aureola->setEmitting(false);
			m_bHelicState = false;
			//使其旋转回与摄像机同步状态
			m_ModelNode->setOrientation(m_ModelNode->getInitialOrientation());
			m_ModelNode->yaw(Ogre::Radian(Ogre::Math::PI / 2), Ogre::Node::TS_LOCAL);
			m_rotateSum = 0.0f;
			m_SoundMgr->StopSound(&m_iHelicChannel);
			m_ChangeSpeed = MOVE_LIMITSPEED;
			//究极螺旋技能状态显示消失
			m_NinthCEGUI->hideSkillState(0);
		}
	}
	else	//普通形态
	{
		if (m_ChangeSpeed <= MOVE_LIMITSPEED)
			m_ChangeSpeed += MOVE_ACCELERATION * evt.timeSinceLastFrame;

		if (direction == Ogre::Vector3(0, 0, 0))
			m_ChangeSpeed = MOVE_SPEED;
		m_MainNode->translate(direction * m_ChangeSpeed * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}
	//激射火焰时技能图标更新
	if (m_bJetFirePer)
	{
		if (m_bMLeft)
		{
			m_lMouseTime += evt.timeSinceLastFrame;
			int nowIndex = (int)(m_lMouseTime / 0.07f) % 9;
			if (nowIndex != m_CycleIndex)
			{
				m_NinthCEGUI->getSkillShadeW()[0]->setProperty("Image", "fire" + Ogre::StringConverter::toString(m_CycleIndex + 1));
				m_CycleIndex = nowIndex;
			}
		}
		else
		{
			m_NinthCEGUI->getSkillShadeW()[0]->hide();
			m_bJetFirePer = false;
		}
	}

	//血量及退格血条更新
	m_NinthCEGUI->getSrcBloodNumW()->setText(
		m_CharName != ""?m_CharName + ": "+ Ogre::StringConverter::toString((int)m_HP):
		"Blood: " + Ogre::StringConverter::toString((int)m_HP));
	
	static int ibloodPart = -1;
	int i = 0;
	while (1)
	{
		if (m_HP <= i * MODEL_MAX_BLOOD / 10)
		{
			if (ibloodPart != i)
			{
				m_NinthCEGUI->getSrcBloodW()->setProperty("Image", "purple" + Ogre::StringConverter::toString(i));
				ibloodPart = i;
			}
			else
				break;
		}
		else
		{
			i++;
			if (i > 10)
				break;
		}
	}
	
	//血量过低时，播放心跳声
	if (m_HP <= (MODEL_MAX_BLOOD/4.0f))
		m_SoundMgr->PlaySound(m_iHeartBeatSd, m_MainNode, &m_iHeartBeatChannel);

	//Q.E
	if (m_bLGlide || m_bRGlide)
	{
		m_rotateSum += m_rotate * evt.timeSinceLastFrame;
		if (m_rotateSum > -ROTATE_MAX_LIMIT && m_rotateSum < ROTATE_MAX_LIMIT)
		{
			m_ModelNode->pitch(Ogre::Radian(m_rotate * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
		}
		else if (m_rotateSum <= -ROTATE_MAX_LIMIT)
		{
			m_rotateSum = -ROTATE_MAX_LIMIT;
		}
		else if (m_rotateSum >= ROTATE_MAX_LIMIT)
		{
			m_rotateSum = ROTATE_MAX_LIMIT;
		}
	}
	else
	{
		//释放Q.E时自动转回原方向
		Ogre::Vector3 src = m_ModelNode->getOrientation()*Ogre::Vector3::UNIT_Y;
		Ogre::Quaternion quat = src.getRotationTo(Ogre::Vector3::UNIT_Y);
		Ogre::Radian angle;
		Ogre::Vector3 axis;
		Ogre::Radian resAngle = Ogre::Radian(Ogre::Math::PI / 3 * evt.timeSinceLastFrame);
		quat.ToAngleAxis(angle, axis);
		if (angle > resAngle)
		{
			m_ModelNode->rotate(axis, resAngle, Ogre::Node::TS_PARENT);
			if (axis.dotProduct(Ogre::Vector3::NEGATIVE_UNIT_Z) >= 0)
				m_rotateSum += resAngle.valueRadians();
			else
				m_rotateSum -= resAngle.valueRadians();
		}
		else
		{
			m_ModelNode->rotate(quat, Ogre::Node::TS_PARENT);
			if (axis.dotProduct(Ogre::Vector3::NEGATIVE_UNIT_Z) >= 0)
				m_rotateSum += angle.valueRadians();
			else
				m_rotateSum -= angle.valueRadians();
		}
	}

	//计算上次至此次所流逝的时间
	m_ConvInterval += evt.timeSinceLastFrame;
	m_WKeyInterval += evt.timeSinceLastFrame;
	//拥有螺旋技能权限时执行
	if (m_bHelSkillPer)
	{
		m_HelicTime += evt.timeSinceLastFrame;
		//究极螺旋技能冷却计时及刷新
		if (m_HelicTime <= HELIC_COOL_TIME)
		{
			Ogre::Real coolTime = HELIC_COOL_TIME - m_HelicTime;
			m_NinthCEGUI->getSkillCoolCalW()[1]->setText(Ogre::StringConverter::toString(
				coolTime < 10.0f ? (int)(coolTime * 1000) / 1000.0f : (int)(coolTime * 100) / 100.0f));
		}
		else
		{
			m_NinthCEGUI->getSkillShadeW()[1]->hide();
			m_NinthCEGUI->getSkillCoolCalW()[1]->hide();
			m_bHelSkillPer = false;
		}
	}
	//左右滑翔
	if (m_bLGlide || m_bRGlide)
	{
		m_Fly3->addTime(evt.timeSinceLastFrame * 2);
		if (m_Fly3->hasEnded())
			m_Fly3->setTimePosition(0);
		//播放滑翔声
		if (m_iGlideChannel == INVALID_SOUND_CHANNEL)
			m_SoundMgr->PlaySound(m_iGlideSd, m_MainNode, &m_iGlideChannel);
	}
	else
	{
		//停止滑翔声
		if (m_iGlideChannel != INVALID_SOUND_CHANNEL && m_Flying != m_Fly3)
			m_SoundMgr->StopSound(&m_iGlideChannel);

		//飞行动画的切换
		switch (m_FlyFlag)
		{
		case 1:
			m_Fly1->addTime(evt.timeSinceLastFrame * 1.2f);
			if (m_Fly1->hasEnded())
				m_Fly1->setTimePosition(0);
			break;

		case 2:
			if (m_Fly1->hasEnded())
			{
				m_Fly1->setEnabled(false);
				m_Fly1to2->setEnabled(true);
				m_Flying = m_Fly1to2;
				m_Fly1to2->addTime(evt.timeSinceLastFrame * 3);
			}
			else
			{
				m_Fly1->addTime(evt.timeSinceLastFrame * 1.2f);
			}
			if (m_Fly1to2->hasEnded())
			{
				m_Fly1to2->setEnabled(false);
				m_Fly2->setEnabled(true);
				m_Flying = m_Fly2;
				m_Fly2->addTime(evt.timeSinceLastFrame * 1.2f);
				//播放翅膀煽动声
				m_SoundMgr->PlaySound(m_iFanWingsSd, m_MainNode, &m_iFanWindsChannel, 0.13f);
			}

			if (m_Fly2->hasEnded())
				m_Fly2->setTimePosition(0);

			break;
		case 3:
			if (m_Fly2->hasEnded())
			{
				m_Fly2->setEnabled(false);
				m_Fly2to3->setEnabled(true);
				m_Flying = m_Fly2to3;
				m_Fly2to3->addTime(evt.timeSinceLastFrame * 3);
			}
			else
				m_Fly2->addTime(evt.timeSinceLastFrame * 1.2f);

			if (m_Fly2to3->hasEnded())
			{
				m_Fly2to3->setEnabled(false);
				m_Fly3->setEnabled(true);
				m_Flying = m_Fly3;
				m_Fly3->addTime(evt.timeSinceLastFrame * 2);
				//播放滑翔声
				m_SoundMgr->PlaySound(m_iGlideSd, m_MainNode, &m_iGlideChannel);
			}

			if (m_Fly3->hasEnded())
				m_Fly3->setTimePosition(0);

			break;
		}
	}

	//死亡时直接返回
	if (CheckAndDieBehavior(evt))
	{
		if (m_iHeartBeatChannel != INVALID_SOUND_CHANNEL)
			m_SoundMgr->StopSound(&m_iHeartBeatChannel);
		//角色死亡时播放的声音
		if (!m_bDieSoundPlayed)
		{
			m_SoundMgr->PlaySound(m_iGameOverSd, m_MainNode, &m_iGameOverChannel);
			m_bDieSoundPlayed = true;
		}
		return true;
	}

	return true;
}

//---------------------------------------【keyPressed()函数】-------------------------------------------
// Desc:模型的键位按下操控
//------------------------------------------------------------------------------------------------------
bool ControlModel::keyPressed(const OIS::KeyEvent & arg)
{
	if (arg.key == OIS::KC_R && m_ConvInterval <= COV_INTERVAL_MIN)
		return true;

	switch (arg.key)
	{
	case OIS::KC_W:
		m_Direction.z += -1.0f;
		if (m_WKeyInterval < KEYDOWN_INTERVAL && m_HelicTime >= HELIC_COOL_TIME)
		{
			m_Aureola->setEmitting(true);
			m_bHelicState = true;
			m_bHelSkillPer = true;
			m_HelicTime = 0.0f;
			m_NinthCEGUI->getSkillShadeW()[1]->show();
			m_NinthCEGUI->getSkillCoolCalW()[1]->show();
			//播放土龙卷声音
			m_SoundMgr->PlaySound(m_iHelicSd, m_MainNode, &m_iHelicChannel);
			//显示技能状态图标
			m_NinthCEGUI->showSkillState(0, 1);
		}
		m_WKeyInterval = 0.0f;
		break;

	case OIS::KC_S:
		m_Direction.z += 0.2f;
		break;

	case OIS::KC_A:
		m_Direction.x += -0.45f;
		m_Flying->setEnabled(false);
		m_Fly3->setEnabled(true);
		m_bLGlide = true;
		m_rotate += -ROTATE_RATE;
		//停止翅膀煽动声
		if (m_iFanWindsChannel != INVALID_SOUND_CHANNEL)
			m_SoundMgr->StopSound(&m_iFanWindsChannel);
		break;

	case OIS::KC_D:
		m_Direction.x += 0.45f;
		m_Flying->setEnabled(false);
		m_Fly3->setEnabled(true);
		m_bRGlide = true;
		m_rotate += ROTATE_RATE;
		//停止翅膀煽动声
		if (m_iFanWindsChannel != INVALID_SOUND_CHANNEL)
			m_SoundMgr->StopSound(&m_iFanWindsChannel);
		break;

	case OIS::KC_LSHIFT:
		m_bSwordMoved = true;
		break;
	case OIS::KC_R:
		m_ConvInterval = 0.0f;
		m_FlyFlag++;
		if (m_FlyFlag > 3)
			m_FlyFlag = 1;

		//标志选择重置飞行动画时间点
		switch (m_FlyFlag)
		{
		case 1:
			m_Fly2to3->setEnabled(false);
			m_Fly3->setEnabled(false);
			m_Fly1->setTimePosition(0);
			m_Fly1->setEnabled(true);
			m_Flying = m_Fly1;
			//停止播放滑翔声
			m_SoundMgr->StopSound(&m_iGlideChannel);
			//播放翅膀煽动声
			m_SoundMgr->PlaySound(m_iFanWingsSd, m_MainNode, &m_iFanWindsChannel, 0.13f);
			break;

		case 2:
			m_Fly1to2->setTimePosition(0);
			//停止翅膀煽动声
			m_SoundMgr->StopSound(&m_iFanWindsChannel);
			break;

		case 3:
			m_Fly2to3->setTimePosition(0);
			//停止翅膀煽动声
			m_SoundMgr->StopSound(&m_iFanWindsChannel);
			break;
		}
		break;

	case OIS::KC_F:		//“独孤九剑,万剑归宗“第三阶段准备完毕时，可以启动第四段连续技“万剑归宗”
		if (!Sword::ms_bDivine4Start && Sword::ms_bDivine3Ready)
		{
			Sword::ms_bDivine4Start = true;
			Sword::ms_bDivine3Ready = false;
			Sword::ms_Divine4WaitTime = 0.0f;
			Ogre::FrameEvent evt;
			DivineSword4(evt);
			m_bDivineSword4 = true;
			m_bBigger = true;
			m_NinthCEGUI->getSkillShadeW()[6]->show();
			m_NinthCEGUI->getSkillCoolCalW()[6]->show();
		}
		break;

	case OIS::KC_E:   //调节监视器显示
		if (m_NinthCEGUI->getMonitorFrameW()->isVisible())
		{
			m_NinthCEGUI->getMonitorFrameW()->hide();
		}
		else
		{
			m_NinthCEGUI->getMonitorFrameW()->show();
		}
		break;
	}
	return true;
}

//---------------------------------------【keyReleased()函数】------------------------------------------
// Desc:模型的键位释放操控
//------------------------------------------------------------------------------------------------------
bool ControlModel::keyReleased(const OIS::KeyEvent & arg)
{

		switch (arg.key)
		{
		case OIS::KC_W:
			m_Direction.z -= -1.0f;
			if (m_bHelicState)
			{
				m_Aureola->setEmitting(false);
				m_bHelicState = false;
				//使其旋转回与摄像机同步状态
				m_ModelNode->setOrientation(m_ModelNode->getInitialOrientation());
				m_ModelNode->yaw(Ogre::Radian(Ogre::Math::PI / 2), Ogre::Node::TS_LOCAL);
				m_ModelNode->rotate(m_CamNode->getOrientation(), Ogre::Node::TS_LOCAL);
				m_rotateSum = 0.0f;
				//停止土龙卷声音
				m_SoundMgr->StopSound(&m_iHelicChannel);
				//螺旋技能状态消失
				m_NinthCEGUI->hideSkillState(0);
			}
			break;

		case OIS::KC_S:
			m_Direction.z -= 0.2f;
			break;

		case OIS::KC_A:
			m_Direction.x -= -0.45f;
			if (!m_bRGlide)
			{
				m_Flying->setEnabled(true);
				m_Fly3->setEnabled(false);
			}
			m_bLGlide = false;
			m_rotate -= -ROTATE_RATE;
			//播放翅膀煽动声
			if (m_Flying != m_Fly3 && !m_bRGlide && m_iFanWindsChannel == INVALID_SOUND_CHANNEL)
				m_SoundMgr->PlaySound(m_iFanWingsSd, m_MainNode, &m_iFanWindsChannel, 0.13f);
			break;

		case OIS::KC_D:
			m_Direction.x -= 0.45f;
			if (!m_bLGlide)
			{
				m_Flying->setEnabled(true);
				m_Fly3->setEnabled(false);
			}
			m_bRGlide = false;
			m_rotate -= ROTATE_RATE;
			//播放翅膀煽动声
			if (m_Flying!=m_Fly3 && !m_bLGlide && m_iFanWindsChannel == INVALID_SOUND_CHANNEL)
				m_SoundMgr->PlaySound(m_iFanWingsSd, m_MainNode, &m_iFanWindsChannel, 0.13f);
			break;
			
		case OIS::KC_LSHIFT:
			m_bSwordMoved = false;
			break;
		}

	return true;
}

//---------------------------------------【mousePressed()函数】-----------------------------------------
// Desc:模型的鼠标点击操控
//------------------------------------------------------------------------------------------------------
bool ControlModel::mousePressed(const OIS::MouseEvent &arg, const OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
	{
		m_bMRight = true;
		//螺旋剑技能的手工与自动切换
		if (Sword::ms_bHelSwordState)
		{
			if (!Sword::ms_bHelSwordManual)
			{
				Sword::ms_HelSCenCore->_setDerivedPosition(Sword::ms_HelicalCore->_getDerivedPosition());
				Sword::ms_HelicalCore->setPosition(0, 0, 0);
			}
			Sword::ms_bHelSwordManual = !Sword::ms_bHelSwordManual;
		}
	}

	if (id == OIS::MB_Left)
	{
		m_NinthCEGUI->getSkillShadeW()[0]->show();
		m_bMLeft = true;
		m_bJetFirePer = true;
		m_RedBeam->setEmitting(true);
		m_SoundMgr->PlaySound(m_iShootSd, m_MainNode, &m_iShootChannel);
	}
	return true;
}

//---------------------------------------【mouseReleased()函数】----------------------------------------
// Desc:模型的鼠标释放操控
//------------------------------------------------------------------------------------------------------
bool ControlModel::mouseReleased(const OIS::MouseEvent &arg, const OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
		m_bMRight = false;

	if (id == OIS::MB_Left)
	{
		m_NinthCEGUI->getSkillShadeW()[0]->hide();
		m_bMLeft = false;
		m_lMouseTime = 0.0f;
		m_RedBeam->setEmitting(false);
		m_SoundMgr->StopSound(&m_iShootChannel);
	}

	return true;
}

//---------------------------------------【mouseMoved()函数】--------------------------------------------
// Desc:模型的鼠标移动操控
//------------------------------------------------------------------------------------------------------
bool ControlModel::mouseMoved(const OIS::MouseEvent & arg)
{
	if (!m_bSwordMoved)
	{
		//X,Y旋转
		m_MainNode->yaw(Ogre::Radian(-arg.state.X.rel * ROTATE_SPEED), Ogre::Node::TS_LOCAL);
		m_MainNode->pitch(Ogre::Radian(-arg.state.Y.rel * ROTATE_SPEED), Ogre::Node::TS_LOCAL);
		m_limitRad += Ogre::Radian(0.8f * arg.state.Y.rel * ROTATE_SPEED);

	}
	
	if (!(m_bSwordMoved && Sword::ms_bHelSwordState))
	{
		//Z缩放
		Ogre::Vector3 dir = m_Camera->getPosition();
		dir.normalise();
		m_Camera->setPosition(m_Camera->getPosition() + (Ogre::Real)arg.state.Z.rel * dir * 0.01f);
	}

	//模型查看旋转
	if (m_bMRight)
	{
		m_CamNode->yaw(Ogre::Radian(-arg.state.X.rel * ROTATE_SPEED), Ogre::Node::TS_LOCAL);
		m_CamNode->pitch(Ogre::Radian(-arg.state.Y.rel * ROTATE_SPEED), Ogre::Node::TS_LOCAL);
	}
	return true;
}

//---------------------------------------【getRMouseDown()函数】------------------------------------
// Desc:得到鼠标右键按下状态
//--------------------------------------------------------------------------------------------------
bool ControlModel::getRMouseDown() const
{
	return m_bMRight;
}

//---------------------------------------【getLMouseDown()函数】------------------------------------
// Desc:得到鼠标右键按下状态
//--------------------------------------------------------------------------------------------------
bool ControlModel::getLMouseDown() const
{
	return m_bMLeft;
}

//------------------------------------【CheckAndDieBehavivor()函数】--------------------------------
// Desc://检测死亡标志,若死亡则进行死亡表现(返回值true:死亡并表现成功,false:存活)
//--------------------------------------------------------------------------------------------------
bool ControlModel::CheckAndDieBehavior(const Ogre::FrameEvent & evt)
{
	if (m_bDead)
	{
		//设置雷龙AI的朝向
		Ogre::Vector3 src = m_MainNode->getOrientation()* Ogre::Vector3::NEGATIVE_UNIT_Y;

		//雷龙AI的-Z轴到重力掉落方向的旋转
		static Ogre::Radian rad;
		static Ogre::Vector3 axis, x, y, z;
		Ogre::Quaternion quat = src.getRotationTo(Ogre::Vector3::UNIT_Y);
		quat.ToAngleAxis(rad, axis);
		if (rad >= Ogre::Radian(Ogre::Math::PI / 100.0f))
			m_MainNode->rotate(axis, Ogre::Radian(Ogre::Math::PI / 100.0f), Ogre::Node::TS_PARENT);
		else
		{
			//死亡时做惯性旋转
			m_CamNode->yaw(Ogre::Radian(evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
			RoateAndDieRotate(evt, Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Y, true, 30.0f, 10.0f, Ogre::Node::TS_PARENT);
			if (!m_bDieScoreWShow)
			{
				m_NinthCEGUI->getSrcDieScoreW()->setVisible(true);
				CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
				m_NinthCEGUI->getOneAgainW()->activate();
				m_NinthCEGUI->getReturnMainW()->activate();
				m_NinthCEGUI->getQuitW()->activate();
				m_bDieScoreWShow = true;
			}
		}

		//向目标方向靠近移动
		m_DownDistance += 10 * MOVE_SPEED * evt.timeSinceLastFrame;
		m_MainNode->translate(Ogre::Vector3::NEGATIVE_UNIT_Y * 10 * MOVE_SPEED * evt.timeSinceLastFrame,
			Ogre::Node::TS_PARENT);
		//死亡显示
		if (m_DownDistance >= 80.0f && !m_bDieShow)
		{		
			//若是键位没有释放，人工调节，避免引起震动
			if (m_Direction.z <= -1.0f)
			{
				OIS::KeyEvent arg(NULL, OIS::KC_W, 0);
				keyReleased(arg);
			}
			else if (m_Direction.z >= 0.2f)
			{
				OIS::KeyEvent arg(NULL, OIS::KC_S, 0);
				keyReleased(arg);
			}

			if (m_Direction.x <= -0.45f)
			{
				OIS::KeyEvent arg(NULL, OIS::KC_A, 0);
				keyReleased(arg);
			}
			else if (m_Direction.x >= 0.45f)
			{
				OIS::KeyEvent arg(NULL, OIS::KC_D, 0);
				keyReleased(arg);
			}
			//技能栏设置为默认状态
			for (size_t i = 0; i < 8; i++)
			{
				m_NinthCEGUI->getSkillShadeW()[i]->hide();
				if (i != 0 && i!= 2)
					m_NinthCEGUI->getSkillCoolCalW()[i]->hide();
			}
			m_NinthCEGUI->getSkillShadeW()[6]->show();

			//进行分数数据读取
			std::ifstream infile("BeyondData.data");
			int score = getScore();	
			size_t i = 0;
			while (!infile.eof())
			{
				if (!(infile >> m_ExistName[i]) || !(infile >> m_ExistScore[i]))
					break;
				i++;
				if (i >= 5)
					break;
			}
			for (size_t j = i; j < 5; j++)
			{
				m_ExistName[j] = "null";
				m_ExistScore[j] = 0;
			}
			infile.close();

			//更改鼠标指针
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseArrow");

			//死亡计分板显示
			DieShowText(score, m_ExistScore);

			//【文件系统及排行榜更新】
			//将现有分数与已存在分数对比，高出则刷掉
			for (size_t i = 0; i < 5; i++)
			{
				if (score >= m_ExistScore[i])
				{
					size_t j = 4;
					for (; j > i; j--)
					{
						m_ExistScore[j] = m_ExistScore[j - 1];
						m_ExistName[j] = m_ExistName[j - 1];
					}
					m_ExistScore[j] = score;
					m_RLUpdateIndex = j;
					m_bChangeExistScore = true;
					break;
				}
			}

			//如果进入排行榜，则显示姓名编辑框
			if (m_bChangeExistScore)
				m_NinthCEGUI->getRNameEditW()->show();
			else
				m_NinthCEGUI->getRNameEditW()->hide();

			m_bDieShow = true;
		}
		return true;
	}
	return false;
}

//------------------------------【DivineSword4()函数】-----------------------------------
// Desc:集齐九剑灵发动之超强技--------独孤九剑,万剑归宗(第四段)之万剑归宗
//---------------------------------------------------------------------------------------
bool ControlModel::DivineSword4(const Ogre::FrameEvent & evt)
{
	for (size_t i = 0; i < DIVINE_SWORD_NUM; i++)
	{
		Ogre::Vector3 src = Ogre::Vector3(0, DIVINE_RISE_DIS, 0);
		m_DivineSword[i]->setFollowRandCylinderPos(src, DIVINE_CYLINDER_FIELD_SIZE / 2, DIVINE_CYLINDER_FIELD_SIZE);
		Ogre::Vector3 dirGo = Ogre::Vector3(DIVINE_MOVE_FIELD_DIS * Ogre::Math::Cos((Ogre::Real)rand()),
			 - m_DivineSword[i]->getPosition().y, DIVINE_MOVE_FIELD_DIS * Ogre::Math::Sin((Ogre::Real)rand()));
		dirGo.normalise();
		m_DivineSword[i]->RotateAndStop(evt, Ogre::Vector3::NEGATIVE_UNIT_Z, dirGo, false, 0.0f, Ogre::Node::TS_PARENT);
	}
	return true;
}
//------------------------------【PlayUnDeadKillSound()函数】----------------------------------
// Desc:根据杀戮的数量来播放杀戮的声音
//---------------------------------------------------------------------------------------------
bool ControlModel::PlayUnDeadKillSound(int killNum)
{
	FMOD::Channel* soundChannel = NULL;
	switch (killNum)
	{
	case 1:
		if (m_iKillChannel == INVALID_SOUND_CHANNEL)
		{
			m_SoundMgr->PlaySound(m_iFirstBloodSd, m_MainNode, &m_iFirstBloodChannel);
			m_iKillChannel = m_iFirstBloodChannel;
		}
		break;
	case 2:
		if (m_iKillChannel == m_iFirstBloodChannel && m_iKillChannel != m_iDoubleKillChannel)
		{
			m_SoundMgr->PlaySound(m_iDoubleKillSd, m_MainNode, &m_iDoubleKillChannel);
			m_iKillChannel = m_iDoubleKillChannel;
		}
		break;
	case 3:
		if (m_iKillChannel == m_iDoubleKillChannel && m_iKillChannel != m_iKillingSpreeChannel)
		{
			m_SoundMgr->PlaySound(m_iKillingSpreeSd, m_MainNode, &m_iKillingSpreeChannel);
			m_iKillChannel = m_iKillingSpreeChannel;
		}
		if (!m_SoundMgr->isPlaying(&m_iKillingSpreeChannel, soundChannel) && m_iKillChannel != m_iTripleKillChannel)
		{
			m_SoundMgr->PlaySound(m_iTripleKillSd, m_MainNode, &m_iTripleKillChannel);
			m_iKillChannel = m_iTripleKillChannel;
		}
		break;
	case 4:
		if ((m_iKillChannel == m_iKillingSpreeChannel || m_iKillChannel == m_iTripleKillChannel) 
			&& m_iKillChannel != m_iDominatingChannel)
		{
			m_SoundMgr->PlaySound(m_iDominatingSd, m_MainNode, &m_iDominatingChannel);
			m_iKillChannel = m_iDominatingChannel;
		}
		if (!m_SoundMgr->isPlaying(&m_iDominatingChannel, soundChannel) && m_iKillChannel != m_iUltraKillChannel)
		{
			m_SoundMgr->PlaySound(m_iUltraKillSd, m_MainNode, &m_iUltraKillChannel);
			m_iKillChannel = m_iUltraKillChannel;
		}
		break;
	case 5:
		if (m_iKillChannel == m_iDominatingChannel || m_iKillChannel == m_iUltraKillChannel
			&& m_iKillChannel != m_iRampageChannel)
		{
			m_SoundMgr->PlaySound(m_iRampageSd, m_MainNode, &m_iRampageChannel);
			m_iKillChannel = m_iRampageChannel;
		}
		if (!m_SoundMgr->isPlaying(&m_iRampageChannel, soundChannel) && m_iKillChannel != m_iMegaKillChannel)
		{
			m_SoundMgr->PlaySound(m_iMegaKillSd, m_MainNode, &m_iMegaKillChannel);
			m_iKillChannel = m_iMegaKillChannel;
		}
		break;
	case 6:
		if (m_iKillChannel == m_iRampageChannel || m_iKillChannel == m_iMegaKillChannel
			&& m_iKillChannel != m_iUnstoppableChannel)
		{
			m_SoundMgr->PlaySound(m_iUnstoppableSd, m_MainNode, &m_iUnstoppableChannel);
			m_iKillChannel = m_iUnstoppableChannel;
		}
		if (!m_SoundMgr->isPlaying(&m_iUnstoppableChannel, soundChannel) && m_iKillChannel != m_iOwnageChannel)
		{
			m_SoundMgr->PlaySound(m_iOwnageSd, m_MainNode, &m_iOwnageChannel);
			m_iKillChannel = m_iOwnageChannel;
		}
		break;
	case 7:
		if (m_iKillChannel == m_iUnstoppableChannel || m_iKillChannel == m_iOwnageChannel
			&& m_iKillChannel != m_iWickedSickChannel)
		{
			m_SoundMgr->PlaySound(m_iWickedSickSd, m_MainNode, &m_iWickedSickChannel);
			m_iKillChannel = m_iWickedSickChannel;
		}
		break;
	case 8:
		if (m_iKillChannel == m_iWickedSickChannel && m_iKillChannel != m_iMonsterKillChannel)
		{
			m_SoundMgr->PlaySound(m_iMonsterKillSd, m_MainNode, &m_iMonsterKillChannel);
			m_iKillChannel = m_iMonsterKillChannel;
		}
		break;
	case 9:
		if (m_iKillChannel == m_iMonsterKillChannel && m_iKillChannel != m_iGodLikeChannel)
		{
			m_SoundMgr->PlaySound(m_iGodLikeSd, m_MainNode, &m_iGodLikeSd);
			m_iKillChannel = m_iGodLikeChannel;
		}
		break;
	case 10:
		if (m_iKillChannel == m_iGodLikeChannel && m_iKillChannel != m_iHolyShitChannel)
		{
			m_SoundMgr->PlaySound(m_iHolyShitSd, m_MainNode, &m_iHolyShitChannel);
			m_iKillChannel = m_iHolyShitChannel;
		}
		break;
	default:
		break;
	}
	return true;
}
//------------------------------【PlayUnStopKillSound()函数】----------------------------------
// Desc:根据段时间内连续杀戮的数量来播放杀戮的声音
//---------------------------------------------------------------------------------------------
bool ControlModel::PlayUnStopKillSound(int killNum, const Ogre::FrameEvent&evt)
{
	m_fIntervalTime += evt.timeSinceLastFrame;
	if (killNum == 0)
		return false;
	if (m_fIntervalTime >= 5.0f && m_iUnStopKillNum != 0)
	{
		m_iUnStopKillNum = 0;
	}
	m_iUnStopKillNum += killNum;
	int tempUnStopKillNum = m_iUnStopKillNum -killNum + 1;
	for (;;)
	{
		switch (tempUnStopKillNum)
		{
		case 1:
			m_iKillChannel = INVALID_SOUND_CHANNEL;
			m_fIntervalTime = 0.0f;
			if (killNum == 1)
				break;
			else
				killNum--;
		case 2:
			if (m_iKillChannel == INVALID_SOUND_CHANNEL)
			{
				m_SoundMgr->PlaySound(m_iDoubleKillSd, m_MainNode, &m_iDoubleKillChannel);
				m_iKillChannel = m_iDoubleKillChannel;
			}
			m_fIntervalTime = 0.0f;
			if (killNum == 1)
				break;
			else
				killNum--;
		case 3:
			if (m_iKillChannel == m_iDoubleKillChannel)
			{
				m_SoundMgr->PlaySound(m_iTripleKillSd, m_MainNode, &m_iTripleKillChannel);
				m_iKillChannel = m_iTripleKillChannel;
			}
			m_fIntervalTime = 0.0f;
			if (killNum == 1)
				break;
			else
				killNum--;
		case 4:
			if (m_iKillChannel == m_iTripleKillChannel)
			{
				m_SoundMgr->PlaySound(m_iUltraKillSd, m_MainNode, &m_iUltraKillChannel);
				m_iKillChannel = m_iUltraKillChannel;
			}
			m_fIntervalTime = 0.0f;
			if (killNum == 1)
				break;
			else
				killNum--;
		case 5:
			if (m_iKillChannel == m_iUltraKillChannel)
			{
				m_SoundMgr->PlaySound(m_iMonsterKillSd, m_MainNode, &m_iMonsterKillChannel);
				m_iKillChannel = m_iMonsterKillChannel;
			}
			m_fIntervalTime = 0.0f;
			if (killNum == 1)
				break;
			else
				killNum--;
		case 6:
			if (m_iKillChannel == m_iMonsterKillChannel)
			{
				m_SoundMgr->PlaySound(m_iOwnageSd, m_MainNode, &m_iOwnageChannel);
				m_iKillChannel = m_iOwnageChannel;
			}
			m_fIntervalTime = 0.0f;
			if (killNum == 1)
				break;
			else
				killNum--;
		default:
			if ((tempUnStopKillNum % 2) != 0)
			{
				if (m_iKillChannel == m_iOwnageChannel)
				{
					m_SoundMgr->PlaySound(m_iMonsterKillSd, m_MainNode, &m_iMonsterKillChannel);
					m_iKillChannel = m_iMonsterKillChannel;
				}
			}
			else
			{
				if (m_iKillChannel == m_iMonsterKillChannel)
				{
					m_SoundMgr->PlaySound(m_iOwnageSd, m_MainNode, &m_iOwnageChannel);
					m_iKillChannel = m_iOwnageChannel;
				}
			}
			m_fIntervalTime = 0.0f;
			if (killNum == 1)
				break;
			else
			{
				tempUnStopKillNum++;
				killNum--;
			}
		}

		if (killNum == 1)
			return true;
	}
}

//-------------------------------------【RotateAndDieRotate()函数】---------------------------------------
// Desc:调转面向一个方向，在决定动作是否分解后，朝着目标方向前进(始终背对Ogre::Vector3::UNIT_Y)
//--------------------------------------------------------------------------------------------------------
void ControlModel::RoateAndDieRotate(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
	const Ogre::Vector3 & dirRotate, bool rotateResolve /* = true */, float rotatePre /* = 30.0f */,
	float dieRoateSpeed /* = 10.0f */, Ogre::Node::TransformSpace relativeTo /* = Ogre::Node::TS_PARENT */)
{
	//设置雷龙AI的朝向
	Ogre::Vector3 src = m_ModelNode->getOrientation()* orientation;

	//雷龙AI的朝向到目标面向方向的旋转
	Ogre::Quaternion quat = src.getRotationTo(dirRotate);
	if (rotateResolve)
	{
		static Ogre::Radian rad;
		static Ogre::Vector3 axis;
		quat.ToAngleAxis(rad, axis);
		if (rad >= Ogre::Radian(Ogre::Math::PI / 30.0f))
			m_ModelNode->rotate(axis, Ogre::Radian(Ogre::Math::PI / rotatePre), relativeTo);
		else
		{
			//死亡时做惯性旋转
			m_ModelNode->pitch(Ogre::Radian(dieRoateSpeed * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
		}
	}
	else
	{
		m_ModelNode->rotate(quat, relativeTo);
	}

	//到目标方向的前进
	Ogre::Vector3 dir = dirRotate;
	dir.normalise();
	m_ModelNode->translate(dir * MOVE_SPEED* 0.17f * evt.timeSinceLastFrame, relativeTo);
}
//---------------------------------【DieShowText()函数】---------------------------------
// Desc:死亡文字显示
//---------------------------------------------------------------------------------------
void ControlModel::DieShowText(int score, int * existScore)
{
	if (score >= existScore[0])
	{
		m_NinthCEGUI->getSrcDieScoreW()->setText(
			"     Congratulations you get " + Ogre::StringConverter::toString(score) + " points,\n" +
			"as the first success.You are super god    \n" +
			"in this world, please enter your name:    ");
	}
	else if (score >= existScore[1])
	{
		m_NinthCEGUI->getSrcDieScoreW()->setText(
			"     Congratulations you get " + Ogre::StringConverter::toString(score) + " points,\n" +
			"won the second prize.You are second \n" +
			"only to the existence of god in the   \n" +
			"world, please enter your name:            ");
	}
	else if (score >= existScore[2])
	{
		m_NinthCEGUI->getSrcDieScoreW()->setText(
			"     Congratulations you get " + Ogre::StringConverter::toString(score) + " points,\n" +
			"won the third place.You are belong to\n" +
			"the existence of the strong in this     \n" +
			"world, please enter your name:           ");
	}
	else if (score >= existScore[3])
	{
		m_NinthCEGUI->getSrcDieScoreW()->setText(
			"     Congratulations you get " + Ogre::StringConverter::toString(score) + " points,\n" +
			"won the fourth place.Your power in the\n" +
			"world is quite good, please enter your  \n" +
			"name:                                                       ");
	}
	else if (score >= existScore[4])
	{
		m_NinthCEGUI->getSrcDieScoreW()->setText(
			"     Congratulations you get " + Ogre::StringConverter::toString(score) + " points,\n" +
			"won the fifth place.You just get the  \n" +
			"essence of martial arts, please enter\n" +
			"your name:                                          ");
	}
	else
	{
		m_NinthCEGUI->getSrcDieScoreW()->setText(
			"        I'm sorry, your score of " + Ogre::StringConverter::toString(score) + " points\n" +
			"is so bad, and even have no qualifications\n" +
			"to see god, please try again later.            ");
	}
}
//---------------------------------【FileAndRankLUpdate()函数】---------------------------------
// Desc:文件数据及排行榜数据更新
//----------------------------------------------------------------------------------------------
bool ControlModel::FileAndRankLUpdate(const CEGUI::EventArgs & arg)
{
	//播放点击音效
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	m_NinthCEGUI->getNameAffirmW()->hide();
	m_NinthCEGUI->getOkW()->hide();
	//已存在分数改变，则重新输入
	m_ExistName[m_RLUpdateIndex] = m_NinthCEGUI->getRListName(); //刷新姓名
	m_NinthCEGUI->getRNameEditW()->setText("");
	m_NinthCEGUI->getRNameEditW()->hide();

	//将分数输出到文件
	if (m_bChangeExistScore)
	{
		std::ofstream outfile("BeyondData.data");
		for (size_t i = 0; i < 5; i++)
		{
			outfile << m_ExistName[i];
			outfile << " ";
			outfile << m_ExistScore[i];
			outfile << " ";
		}
		outfile.close();
	}
	//更新排行榜
	for (size_t i = 0; i < 5; i++)
	{
		Ogre::String postifix;
		switch (i)
		{
		case 0:
			postifix = "st";
			break;
		case 1:
			postifix = "nd";
			break;
		case 2:
			postifix = "rd";
			break;
		case 3:
		case 4:
			postifix = "th";
			break;
		}
		if (m_ExistName[i] != "null")
		{
			Ogre::String tempNum = Ogre::StringConverter::toString(i + 1);
			m_NinthCEGUI->getRewardingTextW()[i]->setText(tempNum + postifix + ",          " + m_ExistName[i] + ","); 
			m_NinthCEGUI->getRewardingTextW()[i+5]->setText("Scores: "+ Ogre::StringConverter::toString(m_ExistScore[i]));
		}
	}
	return true;
}
//--------------------------------------【ScaleChange()函数】-------------------------------------
// Desc:对比例大小进行不断放大或缩小(bigger为true则放大,false为缩小)
//------------------------------------------------------------------------------------------------
bool ControlModel::ScaleChange(const Ogre::FrameEvent &evt, float limitScale,
	float changeRate /* = 3.0f */, bool bigger /* = true */)
{
	Ogre::Vector3 scale = m_MainNode->getScale();
	if (bigger)
	{
		if (scale.x <= limitScale)
		{
			m_MainNode->setScale(
				scale.x + changeRate * evt.timeSinceLastFrame,
				scale.y + changeRate * evt.timeSinceLastFrame,
				scale.z + changeRate * evt.timeSinceLastFrame);
			Ogre::Vector3 pos = m_Camera->getPosition();
			m_Camera->setPosition(0,
				pos.y + 45 * changeRate * evt.timeSinceLastFrame,
				pos.z + 90 * changeRate * evt.timeSinceLastFrame);
			return false;
		}
	}
	else
	{
		if (scale.x >= limitScale)
		{
			m_MainNode->setScale(
				scale.x - changeRate * evt.timeSinceLastFrame,
				scale.y - changeRate * evt.timeSinceLastFrame,
				scale.z - changeRate * evt.timeSinceLastFrame);
			Ogre::Vector3 pos = m_Camera->getPosition();
			m_Camera->setPosition(0,
				pos.y - 45 * changeRate * evt.timeSinceLastFrame,
				pos.z - 90 * changeRate * evt.timeSinceLastFrame);
			return false;
		}
	}
	return true;
}

//--------------------------------【CreateMonitor()函数】-------------------------------
// Desc:创建监视器
//--------------------------------------------------------------------------------------
void ControlModel::CreateMonitor()
{
	//纹理图
	Ogre::TexturePtr tex = m_Root->getTextureManager()->getSingleton().createManual(
		"RTT",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		512,
		512,
		0,
		Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);
	
	//可渲染纹理目标
	Ogre::RenderTexture* renderTex = tex->getBuffer()->getRenderTarget();

	
	//监视器视口
	Ogre::Viewport* moniView = renderTex->addViewport(m_Monitor);
	moniView->setOverlaysEnabled(false);
	moniView->setClearEveryFrame(true);
	moniView->setBackgroundColour(Ogre::ColourValue::Black);
	
	//CEGUI图片纹理	
	CEGUI::Texture & guiTex = MyCEGUISystem::getSingletonPtr()->getRenderer()->createTexture("guiTex", tex);
	
	//CEGUI图像
	CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), guiTex.getOriginalDataSize());
	CEGUI::BasicImage* image = (CEGUI::BasicImage*)(&CEGUI::ImageManager::getSingleton().create("BasicImage", "MoniImage"));
	image->setArea(rect);
	image->setTexture(&guiTex);

	//添加到监视器窗口
	m_NinthCEGUI->getMonitorW()->setProperty("Image", "MoniImage");
}