#pragma once
#include "UIPlugins.h"
#include <osgGA/GUIEventHandler>
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"
#include "MovieEventHandler.h"

USING_NAMESPACE_EARTHMATRIX
class MyMouseEventHandler;
const char* const clsidvlPlayVideo = "00000000-5100-0000-0050-000000000130";
class CvlPlayVideo : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlPlayVideo, clsidvlPlayVideo)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlPlayVideo(void);
	~CvlPlayVideo(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();
	virtual bool Checked();

	virtual bool Activate();
	virtual bool Deactivate();

	bool bInActive;

	osg::ref_ptr<MovieEventHandler> m_spMovieHandler;
};