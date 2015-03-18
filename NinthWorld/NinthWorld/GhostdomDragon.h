/*------------------------------------------------------------------
��ͷ�ļ�����GhostdomDragon.h		Created by Beyond Ray,2014��4��
����������һ����װ��ڤ������AI������Ϊ����
--------------------------------------------------------------------*/

#ifndef H_Mingjieyalong
#define H_Mingjieyalong

#include"ControlModel.h"
#include"Dragon.h"

//#define BSP_MODE

#define GD_CIRCLE_DISTANCE 500      //��������Ŀ��Ļ������߹�������
#define GD_BACK_ATTACK_DIS 400		 //���ܳɹ����ת�����������
#define GD_RUNAWAY_DISTANCE 150      //�ӽ�Ŀ��̫��ʱ�����߾��� 
#define GD_ATTACK_DISTANCE 380		 //��ʼ������Ŀ�귢�������ľ���
#define GD_DOWN_NEWLIFE_DIS 6000    //����ˤ��֮�󣬵���þ��������
#define DRAGON_MASK 1<<1		 //����ڤ������AI�Ĳ�ѯ����

struct Dragon_Object;
class GhostdomDragon
{
private:
	//����-1��1�ĸ��������
	inline float randFloat(){ return ((float)rand() / RAND_MAX)*2.0f - 1.0f; }
	//ڤ������AI���������ı���ʾ
	inline void DieNumChangeShow(){ m_NinthCEGUI->getGDragonDieNumW()->setText("GDragon: " + Ogre::StringConverter::toString(ms_iGDDieNum)); }

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
	GhostdomDragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, Dragon_Object &dragon, ControlModel* model, NinthCEGUI* ninthCEGUI);//���캯��
	GhostdomDragon(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Ogre::String meshName, ControlModel* model, NinthCEGUI* ninthCEGUI);
	void DragonInitialise(Dragon_Object &dragon);
	virtual ~GhostdomDragon();					//��������	
	bool frameRenderingQueued(const Ogre::FrameEvent & evt);	//����Ⱦ�����и���ڤ������AIÿһ֡����Ϊ
	void setPosition(Ogre::Vector3 positon);		//��������ڤ������AIλ��
	void setRandPosition(Ogre::Real range = 2000);	//����ڤ������AI�����λ��
	void setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range = 2000); //����ڤ������AI����Ŀ������λ��

private:
	ControlModel* m_Model;				//������ģ�ͣ�������ϵ
	NinthCEGUI* m_NinthCEGUI;			//CEGUI���ƴ���
	Ogre::String m_MeshName;			//������������

	SoundManager* m_SoundMgr;           //�����������ӿ�
	int m_iDieSd;                       //��������
	int m_iDieChannel;                  //��������ͨ��  
	int m_iScreenSd;					//����������������
	int m_iScreenChannel;				//������������
	int m_iThunderStormSd;				//�����ױ���������
	int m_iThunderStormChannel;         //�����ױ�����
	int m_iAureolaSd;				    //����������
	int m_iAureolaChannel;              //��ʢ�ĵ�����
	int m_iBiggerSd;					//���������������
	int m_iBiggerChannel;               //�����������
	int m_iHarmObjectSd;				//�˺��Է�����������
	int m_iHarmObjectChannel;           //�˺��Է�������
	int m_iJetSd;                       //���乥������������
	int m_iJetChannel;                  //���乥������������

	Ogre::SceneManager* m_SceneMgr;		//����������
	Ogre::SceneNode* m_AttackNode;		//��������Ŀ�������ڵ�λ��
	Ogre::SceneNode* m_TruthNode;		//ڤ������AIʵ�ʴ��ڵĽڵ�
	Ogre::Real m_Distance;				//��������Ŀ��ľ���
	Ogre::Vector3 m_Direcion;			//ڤ������AI������Ŀ��ķ���
	Ogre::SceneNode* m_DragonNode;		//ڤ������AI�ĳ����ڵ�
	Ogre::Entity* m_DraEnt;				//ڤ������AI��ʵ�����

	Dragon_Object m_Dragon;				//ڤ������AI�ĸ������Զ���
	Ogre::Real m_HP;					//ڤ������AI�ɿ��Ƶ�ӵ��Ѫ��

	Ogre::AnimationState* m_Fly;        //��ѡ����ж�����ѭ������1,2,3��
	Ogre::AnimationState* m_Die;		//������ʱ����������

	Ogre::ParticleSystem* m_Beam;		//��ѡ�񹥻�������4����ɫ��2����̬����8�֣�
	Ogre::ParticleSystem* m_RedSmoke;	//���������ʱ���ӵ��̻�Ч��

	Ogre::Quaternion m_Orbit;				//���������Ԫ��
	float m_fNewOrbitTime;                  //���²���һ���¹����ʱ�� 
	bool m_bRunFlag;						//ڤ������AI�����ܱ�־

	Ogre::ParticleSystem* m_Aureola;		//������ʱ��ӿ������

	Ogre::RaySceneQuery* m_RaySceneQuery;	//���߳�����ѯ
	Ogre::SphereSceneQuery* m_SphereSceneQuery;//���泡����ѯ
	bool m_bDead;							//�ж��Ƿ������ı�־
	bool m_bBigger;							//���������ʱ���ڷŴ�״̬֮��
	bool m_bSmaller;						//��������й������������ڵ���д״̬֮��

public:
	static int ms_iGDDieNum;					//ڤ������AI����������������ڤ������AI����һ������)

};

#endif