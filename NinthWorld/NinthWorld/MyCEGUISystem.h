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
	���һ��ͼƬ
	@imageName   ͼƬ������
	@fileName    ͼƬ�ļ���
	-----------------------------------------------------------------------------*/
	static void addFromImageFile(const CEGUI::String& imageName, const CEGUI::String& fileName);
	/**---------------------------------------------------------------------------
	���ͼ�μ�
	params:
	@imagesetName	������Imageset����, ȫ��Ψһ
	@fileName		����Imageset��ͼƬ�ļ�����
	------------------------------------------------------------------------------*/
	static void addImageset(const std::string& imagesetName, const std::string& fileName);

	/**--------------------------------------------------------------------------
	���һ��ͼƬ������ָ��������
	params:
	@window      �þ�̬ͼƬ���ڵ�ָ��
	@imageName   ͼƬ������
	@left        �������Ͻ�������ϲ㴰�ڵĺ�����
	@top         �������Ͻ�������ϲ㴰�ڵ�������
	@width       ����������ϲ㴰�ڵĿ��
	@height      ����������ϲ㴰�ڵĸ߶�
	remark:
	����һ����̬ͼƬ���ڣ�����ͼƬ�����ڴ���֮�ϣ����������ô�������ϲ㴰�ڵ�λ�á�
	-----------------------------------------------------------------------------*/
	void addImageToWindow(CEGUI::Window* window, const CEGUI::String& imageName,
		float left = 0.0f, float top = 0.0f, float width = 1.0f, float height = 1.0f);

	void addImageToWindow(CEGUI::Window* window, const CEGUI::String& imageName,
		const CEGUI::UVector2& pos);

	CEGUI::MouseButton convertMouseButton(const OIS::MouseButtonID id);

	// ע��CEGUI������¼�
	bool injectMouseButtonDown(CEGUI::MouseButton id)
	{
		return m_CEGUISystem->getSingleton().getDefaultGUIContext().injectMouseButtonDown(id);
	}
	bool injectMouseButtonUp(CEGUI::MouseButton id)
	{
		return m_CEGUISystem->getSingleton().getDefaultGUIContext().injectMouseButtonUp(id);
	}
	// ע��CEGUI����ƶ��¼�
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

	