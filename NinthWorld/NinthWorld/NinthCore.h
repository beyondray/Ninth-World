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
static const int SWORD_TO_CENTER_DIS = 3000;	//���󵽳������ĵľ���

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
	bool StartGame(const CEGUI::EventArgs& arg); //������ʼ��Ϸ��������Ϸ״̬
	bool KeyHelp(const CEGUI::EventArgs& arg);   //�����λ��������
	bool SwordSkill(const CEGUI::EventArgs& arg);//���뽣ϵ���ܽ��ܽ���
	bool RankList(const CEGUI::EventArgs& arg);  //���а�
	bool QuitGame(const CEGUI::EventArgs& arg);  //�˳���Ϸ
	bool Back(const CEGUI::EventArgs& arg);      //����������
	bool OneAgain(const CEGUI::EventArgs & arg); //����һ��
	bool ReturnMain(const CEGUI::EventArgs & arg);//����������
	bool Ok(const CEGUI::EventArgs & arg);        //ȷ����������
	bool EditNameChange(const CEGUI::EventArgs & arg); //�༭���Ƹı�
	bool CMapOk(const CEGUI::EventArgs & arg);    //ȷ��ѡ���ͼ
	bool ChooseMap(const CEGUI::EventArgs & arg); //ѡ���ͼ
	bool CMapBack(const CEGUI::EventArgs & arg);  //��ѡ���ͼ���淵��

public:
	NinthCore(void);
	virtual ~NinthCore(void);

private:
	KnotAndFish* m_Knotfish;		  //Ŧ������ľֲ�����
	ControlModel* m_Model;			  //��ģ�Ϳ���
	NinthCEGUI* m_NinthCEGUI;		  //CEGUI���ڼ���
	SoundManager* m_SoundMgr;         //����������

	int m_iSongSd1;                   //��һ�ױ���������������
	int m_iSongChannel1;              //��һ�ױ�����������     
	int m_iSongSd2;                   //�ڶ��ױ���������������
	int m_iSongChannel2;              //�ڶ��ױ�����������
	int m_iSongSd3;                   //�����ױ���������������
	int m_iSongChannel3;              //�����ױ�����������
	int m_iSongingChannel;            //���ڲ��ŵı������ֵ�ͨ��

	Dragon *m_DragonAI[DRAGON_NUM/2];		//��ָ����������������AI
	GhostdomDragon* m_GDragonAI[DRAGON_NUM / 2];	//ڤ������AIָ��
	Sword* m_SwordWaitAI[81];				//��ʼʱλ�ھŰѴ���ߵĽ���AI
	Sword* m_SwordAI[SWORD_NUM];			//��ʼʱ����׷�ٹ���Ŀ��Ľ���AI
	Ogre::SceneNode* m_SwordNode[9];		//ָ��ŰѴ󽣵ĳ����ڵ�ָ��

	Dragon_Object m_Dragon;				//����AI�������Զ���
	Sword_Object m_Sword;				//����AI�������Զ���

	//�������
	CEGUI::Window* m_MainWin;           
	CEGUI::Window* m_StartGameW;		//��ʼ��Ϸ��ť����
	CEGUI::Window* m_StartGameTW;       //��ʼ��Ϸ�ı�����
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

	int m_iClickSd;		 //����ĵ����Ч����
	int m_iClickChannel; //��������Ч������

	int m_iGUIMusicSd;     //GUI������������
	int m_iGUIMusicChannel;//GUI������������

	int m_iChooseMapSd;     //ѡ���ͼ��Ч����
	int m_iChooseMapChannel;//ѡ���ͼ��Ч����
};

#endif // #ifndef __NinthCore_h_