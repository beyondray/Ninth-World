/*------------------------------------------------------------------
��ͷ�ļ�����KnotAndFish.h       Created by Beyond Ray,2014��4��
����������һ����װ��Ŧ������ת���Ƶ���ĳ�������
--------------------------------------------------------------------*/

#ifndef	H_KnotAndFish
#define H_KnotAndFish
using namespace Ogre;

#include "ControlModel.h"

#define  KEEP_RAIN_MAX_TIME 60.0f   //�����������ʱ��
#define  RAIN_COOL_TIME 180.0f      //�������ȴʱ��
#define  REC_BLOOD_PER_SECOND 25    //����ʱÿ���ֻظ���Ѫ��
#define  KNOT_MASK 1<<4				//Ŧ����ѯ����
#define  SPHERE_MASK 1<<5			//����ѯ����
class KnotAndFish
{
public:
	KnotAndFish(Ogre::Root *root, Ogre::SceneManager *sceneMgr, SoundManager* soundMgr, ControlModel* model, 
		NinthCEGUI* ninthCEGUI) :m_NinthCEGUI(ninthCEGUI),m_Model(model), m_AttackNode(model->getMainNode()),
		m_SoundMgr(soundMgr), mFishNode0(NULL), mFishNode1(NULL), mFishNode2(NULL), mFishSwim0(NULL), mFishSwim1(NULL), 
		mFishSwim2(NULL), m_bRaining(false), m_bRecBloodState(false), m_bRainSkillPer(false),
		m_fKeepRainTime(RAIN_COOL_TIME),m_iRainChannel(INVALID_SOUND_CHANNEL), m_iBlisterChannel(INVALID_SOUND_CHANNEL),
		m_iReCoverChannel(INVALID_SOUND_CHANNEL)
	{
		m_SceneMgr = sceneMgr; 
		m_Root = root;
		m_RaySceneQuery = m_SceneMgr->createRayQuery(Ogre::Ray());
		m_RaySceneQuery->setSortByDistance(true);
	}
	virtual ~KnotAndFish(){ m_SceneMgr->destroyQuery(m_RaySceneQuery); };
	void initialise(float x, float y, float z);
	void initialise(Ogre::Vector3 vec);
	bool frameRenderingQueued(const FrameEvent & evt);
	bool BeJetCollDec(const FrameEvent & evt);                 //Ŧ�����ı��������ײ���
	bool RainThroughCollDec(const Ogre::FrameEvent & evt);     //�괩�����ǵ���ײ���

protected:
	ControlModel* m_Model;           //��ģ��
	Ogre::SceneNode* m_AttackNode;   //��ģ�ͽڵ�
	Ogre::RaySceneQuery* m_RaySceneQuery; //���߳�����ѯ
	Ogre::SphereSceneQuery* m_SphereSceneQuery; //���泡����ѯ
	NinthCEGUI* m_NinthCEGUI;       //NinthCEGUI�����
	float m_fKeepRainTime;          //����ĳ���ʱ��
	bool m_bRaining;                //��������ʱΪ��
	bool m_bRecBloodState;          //�ظ�����״̬
	bool m_bRainSkillPer;           //���꼼��Ȩ��

	SoundManager* m_SoundMgr;       //�����������ӿ�
	int m_iRainSd;                  //�������
	int m_iRainChannel;
	int m_iBlisterSd;               //ˮ�ݵ�����
	int m_iBlisterChannel;
	int m_iReCoverSd;               //�ָ�������
	int m_iReCoverChannel;
	SceneNode* mKnotNode;			//Ŧ���ĳ����ڵ�

	SceneNode* mFishNode0;
	SceneNode* mFishNode1;
	SceneNode* mFishNode2;			//������ĳ����ڵ�
	
	AnimationState* mFishSwim0;
	AnimationState* mFishSwim1;
	AnimationState* mFishSwim2;		//������Ķ���

	Root *m_Root;
	SceneManager* m_SceneMgr;		//���ͳ�������

	Vector3 mPosition;				//Ŧ�����㳡���������λ��

	Ogre::ParticleSystem* m_Rain;    //���������Ч
};

#endif