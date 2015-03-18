/*------------------------------------------------------------------
【头文件】：KnotAndFish.h       Created by Beyond Ray,2014年4月
（描述）：一个封装了纽环和旋转环绕的鱼的场景的类
--------------------------------------------------------------------*/

#ifndef	H_KnotAndFish
#define H_KnotAndFish
using namespace Ogre;

#include "ControlModel.h"

#define  KEEP_RAIN_MAX_TIME 60.0f   //下雨的最大持续时间
#define  RAIN_COOL_TIME 180.0f      //下雨的冷却时间
#define  REC_BLOOD_PER_SECOND 25    //下雨时每秒种回复的血量
#define  KNOT_MASK 1<<4				//纽环查询掩码
#define  SPHERE_MASK 1<<5			//球点查询掩码
class KnotAndFish
{
public:
	KnotAndFish(Ogre::Root *root, Ogre::SceneManager *sceneMgr, SoundManager* soundMgr, ControlModel* model, 
		NinthCEGUI* ninthCEGUI) :m_NinthCEGUI(ninthCEGUI),m_Model(model), m_AttackNode(model->getMainNode()),
		m_SoundMgr(soundMgr), mFishNode0(NULL), mFishNode1(NULL), mFishNode2(NULL), mFishSwim0(NULL), mFishSwim1(NULL), 
		mFishSwim2(NULL), m_bRaining(false), m_bRecBloodState(false), m_bRainSkillPer(false),
		m_fKeepRainTime(RAIN_COOL_TIME),m_iRainChannel(INVALID_SOUND_CHANNEL), m_iBlisterChannel(INVALID_SOUND_CHANNEL),
		m_iReCoverChannel(INVALID_SOUND_CHANNEL)
	{
		m_SceneMgr = sceneMgr; 
		m_Root = root;
		m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
		m_RaySceneQuery->setSortByDistance(true);
	}
	virtual ~KnotAndFish(){ m_SceneMgr->destroyQuery(m_RaySceneQuery); };
	void initialise(float x, float y, float z);
	void initialise(Ogre::Vector3 vec);
	bool frameRenderingQueued(const FrameEvent & evt);
	bool BeJetCollDec(const FrameEvent & evt);                 //纽环中心被喷射的碰撞检测
	bool RainThroughCollDec(const Ogre::FrameEvent & evt);     //雨穿过主角的碰撞检测

protected:
	ControlModel* m_Model;           //主模型
	Ogre::SceneNode* m_AttackNode;   //主模型节点
	Ogre::RaySceneQuery* m_RaySceneQuery; //射线场景查询
	Ogre::SphereSceneQuery* m_SphereSceneQuery; //球面场景查询
	NinthCEGUI* m_NinthCEGUI;       //NinthCEGUI界面库
	float m_fKeepRainTime;          //下雨的持续时间
	bool m_bRaining;                //正在下雨时为真
	bool m_bRecBloodState;          //回复生命状态
	bool m_bRainSkillPer;           //下雨技能权限

	SoundManager* m_SoundMgr;       //声音管理器接口
	int m_iRainSd;                  //雨的声音
	int m_iRainChannel;
	int m_iBlisterSd;               //水泡的声音
	int m_iBlisterChannel;
	int m_iReCoverSd;               //恢复的声音
	int m_iReCoverChannel;
	SceneNode* mKnotNode;			//纽环的场景节点

	SceneNode* mFishNode0;
	SceneNode* mFishNode1;
	SceneNode* mFishNode2;			//三条鱼的场景节点
	
	AnimationState* mFishSwim0;
	AnimationState* mFishSwim1;
	AnimationState* mFishSwim2;		//三条鱼的动画

	Root *m_Root;
	SceneManager* m_SceneMgr;		//根和场景管理

	Vector3 mPosition;				//纽环和鱼场景在世界的位置

	Ogre::ParticleSystem* m_Rain;    //雨的粒子特效
};

#endif