#ifndef H_NinthCEGUI
#define H_NinthCEGUI

#include "MyCEGUISystem.h"

class NinthCEGUI
{
private:
	SoundManager* m_SoundMgr;               //声音管理器
	int m_iClickSd;		 //界面的点击音效索引
	int m_iClickChannel; //界面点击音效的声道

	CEGUI::Window* m_DragonDieNumW;			//显示雷龙AI死亡计数的部件窗口
	CEGUI::Window* m_GDragonDieNumW;		//显示冥界亚龙AI死亡计数的部件窗口
	CEGUI::Window* m_SwordDieNumW;			//显示剑体AI死亡计数的部件窗口
	CEGUI::OgreRenderer* m_RendererW;		//基于OGRE的CEGUI渲染器
	CEGUI::Window* m_DieScoreW;				//主模型死亡分数窗口
	CEGUI::Window* m_BloodW;				//血量窗口
	CEGUI::Window* m_HeadW;                  //头像窗口
	CEGUI::Window* m_BloodNumW;              //血量具体数值窗口
	CEGUI::Window* m_FPSW;                   //FPS窗口
	CEGUI::Window* m_OneAgainW;              //再来一次窗口
	CEGUI::Window* m_QuitW;                  //退出窗口
	CEGUI::Window* m_ReturnMainW;            //返回主界面窗口
	CEGUI::Window* m_OkW;                    //确认名称窗口
	CEGUI::Window* m_RNameEditW;             //排行名称编辑窗口
	CEGUI::Window* m_OkAgainW;               //再次确认名称窗口
	CEGUI::Window* m_NameAffirmW;            //名称确认后弹出的再次确认窗口
	Ogre::String m_RankListName;			 //排行名
	CEGUI::Window* m_MonitorFrameW;          //监视器框架
	CEGUI::Window* m_MonitorW;               //监视器
	CEGUI::Window* m_RTextW[10];			 //10个奖赏文本窗口(前5个名次和姓名，后5个分数)
	CEGUI::Window* m_SkillShadeW[8];         //8个技能遮罩窗口
	CEGUI::Window* m_SkillCoolCalW[8];       //6个技能冷却计时窗口(1,3个暂时不用)
	CEGUI::Window* m_SkillStateW[3];         //3个技能状态窗口
	int m_iSkillStateNum;                    //技能状态数累计
	int m_iSSIndex[3];                       //技能状态索引（分别对应3个技能状态显示）

	Ogre::RenderWindow* m_Window;
	bool* m_ShutDown;
	float ratio;                          //窗口横纵比

public:
	NinthCEGUI(Ogre::RenderWindow* window, bool* shutdown, SoundManager* soundMgr);	//构造函数
	~NinthCEGUI();						//析构函数
	void CEGUICreateAndInitialise();	//CEGUI创建和初始化
	void CreateDragonDieNumW();			//创建显示雷龙AI死亡数量的窗口
	void CreateGDragonDestDieNumW();	//创建显示冥界亚龙AI死亡数量的窗口
	void CreateSwordDieNumW();			//创建显示剑体AI死亡数量的窗口
	void CreateSrcBloodShowW(float ratio = 1.6f);//创建主模型血量显示的窗口	
	void CreateSrcDieScoreW();     //创建显示主模型死亡时分数显示的窗口
	void CreateFPSW(float ratio = 1.6f); //创建显示FPS的窗口
	void CreateRewardingTextW();        //创建奖赏相关文本的窗口
	void CreateSShadeAndCoolCW();       //创建技能遮罩和技能冷却计时窗口
	void CreateMonitorW();              //创建监视器窗口
	void ShowAllInterfaceW();           //显示所有界面窗口
	void HideAllInterfaceW();           //隐藏所有界面窗口

	int getSkStateNum(){ return m_iSkillStateNum; }       //得到累计的技能状态数
	int setSkStateNum(int num){ m_iSkillStateNum = num; } //设置技能状态数
	void showSkillState(int index, int imageIndex);       //显示技能状态
	void hideSkillState(int index);					      //隐藏技能状态

	Ogre::String getRListName(){ return m_RankListName; }      //得到编辑的排行名称
	CEGUI::Window* getDragonDieNumW(){return m_DragonDieNumW;};		//得到目标AI死亡数量的窗口
	CEGUI::Window* getGDragonDieNumW(){ return m_GDragonDieNumW; };		//得到目标AI死亡数量的窗口
	CEGUI::Window* getSwordDieNumW(){ return m_SwordDieNumW; };		//得到目标AI死亡数量的窗口
	CEGUI::Window* getSrcDieScoreW(){ return m_DieScoreW; };		//得到主模型死亡分数显示的窗口	
	CEGUI::Window* getSrcBloodW(){ return m_BloodW; };			//得到主模型血量显示的窗口
	CEGUI::Window* getSrcBloodNumW(){ return m_BloodNumW; };     //得到血量具体数值显示的窗口
	CEGUI::Window* getFPSW(){ return m_FPSW; };                   //得到FPS显示的窗口
	CEGUI::Window* getQuitW(){ return m_QuitW; };                 //得到Quit退出窗口
	CEGUI::Window* getOneAgainW(){ return m_OneAgainW; }          //得到OneAgain窗口
	CEGUI::Window* getHeadW(){ return m_HeadW; }                  //得到头像窗口
	CEGUI::Window* getReturnMainW(){ return m_ReturnMainW; }      //得到返回主界面窗口
	CEGUI::Window* getRNameEditW(){ return m_RNameEditW; }        //得到排行榜名称编辑窗口
	CEGUI::Window* getOkW(){ return m_OkW; }                      //得到死亡计分板中的OK按钮
	CEGUI::Window* getOkAgainW(){ return m_OkAgainW; }            //得到名称确认后弹出再次OK确认的按钮
	CEGUI::Window* getNameAffirmW(){ return m_NameAffirmW; }      //得到名称确认后弹出的再次确认窗口
	CEGUI::Window** getRewardingTextW() { return m_RTextW; }      //得到排行榜奖赏的文本窗口
	CEGUI::Window** getSkillShadeW(){ return m_SkillShadeW; }     //得到技能遮罩窗口
	CEGUI::Window** getSkillCoolCalW(){ return m_SkillCoolCalW; } //得到技能冷却计时窗口
	CEGUI::Window* getMonitorFrameW(){ return m_MonitorFrameW; }  //得到监视器窗口
	CEGUI::Window* getMonitorW() { return m_MonitorW; }           //得到监视器
	CEGUI::Window** getSkillStateW(){ return m_SkillStateW; }     //得到技能状态窗口

private:
	bool Quit(const CEGUI::EventArgs & arg){ //播放点击音效
		m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel); 
		*m_ShutDown = true; return true;};   //退出事件
	bool Ok(const CEGUI::EventArgs & arg);        //确认名字输入
	bool EditNameChange(const CEGUI::EventArgs & arg); //编辑名称改变
};

#endif