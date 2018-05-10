#include "stdafx.h"
#include "vlToggleFullscreen.h"
#include "EarthMatrixFunctions.h"
#include "IViewer3D.h"
#include "osgViewer/api/win32/GraphicsWindowWin32"

CvlToggleFullscreen::CvlToggleFullscreen(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"全屏"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"查看"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"视图"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"全屏或取消全屏"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"全屏或取消全屏"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"StatesetFullScreen"));

	m_spToggleFullscreenHandler = nullptr;
}


CvlToggleFullscreen::~CvlToggleFullscreen(void)
{
	m_spToggleFullscreenHandler = nullptr;
}

bool CvlToggleFullscreen::Initialize()
{
	supportsEvent("FullScreenEvent");
	return true;
}

bool CvlToggleFullscreen::UnInitialize()
{
	if( m_spBuddy.valid() )
	{
		x3::Object<IViewer3D> spViewer3D(m_spBuddy);
		if( spViewer3D.valid() )
		{			
			if( m_spToggleFullscreenHandler.valid() )
			{
				spViewer3D->getViewer()->removeEventHandler(m_spToggleFullscreenHandler.get());
				m_spToggleFullscreenHandler = nullptr;
			}
		}
	}
	return true;
}

bool CvlToggleFullscreen::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	spViewer3D->Lock();
	if( !m_spToggleFullscreenHandler.valid() )
		m_spToggleFullscreenHandler = new ToggleFullscreenHandler();
	spViewer3D->getViewer()->addEventHandler(m_spToggleFullscreenHandler.get());
	spViewer3D->Unlock();

	registerHandlers();
	IUICommand::Enable(true);
	return true;
}

// IUICommand
bool CvlToggleFullscreen::OnClick()
{
	USES_CONVERSION;
	IUICommand::Enable(false);
	Checked(false);

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if( !spViewer3D.valid() || !m_spToggleFullscreenHandler.valid() )
		return false;

	spViewer3D->Lock();

	osgViewer::Viewer::Windows windows;
	spViewer3D->getViewer()->getWindows(windows);
	for(osgViewer::Viewer::Windows::iterator itr = windows.begin();
		itr != windows.end();
		++itr)
	{
		m_spToggleFullscreenHandler->toggleFullscreen(*itr);
	}

	spViewer3D->Unlock();

	IUICommand::Enable(true);	
	return true;
}

