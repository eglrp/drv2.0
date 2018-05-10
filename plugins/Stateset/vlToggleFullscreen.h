#pragma once
#include "UIPlugins.h"
#include "UIPluginsEvents.h"
#include "osgGA/StateSetManipulator"
#include "x3py/module/classmacro.h"
#include "osgGA/GUIEventHandler"
#include "osgViewer/GraphicsWindow"

USING_NAMESPACE_EARTHMATRIX

	const char* const clsidToggleFullscreen = "15B1FC53-1AB5-4549-B45A-35D67D22F2AF";
class ToggleFullscreenHandler : public osgGA::GUIEventHandler
{
public:
	ToggleFullscreenHandler();

public:
	void toggleFullscreen(osgViewer::GraphicsWindow *window);
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	bool isFullScreen(osgViewer::GraphicsWindow *window);
	bool GetFullScreenSize(osgViewer::GraphicsWindow *window, unsigned int& width, unsigned int& height);

protected:
	int m_oldX;
	int m_oldY;
	int m_oldWidth;
	int m_oldHeight;
	bool m_bWindowDecoration;
	long m_oldStyle;
	long m_oldExStyle;
	WINDOWPLACEMENT m_oldWndPlacement;
	HWND m_oldParent;
};

class CvlToggleFullscreen : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlToggleFullscreen, clsidToggleFullscreen)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CvlToggleFullscreen(void);
	~CvlToggleFullscreen(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand	
	virtual bool OnClick();

	// IAnythingEventObserver
	virtual bool OnAnything3(x3::IObject* sender, const std::string& eventKey, long wParam, long lParam);

protected:
	osg::ref_ptr<ToggleFullscreenHandler> m_spToggleFullscreenHandler;
};

