#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "osgViewer/ViewerEventHandlers"
#include "CSelfAnimationPathManipulator.h"
#include "CSelfKeySwitchMatrixManipulator.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlWalkPlay = "00000000-2000-0000-0000-000000000000";
class CvlWalkPlay : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlWalkPlay, clsidvlWalkPlay)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlWalkPlay(void);
	~CvlWalkPlay(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename);


	// IUICommand
	virtual bool OnClick();
	virtual bool Checked();

	bool Activate();
	bool Deactivate();

	bool bInPlay;

	osg::ref_ptr<CSelfKeySwitchMatrixManipulator> m_spKeyswitchManipulator;
	osg::ref_ptr<osgGA::CameraManipulator>	_oldManipulator;
	osg::ref_ptr<CSelfAnimationPathManipulator> _spAnimationPathManipulator;

	CString m_sViewport;
};

