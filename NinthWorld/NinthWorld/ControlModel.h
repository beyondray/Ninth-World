/*-------------------------------------------------------------
��ͷ�ļ�����ControlModel.h   Created by Beyond Ray,2014��4��
����������һ����װ����ģ�ͻ�������ϵͳ����
---------------------------------------------------------------*/

#ifndef H_ControlModel
#define H_ControlModel

#include"NinthCEGUI.h"
#include"SoundManager.h"
#include"MeshAndRayCast.h"
#include"fstream"

#define MODEL_MAX_BLOOD 3000.0f //����ģ�͵����Ѫ��
#define MODEL_DAMAGE 200.0f     //ģ�͵�����˺�
#define ROTATE_SPEED 0.005f		//�������ת�ӽ��ٶ�
#define ROTATE_RATE 1	        //ģ����б��ת�ı���
#define ROTATE_MAX_LIMIT 0.3f   //ģ����б��ת������޶�
#define COV_INTERVAL_MIN 1.0f	//���ж����л�֮�����С����ʱ����
#define MAINMODEL_MASK 1<<0		//��ģ�����������ѯ
#define INVICIBLE_MASK 1<<9     //��ʾ�޵�״̬����������

#define MOVE_SPEED  270.0f		 //������ƶ��ٶ�
#define MOVE_ACCELERATION  5.0f  //ģ�͵��ƶ����ٶ�
#define MOVE_LIMITSPEED	  300.0f //ģ�͵���������ƶ��ٶ�

#define KEYDOWN_INTERVAL  0.2f    //����������Чʱ��
#define MOVE_HELIC_ACCEL  50.0f   //����������̬ʱ�ļ��ٶ�
#define MOVE_HELIC_LIMITSPEED 500.0f  //����������̬ʱ���ƶ��ٶ� 
#define HELIC_MAX_TIME	10.0f     //���������������ʱ��
#define HELIC_COOL_TIME 15.0f    //������������ȴʱ��

#define DIVINE_SWORD_NUM  200	  //"�򽣹���"������ʹ�ý�������
#define DIVINE_RISE_DIS 100		  //"�򽣹���"������ʹ�ý��ĳ�ʼ����������������ģ�͵�ƫ����� 
#define DIVINE_CYLINDER_FIELD_SIZE 50//"�򽣹���"���ܽ������ʱ��ʼԲ���ռ������߶Ⱥ�ֱ��
#define DIVINE_MOVE_FIELD_DIS 45  //�򽣹���"���ܽ���ƫ�����ռ���������

class Sword;
class ControlModel
{
private:
	bool CheckAndDieBehavior(const Ogre::FrameEvent & evt);		//���������־,�������������������
	bool BspCollDec(const Ogre::FrameEvent & evt);				//��Bsp��ͼ֮�����ײ���
	bool DivineSword4(const Ogre::FrameEvent & evt);			//"�򽣹���"������
	bool PlayUnDeadKillSound(int killNum); //���Ų�������ɱ¾��Ч
	bool PlayUnStopKillSound(int killNum, const Ogre::FrameEvent&evt); //���Ŷ�ʱ����ɱ��Ч

