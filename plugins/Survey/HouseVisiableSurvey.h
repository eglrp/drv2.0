#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>
#include "osgViewer/Viewer"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/vec3d"
#include "osg/LineWidth"
#include "DLGHouseVisiableSurvey.h"
#include "DLGBuildingVisiableSurvey.h"
#include "IViewer3D.h"
#include <map>
#include <fstream>
using namespace std;
USING_NAMESPACE_EARTHMATRIX

class HOUSEDATA
{
public:
	bool bVisiable;
	std::vector<osg::Vec3d> vecCoord;
	HOUSEDATA& operator=(HOUSEDATA other){
		bVisiable = other.bVisiable;
		vecCoord = other.vecCoord;
		return *this;
	}
};

class houseVisiableSurveyHandler : public osgGA::GUIEventHandler
{
public:
	houseVisiableSurveyHandler(DLGHouseVisiableSurvey* pWin);
	~houseVisiableSurveyHandler(){};
	bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );
	bool SetBuddy(x3::IObject* val);
	void getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);
	void getVisiableBuilding(osg::Vec3d v);
	CString FindSHPFile();
	bool testIntersect(osg::Group* group,osg::Vec3d p1,osg::Vec3d p2,std::vector<osg::Vec3d>& vecIntersect);
	void drawLine(osg::Group* lineGroup,std::vector<osg::Vec3d>& vecInsect,string name,bool bDepthTest = false);
	void changeOneGeometry(CString name,bool bAdd);
	void clearGeodeGroup(osg::Group*);
	void InsertPoints(osg::ref_ptr<osg::Vec3Array> allPoints,double rio);
	DLGHouseVisiableSurvey* m_pDLGHouseVisiableSurveyWin;
	osg::ref_ptr<osg::Group> gTemp;
	x3::Object<IViewer3D> m_spViewer3D;
	
	CString m_sDefLayer;
protected:
	bool _mouseDown;
	bool _isBusy;//标记是否正在执行搜索
	float _mouseDownX,_mouseDownY;//鼠标点击的屏幕坐标
	osg::Vec3d firstPt;//鼠标点击的场景坐标
	std::map<CString,HOUSEDATA,less<CString> > mVecData;

	bool _insert;
	double mValInsert;
	double mValIgnore;
};


const char* const clsidhouseVisiableSurvey = "00000000-3000-0000-0000-000000000003";
class CHouseVisiableSurvey : public IUICommon, public IUICommand, public IUITool
{
	X3BEGIN_CLASS_DECLARE(CHouseVisiableSurvey, clsidhouseVisiableSurvey)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)
		X3DEFINE_INTERFACE_ENTRY(IUITool)
		X3END_CLASS_DECLARE()

public:
	CHouseVisiableSurvey(void);
	~CHouseVisiableSurvey(void);

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

	bool bInSurvey;
	bool bDeactive;
	x3::IObject* m_val;
	DLGHouseVisiableSurvey* p_mDLGHouseVisiableSurveyWin;
	//houseVisiableSurveyHandler* p_mHouseVisiableSurveyHandler;
};

class Point3D 
{
public:
	Point3D(double X = 0,double Y = 0,double Z = 0):x(X),y(Y),z(Z)
	{

	}
	Point3D operator-(const Point3D &t) const  //重载减法
	{
		Point3D p;
		p.x = x - t.x;
		p.y = y - t.y;
		p.z = z - t.z;
		return p;
	}
	double length()
	{
		return sqrt( x*x + y*y + z*z );
	}
	double x;
	double y;
	double z;
};

//建筑可视分析
const char* const clsidBuildingVisiableSurvey = "00000000-3000-0000-0000-000000000004";
class CBuildingVisiableSurvey : public IUICommon, public IUICommand, public IUITool
{
	X3BEGIN_CLASS_DECLARE(CBuildingVisiableSurvey, clsidBuildingVisiableSurvey)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)
		X3DEFINE_INTERFACE_ENTRY(IUITool)
		X3END_CLASS_DECLARE()

public:
	CBuildingVisiableSurvey(void);
	~CBuildingVisiableSurvey(void);

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

	bool bInSurvey;
	bool bDeactive;
	x3::IObject* m_val;
	bool createData(const char* infile,const char* vectorFile,double span);
	CDLGBuildingVisiableSurvey* p_mDLGBuildingVisiableSurveyWin;
	//houseVisiableSurveyHandler* p_mHouseVisiableSurveyHandler;

	void insertPoint(std::vector<Point3D>& vecPoints,Point3D p1,Point3D p2,double span);
	void testVisiable(std::vector<Point3D>& vecAllPts,const char* sBuildingVector,double span);
};