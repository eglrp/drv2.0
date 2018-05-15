#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>

//#ifdef _2D
#include "vlMeasureToolHandler.h"
//#else


#include <osgEarthUtil/MeasureTool>
//#endif
#include "vZLineSurveyInfoWin.h"

using namespace osgEarth::Util;
USING_NAMESPACE_EARTHMATRIX

class MyMeasureToolEventHandler : public CvlMeasureToolHandler::MeasureToolEventHandler
{
public:
	MyMeasureToolEventHandler(CvZLineSurveyInfoWin* win);

	virtual void onDistanceChanged(CvlMeasureToolHandler* sender, double distance, double distance1, double distance2);

	CvZLineSurveyInfoWin* m_pWin;
};

const char* const clsidvlLineSurveyTri = "00000000-4900-1000-0000-000000000002";
class CvlLineSurveyTri : public IUICommon, public IUICommand, public IUITool
{
	X3BEGIN_CLASS_DECLARE(CvlLineSurveyTri, clsidvlLineSurveyTri)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)
		X3DEFINE_INTERFACE_ENTRY(IUITool)
	X3END_CLASS_DECLARE()

public:
	CvlLineSurveyTri(void);
	~CvlLineSurveyTri(void);

	void SetPathMode(bool bPath);

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

public:
	bool bInSurvey;
	osg::ref_ptr<MyMeasureToolEventHandler> m_spMyMeasureEventHandler;
	osg::ref_ptr<CvlMeasureToolHandler> m_spMeasureEventHandler;

	CvZLineSurveyInfoWin* m_pLineSurveyInfoWin;
};

