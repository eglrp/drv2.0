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
	bool _isBusy;//����Ƿ�����ִ������
	float _mouseDownX,_mouseDownY;//���������Ļ����
	osg::Vec3d firstPt;//������ĳ�������
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
	void operator=(Point3D& a){
		x = a.x;
		y = a.y;
		z = a.z;
	}
	Point3D operator-(const Point3D &t) const  //���ؼ���
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

//�̺߳���
void startCreateData(void* lpVoid);

//�������ӷ���
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

public:
	bool bDeactive;
	bool bInSurvey;
	x3::IObject* m_val;
	CDLGBuildingVisiableSurvey* p_mDLGBuildingVisiableSurveyWin;

public:
	//����xml����
	bool createData(const char* infile,const char* vectorFile,double span);
	//����ͨ�Ӽ��
	bool testIntersect(osg::Group* group,osg::Vec3d p1,osg::Vec3d p2,std::vector<osg::Vec3d>& vecIntersect);
	//����䰴ָ�������� �������һ������������һ����ľ���
	double insertPoint(std::vector<osg::Vec3d>& vecPoints,osg::Vec3d p1,osg::Vec3d p2,double span);
	//�۲��ͨ�ӷ���
	void testBuildingVisiable(osg::Group* Root,std::vector<osg::Vec3d>& vecAllPts,const char* sBuildingVector,double span,CDLGBuildingVisiableSurvey* pWin);
	
public:
	//ͨ�ӷ�������
	osg::Group* mRoot;
	double mSpan;
	std::vector<osg::Vec3d> mvecAllPts;
	std::string mBuildingVectorFile;
};

//����ͨ�ӷ����¼�handler
class buildingVisiableSurveyHandler : public osgGA::GUIEventHandler
{
public:
	buildingVisiableSurveyHandler(CDLGBuildingVisiableSurvey* pWin);
	~buildingVisiableSurveyHandler(){};
	bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );
	bool SetBuddy(x3::IObject* val);
	void getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);
	//���ƹ۲��
	void drawObservePt(osg::Vec3d p);
	//���»��ƹ۲��
	void reDrawObservePts(std::vector<osg::Vec3d>& vec);
	//����ֱ��
	void drawLine(osg::Vec3d p1,osg::Vec3d p2);
	//���»��ƣ��۲��͹۲��ߣ�
	void reDrawLines(osg::Vec3d p,std::vector<osg::Vec3d>& vecP,std::vector<osg::Vec3d>& vecObservePt);
	CDLGBuildingVisiableSurvey* m_pDLGBuildingVisiableSurveyWin;
	
public:
	//���ڻ��ƵĽڵ�
	osg::ref_ptr<osg::Group> gTemp;
	x3::Object<IViewer3D> m_spViewer3D;
	//Ĭ��ʸ��
	CString m_sDefLayer;

protected:
	bool _mouseDown;
	float _mouseDownX,_mouseDownY;//���������Ļ����
};