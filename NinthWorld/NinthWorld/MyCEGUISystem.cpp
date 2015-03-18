#include "stdafx.h"
#include "MyCEGUISystem.h"

MyCEGUISystem* MyCEGUISystem::ms_Singleton = NULL;

//----------------------------------��~MyCEGUIManager()������------------------------------------
// Desc:��������
//-----------------------------------------------------------------------------------------------
MyCEGUISystem::~MyCEGUISystem()
{
	CEGUI::OgreRenderer::destroySystem();
	delete ms_Singleton;
	ms_Singleton = NULL;
}

//---------------------------------------��Init()������------------------------------------------
// Desc:CEGUI��ʼ��
//-----------------------------------------------------------------------------------------------
void MyCEGUISystem::Init()
{
	if (!m_Renderer)
	{
		m_Renderer = &CEGUI::OgreRenderer::bootstrapSystem();
		m_CEGUISystem = CEGUI::System::getSingletonPtr();
		//����CEGUI��Դ��ͼ�μ������⣬���壬���ڲ��������ڲ��֣�
		CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
		CEGUI::Scheme::setDefaultResourceGroup("Schemes");
		CEGUI::Font::setDefaultResourceGroup("Fonts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
		//CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
		CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
		//CEGUI::FontManager::getSingleton().createFromFile("beyond.font");
		//m_CEGUISystem->getSingletonPtr()->getDefaultGUIContext().setDefaultFont("chinese");

		//����CEGUI���ڹ������������ײ㴰��
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		m_RootW = wmgr.createWindow("DefaultWindow", "Beyond/Root");
		CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_RootW);
	}
}
//----------------------------------��addFromImageFile()������--------------------------------------
// Desc:���һ��ͼƬ
//--------------------------------------------------------------------------------------------------
void MyCEGUISystem::addFromImageFile(const CEGUI::String& imageName, const CEGUI::String& fileName)
{
	CEGUI::ImageManager::getSingleton().addFromImageFile(imageName, fileName);
}
//------------------------------------��createImageSet()������--------------------------------------
// Desc:���ͼ�μ�
//--------------------------------------------------------------------------------------------------
void MyCEGUISystem::addImageset(const std::string& imagesetName, const std::string& fileName)
{
	CEGUI::ImageManager::getSingleton().loadImageset(fileName);
}
//----------------------------------��createImageWindow()������-------------------------------------
// Desc:������̬ͼƬ����
//--------------------------------------------------------------------------------------------------
void MyCEGUISystem::addImageToWindow(CEGUI::Window* window,const CEGUI::String& imageName, 
	float left /* = 0.0f */, float top /* = 0.0f */, float width /* = 1.0f */, float height/* = 1.0f */)
{
	window->setArea(CEGUI::UDim(left, 0.0f), CEGUI::UDim(top, 0.0f),
		CEGUI::UDim(width, 0.0f), CEGUI::UDim(height, 0.0f));
	window->setProperty("Image", imageName);
}

void MyCEGUISystem::addImageToWindow(CEGUI::Window* window, const CEGUI::String& imageName,
	const CEGUI::UVector2& pos)
{
	CEGUI::BasicImage* image = (CEGUI::BasicImage*)(&CEGUI::ImageManager::getSingleton().get(imageName));
	window->setPosition(pos);
	window->setAutoWindow(true);
	window->setProperty("Image", imageName);
}
//----------------------------------��convertMouseButton()������----------------------------------
// Desc:OIS������ת����CEGUI������
//------------------------------------------------------------------------------------------------
CEGUI::MouseButton MyCEGUISystem::convertMouseButton(const OIS::MouseButtonID id)
{
	switch (id)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	default:
		return CEGUI::LeftButton;
	}
}