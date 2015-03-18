#include "stdafx.h"
#include "NinthCore.h"

//------------------------------��randNum()������--------------------------------------
// Desc:����min��max�������(int, float, double, Ogre::Real��)
//-------------------------------------------------------------------------------------
template <typename T> T randNum(T min, T max)
{
	return (min + ((max - min) * rand() / RAND_MAX));
}
//------------------------------��randInt()������--------------------------------------
// Desc:����0��max������
//-------------------------------------------------------------------------------------
inline int randInt(int max)
{
	return ((max + 1) * rand() / (RAND_MAX + 1));
}
//------------------------------��randReal()������-------------------------------------
// Desc:����0��max�ĸ�����
//-------------------------------------------------------------------------------------
inline Ogre::Real randReal(Ogre::Real max)
{
	return (max * rand() / RAND_MAX);
}


//------------------------------��NinthCore()������------------------------------------
// Desc:���캯��
//-------------------------------------------------------------------------------------
NinthCore::NinthCore(void):
m_iClickChannel(INVALID_SOUND_CHANNEL),
m_iChooseMapChannel(INVALID_SOUND_CHANNEL),
m_iGUIMusicChannel(INVALID_SOUND_CHANNEL),
m_iSongChannel1(INVALID_SOUND_CHANNEL),
m_iSongChannel2(INVALID_SOUND_CHANNEL),
m_iSongChannel3(INVALID_SOUND_CHANNEL),
m_iSongingChannel(INVALID_SOUND_CHANNEL),
m_MapIndex(1)
{
	std::ifstream nameStream("Name.data");

	if (!(nameStream >> m_CharName))
		m_CharName = "";
	nameStream.close();
}
//------------------------------��~NinthCore()������-----------------------------------
// Desc:��������
//-------------------------------------------------------------------------------------
NinthCore::~NinthCore(void)
{
}