	void RoateAndDieRotate(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 30.0f,
		float dieRoateSpeed = 10.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
	bool ScaleChange(const Ogre::FrameEvent &evt, float limitScale,
		float changeRate = 3.0f, bool bigger = true);
	void DieShowText(int score, int * existScore);					//����������ʾ
	bool FileAndRankLUpdate(const CEGUI::EventArgs & arg);          //�ļ����ݼ����а����ݸ���
	void CreateMonitor();                                           //����������

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

	Ogre::SceneNode* getMainNode(){ return m_MainNode; }	//�õ���ģ�͵����糡���ڵ�
	Ogre::Entity* getMainEntity(){ return m_Model; }		//�õ���ģ�͵�ʵ�����
	Ogre::Entity* getMeiZhiEntity(){ return m_MeiZhi; }     //�õ���ֽ��ʵ�����
	Ogre::Entity* getYunTianHeEntity(){ return m_YunTianHe; }//�õ�����ӵ�ʵ�����
	bool getRMouseDown() const;			//��������Ҽ�״̬
	bool getLMouseDown() const;			//����������״̬

	Ogre::Real getModelHP() const{ return m_HP; }				//������ģ�͵�Ѫ��
	void setModelHP(Ogre::Real HPValue) { m_HP = HPValue; }		//������ģ�͵�Ѫ��
	void cutModelHP(Ogre::Real HPValue) { m_HP -= HPValue; }	//����ģ��Ѫ��

	bool getDieFlag ()const{ return m_bDead; }			//�õ������жϱ�־
	void setDieFlag(bool dead){ m_bDead = dead; }		//����������־

	int getScore() const;		//ȡ���ѻ�õķ���
	int getLaunchIndex() const{ return m_LaunchIndex; }			//ȡ�÷��佣�������
	void setLaunchIndex(int index){ m_LaunchIndex = index; }	//���÷��佣�������
	bool getDivine4State() const { return m_bDivineSword4; }	//����"�򽣹���"ִ��״̬����ֵ
	void setDivine4State(bool moveState){ m_bDivineSword4 = moveState; }  //����"�򽣹���"ִ��״̬�Ĳ���ֵ

	Ogre::Real getDivineTime() const { return m_DivineTime; }	//�õ�"���¾Ž�,�򽣹���"����������ʱ��
	void addDivineTime(const Ogre::FrameEvent & evt){ m_DivineTime += evt.timeSinceLastFrame; }
																//����"���¾Ž�,�򽣹���"����������ʱ��
	void setDivineTime(Ogre::Real time){ m_DivineTime = time; } //����"���¾Ž�,�򽣹���"����������ʱ��
	void setRecBloodPar(bool emitting){ m_RecBlood->setEmitting(emitting); } //���������ظ�������Ч״̬
	void setSmallerState(bool smaller){ m_bSmaller = smaller; } //������С״̬
	void setDivSkillPer(bool permission){ m_bDivSkillPer = permission; } //�����򽣹��ڶ��¾Ž�����Ȩ��

private:
	Ogre::String m_CharName;    //��ɫ����
	Ogre::Real m_HP;			//��ģ��Ѫ��
	bool m_bDead;				//��ģ��������־	
	bool m_bDieShow;			//Ϊtrueʱ,ģ�������������
	bool m_bBeginSoundPlayed;    //��Ϸ��ʼʱ�Ѳ���������
	bool m_bDieSoundPlayed;      //����ʱ�Ѳ���������  
	bool m_bDieScoreWShow;       //�����ƷֶԻ������ʾ
	Ogre::Real m_DownDistance;   //����ʱ�������
	float m_fIntervalTime;        //��ɱ���ʱ��        
	int m_iUnStopKillNum;         //������ɱ��
	Ogre::Root* m_Root;           //��ȾԴ
	Ogre::SceneManager* m_SceneMgr;		//����������
	NinthCEGUI* m_NinthCEGUI;			//CEGUI���ڿ���

	SoundManager* m_SoundMgr;           //�����������ӿ�

	int m_iShootSd;						//�������������������
	int m_iShootChannel;                //���������������                       
	int m_iHeartBeatSd;                 //Ѫ������ʱ����������������
	int m_iHeartBeatChannel;            //Ѫ������ʱ��������������
	int m_iGameBeginSd;                 //��Ϸ��ʼʱ����������
	int m_iGameBeginChannel;            //��Ϸ��ʼʱ����������
	int m_iGameOverSd;					//��ɫ����ʱ����������
	int m_iGameOverChannel;             //��ɫ����ʱ����������
	int m_iHelicSd;                     //����������̬����������
	int m_iHelicChannel;                //����������̬����������
	int m_iFanWingsSd;                  //ɿ��������������
	int m_iFanWindsChannel;             //ɿ���������������
	int m_iGlideSd;                     //�������������
	int m_iGlideChannel;                //���������������

	//������ɱ
	int m_iFirstBloodSd;                //��һ��Ѫ
	int m_iKillingSpreeSd;              //�񻶵�ɱ¾
	int m_iDominatingSd;                //���ؾ���
	int m_iMegaKillSd;                  //���µ�ɱ¾
	int m_iUnstoppableSd;               //����ֹͣ
	int m_iWickedSickSd;                //��̬ɱ¾
	int m_iMonsterKillSd;               //���ߵ�ɱ¾
	int m_iGodLikeSd;                   //���ϵ�һ���
	int m_iHolyShitSd;                  //��ʥ��
	//ʱ����ɱ
	int m_iDoubleKillSd;                //˫ɱ
	int m_iTripleKillSd;                //��ɱ
	int m_iUltraKillSd;                 //���˵�����ɱ¾
	int m_iRampageSd;                   //��ɱ��ŭ
	int m_iOwnageSd;                    //��ɱ����

	//������ɱ����
	int m_iFirstBloodChannel;           //��һ��Ѫ����
	int m_iKillingSpreeChannel;         //�񻶵�ɱ¾����
	int m_iDominatingChannel;           //���ؾ�������
	int m_iMegaKillChannel;             //���µ�ɱ¾����
	int m_iUnstoppableChannel;          //����ֹͣ����
	int m_iWickedSickChannel;           //��̬ɱ¾����
	int m_iMonsterKillChannel;          //���ߵ�ɱ¾����
	int m_iGodLikeChannel;              //���ϵ�һ�������
	int m_iHolyShitChannel;             //��ʥ������
	//ʱ����ɱ����
	int m_iDoubleKillChannel;           //˫ɱ����
	int m_iTripleKillChannel;           //��ɱ����
	int m_iUltraKillChannel;            //���˵�����ɱ¾����
	int m_iRampageChannel;              //��ɱ��ŭ����
	int m_iOwnageChannel;               //��ɱ��������
	int m_iKillChannel;                 //ɱ¾��������������

	int m_iPreKillNum;                  //��¼��ǰ�Ļ�ɱ��Ŀ

	Ogre::Camera* m_Camera;				//ģ�Ϳ��������
	Ogre::Camera* m_Monitor;            //ģ�ͼ�����
	Ogre::Vector3 m_Direction;			//������ƶ�����

	Ogre::Entity* m_Model;				//ģ��ʵ��(��������)
	Ogre::Entity* m_MeiZhi;				//ģ��ʵ��(�����ϵ���ֽ)
	Ogre::Entity* m_YunTianHe;			//ģ��ʵ��(�����ϵ������)		

	Ogre::SceneNode* m_MainNode;        //�����������ģ�͵����ڵ�
	Ogre::SceneNode* m_CamNode;			//��������ƽڵ�
	Ogre::SceneNode* m_ModelNode;		//ģ�Ϳ��ƽڵ�(��������)
	Ogre::SceneNode* m_MonitorNode;     //���������ƽڵ�

	Ogre::AnimationState* m_Fly1;       //���ж���1��ѭ����
	Ogre::AnimationState* m_Fly1to2;    //���ж���1��2�Ĺ���
	Ogre::AnimationState* m_Fly2;       //���ж���2��ѭ����
	Ogre::AnimationState* m_Fly2to3;    //���ж���2��3�Ĺ���
	Ogre::AnimationState* m_Fly3;       //���ж���3��ѭ����
	Ogre::AnimationState* m_Flying;     //���ڷ��еĶ���״̬
	bool m_bLeft;                       //����A��״̬ʱΪ��
	bool m_bRight;                      //����D��״̬ʱΪ��
	bool m_bLGlide;                     //�����־(A����ʱΪtrue)
	bool m_bRGlide;                     //�һ����־(D����ʱΪtrue)
	Ogre::Real m_rotateSum;             //��ת��б������ۼƺ�

	Ogre::ParticleSystem* m_RedBeam;	//��ɫ��������ϵͳ
	Ogre::ParticleSystem* m_Aureola;    //����������̬ʱ����������
	Ogre::ParticleSystem* m_RecBlood;   //�ָ�������������Ч

	bool m_bMRight;						//�ж�����Ҽ������µı�־
	bool m_bMLeft;						//�ж������������µı�־	
	bool m_bHelicState;					//���ھ�������״̬
	bool m_bSwordMoved;					//������ƶ�״̬�������Ϊ�棩
	bool m_bDivineSword4;				//"�򽣹���"�����ͷ�״̬
	bool m_bBigger;                     //���ڲ��ϱ��״̬
	bool m_bSmaller;					//���ڲ�����С״̬
	bool m_bHelSkillPer;                //������������Ȩ��
	bool m_bDivSkillPer;                //�򽣹��ڶ��¾Ž�����Ȩ��
	bool m_bJetFirePer;                 //��������Ȩ��
	
	Ogre::Real m_rotate;                //ģ�͵���б��ת��
	Ogre::Real m_ConvInterval;			//���ж����л�����������ʱ��
	Ogre::Real m_WKeyInterval;			//���ϴΰ���W����������ʱ��
	Ogre::Real m_HelicTime;				//ʹ�þ���������̬��������ʱ��
	Ogre::Real m_DivineTime;			//�����¾Ž�,�򽣹��ڡ����ܿ�������������ʱ��
	Ogre::Real m_lMouseTime;             //���������µ�ʱ��
	int m_FlyFlag;						//���ж�����־	
	int m_CycleIndex;                   //ѭ��������

	Ogre::Real m_ChangeSpeed;           //�ı����ƶ��ٶ�
	Ogre::Radian m_limitRad;			//��ת���ƻ���

	Ogre::RaySceneQuery* m_RaySceneQuery; //�������߲�ѯ

	int m_LaunchIndex;					//�����佣�������,0~8 
	Sword* m_DivineSword[DIVINE_SWORD_NUM];  //ָ���򽣵�ָ��(����Ƹý�Ϊ�����ܽ��塱��

	int m_ExistScore[5];                //�Ѵ��ڵ����а�ǰ5������
	Ogre::String m_ExistName[5];        //�Ѵ��ڵ����а�ǰ5������
	bool m_bChangeExistScore;           //�Ƿ�ı��Ѵ��ڷ���
	int m_RLUpdateIndex;                //���а����λ�õ�����

	int m_iClickSd;		 //����ĵ����Ч����
	int m_iClickChannel; //��������Ч������

};


#endif