/*-------------------------------------------------------------
【头文件】：ControlModel.h   Created by Beyond Ray,2014年4月
（描述）：一个封装了主模型基本控制系统的类
---------------------------------------------------------------*/

#ifndef H_ControlModel
#define H_ControlModel

#include"NinthCEGUI.h"
#include"SoundManager.h"
#include"MeshAndRayCast.h"
#include"fstream"

#define MODEL_MAX_BLOOD 3000.0f //控制模型的最大血量
#define MODEL_DAMAGE 200.0f     //模型的输出伤害
#define ROTATE_SPEED 0.005f		//摄像机旋转视角速度
#define ROTATE_RATE 1	        //模型倾斜旋转的比率
#define ROTATE_MAX_LIMIT 0.3f   //模型倾斜旋转的最大限度
#define COV_INTERVAL_MIN 1.0f	//飞行动画切换之间的最小允许时间间隔
#define MAINMODEL_MASK 1<<0		//主模型遮罩掩码查询
#define INVICIBLE_MASK 1<<9     //表示无敌状态的遮罩掩码

#define MOVE_SPEED  270.0f		 //摄像机移动速度
#define MOVE_ACCELERATION  5.0f  //模型的移动加速度
#define MOVE_LIMITSPEED	  300.0f //模型的最大限制移动速度

#define KEYDOWN_INTERVAL  0.2f    //连续按键有效时间
#define MOVE_HELIC_ACCEL  50.0f   //究极螺旋形态时的加速度
#define MOVE_HELIC_LIMITSPEED 500.0f  //究极螺旋形态时的移动速度 
#define HELIC_MAX_TIME	10.0f     //究极螺旋化最长持续时间
#define HELIC_COOL_TIME 15.0f    //究极螺旋化冷却时间

#define DIVINE_SWORD_NUM  200	  //"万剑归宗"技能所使用剑的数量
#define DIVINE_RISE_DIS 100		  //"万剑归宗"技能所使用剑的初始领域中心在纵向到主模型的偏离距离 
#define DIVINE_CYLINDER_FIELD_SIZE 50//"万剑归宗"技能剑体出现时初始圆柱空间的领域高度和直径
#define DIVINE_MOVE_FIELD_DIS 45  //万剑归宗"技能剑体偏离横向空间的领域距离

class Sword;
class ControlModel
{
private:
	bool CheckAndDieBehavior(const Ogre::FrameEvent & evt);		//检测死亡标志,若死亡则进行死亡表现
	bool BspCollDec(const Ogre::FrameEvent & evt);				//与Bsp地图之间的碰撞检测
	bool DivineSword4(const Ogre::FrameEvent & evt);			//"万剑归宗"连续技
	bool PlayUnDeadKillSound(int killNum); //播放不死连续杀戮音效
	bool PlayUnStopKillSound(int killNum, const Ogre::FrameEvent&evt); //播放短时间连杀音效

	void RoateAndDieRotate(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 30.0f,
		float dieRoateSpeed = 10.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
	bool ScaleChange(const Ogre::FrameEvent &evt, float limitScale,
		float changeRate = 3.0f, bool bigger = true);
	void DieShowText(int score, int * existScore);					//死亡文字显示
	bool FileAndRankLUpdate(const CEGUI::EventArgs & arg);          //文件数据及排行榜数据更新
	void CreateMonitor();                                           //创建监视器

public:
	ControlModel(Ogre::Root * root, Ogre::SceneManager* sceneMgr, SoundManager* soundMgr,Ogre::Camera* camera, NinthCEGUI* ninthCEGUI, Ogre::String charName);
	~ControlModel();
	void ModelInitialise(const Ogre::String &entityName, const Ogre::String &meshName,
		const Ogre::Vector3 &position, Ogre::Real HPValue);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent & evt);
	virtual bool keyPressed(const OIS::KeyEvent & arg);
	virtual bool keyReleased(const OIS::KeyEvent & arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, const OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, const OIS::MouseButtonID id);
	virtual bool mouseMoved(const OIS::MouseEvent & arg);

	Ogre::SceneNode* getMainNode(){ return m_MainNode; }	//得到主模型的世界场景节点
	Ogre::Entity* getMainEntity(){ return m_Model; }		//得到主模型的实体对象
	Ogre::Entity* getMeiZhiEntity(){ return m_MeiZhi; }     //得到妹纸的实体对象
	Ogre::Entity* getYunTianHeEntity(){ return m_YunTianHe; }//得到云天河的实体对象
	bool getRMouseDown() const;			//返回鼠标右键状态
	bool getLMouseDown() const;			//返回鼠标左键状态

	Ogre::Real getModelHP() const{ return m_HP; }				//返回主模型的血量
	void setModelHP(Ogre::Real HPValue) { m_HP = HPValue; }		//设置主模型的血量
	void cutModelHP(Ogre::Real HPValue) { m_HP -= HPValue; }	//减少模型血量