//-------------------------------��createCamera()������--------------------------------
// Desc:����һ����һ�˳������
//-------------------------------------------------------------------------------------
void NinthCore::createCamera(void)
{
	m_Camera = mSceneMgr->createCamera("MainCamera");
	m_Camera->setPosition(0, 70, 120);
	m_Camera->lookAt(0, 10, 0);
	m_Camera->setNearClipDistance(5);
	
}
//--------------------------------��createViewports()������----------------------------
// Desc:����һ�����ӿ�
//-------------------------------------------------------------------------------------
void NinthCore::createViewports(void)
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(m_Camera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	// Alter the camera aspect ratio to match the viewport
	m_Camera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------��createMainMenuRelevW()������-----------------------------
// Desc:������Ϸ���˵�������ش���
//--------------------------------------------------------------------------------------
void NinthCore::createMainMenuRelevW(void)
{
	//����������
	m_MainWin = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "mainWin");
	//Ԥ�����4��ͼƬ�����������滻
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("main", "main.jpg");
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("startgame", "startgame.jpg");
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("keyhelp", "keyhelp.jpg");
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("swordskill", "swordskill.jpg");
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("ranklist", "ranklist.jpg");
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("choosemap", "choosemap.jpg");
	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "main");
	MyCEGUISystem::getSingletonPtr()->getRootWindow()->addChild(m_MainWin);

	//��ʼ��Ϸ��ť
	m_StartGameW = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "startGame");
	m_StartGameW->setText("start game");
	m_StartGameW->setArea(CEGUI::UDim(0.425f, 0.0f), CEGUI::UDim(0.39f, 0.0f),
		CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_StartGameW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::StartGame, this));
	m_MainWin->addChild(m_StartGameW);

	//��λ������ť
	m_KeyHelpW = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "keyHelp");
	m_KeyHelpW->setText("key help");
	m_KeyHelpW->setArea(CEGUI::UDim(0.425f, 0.0f), CEGUI::UDim(0.445f, 0.0f),
		CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_KeyHelpW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::KeyHelp, this));
	m_MainWin->addChild(m_KeyHelpW);

	//��ϵ����������ť
	m_SwordSkillW = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "swordSkill");
	m_SwordSkillW->setText("sword skill");
	m_SwordSkillW->setArea(CEGUI::UDim(0.425f, 0.0f), CEGUI::UDim(0.5f, 0.0f),
		CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_SwordSkillW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::SwordSkill, this));
	m_MainWin->addChild(m_SwordSkillW);

	//���а�ť
	m_RankListW = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "rankList");
	m_RankListW->setText("ranklist");
	m_RankListW->setArea(CEGUI::UDim(0.425f, 0.0f), CEGUI::UDim(0.555f, 0.0f),
		CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_RankListW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::RankList, this));
	m_MainWin->addChild(m_RankListW);

	//�˳���Ϸ��ť
	m_QuitGameW = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button", "quitGame");
	m_QuitGameW->setText("quit game");
	m_QuitGameW->setArea(CEGUI::UDim(0.425f, 0.0f), CEGUI::UDim(0.61f, 0.0f),
		CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_QuitGameW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::QuitGame, this));
	m_MainWin->addChild(m_QuitGameW);

	//���ذ�ť
	m_BackW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button", "back");
	m_BackW->setText("back");
	m_BackW->setArea(CEGUI::UDim(0.85f, 0.0f), CEGUI::UDim(0.92f, 0.0f),
		CEGUI::UDim(0.10f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_BackW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::Back, this));
	m_MainWin->addChild(m_BackW);
	m_BackW->hide();
}
//------------------------------��createRListRelevW()������-----------------------------
// Desc:�������а����ؽ��洰��
//--------------------------------------------------------------------------------------
void NinthCore::createRListRelevW(float ratio)
{
	//���а������
	m_RankListSheetW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/StaticText", "RankListSheet");
	m_RankListSheetW->setArea(CEGUI::UDim(0.5f - 0.44f / ratio, 0.0f), CEGUI::UDim(0.2f, 0.0f),
		CEGUI::UDim(0.88f / ratio, 0.0f), CEGUI::UDim(0.6f, 0.0f));
	m_MainWin->addChild(m_RankListSheetW);
	m_RankListSheetW->hide();
	for (size_t i = 0; i < 5; i++)
	{
		m_RankListSheetW->addChild(m_NinthCEGUI->getRewardingTextW()[i]);
		m_RankListSheetW->addChild(m_NinthCEGUI->getRewardingTextW()[i + 5]);
	}

	//������ʾ��̬�ı�
	m_NameTipW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/StaticText", "nameTip");
	m_NameTipW->setText("  Please input your default name:");
	m_NameTipW->setArea(CEGUI::UDim(0.21f, 0.0f), CEGUI::UDim(0.9f, 0.0f),
		CEGUI::UDim(0.48f, 0.0f), CEGUI::UDim(0.06f, 0.0f));
	m_RankListSheetW->addChild(m_NameTipW);
	m_NameTipW->setProperty("BackgroundEnabled", "false");
	m_NameTipW->setProperty("FrameEnabled", "false");
	m_RankListSheetW->addChild(m_NameTipW);

	//�����༭��
	m_NameEditW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Editbox", "nameEdit");
	m_NameEditW->setArea(CEGUI::UDim(0.655f, 0.0f), CEGUI::UDim(0.9f, 0.0f),
		CEGUI::UDim(0.2f, 0.0f), CEGUI::UDim(0.06f, 0.0f));
	m_RankListSheetW->addChild(m_NameEditW);
	m_NameEditW->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&NinthCore::EditNameChange, this));
	m_NameEditW->activate();
	m_NameEditW->setProperty("BlinkCaret", "true");

	//OK��ť
	m_OkW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button", "ok");
	m_OkW->setText("ok");
	m_OkW->setArea(CEGUI::UDim(0.86f, 0.0f), CEGUI::UDim(0.9f, 0.0f),
		CEGUI::UDim(0.1f, 0.0f), CEGUI::UDim(0.06f, 0.0f));
	m_OkW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::Ok, this));
	m_RankListSheetW->addChild(m_OkW);
	m_OkW->hide();

	//������ʾ��
	m_NameShowW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/StaticText", "nameShow");
	m_NameShowW->setText("RoleName: " + m_CharName);
	m_NameShowW->setArea(CEGUI::UDim(0.65f, 0.0f), CEGUI::UDim(0.04f, 0.0f),
		CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.06f, 0.0f));
	m_RankListSheetW->addChild(m_NameShowW);
	m_NameShowW->setProperty("BackgroundEnabled", "false");
	m_NameShowW->setProperty("FrameEnabled", "false");
	m_NameShowW->activate();
}
//-------------------------------��createRewardingW()������-----------------------------
// Desc:�����������ڣ��������ͼƬ
//--------------------------------------------------------------------------------------
void NinthCore::createRewardingW(void)
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* rW[5];
	for (size_t i = 0; i < 5; i++)
	{
		Ogre::String tempNum = Ogre::StringConverter::toString(i + 1);
		rW[i] = wmgr.createWindow("WindowsLook/StaticImage", "Beyond/reward"+ tempNum);
		CEGUI::ImageManager::getSingleton().addFromImageFile("reward"+ tempNum, "reward"+ tempNum + ".jpg");
		MyCEGUISystem::getSingletonPtr()->addImageToWindow(rW[i], "reward"+ tempNum, 0.1f, 0.1f+i*0.165f, 0.1f, 0.14f);
		m_RankListSheetW->addChild(rW[i]);
	}
}
//------------------------------��createCMapRelevW()������------------------------------
// Desc:����ѡ���ͼ������ش���
//--------------------------------------------------------------------------------------

