/*--------------------------------------------------------------------------
【Cpp文件】：Main.cpp        Created by Beyond Ray, 2014年4月
（Desc):主运行程序，以go()函数启动
----------------------------------------------------------------------------*/
#include "stdafx.h"
#include"NinthCore.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int iShowCmd)
#else
	int Main(int argc, char* argv[])
#endif
	{
		NinthCore app;
		try
		{
			app.go();
		}
		catch (Ogre::Exception & e)
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), TEXT("An exception has occured"), MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured": <<e.getFullDescription().c_str() << std::endl;
#endif
		}
		return 0;
	}

#ifdef __cplusplus
}
#endif