	bool getDieFlag ()const{ return m_bDead; }			//得到死亡判断标志
	void setDieFlag(bool dead){ m_bDead = dead; }		//设置死亡标志

	int getScore() const;		//取得已获得的分数
	int getLaunchIndex() const{ return m_LaunchIndex; }			//取得发射剑灵的索引
	void setLaunchIndex(int index){ m_LaunchIndex = index; }	//设置发射剑灵的索引
	bool getDivine4State() const { return m_bDivineSword4; }	//返回"万剑归宗"执行状态布尔值
	void setDivine4State(bool moveState){ m_bDivineSword4 = moveState; }  //设置"万剑归宗"执行状态的布尔值

	Ogre::Real getDivineTime() const { return m_DivineTime; }	//得到"独孤九剑,万剑归宗"发动所经过时间
	void addDivineTime(const Ogre::FrameEvent & evt){ m_DivineTime += evt.timeSinceLastFrame; }
																//增加"独孤九剑,万剑归宗"发动所经过时间
	void setDivineTime(Ogre::Real time){ m_DivineTime = time; } //设置"独孤九剑,万剑归宗"发动所经过时间
	void setRecBloodPar(bool emitting){ m_RecBlood->setEmitting(emitting); } //设置生命回复粒子特效状态
	void setSmallerState(bool smaller){ m_bSmaller = smaller; } //设置缩小状态
	void setDivSkillPer(bool permission){ m_bDivSkillPer = permission; } //设置万剑归宗独孤九剑技能权限

private:
	Ogre::String m_CharName;    //角色名称
	Ogre::Real m_HP;			//主模型血量
	bool m_bDead;				//主模型死亡标志	
	bool m_bDieShow;			//为true时,模型与摄像机分离
	bool m_bBeginSoundPlayed;    //游戏开始时已播放声音？
	bool m_bDieSoundPlayed;      //死亡时已播放声音？  
	bool m_bDieScoreWShow;       //死亡计分对话框的显示
	Ogre::Real m_DownDistance;   //死亡时跌落距离
	float m_fIntervalTime;        //连杀间隔时间        
	int m_iUnStopKillNum;         //不死连杀数
	Ogre::Root* m_Root;           //渲染源
	Ogre::SceneManager* m_SceneMgr;		//场景管理器
	NinthCEGUI* m_NinthCEGUI;			//CEGUI窗口控制

	SoundManager* m_SoundMgr;           //声音管理器接口

	int m_iShootSd;						//光束火焰射击声音索引
	int m_iShootChannel;                //光束火焰射击声道                       
	int m_iHeartBeatSd;                 //血量很少时的心脏跳动声索引
	int m_iHeartBeatChannel;            //血量很少时的心脏跳动声道
	int m_iGameBeginSd;                 //游戏开始时声音的索引
	int m_iGameBeginChannel;            //游戏开始时声音的声道
	int m_iGameOverSd;					//角色死亡时声音的索引
	int m_iGameOverChannel;             //角色死亡时声音的声道
	int m_iHelicSd;                     //究极螺旋形态声音的索引
	int m_iHelicChannel;                //究极螺旋形态声音的声道
	int m_iFanWingsSd;                  //煽动翅膀的声音索引
	int m_iFanWindsChannel;             //煽动翅膀声音的声道
	int m_iGlideSd;                     //滑翔的声音索引
	int m_iGlideChannel;                //滑翔的声音的声道

	//不死连杀
	int m_iFirstBloodSd;                //第一滴血
	int m_iKillingSpreeSd;              //狂欢的杀戮
	int m_iDominatingSd;                //主控局面
	int m_iMegaKillSd;                  //可怕的杀戮
	int m_iUnstoppableSd;               //不可停止
	int m_iWickedSickSd;                //变态杀戮
	int m_iMonsterKillSd;               //暴走的杀戮
	int m_iGodLikeSd;                   //像上帝一般的
	int m_iHolyShitSd;                  //神圣的
	//时间连杀
	int m_iDoubleKillSd;                //双杀
	int m_iTripleKillSd;                //三杀
	int m_iUltraKillSd;                 //极端的连续杀戮
	int m_iRampageSd;                   //连杀暴怒
	int m_iOwnageSd;                    //连杀暴走

