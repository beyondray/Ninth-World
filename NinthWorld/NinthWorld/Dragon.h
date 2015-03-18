/*------------------------------------------------------------------
【头文件】：Dragon.h		Created by Beyond Ray,2014年4月
（描述）：一个封装了雷龙卡雷苟斯AI所有行为的类
--------------------------------------------------------------------*/

#ifndef H_Dragon
#define H_Dragon

#include "SoundManager.h"
#include"ControlModel.h"

//#define BSP_MODE

#define DRAGON_CIRCLE_DISTANCE 500      //距离锁定目标的回旋游走攻击距离
#define DRAGON_BACK_ATTACK_DIS 400		 //逃跑成功后调转攻击缓冲距离
#define DRAGON_RUNAWAY_DISTANCE 150      //接近目标太近时的逃走距离 
#define DRAGON_ATTACK_DISTANCE 380		 //开始对锁定目标发动攻击的距离
#define DRAGON_DOWN_NEWLIFE_DIS 6000    //死亡摔落之后，到达该距离便重生
#define DRAGON_MASK 1<<1		 //设置雷龙AI的查询遮罩
struct Dragon_Object
{
	int m_Index;						//个体标识
	Ogre::Real m_MaxHP;					//拥有的最大血量
	Ogre::Real m_Damage;				//攻击的伤害
	Ogre::Real m_Defense;				//被攻击时的防御
	Ogre::Real m_FlySpeed;				//飞行的速度
	Ogre::Vector3 m_Position;			//出现的位置
	Ogre::Vector3 m_Scale;				//雷龙的比例大小
	int m_FlyIndex;						//飞行动画（循环)的索引（0,1,2)
	int m_JetIndex;						//喷射攻击光束的索引（0:红色,1:蓝色,2:绿色,3:黄色)
};

class Dragon 
{
private:
	//产生-1到1的浮点随机数
	inline float randFloat(){ return ((float)rand() / RAND_MAX)*2.0f - 1.0f; }
	//雷龙AI死亡计数改变显示
	inline void DieNumChangeShow(){m_NinthCEGUI->getDragonDieNumW()->setText("Dragon: " + Ogre::StringConverter::toString(ms_iDDieNum));}

	void RotateAndAttack(const Ogre::FrameEvent & evt, float roateRate = 1.0f,
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);		//周旋环绕攻击

	void RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		const Ogre::Vector3 & dirRotate, Ogre::Vector3 dirGo, bool rotateResolve = true, 
		float rotatePre = 30.0f, bool toSkyOriSet = true, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);	
																			//调转方向并向目标前进
	void RoateAndDieRotate(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float roatePre = 30.0f,
		float roateSpeed = 10.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);		
																			//调转方向并向目标前进

	bool BeJetCollDec();				//被喷射攻击的碰撞检测
	bool JecCollDec();					//喷射攻击的碰撞检测
	bool BeThroughCollDec(const Ogre::FrameEvent & evt);//对剑灵的穿透攻击进行被击中检测
	bool ScaleChange(const Ogre::FrameEvent &evt, float limitScale, float changeRate = 3.0f, bool bigger = true); 
										//按时间对比例大小进行改变(bigger为true则放大,false则缩小)
	inline Ogre::Vector3 ChangePosToWorld(const Ogre::Vector3 &pos) const;	
	//将基于场景根节点的坐标转换为基于世界的(前提：场景根节点pitch了PI/2)

	void ChooseBeam(Ogre::ParticleSystem*& beam, int index);         //选择粒子光束(index从0到7)

public:
	Dragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, Dragon_Object &dragon, ControlModel* model, NinthCEGUI* ninthCEGUI);//构造函数
	Dragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, ControlModel* model, NinthCEGUI* ninthCEGUI);
	void DragonInitialise(Dragon_Object &dragon);
	virtual ~Dragon();					//析构函数	
	bool frameRenderingQueued(const Ogre::FrameEvent & evt);	//在渲染队列中更新雷龙AI每一帧的行为
	void setPosition(Ogre::Vector3 positon);		//定点设置雷龙AI位置
	void setRandPosition(Ogre::Real range = 2000);	//设置雷龙AI的随机位置
	void setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range = 2000); //设置雷龙AI跟随目标的随机位置

private:
	ControlModel* m_Model;				//调用主模型，关联关系
	NinthCEGUI* m_NinthCEGUI;			//CEGUI控制窗口
	Ogre::String m_MeshName;			//网格对象的名称

	SoundManager* m_SoundMgr;           //声音管理器接口
	int m_iDieSd;                       //死亡声音索引
	int m_iDieChannel;                  //死亡声音通道      
	int m_iScreenSd;					//凄厉喊叫声音索引
	int m_iScreenChannel;				//凄厉喊叫声道
	int m_iThunderStormSd;				//滋鸣雷暴声音索引
	int m_iThunderStormChannel;         //滋鸣雷暴声道
	int m_iAureolaSd;					//风声的索引
	int m_iAureolaChannel;              //风声的的声道
	int m_iBiggerSd;					//体积变大的声音索引
	int m_iBiggerChannel;               //体积变大的声道
	int m_iHarmObjectSd;				//伤害对方的声音索引
	int m_iHarmObjectChannel;           //伤害对方的声道
	int m_iJetSd;                       //喷射攻击的声音索引
	int m_iJetChannel;                  //喷射攻击声音的声道

	Ogre::SceneManager* m_SceneMgr;		//场景管理器
	Ogre::SceneNode* m_AttackNode;		//锁定攻击目标的世界节点位置
	Ogre::Real m_Distance;				//距离锁定目标的距离
	Ogre::Vector3 m_Direcion;			//雷龙AI到攻击目标的方向
	Ogre::SceneNode* m_DragonNode;		//雷龙AI的场景节点
	Ogre::Entity* m_DraEnt;				//雷龙AI的实体对象

	Dragon_Object m_Dragon;				//雷龙AI的个体属性对象
	Ogre::Real m_HP;					//雷龙AI可控制的拥有血量

	Ogre::AnimationState* m_Fly;        //可选择飞行动画（循环）【1,2,3】
	Ogre::AnimationState* m_Die;		//被击中时的死亡动画

	Ogre::ParticleSystem* m_Beam;		//可选择攻击光束（4种颜色，2种形态，共8种）
	Ogre::ParticleSystem* m_RedSmoke;	//被剑灵击中时附加的绿火效果

	Ogre::Quaternion m_Orbit;				//轨道方向四元数
	float m_fNewOrbitTime;                  //重新产生一个新轨道的时间                    
	bool m_bRunFlag;						//雷龙AI的逃跑标志

	Ogre::ParticleSystem* m_Aureola;		//被击中时添加狂卷旋风

	Ogre::RaySceneQuery* m_RaySceneQuery;	//射线场景查询
	Ogre::SphereSceneQuery* m_SphereSceneQuery;//球面场景查询
	bool m_bDead;							//判断是否死亡的标志
	bool m_bBigger;							//被剑灵击中时处于放大状态之中
	bool m_bSmaller;						//被剑灵击中过死亡后所处于的缩写状态之中
public:
	static int ms_iDDieNum;					//雷龙AI的死亡计数（所有雷龙AI共享一个计数)

};

#endif