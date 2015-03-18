#include "stdafx.h"
#include "NinthCEGUI.h"

//----------------------------��NinthCEGUI()������-------------------------------------
// Desc:���캯��
//-------------------------------------------------------------------------------------
NinthCEGUI::NinthCEGUI(Ogre::RenderWindow* window, bool* shutdown, SoundManager* soundMgr):
m_iClickChannel(INVALID_SOUND_CHANNEL),
m_SoundMgr(soundMgr),
m_Window(window),
m_ShutDown(shutdown),
m_iSkillStateNum(0),
m_RankListName("")
{
	//�����Ч��ʼ��
	m_iClickSd = m_SoundMgr->CreateSound(Ogre::String("(N)Click.wav"));

	//����״̬������ʼ��
	for (size_t i = 0; i < 3; i++)
		m_iSSIndex[i] = -1;

	 ratio = (float)m_Window->getWidth() / m_Window->getHeight();
	 CEGUICreateAndInitialise();
	 CreateFPSW(ratio);
	 CreateSrcBloodShowW(ratio);
	 CreateDragonDieNumW();
	 CreateGDragonDestDieNumW();
	 CreateSwordDieNumW();
	 CreateSrcDieScoreW();
	 CreateRewardingTextW();
	 CreateSShadeAndCoolCW();
	 CreateMonitorW();
}
//-----------------------------��~NinthCEGUI()������-----------------------------------
// Desc:��������
//-------------------------------------------------------------------------------------
NinthCEGUI::~NinthCEGUI()
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyWindow(m_DragonDieNumW);
	wmgr.destroyWindow(m_GDragonDieNumW);
	wmgr.destroyWindow(m_SwordDieNumW);
	wmgr.destroyWindow(m_DieScoreW);
	wmgr.destroyWindow(m_BloodW);
	wmgr.destroyWindow(m_BloodNumW);
	wmgr.destroyWindow(m_FPSW);
	wmgr.destroyWindow(m_QuitW);
	wmgr.destroyWindow(m_OneAgainW);
	wmgr.destroyWindow(m_HeadW);
}
//----------------------------��CEGUICreateAndInitialise()��---------------------------
// Desc:CEGUI��Ⱦ������Դ��ʼ��
//-------------------------------------------------------------------------------------
void NinthCEGUI::CEGUICreateAndInitialise()
{
	MyCEGUISystem::getSingletonPtr()->Init();
	
}
//----------------------------------��CreateDragonDieNmW()������------------------------------------
// Desc:������ʾĿ��AI���������Ĵ���
//--------------------------------------------------------------------------------------------------

void NinthCEGUI::CreateDragonDieNumW()
{

	//������ʾ����AI���������Ĵ���
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	m_DragonDieNumW = wmgr.createWindow("OgreTray/Titlebar", "Beyond/DieNum");

	//����������������λ�ô�С�������Ϊ�����ڵ��Ӵ���
	CEGUI::Window* root = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	m_DragonDieNumW->setArea(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.95f, 0),
		CEGUI::UDim(0.16f, 0), CEGUI::UDim(0.05f, 0));
	m_DragonDieNumW->hide();
	root->addChild(m_DragonDieNumW);
}

//----------------------------------��CreateGDragonDieNmW()������-----------------------------------
// Desc:������ʾڤ������AI���������Ĵ���
//--------------------------------------------------------------------------------------------------

void NinthCEGUI::CreateGDragonDestDieNumW()
{

	//������ʾڤ������AI���������Ĵ���
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	m_GDragonDieNumW = wmgr.createWindow("OgreTray/Titlebar", "Beyond/DieNum");

	//����������������λ�ô�С�������Ϊ�����ڵ��Ӵ���
	CEGUI::Window* root = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	m_GDragonDieNumW->setArea(CEGUI::UDim(0.84f, 0), CEGUI::UDim(0.95f, 0),
		CEGUI::UDim(0.16f, 0), CEGUI::UDim(0.05f, 0));
	m_GDragonDieNumW->hide();
	root->addChild(m_GDragonDieNumW);
}