bool CvlToggleFullscreen::OnAnything3(x3::IObject* sender, const std::string& eventKey, long wParam, long lParam)
{
	Checked((bool)wParam);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ToggleFullscreenHandler
ToggleFullscreenHandler::ToggleFullscreenHandler()
{

}

bool ToggleFullscreenHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view) return false;

	osgViewer::ViewerBase* viewer = view->getViewerBase();

	if (viewer == NULL)
	{
		return false;
	}

	if (ea.getHandled()) return false;

	switch(ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYUP):
		{
			if (ea.getKey() == 'f' || ea.getKey() == 'F' || ea.getKey()==osgGA::GUIEventAdapter::KEY_F11 || ea.getKey()==osgGA::GUIEventAdapter::KEY_Escape)
			{
				// sleep to allow any viewer rendering threads to complete before we
				// resize the window
				osgViewer::Viewer::Windows windows;
				viewer->getWindows(windows);
				for(osgViewer::Viewer::Windows::iterator itr = windows.begin();
					itr != windows.end();
					++itr)
				{
					if( ea.getKey()!=osgGA::GUIEventAdapter::KEY_Escape || isFullScreen(*itr) )
						toggleFullscreen(*itr);
				}

				aa.requestRedraw();
				return true;
			}
			break;
		}
	default:
		break;
	}
	return false;
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)  
{
	std::vector<RECT>* vtRect = (std::vector<RECT>*)dwData;
	if( vtRect==nullptr )
		return FALSE;

	//保存显示器信息  
	MONITORINFO monitorinfo;           
	monitorinfo.cbSize = sizeof(MONITORINFO);  

	//获得显示器信息，将信息保存到monitorinfo中  
	GetMonitorInfo(hMonitor, &monitorinfo);  

	if(monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
		vtRect->insert(vtRect->begin(), monitorinfo.rcMonitor);
	else
		vtRect->push_back(monitorinfo.rcMonitor);
	return TRUE;  
}  

bool ToggleFullscreenHandler::GetFullScreenSize(osgViewer::GraphicsWindow *window, unsigned int& width, unsigned int& height)
{
	std::vector<RECT> vtRect;
	EnumDisplayMonitors(NULL,NULL, MonitorEnumProc, (long)(&vtRect));
	if( vtRect.size()>1 )
	{
		width = 0;
		height = abs(vtRect.begin()->bottom - vtRect.begin()->top);
		for(std::vector<RECT>::iterator it=vtRect.begin(); it!=vtRect.end(); it++)
			width += abs(vtRect.begin()->right - vtRect.begin()->left);
	}
	else
	{
		osg::GraphicsContext::WindowingSystemInterface    *wsi = osg::GraphicsContext::getWindowingSystemInterface();
		if (wsi == NULL)
			return false;

		wsi->getScreenResolution(*(window->getTraits()), width, height);
	}

	return true;
}

bool ToggleFullscreenHandler::isFullScreen(osgViewer::GraphicsWindow *window)
{
	osg::GraphicsContext::WindowingSystemInterface    *wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (wsi == NULL)
		return false;

	unsigned int    screenWidth;
	unsigned int    screenHeight;

	GetFullScreenSize(window, screenWidth, screenHeight);
	//wsi->getScreenResolution(*(window->getTraits()), screenWidth, screenHeight);

	HWND hWnd = nullptr;
	osg::ref_ptr<osgViewer::GraphicsWindowWin32::WindowData> spWindowData = dynamic_cast<osgViewer::GraphicsWindowWin32::WindowData*>(window->getTraits()->inheritedWindowData.get());
	if( spWindowData.valid() )	
		hWnd = spWindowData->_hwnd;

	bool    isFullScreen = false;
	if( ::IsWindow(hWnd) )
	{
		CRect rect;
		::GetWindowRect(hWnd, rect);
		int x = rect.left;
		int y = rect.top;
		int width = rect.Width();
		int height = rect.Height();
		isFullScreen = width == (int)screenWidth && height == (int)screenHeight;
	}
	else
	{
		int x;
		int y;
		int width;
		int height;

		window->getWindowRectangle(x, y, width, height);
		isFullScreen = x == 0 && y == 0 && width == (int)screenWidth && height == (int)screenHeight;
	}

	return isFullScreen;
}

void ToggleFullscreenHandler::toggleFullscreen(osgViewer::GraphicsWindow *window)
{
	osg::GraphicsContext::WindowingSystemInterface    *wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (wsi == NULL)
		return;

	unsigned int    screenWidth;
	unsigned int    screenHeight;
	GetFullScreenSize(window, screenWidth, screenHeight);
	//wsi->getScreenResolution(*(window->getTraits()), screenWidth, screenHeight);

	HWND hWnd = nullptr;
	osg::ref_ptr<osgViewer::GraphicsWindowWin32::WindowData> spWindowData = dynamic_cast<osgViewer::GraphicsWindowWin32::WindowData*>(window->getTraits()->inheritedWindowData.get());
	if( spWindowData.valid() )	
		hWnd = spWindowData->_hwnd;

	bool    isFullScreen = false;
	if( ::IsWindow(hWnd) )
	{
		CRect rect;
		::GetWindowRect(hWnd, rect);
		int x = rect.left;
		int y = rect.top;
		int width = rect.Width();
		int height = rect.Height();
		isFullScreen = width == (int)screenWidth && height == (int)screenHeight;

		if (isFullScreen)
		{
			::SetWindowLong(hWnd, GWL_STYLE, m_oldStyle);
			::SetWindowLong(hWnd, GWL_EXSTYLE, m_oldExStyle);
			::SetParent(hWnd, m_oldParent);
			::SetWindowPlacement(hWnd, &m_oldWndPlacement);
		}
		else
		{
			m_oldX = x;
			m_oldY = y;
			m_oldWidth = width;
			m_oldHeight = height;
			m_oldParent = ::GetParent(hWnd);
			m_oldStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			m_oldExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			::GetWindowPlacement(hWnd, &m_oldWndPlacement);

			::SetWindowLong(hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
			::SetWindowLong(hWnd, GWL_EXSTYLE, m_oldExStyle & ~WS_EX_CLIENTEDGE);
			::SetParent(hWnd, ::GetDesktopWindow());

			CRect FullScreenRect;
			FullScreenRect.SetRect(0, 0, screenWidth, screenHeight);
			WINDOWPLACEMENT   wndpl;   
			wndpl.length = sizeof(WINDOWPLACEMENT);   
			wndpl.flags = 0;   
			wndpl.showCmd =SW_SHOWNORMAL;   
			wndpl.rcNormalPosition = FullScreenRect;  
			::SetWindowPlacement(hWnd, &wndpl);	
		}
	}
	else
	{
		int x;
		int y;
		int width;
		int height;

		window->getWindowRectangle(x, y, width, height);
		isFullScreen = x == 0 && y == 0 && width == (int)screenWidth && height == (int)screenHeight;

		if (isFullScreen)
		{
			window->setWindowDecoration(m_bWindowDecoration);	
			window->setWindowRectangle(m_oldX, m_oldY, m_oldWidth, m_oldHeight);		
		}
		else
		{
			m_oldX = x;
			m_oldY = y;
			m_oldWidth = width;
			m_oldHeight = height;
			m_bWindowDecoration = window->getWindowDecoration();
			window->setWindowDecoration(false);					
			window->setWindowRectangle(0, 0, screenWidth, screenHeight);
		}
	}

	window->grabFocusIfPointerInWindow();

	FireEventAnything3(nullptr, "FullScreenEvent", !isFullScreen, 0);
}