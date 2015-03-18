#include "stdafx.h"
#include "MyCEGUISystem.h"

MyCEGUISystem* MyCEGUISystem::ms_Singleton = NULL;

//----------------------------------【~MyCEGUIManager()函数】------------------------------------
// Desc:析构函数
//-----------------------------------------------------------------------------------------------
MyCEGUISystem::~MyCEGUISystem()
{
	CEGUI::OgreRenderer::destroySystem();
	delete ms_Singleton;
	ms_Singleton = NULL;
}

//---------------------------------------【Init()函数】------------------------------------------
// Desc:CEGUI初始化
//-----------------------------------------------------------------------------------------------
void MyCEGUISystem::Init()
{
	if (!m_Renderer)
	{
		m_Renderer = &CEGUI::OgreRenderer::bootstrapSystem();
		m_CEGUISystem = CEGUI::System::getSingletonPtr();
		//加载CEGUI资源（图形集，主题，字体，窗口部件，窗口布局）
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

		//引用CEGUI窗口管理器并创建底层窗口
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		m_RootW = wmgr.createWindow("DefaultWindow", "Beyond/Root");
		CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(m_RootW);
	}
}
//----------------------------------【addFromImageFile()函数】--------------------------------------
// Desc:添加一张图片
//--------------------------------------------------------------------------------------------------
void MyCEGUISystem::addFromImageFile(const CEGUI::String& imageName, const CEGUI::String& fileName)
{
	CEGUI::ImageManager::getSingleton().addFromImageFile(imageName, fileName);
}
//------------------------------------【createImageSet()函数】--------------------------------------
// Desc:添加图形集
//--------------------------------------------------------------------------------------------------
void MyCEGUISystem::addImageset(const std::string& imagesetName, const std::string& fileName)
{
	CEGUI::ImageManager::getSingleton().loadImageset(fileName);
}
//----------------------------------【createImageWindow()函数】-------------------------------------
// Desc:创建静态图片窗口
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
//----------------------------------【convertMouseButton()函数】----------------------------------
// Desc:OIS的鼠标键转换成CEGUI的鼠标键
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