#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>
#include <osgEarthUtil/MeasureTool>
#include "vlMeasureToolHandler.h"
#include<osgEarthAnnotation/CircleNode>
#include "vPointSurveyInfoWin.h"
#include "IViewer3D.h"

using namespace osgEarth::Util;
USING_NAMESPACE_EARTHMATRIX

class PointSuerveyToolEventHandler  : public osgGA::GUIEventHandler
{
public:
#ifdef _2D
	PointSuerveyToolEventHandler(CPointSurveyInfoWin* win);
#else
	PointSuerveyToolEventHandler(CPointSurveyInfoWin* win, const osgEarth::SpatialReference* m_pEllipsoid, osgEarth::Annotation::CircleNode* m_pCircleNode);
#endif

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);
	virtual osg::Vec3d PointSuerveyToolEventHandler::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);
	CPointSurveyInfoWin* m_pWin;
	osgEarth::Annotation::CircleNode* m_pCircleNode;
	const osg::EllipsoidModel* m_pEllipsoid;
	const osgEarth::SpatialReference* spf;

	void readPrjInfo();
	void PointToWGS84(double x, double y, double& lon, double& lat);
	std::string replaceFileName(std::string srcStr, std::string replaceStr);
	
	std::string prjInfo;
};
const char* const clsidvlPointSurvey = "00000000-4900-1000-0000-000000000000";
										
class CvlPointSurvey : public IUICommon, public IUICommand, public IUITool
{
    X3BEGIN_CLASS_DECLARE(CvlPointSurvey, clsidvlPointSurvey)
    X3DEFINE_INTERFACE_ENTRY(IUICommon)
    X3DEFINE_INTERFACE_ENTRY(IUICommand)
    X3DEFINE_INTERFACE_ENTRY(IUITool)
    X3END_CLASS_DECLARE()

public:
    CvlPointSurvey(void);
    ~CvlPointSurvey(void);

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

	void readPrjInfo();
	void PointToWGS84(double x, double y, double& lon, double& lat);
	std::string getPrj()
	{
		x3::Object<IViewer3D> spViewer3D(m_spBuddy);
		return spViewer3D->GetPrj();
	}
    bool bInSurvey;

    osg::ref_ptr<PointSuerveyToolEventHandler> m_spPointEventHandler;
	osg::ref_ptr<CvlMeasureToolHandler> m_spMeasureToolEventHandler;
    CPointSurveyInfoWin* m_pPointSurveyInfoWin;

    osgEarth::Symbology::Style m_circleStyle;
    osgEarth::Annotation::CircleNode* m_pCircleNode;
    osg::Group* m_pLayerGroup;

};

