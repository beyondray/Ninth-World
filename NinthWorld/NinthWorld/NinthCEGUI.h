#ifndef H_NinthCEGUI
#define H_NinthCEGUI

#include "MyCEGUISystem.h"

class NinthCEGUI
{
private:
	SoundManager* m_SoundMgr;               //����������
	int m_iClickSd;		 //����ĵ����Ч����
	int m_iClickChannel; //��������Ч������

	CEGUI::Window* m_DragonDieNumW;			//��ʾ����AI���������Ĳ�������
	CEGUI::Window* m_GDragonDieNumW;		//��ʾڤ������AI���������Ĳ�������
	CEGUI::Window* m_SwordDieNumW;			//��ʾ����AI���������Ĳ�������
	CEGUI::OgreRenderer* m_RendererW;		//����OGRE��CEGUI��Ⱦ��
	CEGUI::Window* m_DieScoreW;				//��ģ��������������
	CEGUI::Window* m_BloodW;				//Ѫ������
	CEGUI::Window* m_HeadW;                  //ͷ�񴰿�
	CEGUI::Window* m_BloodNumW;              //Ѫ��������ֵ����
	CEGUI::Window* m_FPSW;                   //FPS����
	CEGUI::Window* m_OneAgainW;              //����һ�δ���
	CEGUI::Window* m_QuitW;                  //�˳�����
	CEGUI::Window* m_ReturnMainW;            //���������洰��
	CEGUI::Window* m_OkW;                    //ȷ�����ƴ���
	CEGUI::Window* m_RNameEditW;             //�������Ʊ༭����
	CEGUI::Window* m_OkAgainW;               //�ٴ�ȷ�����ƴ���
	CEGUI::Window* m_NameAffirmW;            //����ȷ�Ϻ󵯳����ٴ�ȷ�ϴ���
	Ogre::String m_RankListName;			 //������
	CEGUI::Window* m_MonitorFrameW;          //���������
	CEGUI::Window* m_MonitorW;               //������
	CEGUI::Window* m_RTextW[10];			 //10�������ı�����(ǰ5�����κ���������5������)
	CEGUI::Window* m_SkillShadeW[8];         //8���������ִ���
	CEGUI::Window* m_SkillCoolCalW[8];       //6��������ȴ��ʱ����(1,3����ʱ����)
	CEGUI::Window* m_SkillStateW[3];         //3������״̬����
	int m_iSkillStateNum;                    //����״̬���ۼ�
	int m_iSSIndex[3];                       //����״̬�������ֱ��Ӧ3������״̬��ʾ��

	Ogre::RenderWindow* m_Window;
	bool* m_ShutDown;
	float ratio;                          //���ں��ݱ�

public:
	NinthCEGUI(Ogre::RenderWindow* window, bool* shutdown, SoundManager* soundMgr);	//���캯��
	~NinthCEGUI();						//��������
	void CEGUICreateAndInitialise();	//CEGUI�����ͳ�ʼ��
	void CreateDragonDieNumW();			//������ʾ����AI���������Ĵ���
	void CreateGDragonDestDieNumW();	//������ʾڤ������AI���������Ĵ���
	void CreateSwordDieNumW();			//������ʾ����AI���������Ĵ���
	void CreateSrcBloodShowW(float ratio = 1.6f);//������ģ��Ѫ����ʾ�Ĵ���	
	void CreateSrcDieScoreW();     //������ʾ��ģ������ʱ������ʾ�Ĵ���
	void CreateFPSW(float ratio = 1.6f); //������ʾFPS�Ĵ���
	void CreateRewardingTextW();        //������������ı��Ĵ���
	void CreateSShadeAndCoolCW();       //�����������ֺͼ�����ȴ��ʱ����
	void CreateMonitorW();              //��������������
	void ShowAllInterfaceW();           //��ʾ���н��洰��
	void HideAllInterfaceW();           //�������н��洰��

	int getSkStateNum(){ return m_iSkillStateNum; }       //�õ��ۼƵļ���״̬��
	int setSkStateNum(int num){ m_iSkillStateNum = num; } //���ü���״̬��
	void showSkillState(int index, int imageIndex);       //��ʾ����״̬
	void hideSkillState(int index);					      //���ؼ���״̬

	Ogre::String getRListName(){ return m_RankListName; }      //�õ��༭����������
	CEGUI::Window* getDragonDieNumW(){return m_DragonDieNumW;};		//�õ�Ŀ��AI���������Ĵ���
	CEGUI::Window* getGDragonDieNumW(){ return m_GDragonDieNumW; };		//�õ�Ŀ��AI���������Ĵ���
	CEGUI::Window* getSwordDieNumW(){ return m_SwordDieNumW; };		//�õ�Ŀ��AI���������Ĵ���
	CEGUI::Window* getSrcDieScoreW(){ return m_DieScoreW; };		//�õ���ģ������������ʾ�Ĵ���	
	CEGUI::Window* getSrcBloodW(){ return m_BloodW; };			//�õ���ģ��Ѫ����ʾ�Ĵ���
	CEGUI::Window* getSrcBloodNumW(){ return m_BloodNumW; };     //�õ�Ѫ��������ֵ��ʾ�Ĵ���
	CEGUI::Window* getFPSW(){ return m_FPSW; };                   //�õ�FPS��ʾ�Ĵ���
	CEGUI::Window* getQuitW(){ return m_QuitW; };                 //�õ�Quit�˳�����
	CEGUI::Window* getOneAgainW(){ return m_OneAgainW; }          //�õ�OneAgain����
	CEGUI::Window* getHeadW(){ return m_HeadW; }                  //�õ�ͷ�񴰿�
	CEGUI::Window* getReturnMainW(){ return m_ReturnMainW; }      //�õ����������洰��
	CEGUI::Window* getRNameEditW(){ return m_RNameEditW; }        //�õ����а����Ʊ༭����
	CEGUI::Window* getOkW(){ return m_OkW; }                      //�õ������Ʒְ��е�OK��ť
	CEGUI::Window* getOkAgainW(){ return m_OkAgainW; }            //�õ�����ȷ�Ϻ󵯳��ٴ�OKȷ�ϵİ�ť
	CEGUI::Window* getNameAffirmW(){ return m_NameAffirmW; }      //�õ�����ȷ�Ϻ󵯳����ٴ�ȷ�ϴ���
	CEGUI::Window** getRewardingTextW() { return m_RTextW; }      //�õ����а��͵��ı�����
	CEGUI::Window** getSkillShadeW(){ return m_SkillShadeW; }     //�õ��������ִ���
	CEGUI::Window** getSkillCoolCalW(){ return m_SkillCoolCalW; } //�õ�������ȴ��ʱ����
	CEGUI::Window* getMonitorFrameW(){ return m_MonitorFrameW; }  //�õ�����������
	CEGUI::Window* getMonitorW() { return m_MonitorW; }           //�õ�������
	CEGUI::Window** getSkillStateW(){ return m_SkillStateW; }     //�õ�����״̬����

private:
	bool Quit(const CEGUI::EventArgs & arg){ //���ŵ����Ч
		m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel); 
		*m_ShutDown = true; return true;};   //�˳��¼�
	bool Ok(const CEGUI::EventArgs & arg);        //ȷ����������
	bool EditNameChange(const CEGUI::EventArgs & arg); //�༭���Ƹı�
};

#endif