void NinthCore::createCMapRelevW(float ratio)
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	for (size_t i = 0; i < 6; i++)
	{
		Ogre::String tempNum = Ogre::StringConverter::toString(i + 1);
		m_MapW[i] = wmgr.createWindow("WindowsLook/StaticImage", "Beyond/map" + tempNum);
		CEGUI::ImageManager::getSingleton().addFromImageFile("map" + tempNum, "map" + tempNum + ".jpg");
		if (i < 3)
			MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MapW[i], "map" + tempNum, 0.075f, 0.2f + i*0.22f, 0.3f / ratio, 0.17f);
		else
			MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MapW[i], "map" + tempNum, 0.36f, 0.2f + (i -3)*0.22f, 0.3f / ratio, 0.17f);
		m_MapW[i]->subscribeEvent(CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&NinthCore::ChooseMap, this));
		m_MainWin->addChild(m_MapW[i]);
		m_MapW[i]->hide();
		//Ĭ��ѡ�е�һ����ͼ
		if (i!=0)
			m_MapW[i]->setProperty("FrameColours", "tl:88BBBBBB tr:88BBBBBB bl:88BBBBBB br:88BBBBBB");
	}
	
	//ѡ���ͼ��OKȷ�ϰ�ť
	m_CMapOkW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button", "cMapOk");
	m_CMapOkW->setText("ok");
	m_CMapOkW->setArea(CEGUI::UDim(0.85f, 0.0f), CEGUI::UDim(0.92f, 0.0f),
		CEGUI::UDim(0.1f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_CMapOkW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::CMapOk, this));
	m_MainWin->addChild(m_CMapOkW);
	m_CMapOkW->hide();

	//ѡ���ͼ����ķ��ذ�ť
	m_MapBackW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button", "mapBack");
	m_MapBackW->setText("back");
	m_MapBackW->setArea(CEGUI::UDim(0.05f, 0.0f), CEGUI::UDim(0.92f, 0.0f),
		CEGUI::UDim(0.10f, 0.0f), CEGUI::UDim(0.04f, 0.0f));
	m_MapBackW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::CMapBack, this));
	m_MainWin->addChild(m_MapBackW);
	m_MapBackW->hide();

	//������ʾ��̬�ı�
	m_MapTipW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/StaticText", "mapTip");
	m_MapTipW->setText("  Please choose the map you love:");
	m_MapTipW->setArea(CEGUI::UDim(0.025f, 0.0f), CEGUI::UDim(0.12f, 0.0f),
		CEGUI::UDim(0.48f, 0.0f), CEGUI::UDim(0.06f, 0.0f));
	m_MapTipW->setProperty("BackgroundEnabled", "false");
	m_MapTipW->setProperty("FrameEnabled", "false");
	m_MapTipW->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
	m_MainWin->addChild(m_MapTipW);
	m_MapTipW->hide();
}
//--------------------------------��createGUI()������-----------------------------------
// Desc:����GUI����
//--------------------------------------------------------------------------------------
void NinthCore::createGUI(void)
{
	//��ʼ��
	MyCEGUISystem::getSingletonPtr()->Init();

	//����ϵͳ��������ʼ��,GUI��Ч
	m_SoundMgr = new SoundManager;
	m_SoundMgr->Initialize();
	m_iGUIMusicSd = m_SoundMgr->CreateLoopedSound(Ogre::String("(N)̫��ͭ��.wav"));
	m_iClickSd = m_SoundMgr->CreateSound(Ogre::String("(N)Click.wav"));
	m_iChooseMapSd = m_SoundMgr->CreateSound(Ogre::String("(N)chooseMap.wav"));

	//NinthCEGUI�и��ִ��ڵĳ�ʼ��
	m_NinthCEGUI = new NinthCEGUI(mWindow, &mbShutDown, m_SoundMgr);
	m_NinthCEGUI->getOneAgainW()->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::OneAgain, this));
	m_NinthCEGUI->getReturnMainW()->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCore::ReturnMain, this));
	
	//���˵�����
	createMainMenuRelevW();

	//���а������
	float ratio = (float)mWindow->getWidth() / mWindow->getHeight();
	createRListRelevW(ratio);

	//������Ƥ��
	m_SkillSkinW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/StaticImage", "skillSkinW");
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("skillSkin", "skillSkin_ex.tga");
	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_SkillSkinW, "skillSkin");
	MyCEGUISystem::getSingletonPtr()->getRootWindow()->addChild(m_SkillSkinW);
	m_SkillSkinW->setProperty("BackgroundEnabled", "false");
	m_SkillSkinW->setProperty("FrameEnabled", "false");
	m_SkillSkinW->setArea(CEGUI::UDim(0.5f - 0.40793f/ratio, 0.0f), CEGUI::UDim(0.885f, 0.0f),
	CEGUI::UDim(0.81586f/ratio, 0.0f), CEGUI::UDim(0.14f, 0.0f));
	m_SkillSkinW->hide();
	//��Ӽ������ֺ���ȴ��ʱ����
	for (size_t i = 0; i < 8; i++)
	{
		m_SkillSkinW->addChild(m_NinthCEGUI->getSkillShadeW()[i]);
		if (i!=0 && i!=2)
			m_SkillSkinW->addChild(m_NinthCEGUI->getSkillCoolCalW()[i]);
	}

	//����ͼƬ����
	createRewardingW();

	//������ͼ������ش���
	createCMapRelevW(ratio);

	//����GUI��������
	m_SoundMgr->PlaySound(m_iGUIMusicSd, NULL, &m_iGUIMusicChannel, 0.37f);
}
//------------------------------��createActiveScene()������----------------------------
// Desc:������ʼ���̶�����
//-------------------------------------------------------------------------------------
void NinthCore::createFixedScene(void)
{	
	//��Ϸ�������ִ���
	m_iSongSd1 = m_SoundMgr->CreateLoopedStream(Ogre::String("No Game No Life.mp3"));
	m_iSongSd2 = m_SoundMgr->CreateLoopedStream(Ogre::String("Lisa - Rising Hope.mp3"));
	m_iSongSd3 = m_SoundMgr->CreateLoopedStream(Ogre::String("�δ����˼�.mp3"));

	//���û�����,���Դ
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	mSceneMgr->createLight("Light1")->setPosition(20, 80, 50);   // add basic point light
	
}
//------------------------------��createActiveScene()������----------------------------
// Desc:������ʼ����Ծ����
//-------------------------------------------------------------------------------------
void NinthCore::createActiveScene(void)
{
	srand(GetTickCount());
	//ѡ����պе�ͼ
	switch (m_MapIndex)
	{
	case 1:
		mSceneMgr->setSkyBox(true, "SkyBox/Cloudy", 20.0f);
		break;
	case 2:
		mSceneMgr->setSkyBox(true, "SkyBox/SunSet", 20.0f);
		break;
	case 3:
		mSceneMgr->setSkyBox(true, "SkyBox/TropicalSunnyDay", 20.0f);
		break;
	case 4:
		mSceneMgr->setSkyBox(true, "SkyBox/ThickCloudsWater", 20.0f);
		break;
	case 5:
		mSceneMgr->setSkyBox(true, "SkyBox/FullMoon", 20.0f);
		break;
	case 6:
		mSceneMgr->setSkyBox(true, "SkyBox/DarkStormy", 20.0f);
		break;
	}

	//����һ����ɽ
	Ogre::Entity* moutain = mSceneMgr->createEntity("Moutain.MESH.mesh");
	Ogre::SceneNode* moutainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	moutainNode->attachObject(moutain);
	moutainNode->setPosition(0, -1000, 300);
	moutainNode->setScale(0.20f, 0.20f, 0.20f);

	//��ģ�Ϳ���ϵͳ����
	m_Model = new ControlModel(mRoot, mSceneMgr, m_SoundMgr, m_Camera, m_NinthCEGUI, m_CharName);
	m_Model->ModelInitialise("Dragon", "Dragon.MESH.mesh", Ogre::Vector3(0, 0, 0), MODEL_MAX_BLOOD);

	//Ŧ������ľֲ�����
	m_Knotfish = new KnotAndFish(mRoot, mSceneMgr, m_SoundMgr, m_Model, m_NinthCEGUI);
	m_Knotfish->initialise(0, 1000, 0);
	
	//�����Űѽ����еĴ�
	Ogre::Entity* swordEnt = NULL;
	for (size_t i = 0; i < 9; i++)
	{
		//��������AI��ʵ����󣬲���ӵ������ڵ�
		switch (i)
		{
		case 0:
			swordEnt = mSceneMgr->createEntity("RedSword.MESH.mesh");
			break;
		case 1:
			swordEnt = mSceneMgr->createEntity("GoldenSword.MESH.mesh");
			break;
		case 2:
			swordEnt = mSceneMgr->createEntity("HelicalSword.MESH.mesh");
			break;
		case 3:
			swordEnt = mSceneMgr->createEntity("SpinousSword.MESH.mesh");
			break;
		case 4:
			swordEnt = mSceneMgr->createEntity("PurpleSword.MESH.mesh");
			break;
		case 5:
			swordEnt = mSceneMgr->createEntity("BigSword.MESH.mesh");
			break;
		case 6:
			swordEnt = mSceneMgr->createEntity("GreenSword.MESH.mesh");
			break;
		case 7:
			swordEnt = mSceneMgr->createEntity("GraySword.MESH.mesh");
			break;
		case 8:
			swordEnt = mSceneMgr->createEntity("BlackWhiteSword.MESH.mesh");
			break;
		}
		m_SwordNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		m_SwordNode[i]->attachObject(swordEnt);
		//�õ���������,���䳯��������
		m_SwordNode[i]->setPosition(SWORD_TO_CENTER_DIS * Ogre::Math::Cos(i * 40.0f), 0, SWORD_TO_CENTER_DIS * Ogre::Math::Sin(i * 40.0f));
		m_SwordNode[i]->setScale(80, 80, 80);
		Ogre::Vector3 src = m_SwordNode[i]->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		Ogre::Quaternion quat = src.getRotationTo(-m_SwordNode[i]->getPosition());
		m_SwordNode[i]->rotate(quat, Ogre::Node::TS_PARENT);
		//����ֱ������,������ƽ��,ʹ�俴��������ɽ��ͬһ����ռ䵱��
		m_SwordNode[i]->pitch(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
		m_SwordNode[i]->translate(Ogre::Vector3(0, -500, 0), Ogre::Node::TS_PARENT);
		//�Ű�Χ�Ƶ�С�ľŽ�
		m_Sword.m_Scale = Vector3(5, 5, 5);
		for (size_t j = 0; j < 9; j++)
		{
			m_Sword.m_ObjectIndex = i * 9 + j;
			m_Sword.m_CategoryIndex = randNum(0, 14);
			m_Sword.m_Damage = randNum(50.0f, 100.0f);
			m_Sword.m_Defense = randNum(50.0f, 100.0f);
			m_Sword.m_MaxHP = randNum(100.0f, 200.0f);
			m_Sword.m_FlySpeed = randNum(700, 800);
			m_Sword.m_Position = Ogre::Vector3(
				m_SwordNode[i]->getPosition().x + 100 * Ogre::Math::Cos(j * 40.0f),
				m_SwordNode[i]->getPosition().y,
				m_SwordNode[i]->getPosition().z + 100 * Ogre::Math::Sin(j * 40.0f));
			m_Sword.m_bAttackRotate = true;
			m_Sword.m_bNoStopAttack = true;
			m_Sword.m_bOneAttack = false;
			m_SwordWaitAI[i * 9 + j] = new Sword(mSceneMgr, m_SoundMgr, m_Model, m_NinthCEGUI);
			m_SwordWaitAI[i * 9 + j]->SwordInitialise(m_Sword);
		}
	}

	//��ʼ��ʱ����׷�ٵĽ���AI
	m_Sword.m_Scale = Vector3(5, 5, 5);
	for (size_t i = 0; i < SWORD_NUM; i++)
	{
		m_Sword.m_ObjectIndex = 81 + i;
		m_Sword.m_CategoryIndex = i;
		m_Sword.m_Damage = randNum(50.0f, 100.0f);
		m_Sword.m_Defense = randNum(50.0f, 100.0f);
		m_Sword.m_MaxHP = randNum(200.0f, 400.0f);
		m_Sword.m_FlySpeed = randNum(700, 800);
		m_Sword.m_bAttackRotate = true;
		m_Sword.m_bNoStopAttack = true;
		m_Sword.m_bOneAttack = false;
		m_SwordAI[i] = new Sword(mSceneMgr, m_SoundMgr, m_Model, m_NinthCEGUI);
		m_SwordAI[i]->SwordInitialise(m_Sword);
		Ogre::Real a = randReal(495) - 247.5f;
		Ogre::Real b = randReal(495) - 247.5f;
		Ogre::Real c = sqrt(490000 - a*a - b*b);
		if ((int)(rand()) % 2 == 0)c = -c;
		m_SwordAI[i]->setPosition(Ogre::Vector3(a, b, c));
	}

	//����������ڤ������AI����
	m_Dragon.m_Scale = Vector3(1, 1, 1);
	for (size_t i = 0; i < DRAGON_NUM; i++)
	{
		m_Dragon.m_Index = i;
		m_Dragon.m_FlyIndex = randInt(2);
		m_Dragon.m_JetIndex = randInt(7);
		m_Dragon.m_Damage = randNum(50.0f, 100.0f);
		m_Dragon.m_Defense = randNum(50.0f, 70.0f);
		m_Dragon.m_MaxHP = randNum(1000.0f, 1500.0f);
		m_Dragon.m_FlySpeed = randNum(270.0f, 300.0f);
		if (i < 3)
		{
			m_DragonAI[i] = new Dragon(mSceneMgr, m_SoundMgr,"Dragon.mesh", m_Model, m_NinthCEGUI);
			m_DragonAI[i]->DragonInitialise(m_Dragon);
			m_DragonAI[i]->setRandPosition(5000);
		}
		else
		{
			m_GDragonAI[i - 3] = new GhostdomDragon(mSceneMgr, m_SoundMgr,"GhostDragon.MESH.mesh", m_Model, m_NinthCEGUI);
			m_GDragonAI[i - 3]->DragonInitialise(m_Dragon);
			m_GDragonAI[i - 3]->setRandPosition(5000);
		}
	}
	
}
//------------------------------��createFrameListener()������-----------------------------
// Desc:ÿһ֡�Ĵ���׼������
//----------------------------------------------------------------------------------------
void NinthCore::createFrameListener(void)
{
	static int carryCount = 0;
	if (carryCount == 0)
	{
		BaseApplication::createFrameListener();
		carryCount++;
	}
}

//------------------------------��frameRenderingQueued()������----------------------------
// Desc:ÿһ֡����Ⱦ���¹���
//----------------------------------------------------------------------------------------
bool NinthCore::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (!BaseApplication::frameRenderingQueued(evt))
		return false;
	
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(evt.timeSinceLastFrame);
	if (!mbStartGame) 
		return true;

	m_NinthCEGUI->getFPSW()->setText("FPS:"+Ogre::StringConverter::toString(mWindow->getAverageFPS()));

	//-------��ʹ��ճ�����������----------
	mSceneMgr->getSkyBoxNode()->yaw(Ogre::Radian(0.01f * evt.timeSinceLastFrame), Ogre::Node::TS_PARENT);

	//-------��Ŧ������ֲ�������Ⱦ��------
	m_Knotfish->frameRenderingQueued(evt);

	//-------��ģ���ƶ���ÿ֪֡ͨ��--------
	m_Model->frameRenderingQueued(evt);
	//*/
	//-------������AI��ÿ֡��Ϊ��⡿------
	for (size_t i = 0; i < DRAGON_NUM/2; i++)
	{		
		m_DragonAI[i]->frameRenderingQueued(evt);
	}

	//-------��ڤ������AIÿ֡��Ϊ��⡿-----
	for (size_t i = 0; i < DRAGON_NUM / 2; i++)
	{
		m_GDragonAI[i]->frameRenderingQueued(evt);
	}
	
	//-------����ʼʱ�����׷�ٵĽ���AIÿ֡��Ϊ��⡿---------
	for (size_t i = 0; i < SWORD_NUM; i++)
	{
		m_SwordAI[i]->frameRenderingQueued(evt);
	}
	//*/
	//-------���ŰѴ�����������ת��----
	for (size_t i = 0; i < 9; i++)
	{
		m_SwordNode[i]->roll(Ogre::Radian(Ogre::Math::PI * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
	}
	
	//-------����ʼʱ�ھŰѴ���Χ�Ľ���AI����Ϊ��⡿--------
	for (size_t i = 0; i < 81; i++)
	{
		m_SwordWaitAI[i]->frameRenderingQueued(evt);
	}
	//*/
	return true;
}

//-----------------------------------��keyPressed()������---------------------------------
// Desc:��OIS�Լ�λ���²������д���
//----------------------------------------------------------------------------------------
bool NinthCore::keyPressed(const OIS::KeyEvent &arg)
{
	BaseApplication::keyPressed(arg);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)arg.key);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((CEGUI::Key::Scan)arg.text);
	
	if (!mbStartGame)
		return true;

	//���������л�����������
	float fVolume = 0.0f;
	switch (arg.key)
	{
	case OIS::KC_0:
		m_SoundMgr->StopSound(&m_iSongChannel1);
		m_SoundMgr->StopSound(&m_iSongChannel2);
		m_SoundMgr->StopSound(&m_iSongChannel3);
		return true;
	case OIS::KC_1:
		if (m_iSongingChannel == m_iSongChannel1 && m_iSongingChannel!=INVALID_SOUND_CHANNEL) return true;
		m_SoundMgr->StopSound(&m_iSongChannel2);
		m_SoundMgr->StopSound(&m_iSongChannel3);
		m_SoundMgr->PlaySound(m_iSongSd1, NULL, &m_iSongChannel1);
		m_iSongingChannel = m_iSongChannel1;
		return true;
	case OIS::KC_2:
		if (m_iSongingChannel == m_iSongChannel2 && m_iSongingChannel != INVALID_SOUND_CHANNEL) return true;
		m_SoundMgr->StopSound(&m_iSongChannel1);
		m_SoundMgr->StopSound(&m_iSongChannel3);
		m_SoundMgr->PlaySound(m_iSongSd2, NULL, &m_iSongChannel2);
		m_iSongingChannel = m_iSongChannel2;
		return true;
	case OIS::KC_3:
		if (m_iSongingChannel == m_iSongChannel3 && m_iSongingChannel != INVALID_SOUND_CHANNEL) return true;
		m_SoundMgr->StopSound(&m_iSongChannel1);
		m_SoundMgr->StopSound(&m_iSongChannel2);
		m_SoundMgr->PlaySound(m_iSongSd3, NULL, &m_iSongChannel3);
		m_iSongingChannel = m_iSongChannel3;
		return true;

	case OIS::KC_MINUS:
	case OIS::KC_VOLUMEDOWN:
		m_SoundMgr->getVolume(&fVolume, &m_iSongingChannel);
		fVolume -= 0.05f;
		if (fVolume < 0.0f)
			fVolume = 0.0f;
		m_SoundMgr->setVolume(fVolume, &m_iSongingChannel);
		return true;
	case OIS::KC_EQUALS:
	case OIS::KC_VOLUMEUP:
		m_SoundMgr->getVolume(&fVolume, &m_iSongingChannel);
		fVolume += 0.05f;
		if (fVolume > 1.0f)
			fVolume = 1.0f;
		m_SoundMgr->setVolume(fVolume, &m_iSongingChannel);
		return true;
	}
	//��ɫ����ֱ�ӷ���
	if (m_Model->getDieFlag())
		return true;

	//��ģ��
	m_Model->keyPressed(arg);
	//��ʼ��ʱ����׷�ٵĽ�
	//*
	for (size_t i = 0; i < SWORD_NUM; i++)
	{
		m_SwordAI[i]->keyPressed(arg);
	}
	//��ʼ��ʱ�ھŰѴ��ԱߵĽ�
	for (size_t i = 0; i < 81; i++)
	{
		m_SwordWaitAI[i]->keyPressed(arg);
	}
	//*/
	return true;
}
//-----------------------------------��keyReleased()������--------------------------------
// Desc:��OIS�Լ�λ�ͷŲ������д���
//----------------------------------------------------------------------------------------
bool NinthCore::keyReleased(const OIS::KeyEvent &arg)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
	if (!mbStartGame)
		return true;

	//��ɫ����ֱ�ӷ���
	if (m_Model->getDieFlag())
		return true;

	//��ģ��
	m_Model->keyReleased(arg);
	//��ʼ��ʱ����׷�ٵĽ�
	//*
	for (size_t i = 0; i < SWORD_NUM; i++)
	{
		m_SwordAI[i]->keyReleased(arg);
	}
	//��ʼ��ʱ�ھŰѴ��ԱߵĽ�
	for (size_t i = 0; i < 81; i++)
	{
		m_SwordWaitAI[i]->keyReleased(arg);
	}
	//*/
	return true;
}

