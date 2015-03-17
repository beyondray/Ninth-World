/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------
 
This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
	  Tutorial Framework
	  http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "BaseApplication.h"
 
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <macUtils.h>
#endif

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
	: mRoot(0),
	m_Camera(0),
	mSceneMgr(0),
	mWindow(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mTrayMgr(0),
	mDetailsPanel(0),
	mbCursorWasVisible(false),
	mbShutDown(false),
	mbStartGame(false),
	mbFSceneRendered(false),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0),
	mOverlaySystem(0)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif

}
 
//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	if (mTrayMgr) delete mTrayMgr;
	if (mOverlaySystem) delete mOverlaySystem;
 
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}
 
//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true, "Welcome to NinthWorld!!!  ---beyondray");

		//加载图标，注意 IDI_ICON就是在资源头文件定义的名字
		HWND hwnd;
		mWindow->getCustomAttribute("WINDOW", (void *)(&hwnd));
		LONG iconId = (LONG)LoadIcon( GetModuleHandle(0),MAKEINTRESOURCE(IDI_ICON1) );
		SetClassLong(hwnd, GCL_HICON, iconId);
		return true;
	}
	else
	{
		return false;
	}
}

//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
#ifdef BSP_MODE
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../media/packs/map_oxodm3.pk3",
		"Zip", Ogre::ResourceGroupManager::getSingleton().getWorldResourceGroupName(), true);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(
		Ogre::ResourceGroupManager::getSingleton().getWorldResourceGroupName());
	mSceneMgr = mRoot->createSceneManager("BspSceneManager");
	mSceneMgr->setWorldGeometry("maps/oxodm3.bsp");
#else
	// Get the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
#endif
	// initialize the OverlaySystem (changed for 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
 
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
	mInputManager = OIS::InputManager::createInputSystem( pl );
 
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
 
	//Set initial mouse clipping size
	windowResized(mWindow);
 
	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
	mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;

	//预先创建Sdk和Font资源
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
	
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
	//mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	mTrayMgr->hideLogo();
	mTrayMgr->hideCursor();	
 
	mRoot->addFrameListener(this);
}

//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app,
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location
			if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
				archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{
	
}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
	mTrayMgr->showLoadingBar(1, 0);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Popular");
	mTrayMgr->hideLoadingBar();
	//mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
	#ifndef OGRE_STATIC_LIB	
		mResourcesCfg = m_ResourcePath + "resources_d.cfg";
		mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
	#else
		mResourcesCfg = "resources_d.cfg";
		mPluginsCfg = "plugins_d.cfg";
	#endif
#else
	#ifndef OGRE_STATIC_LIB
		mResourcesCfg = m_ResourcePath + "resources.cfg";
		mPluginsCfg = m_ResourcePath + "plugins.cfg";
	#else
		mResourcesCfg = "resources.cfg";
		mPluginsCfg = "plugins.cfg";
	#endif
#endif
 
	if (!setup())
		return;

	mRoot->startRendering();
	// clean up
	
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
	mRoot = new Ogre::Root(mPluginsCfg);
 
	setupResources();
 
	bool carryOn = configure();
	if (!carryOn) return false;
 
	chooseSceneManager();
	createCamera();
	createViewports();
 
	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
 
	// Create any resource listeners (for loading screens)
	createResourceListener();

	createFrameListener();

	// Load resources
	loadResources();
	// Create GUI scene
	createGUI();
	
	return true;
};
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;
 
	if(mbShutDown)
		return false;
 
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
 
	mTrayMgr->frameRenderingQueued(evt);
 
	return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
	if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up
 
	
	if(arg.key == OIS::KC_F5)   // refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_F9) //OIS::KC_SYSRQ)   // take a screenshot
	{
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mbShutDown = true;
	}
 
	return true;
}
 
bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}
 
bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    if (mTrayMgr->injectMouseMove(arg)) return true;
	return true;
}
 
bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
	return true;
}
 
bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
	return true;
}
 
//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);
 
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}
 
//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );
 
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}
