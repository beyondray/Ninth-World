/*------------------------------------------------------------------
��ͷ�ļ�����Sword.h		Created by Beyond Ray,2014��4��
����������һ����װ�˽���AI������Ϊ����
--------------------------------------------------------------------*/

#ifndef H_Sword
#define H_Sword

#include"ControlModel.h"

#define SWORD_ATTACK_DISTANCE 600     //ĳЩ���岻�Ϲ�������Ϊ�ٴη���������Բ����
#define SWORD_DEFENSE_DISTANCE 700	 //������ʶ�ķ������򣨽���������ڣ��ͻᷢ��������
#define SWORD_STOP_DISTANCE 800		 //ĳЩ����ֻ����һ�ι����������������Ŀ��þ����ͣ��
#define SWORD_HOMING_DISTANCE 20	 //�����λʱ���뽣�˵���Զ����
#define SWORD_LAUNCH_VALID_DIS 1800   //���鷢��������Ч����,�����þ����ʧЧ
#define HELSWORD_LAUNCH_VALID_DIS 200 //����������������Ч����,�����þ����ʧЧ
#define SWORD_NORMAL_MASK 1<<2		 //������ͨ����AI�Ĳ�ѯ����
#define SWORD_JIANLING_MASK 1<<3     //���ý���AI�Ĳ�ѯ����
#define SWORD_PUPPET_MASK 1<<6       //���ܽ���Ĳ�ѯ����

#define SWORD_ROTATE_SPEED 0.003f	 //����������ƫ���ٶ�
#define SWORD_MAX_DENFENSE_TIME 10.0f //������ת�������������������ʱ��
#define SWORD_DEFENSE_COOL_TIME 60.0f //������ת�������ܵ���ȴʱ��
#define SWORD_DIVINE_COOL_TIME 30.0f  //"���¾Ž�,�򽣹���"���ܵ���ȴʱ��
#define SWORD_HELSWORD_KEEP_TIME 14.0f //���������ܳ���ʱ��
#define SWORD_RISE_MAX_HEIGHT  100	 //"���¾Ž�,�򽣹���"���ܷ���ʱ���������������߶�
#define SWORD_SHOOT_DEVIATE_DIS 45   //"���¾Ž�,�򽣹���"���ܵڶ�������㵽���ĵ�ƫ�����
#define SWORD_DIVINE2_SCALE 4		 //"���¾Ž�,�򽣹���"���ܵڶ��η���ʱ����ı�����С
#define SWORD_DIVINE3_SWEEP_DIS 800  //"���¾Ž�,�򽣹���"���ܵ����η���ʱ�����ɨ�ľ���
#define SWORD_DIVINE4_WAIT_TIME 10   //"���¾Ž�,�򽣹���"���ܵ��Ķ��������ͷŵ����ȴ�ʱ��(������ʱ���Զ��ͷ�)

//--------------------------------�����������֡���----------------------------------------
// ��һΪһ�ι����������������Զ��ͣ���������жϵ��Է��������������Ż��ٴη�������
// ���Ϊ���ع�������������Ǳ��ʶΪ�����ٴι�������������ٷ�������,����Ŀ���뿪�˷�������
// ����Ϊ��׷���ὣ, �����󵽴��ٴι���������룬Ȼ���ٴη���������׷�������ӷ�������,
// ���޹�����ֱ��������
//----------------------------------------------------------------------------------------
struct Sword_Object
{
	int m_ObjectIndex;					//�����ʶ(����ÿ������Ψһ)
	int m_CategoryIndex;				//�����ʶ(�ܹ���16��)
	Ogre::Real m_MaxHP;					//ӵ�е����Ѫ��
	Ogre::Real m_Damage;				//�������˺�
	Ogre::Real m_Defense;				//������ʱ�ķ���
	Ogre::Real m_FlySpeed;				//���е��ٶ�
	Ogre::Vector3 m_Position;			//���ֵ�λ��
	Ogre::Vector3 m_Scale;				//����ı�����С
	bool m_bOneAttack;					//�Ƿ�Ϊ���ι�������
	bool m_bNoStopAttack;				//�Ƿ�Ϊ��׷���ὣ
	//��ֻҪ����������򣬲��������ݣ�<m_bNoStopAttack��m_bOneAttack���ɵ��ӣ��������ֵ����ֳ�;��׷��>
	bool m_bAttackRotate;				//���幥��ʱ�Ƿ���ת
};

class Sword
{
private:
	//����-1��1�ĸ��������
	inline float randFloat(){ return ((float)rand() / RAND_MAX)*2.0f - 1.0f; }
	//����AI���������ı���ʾ
	inline void DieNumChangeShow(){ m_NinthCEGUI->getSwordDieNumW()->setText("Sword: " + Ogre::StringConverter::toString(ms_iDieNum)); }

