#pragma once
#include "UIPlugins.h"
#include "UIPluginsEvents.h"
#include "osgGA/StateSetManipulator"
#include "x3py/module/classmacro.h"
#include "osgGA/GUIEventHandler"
#include "osgViewer/GraphicsWindow"
#include <osg/DisplaySettings>

USING_NAMESPACE_EARTHMATRIX

const char* const clsidToggle3D = "15B1FC53-1AB5-4549-B45A-35D67D22F2AD";
class CvZToggle3DHandler : public osgGA::GUIEventHandler
{
public:
	CvZToggle3DHandler(void);
	CvZToggle3DHandler(osg::DisplaySettings* pds);
	~CvZToggle3DHandler(void);

public:
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	osg::ref_ptr<osg::DisplaySettings> _pds;
	bool _stereo;
	int _stereoMode;
	int _displayType;
	double _eyeSeperation;
};

class CvZToggle3D : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvZToggle3D, clsidToggle3D)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CvZToggle3D(void);
	~CvZToggle3D(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	virtual bool Activate();
	virtual bool Deactivate();
	virtual bool Checked();

	// IUICommand	
	virtual bool OnClick();

	// IAnythingEventObserver
	virtual bool OnAnything3(x3::IObject* sender, const std::string& eventKey, long wParam, long lParam);

protected:
	osg::ref_ptr<CvZToggle3DHandler> m_spHandler;
	bool bIn3D;
};