//----------------------------------��CreateSwordDieNmW()������-------------------------------------
// Desc:������ʾ����AI���������Ĵ���
//--------------------------------------------------------------------------------------------------

void NinthCEGUI::CreateSwordDieNumW()
{

	//������ʾ����AI���������Ĵ���
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	m_SwordDieNumW = wmgr.createWindow("OgreTray/Titlebar", "Beyond/DieNum");

	//����������������λ�ô�С�������Ϊ�����ڵ��Ӵ���
	CEGUI::Window* root = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	m_SwordDieNumW->setArea(CEGUI::UDim(0.42f, 0), CEGUI::UDim(0.01f, 0),
		CEGUI::UDim(0.16f, 0), CEGUI::UDim(0.05f, 0));
	m_SwordDieNumW->hide();
	root->addChild(m_SwordDieNumW);
}

//----------------------------------��CreateSrcBloodShowW()������---------------------------------------
// Desc:������ģ��Ѫ����ʾ�Ĵ���
//------------------------------------------------------------------------------------------------------
void NinthCEGUI::CreateSrcBloodShowW(float ratio /* = 1.6f */)
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	
	//�����˸�Ѫ����Ѫ������������ͷ����������
	m_BloodW = wmgr.createWindow("WindowsLook/StaticImage", "Beyond/Blood");
	m_BloodNumW = wmgr.createWindow("OgreTray/StaticText", "Beyond/BloodNum");
	m_HeadW = wmgr.createWindow("WindowsLook/StaticImage", "Beyond/head");
	m_HeadW->setProperty("FrameEnabled", "false");
	m_HeadW->setProperty("BackgroundEnabled", "false");
	//����״̬����
	for (size_t i = 0; i < 3; i++)
	{
		Ogre::String tmepNum = Ogre::StringConverter::toString(i+1);
		m_SkillStateW[i] = wmgr.createWindow("WindowsLook/StaticImage", "skillState" + tmepNum);
		MyCEGUISystem::getSingletonPtr()->addFromImageFile("ss" + tmepNum, "ss" + tmepNum + ".jpg");
		MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_SkillStateW[i], "ss"+tmepNum, 
			(0.12f + i*0.033f) / ratio, 0.122f, 0.028f / ratio, 0.028f);
		m_SkillStateW[i]->setProperty("FrameEnabled", "false");
		m_SkillStateW[i]->hide();
	}
	//�������ͼƬ
	CEGUI::ImageManager::getSingleton().addFromImageFile("head", "head.png");
	for (size_t i = 0; i <= 10; i++)
		CEGUI::ImageManager::getSingleton().addFromImageFile("purple"+
		Ogre::StringConverter::toString(i), "purple_"+ Ogre::StringConverter::toString(i)+".jpg");
	//��ͼƬ��ӵ�������
	//MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_HeadW, "head", 0.02f/ratio, 0.02f, 0.1f/ratio, 0.1f);
	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_HeadW, "head", 0.0f, 0.0f, 0.16f / ratio, 0.16f);
	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_BloodW, "purple10", 0.11f / ratio, 0.085f, 0.25f/ratio, 0.035f);
	m_BloodNumW->setArea(CEGUI::UDim(0.135f / ratio, 0), CEGUI::UDim(0.05f, 0), 
		CEGUI::UDim(0.5f / ratio , 0), CEGUI::UDim(0.04f, 0));
	
	//ȥ��Ѫ�����ڵ��ı��߿�ͱ����������ı�Ϊǳ��ɫ
	m_BloodNumW->setProperty("FrameEnabled", "false");
	m_BloodNumW->setProperty("BackgroundEnabled", "false");
	m_BloodNumW->setProperty("TextColours", "tl:FFFFAAFF tr:FFFFAAFF bl:FFFFAAFF br:FFFFAAFF");

	//��ӵ�������
	CEGUI::Window* root = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	root->addChild(m_BloodW);
	root->addChild(m_BloodNumW);
	root->addChild(m_HeadW);
	for (size_t i = 0; i < 3; i++)
	{
		root->addChild(m_SkillStateW[i]);
	}
}

