#ifndef __NinthCore_h_
#define __NinthCore_h_


#include "BaseApplication.h"
#include "KnotAndFish.h"
#include "ControlModel.h"
#include "Dragon.h"
#include "NinthCEGUI.h"
#include "GhostdomDragon.h"
#include "Sword.h"
#include "MyCEGUISystem.h"

using namespace Ogre;

static const int DRAGON_NUM = 6;
static const int SWORD_NUM = 15;
static const int SWORD_TO_CENTER_DIS = 3000;	//剑阵到场景中心的距离

class NinthCore : public BaseApplication
{
private:
	virtual void createGUI(void);
	virtual void createFixedScene(void);
	virtual void createActiveScene(void);

	virtual void destroyActiveScene(void);
	virtual void destroyGUI(void);

	virtual void createCamera(void);
	virtual void createViewports(void);

	virtual void createRewardingW(void);
	virtual void createMainMenuRelevW(void);
	virtual void createRListRelevW(float ratio);
	virtual void createCMapRelevW(float ratio);

	// Ogre::FrameListener
	virtual void createFrameListener(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	// OIS::KeyListener
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	// OIS::MouseListener
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	//void generate();
	bool StartGame(const CEGUI::EventArgs& arg); //触发开始游戏，进入游戏状态
	bool KeyHelp(const CEGUI::EventArgs& arg);   //进入键位帮助界面
	bool SwordSkill(const CEGUI::EventArgs& arg);//进入剑系技能介绍界面
	bool RankList(const CEGUI::EventArgs& arg);  //排行榜
	bool QuitGame(const CEGUI::EventArgs& arg);  //退出游戏
	bool Back(const CEGUI::EventArgs& arg);      //返回主界面
	bool OneAgain(const CEGUI::EventArgs & arg); //再来一局
	bool ReturnMain(const CEGUI::EventArgs & arg);//返回主界面
	bool Ok(const CEGUI::EventArgs & arg);        //确认名字输入
	bool EditNameChange(const CEGUI::EventArgs & arg); //编辑名称改变
	bool CMapOk(const CEGUI::EventArgs & arg);    //确认选择地图
	bool ChooseMap(const CEGUI::EventArgs & arg); //选择地图
	bool CMapBack(const CEGUI::EventArgs & arg);  //从选择地图界面返回

public:
	NinthCore(void);
	virtual ~NinthCore(void);

private:
	KnotAndFish* m_Knotfish;		  //纽环和鱼的局部场景
	ControlModel* m_Model;			  //主模型控制
	NinthCEGUI* m_NinthCEGUI;		  //CEGUI窗口集合
	SoundManager* m_SoundMgr;         //声音管理器

	int m_iSongSd1;                   //第一首背景音乐声音索引
	int m_iSongChannel1;              //第一首背景音乐声道     
	int m_iSongSd2;                   //第二首背景音乐声音索引
	int m_iSongChannel2;              //第二首背景音乐声道
	int m_iSongSd3;                   //第三首背景音乐声音索引
	int m_iSongChannel3;              //第三首背景音乐声道
	int m_iSongingChannel;            //正在播放的背景音乐的通道

	Dragon *m_DragonAI[DRAGON_NUM/2];		//用指针数组关联多个雷龙AI
	GhostdomDragon* m_GDragonAI[DRAGON_NUM / 2];	//冥界亚龙AI指针
	Sword* m_SwordWaitAI[81];				//初始时位于九把大剑身边的剑体AI
	Sword* m_SwordAI[SWORD_NUM];			//初始时可能追踪攻击目标的剑体AI
	Ogre::SceneNode* m_SwordNode[9];		//指向九把大剑的场景节点指针

	Dragon_Object m_Dragon;				//雷龙AI个体属性对象
	Sword_Object m_Sword;				//剑体AI个体属性对象

	//界面相关
	CEGUI::Window* m_MainWin;           
	CEGUI::Window* m_StartGameW;		//开始游戏按钮窗口
	CEGUI::Window* m_StartGameTW;       //开始游戏文本窗口
	CEGUI::Window* m_KeyHelpW;
	CEGUI::Window* m_SwordSkillW;
	CEGUI::Window* m_RankListW;
	CEGUI::Window* m_QuitGameW;
	CEGUI::Window* m_BackW;
	CEGUI::Window* m_RankListSheetW;
	CEGUI::Window* m_NameTipW;
	CEGUI::Window* m_NameEditW;
	CEGUI::Window* m_OkW;
	CEGUI::Window* m_NameShowW;
	Ogre::String m_CharName;	
	CEGUI::Window* m_SkillSkinW;
	CEGUI::Window* m_CMapOkW;
	CEGUI::Window* m_MapBackW;
	CEGUI::Window* m_MapTipW;
	CEGUI::Window* m_MapW[6];
	int m_MapIndex;

	int m_iClickSd;		 //界面的点击音效索引
	int m_iClickChannel; //界面点击音效的声道

	int m_iGUIMusicSd;     //GUI背景音乐索引
	int m_iGUIMusicChannel;//GUI背景音乐声道

	int m_iChooseMapSd;     //选择地图音效索引
	int m_iChooseMapChannel;//选择地图音效声道
};

#endif // #ifndef __NinthCore_h_