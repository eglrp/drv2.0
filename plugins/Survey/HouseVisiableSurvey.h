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
#include "IViewer3D.h"
#include <map>
using namespace std;
USING_NAMESPACE_EARTHMATRIX

class HOUSEDATA
{
public:
	CString name;
	bool bVisiable;
	std::vector<osg::Vec3d> vecCoord;
	HOUSEDATA& operator=(HOUSEDATA other){
		name = other.name;
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

	DLGHouseVisiableSurvey* m_pDLGHouseVisiableSurveyWin;
	osg::ref_ptr<osg::Group> gTemp;
	x3::Object<IViewer3D> m_spViewer3D;
	
	CString m_sDefLayer;
protected:
	bool _mouseDown;
	float _mouseDownX,_mouseDownY;//鼠标点击的屏幕坐标
	osg::Vec3d firstPt;//鼠标点击的场景坐标
	HOUSEDATA mHouseData;
	std::vector<HOUSEDATA > mVecData;
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
	x3::IObject* m_val;
	DLGHouseVisiableSurvey* p_mDLGHouseVisiableSurveyWin;
	//houseVisiableSurveyHandler* p_mHouseVisiableSurveyHandler;
};