//----------------------------------��CreateSrcDieScoreW()������----------------------------------------
// Desc:������ʾ��ģ������ʱ������ʾ�Ĵ���
//------------------------------------------------------------------------------------------------------
void NinthCEGUI::CreateSrcDieScoreW()
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	//�����Ʒְ�
	m_DieScoreW = wmgr.createWindow("OgreTray/StaticText", "Beyond/DieScore");
	m_DieScoreW->setArea(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.25f, 0),
		CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.5f, 0));
	m_DieScoreW->setVisible(false);
	m_DieScoreW->setProperty("HorzFormatting", "WordWrapCentreAligned");

	//����һ�ְ�ť
	m_OneAgainW = wmgr.createWindow("OgreTray/Button", "Beyond/Quit");
	m_OneAgainW->setArea(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.85f, 0),
		CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.08f, 0));
	m_OneAgainW->setText("one again");
	m_OneAgainW->activate();

	//���������水ť
	m_ReturnMainW = wmgr.createWindow("OgreTray/Button", "Beyond/ReturnMain");
	m_ReturnMainW->setArea(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.85f, 0),
		CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.08f, 0));
	m_ReturnMainW->setText("return main");
	m_ReturnMainW->activate();

	//�˳���ť
	m_QuitW = wmgr.createWindow("OgreTray/Button", "Beyond/Quit");
	m_QuitW->setArea(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.85f, 0),
		CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.08f, 0));
	m_QuitW->setText("Quit");
	m_QuitW->activate();
	m_QuitW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCEGUI::Quit, this));

	//�����༭��
	m_RNameEditW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Editbox", "rNameEdit");
	m_RNameEditW->setArea(CEGUI::UDim(0.35f, 0.0f), CEGUI::UDim(0.7f, 0.0f),
		CEGUI::UDim(0.2f, 0.0f), CEGUI::UDim(0.06f, 0.0f));
	m_RNameEditW->activate();
	m_RNameEditW->hide();
	m_RNameEditW->setProperty("BlinkCaret", "true");
	m_RNameEditW->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&NinthCEGUI::EditNameChange, this));

	//OK��ť
	m_OkW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button", "ok");
	m_OkW->setText("ok");
	m_OkW->setArea(CEGUI::UDim(0.555f, 0.0f), CEGUI::UDim(0.7f, 0.0f),
		CEGUI::UDim(0.1f, 0.0f), CEGUI::UDim(0.06f, 0.0f));
	m_OkW->activate();
	m_OkW->hide();
	m_OkW->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NinthCEGUI::Ok, this));
	m_OkW->subscribeEvent(CEGUI::PushButton::EventKeyDown, CEGUI::Event::Subscriber(&NinthCEGUI::Ok, this));

	//����ȷ�Ϻ󵯳����ٴ�ȷ�Ͻ���
	m_NameAffirmW= wmgr.createWindow("OgreTray/StaticText", "nameAffirm");
	m_NameAffirmW->setArea(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.2f, 0),
		CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.6f, 0));
	m_NameAffirmW->setProperty("HorzFormatting", "WordWrapCentreAligned");
	m_NameAffirmW->setText("         Are you sure to\n use this name to record\n your achievement?        ");
	m_NameAffirmW->hide();

	//�ٴ�ȷ�ϵ�OK��ť
	m_OkAgainW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/Button", "okAgain");
	m_OkAgainW->setText("ok");
	m_OkAgainW->setArea(CEGUI::UDim(0.75f, 0.0f), CEGUI::UDim(0.8f, 0.0f),
		CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.08f, 0.0f));
	m_OkAgainW->activate();
	m_NameAffirmW->addChild(m_OkAgainW);

	//��ӵ������Ʒְ崰��
	CEGUI::Window* root = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	root->addChild(m_DieScoreW);
	m_DieScoreW->addChild(m_QuitW);
	m_DieScoreW->addChild(m_ReturnMainW);
	m_DieScoreW->addChild(m_OneAgainW);
	m_DieScoreW->addChild(m_RNameEditW);
	m_DieScoreW->addChild(m_OkW);
	m_DieScoreW->addChild(m_NameAffirmW);
}