	//不死连杀声道
	int m_iFirstBloodChannel;           //第一滴血声道
	int m_iKillingSpreeChannel;         //狂欢的杀戮声道
	int m_iDominatingChannel;           //主控局面声道
	int m_iMegaKillChannel;             //可怕的杀戮声道
	int m_iUnstoppableChannel;          //不可停止声道
	int m_iWickedSickChannel;           //变态杀戮声道
	int m_iMonsterKillChannel;          //暴走的杀戮声道
	int m_iGodLikeChannel;              //像上帝一般的声道
	int m_iHolyShitChannel;             //神圣的声道
	//时间连杀声道
	int m_iDoubleKillChannel;           //双杀声道
	int m_iTripleKillChannel;           //三杀声道
	int m_iUltraKillChannel;            //极端的连续杀戮声道
	int m_iRampageChannel;              //连杀暴怒声道
	int m_iOwnageChannel;               //连杀暴走声道
	int m_iKillChannel;                 //杀戮声音的声道声道

	int m_iPreKillNum;                  //记录先前的击杀数目

	Ogre::Camera* m_Camera;				//模型控制摄像机
	Ogre::Camera* m_Monitor;            //模型监视器
	Ogre::Vector3 m_Direction;			//摄像机移动方向

	Ogre::Entity* m_Model;				//模型实体(飞行坐骑)
	Ogre::Entity* m_MeiZhi;				//模型实体(坐骑上的妹纸)
	Ogre::Entity* m_YunTianHe;			//模型实体(坐骑上的云天河)		

	Ogre::SceneNode* m_MainNode;        //控制摄像机和模型的主节点
	Ogre::SceneNode* m_CamNode;			//摄像机控制节点
	Ogre::SceneNode* m_ModelNode;		//模型控制节点(飞行坐骑)
	Ogre::SceneNode* m_MonitorNode;     //监视器控制节点

	Ogre::AnimationState* m_Fly1;       //飞行动画1（循环）
	Ogre::AnimationState* m_Fly1to2;    //飞行动画1到2的过渡
	Ogre::AnimationState* m_Fly2;       //飞行动画2（循环）
	Ogre::AnimationState* m_Fly2to3;    //飞行动画2到3的过渡
	Ogre::AnimationState* m_Fly3;       //飞行动画3（循环）
	Ogre::AnimationState* m_Flying;     //正在飞行的动画状态
	bool m_bLeft;                       //按下A键状态时为真
	bool m_bRight;                      //按下D键状态时为真
	bool m_bLGlide;                     //左滑翔标志(A按下时为true)
	bool m_bRGlide;                     //右滑翔标志(D按下时为true)
	Ogre::Real m_rotateSum;             //旋转倾斜的最大累计和

	Ogre::ParticleSystem* m_RedBeam;	//红色光束粒子系统
	Ogre::ParticleSystem* m_Aureola;    //究极螺旋形态时带起的龙卷风
	Ogre::ParticleSystem* m_RecBlood;   //恢复生命的粒子特效

	bool m_bMRight;						//判断鼠标右键被按下的标志
	bool m_bMLeft;						//判断鼠标左键被按下的标志	
	bool m_bHelicState;					//处于究竟螺旋状态
	bool m_bSwordMoved;					//剑灵可移动状态（射出后为真）
	bool m_bDivineSword4;				//"万剑归宗"技能释放状态
	bool m_bBigger;                     //处于不断变大状态
	bool m_bSmaller;					//处于不断缩小状态
	bool m_bHelSkillPer;                //究极螺旋技能权限
	bool m_bDivSkillPer;                //万剑归宗独孤九剑技能权限
	bool m_bJetFirePer;                 //喷射火焰的权限
	
	Ogre::Real m_rotate;                //模型的倾斜旋转量
	Ogre::Real m_ConvInterval;			//飞行动画切换后所经过的时间
	Ogre::Real m_WKeyInterval;			//自上次按下W键所经过的时间
	Ogre::Real m_HelicTime;				//使用究极螺旋形态所经过的时间
	Ogre::Real m_DivineTime;			//“独孤九剑,万剑归宗”技能开启后所经过的时间
	Ogre::Real m_lMouseTime;             //鼠标左键按下的时间
	int m_FlyFlag;						//飞行动画标志	
	int m_CycleIndex;                   //循环索引数

	Ogre::Real m_ChangeSpeed;           //改变后的移动速度
	Ogre::Radian m_limitRad;			//旋转限制弧度

	Ogre::RaySceneQuery* m_RaySceneQuery; //场景射线查询

	int m_LaunchIndex;					//欲发射剑灵的索引,0~8 
	Sword* m_DivineSword[DIVINE_SWORD_NUM];  //指向万剑的指针(这里称该剑为“傀儡剑体”）

	int m_ExistScore[5];                //已存在的排行榜前5个分数
	Ogre::String m_ExistName[5];        //已存在的排行榜前5个名字
	bool m_bChangeExistScore;           //是否改变已存在分数
	int m_RLUpdateIndex;                //排行榜更新位置的索引

	int m_iClickSd;		 //界面的点击音效索引
	int m_iClickChannel; //界面点击音效的声道

};


#endif