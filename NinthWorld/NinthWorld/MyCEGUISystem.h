#pragma  once

#include<CEGUI/CEGUI.h>
#include<CEGUI/RendererModules/Ogre/Renderer.h>
#include<Ogre.h>
#include<OIS.h>



class MyCEGUISystem
{
public:
	MyCEGUISystem() :m_Renderer(NULL),m_CEGUISystem(NULL){};
	MyCEGUISystem(const MyCEGUISystem& guiSystem){};
	MyCEGUISystem& operator=(const MyCEGUISystem& guiSystem){};
	virtual ~MyCEGUISystem();

	void Init();
	static MyCEGUISystem* getSingletonPtr()
	{
		if (ms_Singleton == NULL)
		{
			ms_Singleton = new MyCEGUISystem;
		}
		return ms_Singleton;
	}

	static MyCEGUISystem getSingleton()
	{
		if (ms_Singleton == NULL)
		{
			ms_Singleton = new MyCEGUISystem;
		}
		return (*ms_Singleton);
	}
	CEGUI::OgreRenderer* getRenderer(){ return m_Renderer; };

	/**--------------------------------------------------------------------------
	添加一张图片
	@imageName   图片的名字
	@fileName    图片文件名
	-----------------------------------------------------------------------------*/
	static void addFromImageFile(const CEGUI::String& imageName, const CEGUI::String& fileName);
	/**---------------------------------------------------------------------------
	添加图形集
	params:
	@imagesetName	创建的Imageset名称, 全局唯一
	@fileName		创建Imageset的图片文件名称
	------------------------------------------------------------------------------*/
	static void addImageset(const std::string& imagesetName, const std::string& fileName);

	/**--------------------------------------------------------------------------
	添加一张图片到窗口指定窗口中
	params:
	@window      该静态图片窗口的指针
	@imageName   图片的名字
	@left        窗口左上角相对于上层窗口的横坐标
	@top         窗口左上角相对于上层窗口的纵坐标
	@width       窗口相对于上层窗口的宽度
	@height      窗口相对于上层窗口的高度
	remark:
	创建一个静态图片窗口，并将图片覆盖在窗口之上，可自行设置窗口相对上层窗口的位置。
	-----------------------------------------------------------------------------*/
	void addImageToWindow(CEGUI::Window* window, const CEGUI::String& imageName,
		float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);

	void addImageToWindow(CEGUI::Window* window, const CEGUI::String& imageName,
		const CEGUI::UVector2& pos);

	CEGUI::MouseButton convertMouseButton(const OIS::MouseButtonID id);

	// 注入CEGUI鼠标点击事件
	bool injectMouseButtonDown(CEGUI::MouseButton id)
	{
		return m_CEGUISystem->getSingleton().getDefaultGUIContext().injectMouseButtonDown(id);
	}
	bool injectMouseButtonUp(CEGUI::MouseButton id)
	{
		return m_CEGUISystem->getSingleton().getDefaultGUIContext().injectMouseButtonUp(id);
	}
	// 注入CEGUI鼠标移动事件
	bool injectMouseMove(float xRel, float yRel)
	{
		return m_CEGUISystem->getSingleton().getDefaultGUIContext().injectMouseMove(xRel, yRel);
	}

	CEGUI::Window* getRootWindow()const{ return m_RootW; };

protected:
	CEGUI::OgreRenderer* m_Renderer;
	static MyCEGUISystem* ms_Singleton;
	CEGUI::System* m_CEGUISystem;
	CEGUI::Window* m_RootW;
};

	