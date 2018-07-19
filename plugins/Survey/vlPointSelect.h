#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>
#include <osgEarthUtil/MeasureTool>
#include<osgEarthAnnotation/CircleNode>
#include "osg\Billboard"

#include <osgGA/GUIEventHandler>
#include <osgManipulator/Dragger>
#include <osgManipulator/CommandManager>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/RotateSphereDragger>
#include <osgManipulator/ScaleAxisDragger>

#include "IViewer3D.h"
#include "AttInfoDlg.h"
#include "HtmlDlg.h"
#include "ShowPictureDlg.h"
#include "CVMediaPlayerDlg.h"
#include "DLGInternetInfo.h"
#include "VLCPlayerDlg.h"

#include "dxfReader.h"
using namespace osgEarth::Util;
USING_NAMESPACE_EARTHMATRIX

class PickModelHandler : public osgGA::GUIEventHandler
{
public:
    PickModelHandler() : _activeDragger( 0 ), _enablePick( false ), _hasSelected( false )
    {
        Init();
    }

    bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );
	bool SetBuddy(x3::IObject* val);
	void AddCylinderNode(double x, double y, double z, double r);
	osg::ref_ptr<osg::Geode> AddPolygonNode(osg::Vec3Array* arr,osg::Vec4 color = osg::Vec4(1.0,1.0,0.0,0.8));
	void drawLine(osg::Group* lineGroup,std::vector<osg::Vec3d>& vecInsect,bool bDepthTest = false);
	osg::ref_ptr<osg::Group> gTemp;

protected:
    osgManipulator::Dragger* _activeDragger;
    osgManipulator::PointerInfo _pointer;
    osg::ref_ptr<osgManipulator::Selection> _selection;
    osg::ref_ptr<osgManipulator::TranslateAxisDragger> _draggerTranslate;
    osg::ref_ptr<osgManipulator::RotateSphereDragger> _draggerRotate;
    osg::ref_ptr<osgManipulator::ScaleAxisDragger> _draggerScale;
    osg::ref_ptr<osgManipulator::Dragger> _dragger;
    osg::ref_ptr<osgManipulator::CommandManager> _manager;

    bool _enablePick;
    bool _hasSelected;
	bool _buttonDown;

    void Init();
    bool SelectNode( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );
    bool DragNode( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );

	double GetBuildingHeight();
	bool BuildingInThisHeight(double height, int & nCrossNum);
	inline double distance2D(osg::Vec3d p1,osg::Vec3d p2)
	{
		return sqrt((p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y()) + 
			(p1.z() - p2.z())*(p1.z() - p2.z()));
	}
public:
    enum SELECT { MODEL, GEOD };
    enum OPERATION { TRANSLATE, ROTATE, SCALE };
    void EnableSelect( bool flag = false )
    {
        _enablePick = flag;
    }
    bool GetEnableSelect()
    {
        return _enablePick;
    }
    void SetSelectType( SELECT type )
    {
        _selectType = type;
    }
    SELECT GetSelectType()
    {
        return _selectType;
    }
    void SetOperationType( OPERATION type );//{ _operationType = type; }
    OPERATION GetOperationType()
    {
        return _operationType;
    }
    void TestOperationType();

	osg::ref_ptr<osg::Geode> drawBoxArea(osg::BoundingBox box,osg::Vec4 color);
	osg::ref_ptr<osg::Geode> drawPolyArea(std::vector<osg::Vec3d> vec,osg::Vec4 color);
	osg::ref_ptr<osg::Billboard> createText(osg::Vec3d pos,osg::Vec4 textColor,osg::Vec4 outlineColor, float layoutCharacterSize, std::string title);
	std::string queryOSG(osg::Vec3d v,osg::BoundingBox& box);
	bool queryDXF(osg::Vec3d v,_DXF::DL_DATA& dlData);
	void MakeBuilding3DBorder(osg::Vec3d v);

	void MakeFloor3DBorder(osgEarth::Features::Feature*, double dFloor, double dCeil,osg::Vec4 color = osg::Vec4(1,1,0,1));
	CString FindSHPFile();
	void Clear();

	x3::Object<IViewer3D> m_spViewer3D;

protected:
    SELECT _selectType;
    OPERATION _operationType;

	double dStep;
	double dStartHeight;
	double dEndHeight;
	bool bFindedStart;
	bool bFindedEnd;
	int aPtsInHeight[1024];
	CString m_sDefLayer;

	osg::ref_ptr<osgEarth::Symbology::Geometry> m_geoPart;
	
	float eaX,eaY;//鼠标点击的屏幕坐标
	osg::Vec3d firstPt;//鼠标点击的场景坐标

	double dxf_zOffset;
};

const char* const clsidvlPointSelect = "00000000-3000-0000-0000-000000000000";
class CvlPointSelect : public IUICommon, public IUICommand, public IUITool
{
    X3BEGIN_CLASS_DECLARE(CvlPointSelect, clsidvlPointSelect)
    X3DEFINE_INTERFACE_ENTRY(IUICommon)
	X3DEFINE_INTERFACE_ENTRY(IUICommand)
    X3DEFINE_INTERFACE_ENTRY(IUITool)
    X3END_CLASS_DECLARE()

public:
    CvlPointSelect(void);
    ~CvlPointSelect(void);

public:
    // IUICommon
    virtual bool Initialize();
    virtual bool UnInitialize();
    virtual bool SetBuddy(x3::IObject* val);

	virtual bool Activate();
	virtual bool Deactivate();

    // IUICommand
    virtual bool OnClick();
    virtual bool Checked();
    bool bInSurvey;

	//Tool
	bool allAlpha(std::string str);
	std::string getPrj()
	{
		x3::Object<IViewer3D> spViewer3D(m_spBuddy);
		return spViewer3D->GetPrj();
	}
    osg::ref_ptr<PickModelHandler> pickhandler;
    osg::Group* m_pLayerGroup;
	x3::IObject* m_val;

	CAttInfoDlg* m_pAttInfoWin; 
	CHtmlDlg* m_pHtmlInfoWin;
	CShowPictureDlg* m_pShowPictureInfoWin;
	CCVMediaPlayerDlg* m_pCVMediaPlayerInfoWin;
	CDLGInternetInfo* m_pInternetInfoWin;
	CVLCPlayerDlg* m_pVLCPlayerInfoWin;
};