//-----------------------------------��mouseMoved()������--------------------------------
// Desc:��OIS������ƶ��������д���
//---------------------------------------------------------------------------------------
bool NinthCore::mouseMoved(const OIS::MouseEvent &arg)
{
	BaseApplication::mouseMoved(arg);

	if (!mbStartGame || m_Model->getDieFlag())
	{
		CEGUI::GUIContext& conText = CEGUI::System::getSingleton().getDefaultGUIContext();
		if (conText.injectMouseMove(arg.state.X.rel, arg.state.Y.rel))
		{
			conText.injectMouseWheelChange(arg.state.Z.rel / 120.f);
		}
		return true;
	}
	//��ģ��
	m_Model->mouseMoved(arg);
	//��ʼ��ʱ����׷�ٵĽ�
	//*
	for (size_t i = 0; i < SWORD_NUM; i++)
	{
		m_SwordAI[i]->mouseMoved(arg);
	}
	for (size_t i = 0; i < 81; i++)
	{
		m_SwordWaitAI[i]->mouseMoved(arg);
	}
	//*/
	return true;
}

//-----------------------------------��mousePressed()������------------------------------
// Desc:��OIS����갴�²������д���
//---------------------------------------------------------------------------------------
bool NinthCore::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	BaseApplication::mousePressed(arg, id);

	if (!mbStartGame || m_Model->getDieFlag())
	{
		CEGUI::GUIContext& conText = CEGUI::System::getSingleton().getDefaultGUIContext();
		conText.injectMouseButtonDown(MyCEGUISystem::getSingletonPtr()->convertMouseButton(id));
		return true;
	}
	m_Model->mousePressed(arg, id);
	return true;
}

