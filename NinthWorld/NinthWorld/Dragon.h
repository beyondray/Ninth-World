/*------------------------------------------------------------------
��ͷ�ļ�����Dragon.h		Created by Beyond Ray,2014��4��
����������һ����װ���������׹�˹AI������Ϊ����
--------------------------------------------------------------------*/

#ifndef H_Dragon
#define H_Dragon

#include "SoundManager.h"
#include"ControlModel.h"

//#define BSP_MODE

#define DRAGON_CIRCLE_DISTANCE 500      //��������Ŀ��Ļ������߹�������
#define DRAGON_BACK_ATTACK_DIS 400		 //���ܳɹ����ת�����������
#define DRAGON_RUNAWAY_DISTANCE 150      //�ӽ�Ŀ��̫��ʱ�����߾��� 
#define DRAGON_ATTACK_DISTANCE 380		 //��ʼ������Ŀ�귢�������ľ���
#define DRAGON_DOWN_NEWLIFE_DIS 6000    //����ˤ��֮�󣬵���þ��������
#define DRAGON_MASK 1<<1		 //��������AI�Ĳ�ѯ����
struct Dragon_Object
{
	int m_Index;						//�����ʶ
	Ogre::Real m_MaxHP;					//ӵ�е����Ѫ��
	Ogre::Real m_Damage;				//�������˺�
	Ogre::Real m_Defense;				//������ʱ�ķ���
	Ogre::Real m_FlySpeed;				//���е��ٶ�
	Ogre::Vector3 m_Position;			//���ֵ�λ��
	Ogre::Vector3 m_Scale;				//�����ı�����С
	int m_FlyIndex;						//���ж�����ѭ��)��������0,1,2)
	int m_JetIndex;						//���乥��������������0:��ɫ,1:��ɫ,2:��ɫ,3:��ɫ)
};

class Dragon 
{
private:
	//����-1��1�ĸ��������
	inline float randFloat(){ return ((float)rand() / RAND_MAX)*2.0f - 1.0f; }
	//����AI���������ı���ʾ
	inline void DieNumChangeShow(){m_NinthCEGUI->getDragonDieNumW()->setText("Dragon: " + Ogre::StringConverter::toString(ms_iDDieNum));}

	void RotateAndAttack(const Ogre::FrameEvent & evt, float roateRate = 1.0f,
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);		//�������ƹ���

	void RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		const Ogre::Vector3 & dirRotate, Ogre::Vector3 dirGo, bool rotateResolve = true, 
		float rotatePre = 30.0f, bool toSkyOriSet = true, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);	
																			//��ת������Ŀ��ǰ��
	void RoateAndDieRotate(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float roatePre = 30.0f,
		float roateSpeed = 10.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);		
																			//��ת������Ŀ��ǰ��

	bool BeJetCollDec();				//�����乥������ײ���
	bool JecCollDec();					//���乥������ײ���
	bool BeThroughCollDec(const Ogre::FrameEvent & evt);//�Խ���Ĵ�͸�������б����м��
	bool ScaleChange(const Ogre::FrameEvent &evt, float limitScale, float changeRate = 3.0f, bool bigger = true); 
										//��ʱ��Ա�����С���иı�(biggerΪtrue��Ŵ�,false����С)
	inline Ogre::Vector3 ChangePosToWorld(const Ogre::Vector3 &pos) const;	
	//�����ڳ������ڵ������ת��Ϊ���������(ǰ�᣺�������ڵ�pitch��PI/2)

	void ChooseBeam(Ogre::ParticleSystem*& beam, int index);         //ѡ�����ӹ���(index��0��7)

public:
	Dragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, Dragon_Object &dragon, ControlModel* model, NinthCEGUI* ninthCEGUI);//���캯��
	Dragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, ControlModel* model, NinthCEGUI* ninthCEGUI);
	void DragonInitialise(Dragon_Object &dragon);
	virtual ~Dragon();					//��������	
	bool frameRenderingQueued(const Ogre::FrameEvent & evt);	//����Ⱦ�����и�������AIÿһ֡����Ϊ
	void setPosition(Ogre::Vector3 positon);		//������������AIλ��
	void setRandPosition(Ogre::Real range = 2000);	//��������AI�����λ��
	void setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range = 2000); //��������AI����Ŀ������λ��

private:
	ControlModel* m_Model;				//������ģ�ͣ�������ϵ
	NinthCEGUI* m_NinthCEGUI;			//CEGUI���ƴ���
	Ogre::String m_MeshName;			//������������

	SoundManager* m_SoundMgr;           //�����������ӿ�
	int m_iDieSd;                       //������������
	int m_iDieChannel;                  //��������ͨ��      
	int m_iScreenSd;					//����������������
	int m_iScreenChannel;				//������������
	int m_iThunderStormSd;				//�����ױ���������
	int m_iThunderStormChannel;         //�����ױ�����
	int m_iAureolaSd;					//����������
	int m_iAureolaChannel;              //�����ĵ�����
	int m_iBiggerSd;					//���������������
	int m_iBiggerChannel;               //�����������
	int m_iHarmObjectSd;				//�˺��Է�����������
	int m_iHarmObjectChannel;           //�˺��Է�������
	int m_iJetSd;                       //���乥������������
	int m_iJetChannel;                  //���乥������������

	Ogre::SceneManager* m_SceneMgr;		//����������
	Ogre::SceneNode* m_AttackNode;		//��������Ŀ�������ڵ�λ��
	Ogre::Real m_Distance;				//��������Ŀ��ľ���
	Ogre::Vector3 m_Direcion;			//����AI������Ŀ��ķ���
	Ogre::SceneNode* m_DragonNode;		//����AI�ĳ����ڵ�
	Ogre::Entity* m_DraEnt;				//����AI��ʵ�����

	Dragon_Object m_Dragon;				//����AI�ĸ������Զ���
	Ogre::Real m_HP;					//����AI�ɿ��Ƶ�ӵ��Ѫ��

	Ogre::AnimationState* m_Fly;        //��ѡ����ж�����ѭ������1,2,3��
	Ogre::AnimationState* m_Die;		//������ʱ����������

	Ogre::ParticleSystem* m_Beam;		//��ѡ�񹥻�������4����ɫ��2����̬����8�֣�
	Ogre::ParticleSystem* m_RedSmoke;	//���������ʱ���ӵ��̻�Ч��

	Ogre::Quaternion m_Orbit;				//���������Ԫ��
	float m_fNewOrbitTime;                  //���²���һ���¹����ʱ��                    
	bool m_bRunFlag;						//����AI�����ܱ�־

	Ogre::ParticleSystem* m_Aureola;		//������ʱ��ӿ������

	Ogre::RaySceneQuery* m_RaySceneQuery;	//���߳�����ѯ
	Ogre::SphereSceneQuery* m_SphereSceneQuery;//���泡����ѯ
	bool m_bDead;							//�ж��Ƿ������ı�־
	bool m_bBigger;							//���������ʱ���ڷŴ�״̬֮��
	bool m_bSmaller;						//��������й������������ڵ���д״̬֮��
public:
	static int ms_iDDieNum;					//����AI��������������������AI����һ������)

};

#endif