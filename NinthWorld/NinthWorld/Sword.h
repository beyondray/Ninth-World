/*------------------------------------------------------------------
【头文件】：Sword.h		Created by Beyond Ray,2014年4月
（描述）：一个封装了剑体AI所有行为的类
--------------------------------------------------------------------*/

#ifndef H_Sword
#define H_Sword

#include"ControlModel.h"

#define SWORD_ATTACK_DISTANCE 600     //某些剑体不断攻击，此为再次发动攻击的圆领域
#define SWORD_DEFENSE_DISTANCE 700	 //剑体意识的防御领域（进入该领域内，就会发动攻击）
#define SWORD_STOP_DISTANCE 800		 //某些剑体只进行一次攻击，攻击射出距离目标该距离后停留
#define SWORD_HOMING_DISTANCE 20	 //剑灵归位时距离剑核的最远距离
#define SWORD_LAUNCH_VALID_DIS 1800   //剑灵发射的最大有效距离,超出该距离后失效
#define HELSWORD_LAUNCH_VALID_DIS 200 //螺旋剑发射的最大有效距离,超出该距离后失效
#define SWORD_NORMAL_MASK 1<<2		 //设置普通剑体AI的查询遮罩
#define SWORD_JIANLING_MASK 1<<3     //设置剑灵AI的查询遮罩
#define SWORD_PUPPET_MASK 1<<6       //傀儡剑体的查询遮罩

#define SWORD_ROTATE_SPEED 0.003f	 //剑灵射出后的偏移速度
#define SWORD_MAX_DENFENSE_TIME 10.0f //剑灵旋转防御技能所持续的最大时间
#define SWORD_DEFENSE_COOL_TIME 60.0f //剑灵旋转防御技能的冷却时间
#define SWORD_DIVINE_COOL_TIME 30.0f  //"独孤九剑,万剑归宗"技能的冷却时间
#define SWORD_HELSWORD_KEEP_TIME 14.0f //螺旋剑技能持续时间
#define SWORD_RISE_MAX_HEIGHT  100	 //"独孤九剑,万剑归宗"技能发动时剑体所上升的最大高度
#define SWORD_SHOOT_DEVIATE_DIS 45   //"独孤九剑,万剑归宗"技能第二段射击点到中心的偏离距离
#define SWORD_DIVINE2_SCALE 4		 //"独孤九剑,万剑归宗"技能第二段发动时剑体的比例大小
#define SWORD_DIVINE3_SWEEP_DIS 800  //"独孤九剑,万剑归宗"技能第三段发动时向外横扫的距离
#define SWORD_DIVINE4_WAIT_TIME 10   //"独孤九剑,万剑归宗"技能第四段连续技释放的最大等待时间(超过该时间自动释放)

//--------------------------------【剑体有三种】：----------------------------------------
// 其一为一次攻击剑，攻击后会在远处停留，除非判断到对方又侵入防御领域才会再次发动攻击
// 其二为多重攻击剑，攻击后潜意识为到达再次攻击的领域距离再发动攻击,除非目标离开了防御领域
// 其三为穷追不舍剑, 攻击后到达再次攻击领域距离，然后再次发动攻击，追击后无视防御领域,
// 无限攻击，直至死亡。
//----------------------------------------------------------------------------------------
struct Sword_Object
{
	int m_ObjectIndex;					//个体标识(对于每个个体唯一)
	int m_CategoryIndex;				//种类标识(总共有16种)
	Ogre::Real m_MaxHP;					//拥有的最大血量
	Ogre::Real m_Damage;				//攻击的伤害
	Ogre::Real m_Defense;				//被攻击时的防御
	Ogre::Real m_FlySpeed;				//飞行的速度
	Ogre::Vector3 m_Position;			//出现的位置
	Ogre::Vector3 m_Scale;				//剑体的比例大小
	bool m_bOneAttack;					//是否为单次攻击剑体
	bool m_bNoStopAttack;				//是否为穷追不舍剑
	//（只要进入防御领域，不死不罢休）<m_bNoStopAttack和m_bOneAttack不可叠加，否则会出现第四种长途穷追剑>
	bool m_bAttackRotate;				//定义攻击时是否旋转
};

class Sword
{
private:
	//产生-1到1的浮点随机数
	inline float randFloat(){ return ((float)rand() / RAND_MAX)*2.0f - 1.0f; }
	//剑体AI死亡计数改变显示
	inline void DieNumChangeShow(){ m_NinthCEGUI->getSwordDieNumW()->setText("Sword: " + Ogre::StringConverter::toString(ms_iDieNum)); }