//-----------------------------------��mouseReleased()������-----------------------------
// Desc:��OIS������ͷŲ������д���
//---------------------------------------------------------------------------------------
bool NinthCore::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	BaseApplication::mouseReleased(arg, id);
	if (!mbStartGame || m_Model->getDieFlag())
	{
		CEGUI::GUIContext& conText = CEGUI::System::getSingleton().getDefaultGUIContext();
		conText.injectMouseButtonUp(MyCEGUISystem::getSingletonPtr()->convertMouseButton(id));
		return true;
	}
	m_Model->mouseReleased(arg, id);
	return true;
}

//------------------------------��StartGame()������---------------------------------------
// Desc:��ʼ��Ϸ�����ѡ���ͼ����
//----------------------------------------------------------------------------------------
bool NinthCore::StartGame(const CEGUI::EventArgs& arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "choosemap");
	m_NameEditW->disable();
	m_StartGameW->hide();
	m_KeyHelpW->hide();
	m_SwordSkillW->hide();
	m_RankListW->hide();
	m_QuitGameW->hide();
	m_NameEditW->disable();
	
	m_CMapOkW->show();
	m_MapBackW->show();
	m_MapTipW->show();
	for (size_t i = 0; i < 6; i++)
		m_MapW[i]->show();
	return true;
}
//---------------------------------��CMapOk()������-------------------------------------
// Desc:ѡ����Ϸ��ͼ��������Ϸ״̬����ʽ������Ϸ
//--------------------------------------------------------------------------------------
bool NinthCore::CMapOk(const CEGUI::EventArgs & arg)
{
	//�ر�GUI��������
	m_SoundMgr->StopSound(&m_iGUIMusicChannel);

	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "startgame");
	for (size_t i = 0; i < 6; i++)
		m_MapW[i]->hide();
	m_CMapOkW->hide();
	m_MapBackW->hide();
	m_MapTipW->hide();

	mRoot->renderOneFrame();

	// Create the scene
	if (!mbFSceneRendered)
	{
		createFixedScene();
		mbFSceneRendered = true;
	}
	createActiveScene();
	mbStartGame = true;
	m_MainWin->hide();
	m_SkillSkinW->show();
	m_NinthCEGUI->ShowAllInterfaceW();
	mRoot->startRendering();
	return true;
}
//---------------------------------��CMapBack()������-------------------------------------
// Desc:��ѡ���ͼ�����˻�������
//----------------------------------------------------------------------------------------
bool NinthCore::CMapBack(const CEGUI::EventArgs & arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "main");
	m_CMapOkW->hide();
	m_MapBackW->hide();
	m_MapTipW->hide();
	for (size_t i = 0; i < 6; i++)
		m_MapW[i]->hide();

	m_StartGameW->show();
	m_KeyHelpW->show();
	m_SwordSkillW->show();
	m_RankListW->show();
	m_QuitGameW->show();
	return true;
}
//-------------------------------��KeyHelp()������----------------------------------------
// Desc:�����λ�������棬����back���򷵻�
//----------------------------------------------------------------------------------------
bool NinthCore::KeyHelp(const CEGUI::EventArgs& arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "keyhelp");
	m_StartGameW->hide();
	m_KeyHelpW->hide();
	m_SwordSkillW->hide();
	m_RankListW->hide();
	m_QuitGameW->hide();

	m_BackW->show();
	return true;
}

