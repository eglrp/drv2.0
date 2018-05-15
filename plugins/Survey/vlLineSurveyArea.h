#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>

//#ifdef _2D
#include "vlMeasureToolHandler.h"
//#else


#include <osgEarthUtil/MeasureTool>
//#endif
#include "AreaSurveyDlg.h"

using namespace osgEarth::Util;
USING_NAMESPACE_EARTHMATRIX

class MyMeasureToolEventHandlerArea : public CvlMeasureToolHandler::MeasureToolEventHandler
{
public:
	MyMeasureToolEventHandlerArea(CAreaSurveyDlg* win);

	virtual void onDistanceChanged(CvlMeasureToolHandler* sender, double distance, double distance1, double distance2);

	CAreaSurveyDlg* m_pWin;
};

const char* const clsidvlLineSurveyArea = "00000000-4900-1000-0000-000000000003";
class CvlLineSurveyArea : public IUICommon, public IUICommand, public IUITool
{
	X3BEGIN_CLASS_DECLARE(CvlLineSurveyArea, clsidvlLineSurveyArea)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)
		X3DEFINE_INTERFACE_ENTRY(IUITool)
	X3END_CLASS_DECLARE()

public:
	CvlLineSurveyArea(void);
	~CvlLineSurveyArea(void);

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
	osg::ref_ptr<MyMeasureToolEventHandlerArea> m_spMyMeasureEventHandler;
	osg::ref_ptr<CvlMeasureToolHandler> m_spMeasureEventHandler;

	CAreaSurveyDlg* m_pWin;
};