//----------------------------------��CreateFPSW()������------------------------------------------
// Desc:������ʾFPS��͸������
//------------------------------------------------------------------------------------------------
void NinthCEGUI::CreateFPSW(float ratio /*=1.6f*/)
{
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();	
	m_FPSW = wmgr.createWindow("OgreTray/StaticText", "Beyond/FPS");

	//ȥ���ı��߿�ͱ����������ı�Ϊ��ɫ
	m_FPSW->setProperty("FrameEnabled", "false");
	m_FPSW->setProperty("BackgroundEnabled", "false");
	m_FPSW->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");

	//��ӵ�������
	CEGUI::Window* root = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	m_FPSW->setArea(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.80f, 0),
		CEGUI::UDim(0.17f / ratio, 0), CEGUI::UDim(0.084f, 0));
	root->addChild(m_FPSW);
}
//--------------------------------��ShowAllInterfaceW()������--------------------------------
// Desc:��ʾ���н��洰��
//-------------------------------------------------------------------------------------------
void NinthCEGUI::ShowAllInterfaceW()
{
	m_HeadW->show();
	m_BloodW->show();
	m_BloodNumW->show();
	m_DragonDieNumW->show();
	m_GDragonDieNumW->show();
	m_SwordDieNumW->show();
	m_FPSW->show();
}
//--------------------------------��HideAllInterfaceW()������--------------------------------
// Desc:�������н��洰��
//-------------------------------------------------------------------------------------------
void NinthCEGUI::HideAllInterfaceW()
{
	m_HeadW->hide();
	m_BloodW->hide();
	m_BloodNumW->hide();
	m_DragonDieNumW->hide();
	m_GDragonDieNumW->hide();
	m_SwordDieNumW->hide();
	m_FPSW->hide();
}
//---------------------------------------��Ok()������---------------------------------------------
// Desc��ȷ�����ֵ�����
//------------------------------------------------------------------------------------------------
bool NinthCEGUI::Ok(const CEGUI::EventArgs & arg)
{
	//���ŵ����Ч
	m_SoundMgr->PlaySound(m_iClickSd, NULL, &m_iClickChannel);

	m_NameAffirmW->show();
	m_OkAgainW->activate();
	return true;
}
//-------------------------------------��EditNameChange()������-----------------------------------
// Desc���༭���Ƹı�
//------------------------------------------------------------------------------------------------
bool NinthCEGUI::EditNameChange(const CEGUI::EventArgs & arg)
{
	m_RankListName = m_RNameEditW->getText().c_str();
	if (m_RankListName != Ogre::String(""))
	{
		m_OkW->show();
	}
	else
	{
		m_OkW->hide();
	}
	return true;
}
//------------------------------��createRewardingTextW()������-----------------------------
// Desc:������ʾ���а����ݵ�5������
//-----------------------------------------------------------------------------------------
void NinthCEGUI::CreateRewardingTextW()
{
	//���ļ��ж�ȡ���а�����
	std::ifstream infile("BeyondData.data");
	Ogre::String name[5];
	int score[5];
	int i = 0;
	while (!infile.eof())
	{
		if (!(infile >> name[i]) || !(infile >> score[i]))
			break;
		i++;
		if (i >= 5)
			break;
	}
	for (size_t j = i; j < 5; j++)
	{
		name[j] = "null";
		score[j] = 0;
	}
	infile.close();

	//ˢ�����а�
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	for (size_t i = 0; i < 5; i++)
	{
		//�������а��ı�����
		Ogre::String tempNum = Ogre::StringConverter::toString(i + 1);
		Ogre::String tempNum2 = Ogre::StringConverter::toString(i + 6);
		m_RTextW[i] = wmgr.createWindow("WindowsLook/StaticText", "Beyond/rewardingText" + tempNum);

		m_RTextW[i]->setArea(CEGUI::UDim(0.25f, 0.0f), CEGUI::UDim(0.1f + i*0.165f, 0.0f),
			CEGUI::UDim(0.5f, 0.0f), CEGUI::UDim(0.14f, 0.0f));
		m_RTextW[i]->setProperty("BackgroundEnabled", "false");
		m_RTextW[i]->setProperty("FrameEnabled", "false");
		m_RTextW[i+5] = wmgr.createWindow("WindowsLook/StaticText", "Beyond/rewardingText" + tempNum2);

		m_RTextW[i+5]->setArea(CEGUI::UDim(0.6f, 0.0f), CEGUI::UDim(0.1f + i*0.165f, 0.0f),
			CEGUI::UDim(0.3f, 0.0f), CEGUI::UDim(0.14f, 0.0f));
		m_RTextW[i+5]->setProperty("BackgroundEnabled", "false");
		m_RTextW[i+5]->setProperty("FrameEnabled", "false");

		//�������а�����	
		Ogre::String postifix;
		switch (i)
		{
		case 0:
			postifix = "st";
			m_RTextW[i]->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
			m_RTextW[i+5]->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
			break;
		case 1:
			postifix = "nd";
			m_RTextW[i]->setProperty("TextColours", "tl:FFFF00FF tr:FFFF00FF bl:FFFF00FF br:FFFF00FF");
			m_RTextW[i+5]->setProperty("TextColours", "tl:FFFF00FF tr:FFFF00FF bl:FFFF00FF br:FFFF00FF");
			break;
		case 2:
			postifix = "rd";
			m_RTextW[i]->setProperty("TextColours", "tl:FF0000FF tr:FF0000FF bl:FF0000FF br:FF0000FF");
			m_RTextW[i+5]->setProperty("TextColours", "tl:FF0000FF tr:FF0000FF bl:FF0000FF br:FF0000FF");
			break;
		case 3:
			postifix = "th";
			m_RTextW[i]->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
			m_RTextW[i+5]->setProperty("TextColours", "tl:FF00FF00 tr:FF00FF00 bl:FF00FF00 br:FF00FF00");
			break;
		case 4:
			postifix = "th";
			m_RTextW[i]->setProperty("TextColours", "tl:FF00AAFF tr:FF00AAFF bl:FF00AAFF br:FF00AAFF");
			m_RTextW[i+5]->setProperty("TextColours", "tl:FF00AAFF tr:FF00AAFF bl:FF00AAFF br:FF00AAFF");
			break;
		}
		
		if (name[i] != "null")
		{
			m_RTextW[i]->setText(tempNum + postifix + ",          " + name[i] + ",");
			m_RTextW[i+5]->setText("Scores: "+ Ogre::StringConverter::toString(score[i]));
		}
	}
}
//------------------------------��CreateSShadeAndCoolCW()������-----------------------------
// Desc:�����������ֺͼ�����ȴ��ʱ����
//------------------------------------------------------------------------------------------
void NinthCEGUI::CreateSShadeAndCoolCW()
{
	CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();
	//��1����������
	Ogre::String tempNum = Ogre::StringConverter::toString(1);
	m_SkillShadeW[0] = wmgr.createWindow("TaharezLook/StaticText", "skillshade" + tempNum);
	m_SkillShadeW[0]->setArea(CEGUI::UDim(0.112f, 0.0f), CEGUI::UDim(0.3f, 0.0f),
		CEGUI::UDim(0.078f, 0.0f), CEGUI::UDim(0.45f, 0.0f));
	m_SkillShadeW[0]->setAlpha(0.75f);
	m_SkillShadeW[0]->setProperty("FrameEnabled", "false");
	//��2,...,8����������
	for (size_t i = 0; i < 8; i++)
	{
		tempNum = Ogre::StringConverter::toString(i+1);
		m_SkillShadeW[i] = m_SkillShadeW[0]->clone(false);
		m_SkillShadeW[i]->setName("skillshade" + tempNum);
	}
	
	m_SkillShadeW[1]->setArea(CEGUI::UDim(0.2148f, 0.0f), CEGUI::UDim(0.3f, 0.0f),
		CEGUI::UDim(0.082f, 0.0f), CEGUI::UDim(0.45f, 0.0f));
	m_SkillShadeW[2]->setArea(CEGUI::UDim(0.315f, 0.0f), CEGUI::UDim(0.3f, 0.0f),
		CEGUI::UDim(0.075f, 0.0f), CEGUI::UDim(0.45f, 0.0f));
	m_SkillShadeW[3]->setArea(CEGUI::UDim(0.415f, 0.0f), CEGUI::UDim(0.3f, 0.0f),
		CEGUI::UDim(0.078f, 0.0f), CEGUI::UDim(0.45f, 0.0f));
	m_SkillShadeW[4]->setArea(CEGUI::UDim(0.518f, 0.0f), CEGUI::UDim(0.3f, 0.0f),
		CEGUI::UDim(0.076f, 0.0f), CEGUI::UDim(0.45f, 0.0f));
	m_SkillShadeW[5]->setArea(CEGUI::UDim(0.633f, 0.0f), CEGUI::UDim(0.358f, 0.0f),
		CEGUI::UDim(0.067f, 0.0f), CEGUI::UDim(0.374f, 0.0f));
	m_SkillShadeW[6]->setArea(CEGUI::UDim(0.723f, 0.0f), CEGUI::UDim(0.358f, 0.0f),
		CEGUI::UDim(0.065f, 0.0f), CEGUI::UDim(0.374f, 0.0f));
	m_SkillShadeW[7]->setArea(CEGUI::UDim(0.824f, 0.0f), CEGUI::UDim(0.37f, 0.0f),
		CEGUI::UDim(0.058f, 0.0f), CEGUI::UDim(0.348f, 0.0f));

	//��1,...,6��������ȴ������
	for (size_t i = 0; i < 8; i++)
	{
		if (i == 0 || i == 2)continue;
		tempNum = Ogre::StringConverter::toString(i + 1);
		m_SkillCoolCalW[i] = m_SkillShadeW[i]->clone(false);
		m_SkillCoolCalW[i]->setName("skillCoolCal" + tempNum);
		m_SkillCoolCalW[i]->setProperty("BackgroundEnabled", "false");
		m_SkillCoolCalW[i]->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
		m_SkillCoolCalW[i]->hide();
	}
	//Ĭ�Ͽ����ļ�������
	m_SkillShadeW[6]->show();
	CEGUI::WindowManager::getSingleton().destroyWindow(m_SkillShadeW[0]);
	tempNum = Ogre::StringConverter::toString(1);
	m_SkillShadeW[0] = wmgr.createWindow("TaharezLook/StaticImage", "skillshade" + tempNum);
	m_SkillShadeW[0]->setArea(CEGUI::UDim(0.06f, 0.0f), CEGUI::UDim(0.08f, 0.0f),
		CEGUI::UDim(0.12133f, 0.0f), CEGUI::UDim(0.7f, 0.0f));
	m_SkillShadeW[0]->setProperty("FrameEnabled", "false");
	m_SkillShadeW[0]->setProperty("BackgroundEnabled", "false");
	for (size_t i = 1; i <= 9; i++)
	{
		tempNum = Ogre::StringConverter::toString(i);
		CEGUI::ImageManager::getSingleton().addFromImageFile("fire" + tempNum, "fire_" + tempNum + ".png");
	}
	m_SkillShadeW[0]->hide();
	m_SkillShadeW[1]->hide();
	m_SkillShadeW[7]->hide();
	m_SkillCoolCalW[7]->setProperty("HorzFormatting", "WordWrapCentreAligned");
}
//----------------------------------��CreateMonitorW()������---------------------------------
// Desc:����������
//-------------------------------------------------------------------------------------------
void NinthCEGUI::CreateMonitorW()
{
	//���������
	m_MonitorFrameW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/StaticImage", "monitorFrameW");
	MyCEGUISystem::getSingletonPtr()->addFromImageFile("monitorFrame", "monitorFrame.tga");
	MyCEGUISystem::getSingletonPtr()->addImageToWindow(m_MonitorFrameW, "monitorFrame");
	MyCEGUISystem::getSingletonPtr()->getRootWindow()->addChild(m_MonitorFrameW);
	m_MonitorFrameW->setProperty("BackgroundEnabled", "false");
	m_MonitorFrameW->setProperty("FrameEnabled", "false");
	m_MonitorFrameW->setArea(CEGUI::UDim(1.0f-0.43483f/ratio, 0.0f), CEGUI::UDim(0.0f, 0.0f),
		CEGUI::UDim(0.43483f/ratio, 0.0f), CEGUI::UDim(0.38f, 0.0f));
	m_MonitorFrameW->hide();
	CEGUI::Window* root = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
	root->addChild(m_MonitorFrameW);

	//������
	m_MonitorW = CEGUI::WindowManager::getSingleton().createWindow("OgreTray/StaticImage", "monitorW");
	m_MonitorW->setProperty("FrameEnabled", "false");
	m_MonitorW->setArea(CEGUI::UDim(0.24f, 0.0f), CEGUI::UDim(0.162f, 0.0f),
		CEGUI::UDim(0.795f/1.065f, 0.0f), CEGUI::UDim(0.798f, 0.0f));
	m_MonitorFrameW->addChild(m_MonitorW);
}
//-------------------------------��showSkillState()������--------------------------------
// Desc:��ʾ����״̬
//---------------------------------------------------------------------------------------
void NinthCEGUI::showSkillState(int index, int imageIndex)
{
	if (index <0 || index > 3) return;
	//��ʾ����״̬ͼ��
	int ssNum = m_iSkillStateNum;
	Ogre::String tempNum = Ogre::StringConverter::toString(imageIndex);
	
	m_SkillStateW[ssNum]->setProperty("Image", "ss" + tempNum);
	m_SkillStateW[ssNum]->show();

	m_iSSIndex[index] = ssNum;
	m_iSkillStateNum++;

}
//-------------------------------��hideSkillState()������--------------------------------
// Desc:���ؼ���״̬
//---------------------------------------------------------------------------------------
void NinthCEGUI::hideSkillState(int index)
{
	int maxIndex = m_iSkillStateNum - 1;
	//������
	if (index < 0 || index > 3 || (m_iSSIndex[index] > maxIndex) || m_iSSIndex[index] == -1)
		return;

	//���ؼ���״̬
	m_SkillStateW[maxIndex]->hide();
	int i = m_iSSIndex[index];
	while (i != maxIndex)
	{
		m_SkillStateW[i]->setProperty("Image", m_SkillStateW[i+1]->getProperty("Image"));
		i++;
	}
	
	//���ü���״̬����,���ټ���״̬ͼ������
	for (size_t i = 0; i < 3; i++)
	{
		if (m_iSSIndex[i] > m_iSSIndex[index])
			m_iSSIndex[i]--;
	}
	m_iSSIndex[index] = -1;
	m_iSkillStateNum--;
}