	void RotateAndGo(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		Ogre::Vector3 destPos, bool rotateResolve = true, float rotatePre = 1.0f, 
		bool rotateItself = true, float rotateRate = 15.0f, 
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); //��ת������Ŀ��ǰ��

	void RotateAndDenfense(const Ogre::FrameEvent & evt, float rotateRate = 10.0f,
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
																		//�����ƽ�����ת�ķ�����ϵ
	bool RotateAndStop(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 1.0f,
		Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); //��ת��һ������ֹͣ

	void DieToHoming(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		const Ogre::Vector3 & dirRotate,bool rotateResolve = true, float rotatePre = 30.0f,
		float ownRotateRate = 1.0f,Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); 
																		//�����������λ
	bool HelicalSword(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 0.3f,
		float ownRotateRate = 1.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
	                                                                    //����������
	bool DivineSword1(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		Ogre::Vector3 destPos, float ownCenterRotateRate = 2.0f, bool rotateItself = true,
		float rotateRate = 1.0f,Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT); 
																		//���¾Ž�,�򽣹���(��һ��)֮�Ž���һ

	bool DivineSword2(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation, 
		float shootSpeedRate = 3.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);	
																		//���¾Ž�,�򽣹���(�ڶ���)֮���¾Ž�

	bool DivineSword3(const Ogre::FrameEvent & evt, const Ogre::Vector3 & orientation,
		const Ogre::Vector3 & dirRotate, bool rotateResolve = true, float rotatePre = 1.0f,
		 bool rotateItself = true, float ownRotateRate = 1.0f, float coreRotateRate = 1.0f,
		float flyRate = 1.0f, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TS_PARENT);
																		//���¾Ž�,�򽣹���(������)֮�Ž��ֻ�

	bool BeJetCollDec();				//�����乥������ײ���
	bool ThroughCollDec();				//��͸��������ײ���(����Ҷ���)
	bool ThroughAICollDec();			//��͸��������ײ���(��AI����)
	inline Ogre::Vector3 ChangePosToWorld(const Ogre::Vector3 &pos) const;
	//�����ڳ������ڵ������ת��Ϊ���������(ǰ�᣺�������ڵ�pitch��PI/2)

public:
	Sword(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, Sword_Object &sword, ControlModel* model, NinthCEGUI* ninthCEGUI);//���캯��
	Sword(Ogre::SceneManager* sceneMgr, SoundManager* soundMgr, ControlModel* model, NinthCEGUI* ninthCEGUI);
	virtual ~Sword();					//��������
	void SwordInitialise(const Sword_Object & sword);			//��ͨ�����ʼ��
	void PuppetSwordInitialise(const Sword_Object & sword);		//���ܽ����ʼ��
	bool frameRenderingQueued(const Ogre::FrameEvent & evt);	//����Ⱦ�����и��½���AIÿһ֡����Ϊ
	bool PuppetFrameRenderingQueued(const Ogre::FrameEvent & evt); //����Ϊ�ض��Ŀ��ܽ���ٿ�ʱ�����µ�ÿһ֡��Ϊ
	bool HelicalSwordFrameRendering(const Ogre::FrameEvent & evt); //���������ܷ���ʱÿһ֡����Ϊ

	Ogre::Vector3 getPosition()const { return m_SwordNode->getPosition(); } //���ؽ�����Ը��ռ������
	void setPosition(Ogre::Vector3 positon);		//�������ý���AIλ��
	void setRandPosition(Ogre::Real range = 2000);	//���ý���AI�����λ��
	void setRandCylinderPos(Ogre::Real rad, Ogre::Real height);		//����Բ��������Ľ���AI���λ��
	void setFollowRandPos(const Ogre::Vector3 &position, Ogre::Real range = 2000); //���ý���AI����Ŀ������λ��
	void setFollowRandCylinderPos(const Ogre::Vector3 &position, Ogre::Real rad, Ogre::Real height);
																			//���ý���AI����Ŀ���Բ�������λ��
	bool keyPressed(const OIS::KeyEvent & arg);		//���ý���AI��Ϊ��������ؼ��̰��²���
	bool keyReleased(const OIS::KeyEvent & arg);    //���ý���AI��Ϊ��������ؼ����ͷŲ���
	bool mousePressed(const OIS::MouseEvent & arg, const OIS::MouseButtonID id);//������갴�µļ�λ����
	bool mouseMoved(const OIS::MouseEvent & arg);	//����������������
	Ogre::SceneNode* getSwordNode(){ return m_SwordNode; } //���ؽ���ڵ�
	friend class ControlModel;
private:
	ControlModel* m_Model;				//������ģ�ͣ�������ϵ
	NinthCEGUI* m_NinthCEGUI;			//CEGUI���ƴ���

	SoundManager* m_SoundMgr;           //�����������ӿ�
	//��������,��������
	int m_iThroughtDestSd;				//��͸Ŀ����������
	int m_iThroughtDestChannel;         //��͸Ŀ������������
	int m_iHowlSd;                      //�����Х��������
	int m_iHowlChannel;                 //�����Х����������
	int m_iHomingSd;                    //�����λ����������
	int m_iHomingChannel;               //�����λ����������
	int m_iSwordShootSd1;               //�����������������1
	int m_iSwordShootChannel1;          //�������������������1
	int m_iSwordShootSd2;               //�����������������2
	int m_iSwordShootChannel2;          //�������������������2
	int m_iIkuSoSd;                     //(������-���߷緶)��������
	int m_iIkuSochannel;                //(������-���߷緶)��������
	int m_iShootSwordSd;				//���û���������
	int m_iShootSwordChannel;           //���û�����
	
	int m_iSDefenseSd;					//����������������
	static int ms_iSDefenseChannel;		//������������������
	int m_iDefenseScreamSd;             //��ǿ�����ź�������
	static int ms_iDefenseScreamChannel;//��ǿ�����ź�������
	int m_iDiv1_TrasSd;                 //"���¾Ž�,�򽣹���"���ܵ�һ��֮ƽ��
	static int ms_iDiv1_TrasChannel;
	int m_iDiv1_RotSd;                  //"���¾Ž�,�򽣹���"���ܵ�һ��֮��ת
	static int ms_iDiv1_RotChannel;
	int m_iDiv1_ShootSd;                //"���¾Ž�,�򽣹���"���ܵ�һ��֮���
	static int ms_iDiv1_ShootChannel;
	int m_iDiv2Sd;                      //"���¾Ž�,�򽣹���"���ܵڶ���
	int m_iDiv2Channel;
	int m_iDiv3_RotSd;                  //"���¾Ž�,�򽣹���"���ܵ�����֮��ת
	static int ms_iDiv3_RotChannel;
	int ms_iDiv3_ShootSd;               //"���¾Ž�,�򽣹���"���ܵ�����֮���
	static int ms_iDiv3_ShootChannel;
	int m_iDiv3_SThunderSd;             //"���¾Ž�,�򽣹���"���ܵ�����֮��������
	static int ms_iDiv3_SThunderChannel;
	int m_iDiv3_ThunderSd;              //"���¾Ž�,�򽣹���"���ܵ�����֮����
	static int ms_iDiv3_ThunderChannel;
	
	int m_iDiv4_ShootSd;                //"�򽣹���"���ܵ��Ķ�֮���������
	int m_iDiv4_ShootChannel;
	int m_iDiv4_LThunderSd;             //"�򽣹���"���ܵ��Ķ�֮��������
	int m_iDiv4_LThunderChannel;

	int m_iHelSword_FengDunSd;          //����������｣��Ч
	static int ms_iHelSword_FengDunChannel;
	int m_iHelSword_SThunderSd;          //����������ʱ�Ķ�������
	static int ms_iHelSword_SThunderCannel;
	int m_iHelSword_RotSd1;              //��������ת��Ч1
	static int ms_iHelSword_RotChannel1;
	int m_iHelSword_RotSd2;              //��������ת��Ч2     
	static int ms_iHelSword_RotChannel2;
	
	Ogre::SceneManager* m_SceneMgr;		//����������
	Ogre::SceneNode* m_AttackNode;		//��������Ŀ�������ڵ�λ��
	Ogre::Real m_Distance;				//��������Ŀ��ľ���
	Ogre::Vector3 m_Direction;			//����AI������Ŀ��ķ���
	Ogre::SceneNode* m_SwordNode;		//����AI�ĳ����ڵ�
	Ogre::Entity* m_SwordEnt;			//����AI��ʵ�����

	Sword_Object m_Sword;				//����AI�ĸ������Զ���
	Ogre::Real m_HP;					//����AI�ɿ��Ƶ�ӵ��Ѫ��

	Ogre::Quaternion m_Orbit;				//���������Ԫ��

	Ogre::RaySceneQuery* m_RaySceneQuery;	//���߳�����ѯ
	Ogre::AxisAlignedBoxSceneQuery* m_AABBSceneQuery;	

	Ogre::Vector3 m_AttackPos;				//��¼������Ŀ���
	bool m_bArriveAttackPos;				//�ж��Ƿ񵽴�Ŀ�깥����
	bool m_bDenFirstAttack;					//�����������δ����Ŀ�깥����ǰΪtrue,�����ƣ���������һ�Σ�
	bool m_bAttackState;					//�ڷ��������ڣ��򹥻�״̬Ϊ��
	bool m_bGetAttackPos;					//�Ѿ��õ�����Ŀ��λ�õı�־
	bool m_bDead;							//�ж��Ƿ������ı�־
	bool m_bLaunchState;					//��Ϊ������Ƿ��ڷ���״̬�ı�־
	bool m_bSwordMoved;						//������ƶ�״̬�������Ϊ�棩
	bool m_bRotateDenfense;					//�������ת����״̬������X������
	bool m_bDivineSword123;					//�����¾Ž�,�򽣹��ڡ��ͷ�״̬(����F������(��ǰ����)
	bool m_bHelicalSword;                   //�����������ͷ�״̬(����R����)
	bool m_bSpace;                          //����SpaceʱΪtrue
	bool m_bDenSkCoolPer;                   //��������������ȴȨ�ޣ����޷�����ϵ���ܵ�����һֻ���壩
	bool m_bHelSSCoolPer;                   //������������ȴȨ�ޣ����޷�����ϵ���ܵ�����һֻ���壩

	int m_iHelicIndex;                      //����������ʱ�Ľ�λ��ʶ
	int m_iDivineSword;						//�����¾Ž�,�򽣹��ڡ����ܴ��ڵڼ���?
	static bool ms_bNinthSwordShoot;		//�����¾Ž�,�򽣹��ڡ����ܵھŽ����������ʱΪ��
	static Ogre::Real ms_DenfenseTime;		//��ת�������ܿ�������������ʱ��
	static int ms_iRotateDenSucNum;          //��ת���������ɹ��Ľ�����
	Ogre::Real m_Divine1RotateTime;			//�����¾Ž�,�򽣹��ڡ����ܵ�һ�׶�ʱ����������ת��������ʱ��

	Ogre::Real m_PuppleSwordShootTime;		//���ܽ��弤����������ʱ��
	static bool ms_bPuppetStop;			    //���ܽ���ֹͣ����ı�־

	static Ogre::SceneNode* ms_SwordCore;	//����������Ϊ�������Χ����ת�ĺ˽ڵ�
	static bool ms_bCoreHaved;				//�Ѿ��������ʱΪ��
	static Ogre::SceneNode* ms_HelSCenCore; //���������ĺˣ��������ƣ��������Ĳ�����ƽ��
	static Ogre::SceneNode* ms_HelicalCore; //��������������ʱ��������Χ����ת�ĺ˽ڵ�
	static bool ms_bHCorehaved;             //�Ѿ����������˺����������ĺ�ʱΪ��
	static bool ms_bHelSwordAchieved;       //���������ܷ������ʱΪ��
	static bool ms_bHelSwordState;          //��������������״̬ʱΪ��
	static bool ms_bHCLaunchState;          //�����˴��ڷ���״̬ʱΪ��
	static bool ms_bHelSwordManual;         //���������������ֹ�����ʱΪ��
	static int ms_iRKeySusCount;            //�ۼ����������ܷ����ɹ��Ľ���
	static int ms_iRevSwordFromHC;          //�ۼƼ��ܷ�����Ϻ�����������гɹ��Ƴ��Ľ�������
	Ogre::Real m_HelSwordKTime;             //���������ܷ�����������ʱ��

	static int ms_iRotToCoreNum;			//������,��СΪ0, ���Ϊ9
	static int ms_iLaunchNum;				//���ڷ����еĽ�����
	static bool ms_bDivine3LayDown;			//�����¾Ž�,�򽣹��ڡ����ܵ����׶�ʱ����ȫ������ʱΪ��
	static bool ms_bDivine4Start;			//�����¾Ž�,�򽣹��ڡ����ܵ��Ľ׶ο���ʱΪ��
	static bool ms_bDivine3Ready;		    //�����¾Ž�,�򽣹��ڡ����ܵ����׶ε����Ľ׶οɹ���ʱ��׼�����,Ϊ��
	static Ogre::Real ms_Divine3Scale;		//�����¾Ž�,�򽣹��ڡ����ܵ����׶�ʱ�Ŵ󵽵ı���
	static Ogre::Real ms_Divine4WaitTime;	//�����¾Ž�,�򽣹��ڡ��ȴ����Ķ�������������������ʱ��
	int m_iRotToCoreIndex;					//�����ʶ����,0~8
	int m_iLaunchIndex;						//����һ��������Ľ�������ֵ
	bool m_bDiv3LaySuc;						//�����¾Ž�,�򽣹��ڡ������׶γɹ�����ʱΪ��
	static int ms_iDivSucNum;				//�ۼơ����¾Ž�,�򽣹��ڡ��ɹ������Ľ�����
	static int ms_iDiv3LaySuc;				//�ۼơ����¾Ž�,�򽣹��ڡ������׶γɹ����µĽ�����
public:
	static int ms_iDieNum;					//����AI���������������н���AI����һ������)
};

#endif