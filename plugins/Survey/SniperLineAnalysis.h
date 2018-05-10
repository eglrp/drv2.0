#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>
#include <osgEarthUtil/MeasureTool>
#include "IViewer3D.h"
#include <vector>
#include <osgUtil/IntersectVisitor>
#include <osg/LineSegment>

using namespace osgEarth::Util;
using namespace std;
USING_NAMESPACE_EARTHMATRIX

class LineAnalysisEventHandler  : public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);
	
	void getIntersectPt(osg::Group* group,osg::Vec3d p1,osg::Vec3d p2,vector<osg::Vec3d>& vecInsect);
	void drawSniperLine(osg::Group* lineGroup,vector<osg::Vec3d>& vecInsect);

	LineAnalysisEventHandler(osg::Group* group);
	~LineAnalysisEventHandler(){};

public:
	vector<osg::Vec3d> vecCoord;
	osg::ref_ptr< osg::Group > _group;
	osg::ref_ptr< osg::Group > lineGroup;
	osg::ref_ptr< osg::Group > saveGroup;
};

const char* const clsidSniperLineAnalysis = "00000000-4900-1000-0000-000000000003";

class CSniperLineAnalysis : public IUICommon, public IUICommand, public IUITool
{
	X3BEGIN_CLASS_DECLARE(CSniperLineAnalysis, clsidSniperLineAnalysis)
		X3DEFINE_INTERFACE_ENTRY(IUICommon)
		X3DEFINE_INTERFACE_ENTRY(IUICommand)
		X3DEFINE_INTERFACE_ENTRY(IUITool)
		X3END_CLASS_DECLARE()

public:
	CSniperLineAnalysis(void);
	~CSniperLineAnalysis(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	x3::Object<IViewer3D> m_spViewer3D;

	// IUICommand
	virtual bool OnClick();
	virtual bool Checked();

	virtual bool Activate();
	virtual bool Deactivate();

	bool bActive;

	osg::ref_ptr<LineAnalysisEventHandler> m_spLineAnalysisEventHandler;
};