	void RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		Ogre::Vector3 destPos, bool rotateResolve = true, float rotatePre = 1.0f, 
		bool rotateItself = true, float rotateRate = 15.0f, 
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); //调转方向并向目标前进

	void RotateAndDenfense(const Ogre::FrameEvent & evt, float rotateRate = 10.0f,
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
																		//剑灵绕剑核旋转的防御体系
	bool RotateAndStop(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 1.0f,
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); //调转到一个方向并停止

	void DieToHoming(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		const Ogre::Vector3 & dirRotate,bool rotateResolve = true, float rotatePre = 30.0f,
		float ownRotateRate = 1.0f,Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); 
																		//剑体的死亡归位
	bool HelicalSword(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 0.3f,
		float ownRotateRate = 1.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
	                                                                    //螺旋剑技能
	bool DivineSword1(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		Ogre::Vector3 destPos, float ownCenterRotateRate = 2.0f, bool rotateItself = true,
		float rotateRate = 1.0f,Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); 
																		//独孤九剑,万剑归宗(第一段)之九剑归一

	bool DivineSword2(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		float shootSpeedRate = 3.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);	
																		//独孤九剑,万剑归宗(第二段)之独孤九剑

	bool DivineSword3(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 1.0f,
		 bool rotateItself = true, float ownRotateRate = 1.0f, float coreRotateRate = 1.0f,
		float flyRate = 1.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
																		//独孤九剑,万剑归宗(第三段)之九剑轮回

	bool BeJetCollDec();				//被喷射攻击的碰撞检测
	bool ThroughCollDec();				//穿透攻击的碰撞检测(对玩家而言)
	bool ThroughAICollDec();			//穿透攻击的碰撞检测(对AI而言)
	inline Ogre::Vector3 ChangePosToWorld(const Ogre::Vector3 &pos) const;
	//将基于场景根节点的坐标转换为基于世界的(前提：场景根节点pitch了PI/2)

public:
	Sword(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Sword_Object &sword, ControlModel* model, NinthCEGUI* ninthCEGUI);//构造函数
	Sword(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, ControlModel* model, NinthCEGUI* ninthCEGUI);
	virtual ~Sword();					//析构函数
	void SwordInitialise(const Sword_Object & sword);			//普通剑体初始化
	void PuppetSwordInitialise(const Sword_Object & sword);		//傀儡剑体初始化
	bool frameRenderingQueued(const Ogre::FrameEvent & evt);	//在渲染队列中更新剑体AI每一帧的行为
	bool PuppetFrameRenderingQueued(const Ogre::FrameEvent & evt); //仅作为特定的傀儡剑体操控时所更新的每一帧行为
	bool HelicalSwordFrameRendering(const Ogre::FrameEvent & evt); //螺旋剑技能发动时每一帧的行为

	Ogre::Vector3 getPosition()const { return m_SwordNode->getPosition(); } //返回剑体相对父空间的坐标
	void setPosition(Ogre::Vector3 positon);		//定点设置剑体AI位置
	void setRandPosition(Ogre::Real range = 2000);	//设置剑体AI的随机位置
	void setRandCylinderPos(Ogre::Real rad, Ogre::Real height);		//设置圆柱体领域的剑体AI随机位置
	void setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range = 2000); //设置剑体AI跟随目标的随机位置
	void setFollowRandCylinderPos(const Ogre::Vector3 &position, Ogre::Real rad, Ogre::Real height);
																			//设置剑体AI跟随目标的圆柱体随机位置
	bool keyPressed(const OIS::KeyEvent & arg);		//设置剑体AI成为剑灵后的相关键盘按下操作
	bool keyReleased(const OIS::KeyEvent & arg);    //设置剑体AI成为剑灵后的相关键盘释放操作
	bool mousePressed(const OIS::MouseEvent & arg, const OIS::MouseButtonID id);//设置鼠标按下的键位操作
	bool mouseMoved(const OIS::MouseEvent & arg);	//设置射出后剑灵的移向
	Ogre::SceneNode* getSwordNode(){ return m_SwordNode; } //返回剑体节点
	friend class ControlModel;
private:
	ControlModel* m_Model;				//调用主模型，关联关系
	NinthCEGUI* m_NinthCEGUI;			//CEGUI控制窗口

	SoundManager* m_SoundMgr;           //声音管理器接口
	//声音索引,声道索引
	int m_iThroughtDestSd;				//穿透目标声音索引
	int m_iThroughtDestChannel;         //穿透目标声音的声道
	int m_iHowlSd;                      //剑体呼啸声音索引
	int m_iHowlChannel;                 //剑体呼啸声音的声道
	int m_iHomingSd;                    //剑灵归位的声音索引
	int m_iHomingChannel;               //剑灵归位声音的声道
	int m_iSwordShootSd1;               //剑灵射出的声音索引1
	int m_iSwordShootChannel1;          //剑灵射出的声音的声道1
	int m_iSwordShootSd2;               //剑灵射出的声音索引2
	int m_iSwordShootChannel2;          //剑灵射出的声音的声道2
	int m_iIkuSoSd;                     //(画中音-王者风范)声音索引
	int m_iIkuSochannel;                //(画中音-王者风范)声音声道
	int m_iShootSwordSd;				//剑敲击声音索引
	int m_iShootSwordChannel;           //剑敲击声道
	
	int m_iSDefenseSd;					//剑道防御声音索引
	static int ms_iSDefenseChannel;		//剑道防御声音的声道
	int m_iDefenseScreamSd;             //最强防御呐喊声索引
	static int ms_iDefenseScreamChannel;//最强防御呐喊声声道
	int m_iDiv1_TrasSd;                 //"独孤九剑,万剑归宗"技能第一段之平移
	static int ms_iDiv1_TrasChannel;
	int m_iDiv1_RotSd;                  //"独孤九剑,万剑归宗"技能第一段之旋转
	static int ms_iDiv1_RotChannel;
	int m_iDiv1_ShootSd;                //"独孤九剑,万剑归宗"技能第一段之射出
	static int ms_iDiv1_ShootChannel;
	int m_iDiv2Sd;                      //"独孤九剑,万剑归宗"技能第二段
	int m_iDiv2Channel;
	int m_iDiv3_RotSd;                  //"独孤九剑,万剑归宗"技能第三段之旋转
	static int ms_iDiv3_RotChannel;
	int ms_iDiv3_ShootSd;               //"独孤九剑,万剑归宗"技能第三段之射出
	static int ms_iDiv3_ShootChannel;
	int m_iDiv3_SThunderSd;             //"独孤九剑,万剑归宗"技能第三段之短暂雷鸣
	static int ms_iDiv3_SThunderChannel;
	int m_iDiv3_ThunderSd;              //"独孤九剑,万剑归宗"技能第三段之雷声
	static int ms_iDiv3_ThunderChannel;
	
	int m_iDiv4_ShootSd;                //"万剑归宗"技能第四段之剑鸣射出声
	int m_iDiv4_ShootChannel;
	int m_iDiv4_LThunderSd;             //"万剑归宗"技能第四段之残留雷鸣
	int m_iDiv4_LThunderChannel;

	int m_iHelSword_FengDunSd;          //风遁螺旋手里剑音效
	static int ms_iHelSword_FengDunChannel;
	int m_iHelSword_SThunderSd;          //螺旋剑发动时的短暂雷鸣
	static int ms_iHelSword_SThunderCannel;
	int m_iHelSword_RotSd1;              //螺旋剑旋转声效1
	static int ms_iHelSword_RotChannel1;
	int m_iHelSword_RotSd2;              //螺旋剑旋转声效2     
	static int ms_iHelSword_RotChannel2;
	
	Ogre::SceneManager* m_SceneMgr;		//场景管理器
	Ogre::SceneNode* m_AttackNode;		//锁定攻击目标的世界节点位置
	Ogre::Real m_Distance;				//距离锁定目标的距离
	Ogre::Vector3 m_Direction;			//剑体AI到攻击目标的方向
	Ogre::SceneNode* m_SwordNode;		//剑体AI的场景节点
	Ogre::Entity* m_SwordEnt;			//剑体AI的实体对象

	Sword_Object m_Sword;				//剑体AI的个体属性对象
	Ogre::Real m_HP;					//剑体AI可控制的拥有血量

	Ogre::Quaternion m_Orbit;				//轨道方向四元数

	Ogre::RaySceneQuery* m_RaySceneQuery;	//射线场景查询
	Ogre::AxisAlignedBoxSceneQuery* m_AABBSceneQuery;	

	Ogre::Vector3 m_AttackPos;				//记录攻击的目标点
	bool m_bArriveAttackPos;				//判断是否到达目标攻击点
	bool m_bDenFirstAttack;					//侵入防御领域未到达目标攻击点前为true,表趋势（非真正第一次）
	bool m_bAttackState;					//在防御领域内，则攻击状态为真
	bool m_bGetAttackPos;					//已经得到攻击目标位置的标志
	bool m_bDead;							//判断是否死亡的标志
	bool m_bLaunchState;					//成为剑灵后是否处于发射状态的标志
	bool m_bSwordMoved;						//剑灵可移动状态（射出后为真）
	bool m_bRotateDenfense;					//剑灵的旋转防御状态（按下X开启）
	bool m_bDivineSword123;					//“独孤九剑,万剑归宗”释放状态(按下F开启）(限前三段)
	bool m_bHelicalSword;                   //螺旋剑技能释放状态(按下R开启)
	bool m_bSpace;                          //按下Space时为true
	bool m_bDenSkCoolPer;                   //剑道防御技能冷却权限（仅限发动剑系技能的其中一只剑体）
	bool m_bHelSSCoolPer;                   //螺旋剑技能冷却权限（仅限发动剑系技能的其中一只剑体）

	int m_iHelicIndex;                      //发动螺旋剑时的剑位标识
	int m_iDivineSword;						//“独孤九剑,万剑归宗”技能处于第几段?
	static bool ms_bNinthSwordShoot;		//“独孤九剑,万剑归宗”技能第九剑暴射已完成时为真
	static Ogre::Real ms_DenfenseTime;		//旋转防御技能开启后所经过的时间
	static int ms_iRotateDenSucNum;          //旋转防御发动成功的剑体数
	Ogre::Real m_Divine1RotateTime;			//“独孤九剑,万剑归宗”技能第一阶段时剑灵自身旋转所经过的时间

	Ogre::Real m_PuppleSwordShootTime;		//傀儡剑体激射所经过的时间
	static bool ms_bPuppetStop;			    //傀儡剑体停止激射的标志

	static Ogre::SceneNode* ms_SwordCore;	//剑体死亡成为剑灵后所围绕旋转的核节点
	static bool ms_bCoreHaved;				//已经创建检核时为真
	static Ogre::SceneNode* ms_HelSCenCore; //螺旋剑中心核，与检核类似，处于中心不向外平移
	static Ogre::SceneNode* ms_HelicalCore; //发动螺旋剑技能时三柄剑所围绕旋转的核节点
	static bool ms_bHCorehaved;             //已经创建螺旋核和螺旋剑中心核时为真
	static bool ms_bHelSwordAchieved;       //螺旋剑技能发动完成时为真
	static bool ms_bHelSwordState;          //处于螺旋剑技能状态时为真
	static bool ms_bHCLaunchState;          //螺旋核处于发射状态时为真
	static bool ms_bHelSwordManual;         //螺旋剑技能用于手工操作时为真
	static int ms_iRKeySusCount;            //累计螺旋剑技能发动成功的剑数
	static int ms_iRevSwordFromHC;          //累计技能发动完毕后从螺旋剑核中成功移除的剑体数量
	Ogre::Real m_HelSwordKTime;             //螺旋剑技能发动所经过的时间

	static int ms_iRotToCoreNum;			//剑灵数,最小为0, 最大为9
	static int ms_iLaunchNum;				//处于发射中的剑灵数
	static bool ms_bDivine3LayDown;			//“独孤九剑,万剑归宗”技能第三阶段时剑灵全部躺下时为真
	static bool ms_bDivine4Start;			//“独孤九剑,万剑归宗”技能第四阶段开启时为真
	static bool ms_bDivine3Ready;		    //“独孤九剑,万剑归宗”技能第三阶段到第四阶段可过渡时即准备完毕,为真
	static Ogre::Real ms_Divine3Scale;		//“独孤九剑,万剑归宗”技能第三阶段时放大到的比例
	static Ogre::Real ms_Divine4WaitTime;	//“独孤九剑,万剑归宗”等待第四段连续技开启所经过的时间
	int m_iRotToCoreIndex;					//剑灵标识索引,0~8
	int m_iLaunchIndex;						//保存一个欲发射的剑灵索引值
	bool m_bDiv3LaySuc;						//“独孤九剑,万剑归宗”第三阶段成功躺下时为真
	static int ms_iDivSucNum;				//累计“独孤九剑,万剑归宗”成功发动的剑灵数
	static int ms_iDiv3LaySuc;				//累计“独孤九剑,万剑归宗”第三阶段成功躺下的剑灵数
public:
	static int ms_iDieNum;					//剑体AI的死亡计数（所有剑体AI共享一个计数)
};

#endif