//-------------------------------��SwordSkill()������-------------------------------------
// Desc:����Ž����ܽ��ܽ��棬����back���򷵻�
//----------------------------------------------------------------------------------------
bool NinthCore::SwordSkill(const CEGUI::EventArgs& arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "swordskill");
	m_StartGameW->hide();
	m_KeyHelpW->hide();
	m_SwordSkillW->hide();
	m_RankListW->hide();
	m_QuitGameW->hide();

	m_BackW->show();
	return true;
}

//-------------------------------��RankList()������-------------------------------------
// Desc:�������а���棬����back���򷵻�
//----------------------------------------------------------------------------------------
bool NinthCore::RankList(const CEGUI::EventArgs& arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "ranklist");
	m_StartGameW->hide();
	m_KeyHelpW->hide();
	m_SwordSkillW->hide();
	m_RankListW->hide();
	m_QuitGameW->hide();

	m_RankListSheetW->show();
	m_BackW->show();
	m_NameEditW->activate();
	m_OkW->activate();
	return true;
}

//------------------------------��QuitGame()������----------------------------------------
// Desc:�˳���Ϸ
//----------------------------------------------------------------------------------------
bool NinthCore::QuitGame(const CEGUI::EventArgs& arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	mbShutDown = true;
	return true;
}
//------------------------------��destroyGUI()������--------------------------------------
// Desc:����GUI
//----------------------------------------------------------------------------------------
void NinthCore::destroyGUI(void)
{

}
//------------------------------��destroyActiveScene()������------------------------------
// Desc:���ٻ�Ծ����
//----------------------------------------------------------------------------------------
void NinthCore::destroyActiveScene(void)
{
	mSceneMgr->destroyAllEntities();
	mSceneMgr->destroyAllAnimationStates();
	mSceneMgr->destroyAllParticleSystems();
	mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
	mSceneMgr->destroyCamera("Monitor");

	MyCEGUISystem::getSingletonPtr()->getRenderer()->destroyTexture("guiTex");
	CEGUI::ImageManager::getSingleton().destroy("MoniImage");

	delete m_Model;
	delete m_Knotfish;
	for (size_t i = 0; i < DRAGON_NUM / 2; i++)
	{
		delete m_DragonAI[i];
		delete m_GDragonAI[i];
	}
	for (size_t i = 0; i < SWORD_NUM; i++)
		delete m_SwordAI[i];
	for (size_t i = 0; i < 81; i++)
		delete m_SwordWaitAI[i];
}
//---------------------------------��Back()������-----------------------------------------
//Desc:���ص�������
//----------------------------------------------------------------------------------------
bool NinthCore::Back(const CEGUI::EventArgs& arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	m_RankListSheetW->hide();
	m_BackW->hide();

	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MainWin, "main");
	m_StartGameW->show();
	m_KeyHelpW->show();
	m_SwordSkillW->show();
	m_RankListW->show();
	m_QuitGameW->show();

	return true;
}
//------------------------------------��OneAgain()������------------------------------------------
// Desc������һ���¼�
//------------------------------------------------------------------------------------------------
bool NinthCore::OneAgain(const CEGUI::EventArgs & arg)
{
	//������Ϸ���漰��Ϸ״̬
	mbStartGame = false;
	m_SkillSkinW->hide();
	m_NinthCEGUI->HideAllInterfaceW();
	m_NinthCEGUI->getSrcDieScoreW()->hide();
	//����������
	for (size_t i = 2; i <= 6; i++)
		m_NinthCEGUI->getSkillShadeW()[i]->show();

	//��ֹͣ�����������򳡾��ڵ������������쳣
	m_SoundMgr->StopAllSounds();
	//����ԭ���Ļ�Ծ����
	destroyActiveScene();

	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	//����������Ϸ
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	m_MainWin->show();
	mRoot->renderOneFrame();
	createActiveScene();
	mbStartGame = true;
	m_MainWin->hide();
	m_SkillSkinW->show();
	m_NinthCEGUI->ShowAllInterfaceW();
	mRoot->startRendering();
	return true;
}
//------------------------------------��ReturnMain()������----------------------------------------
// Desc������������
//------------------------------------------------------------------------------------------------
bool NinthCore::ReturnMain(const CEGUI::EventArgs & arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	//������Ϸ���漰��Ϸ״̬
	mbStartGame = false;
	m_NinthCEGUI->HideAllInterfaceW();
	m_NinthCEGUI->getSrcDieScoreW()->hide();
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	//����������
	for (size_t i = 2; i <= 6;i++)
		m_NinthCEGUI->getSkillShadeW()[i]->show();

	//��ֹͣ�����������򳡾��ڵ������������쳣
	m_SoundMgr->StopAllSounds();
	//����ԭ���Ļ�Ծ����
	destroyActiveScene();

	//���ص�������
	m_NameEditW->enable();
	m_MainWin->show();
	Back(arg);
	
	return true;
}
//---------------------------------------��Ok()������---------------------------------------------
// Desc��ȷ�����ֵ�����
//------------------------------------------------------------------------------------------------
bool NinthCore::Ok(const CEGUI::EventArgs & arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	std::ofstream nameStream("Name.data");
	nameStream << m_CharName;
	m_NameShowW->setText("RoleName: " + m_CharName);
	nameStream.close();
	return true;
}
//-------------------------------------��EditNameChange()������-----------------------------------
// Desc���༭���Ƹı�
//------------------------------------------------------------------------------------------------
bool NinthCore::EditNameChange(const CEGUI::EventArgs & arg)
{
	m_CharName = m_NameEditW->getText().c_str();
	if (m_CharName != Ogre::String(""))
	{
		m_OkW->show();
	}
	else
	{
		m_OkW->hide();
	}
	return true;
}
//-------------------------------------��ChooseMap()������----------------------------------------
// Desc:ѡ���ͼ
//------------------------------------------------------------------------------------------------
bool NinthCore::ChooseMap(const CEGUI::EventArgs & arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iChooseMapSd, NULL, &m_iChooseMapChannel);

	const CEGUI::WindowEventArgs* pActEventArgs = dynamic_cast<const CEGUI::WindowEventArgs*>(&arg);
	CEGUI::Window* w = pActEventArgs->window;
	for (size_t i = 0; i < 6; i++)
	{
		if (w->getName() == m_MapW[i]->getName())
		{
			m_MapW[i]->setProperty("FrameColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
			m_MapIndex = i + 1;
		}
		else
			m_MapW[i]->setProperty("FrameColours", "tl:88BBBBBB tr:88BBBBBB bl:88BBBBBB br:88BBBBBB");
	}
	
	return true;
}