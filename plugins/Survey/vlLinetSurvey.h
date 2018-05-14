#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>
#include "vZLineSurveyInfoWin.h"
#include <osgGA/GUIEventHandler>
#include "osgViewer/Viewer"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/vec3d"
#include "osg/LineWidth"
using namespace std;
USING_NAMESPACE_EARTHMATRIX

class LineSurveyEventHandler  : public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);

	void drawSniperLine(osg::Group* lineGroup,vector<osg::Vec3d>& vecInsect);
	void setDis(double , double , double);
	LineSurveyEventHandler(osg::Group* group,CvZLineSurveyInfoWin* pWin);
	~LineSurveyEventHandler(){};

public:
	osg::Vec3d mlastPt;
	osg::ref_ptr< osg::Group > _group;
	osg::ref_ptr< osg::Group > lineGroup;
	vector<osg::Vec3d> vecCoord;
	CvZLineSurveyInfoWin* m_pLineSurveyInfoWin;
};

const char* const clsidvlLineSurvey = "00000000-4900-0000-1000-000000000001";
class CvlLineSurvey : public IUICommon, public IUICommand, public IUITool
{
	X3BEGIN_CLASS_DECLARE(CvlLineSurvey, clsidvlLineSurvey)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)
		X3DEFINE_INTERFACE_ENTRY(IUITool)
	X3END_CLASS_DECLARE()

public:
	CvlLineSurvey(void);
	~CvlLineSurvey(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();
	bool Checked();
	virtual bool Activate();
	virtual bool Deactivate();
	double GetDis(osg::Vec3 from, osg::Vec3 to);
	double GetDis(osg::Vec3Array * vec);
	bool bInSurvey;


public:

	//
	bool isTestJu;

	//0代表并未双击开始测试距离
	//1已经设置了起点
	//2已经设置了终点
	//
	int jieDuan;

	osg::Vec3 startPoint;
	osg::Vec3 endPoint;
	osg::ref_ptr<osg::Vec3Array> vectex;
	CvZLineSurveyInfoWin* m_pLineSurveyInfoWin;
	osg::ref_ptr<LineSurveyEventHandler> m_spLineSurveyEventHandler;
};

