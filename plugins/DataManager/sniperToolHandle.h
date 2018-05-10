#pragma once
#include "IViewer3D.h"
#include <vector>
#include <osgUtil/IntersectVisitor>
#include <osg/LineSegment>
#include <string>
#include "DataManager.h"
using namespace osgEarth::Util;
using namespace std;

class GetPosHandler : public osgGA::GUIEventHandler
{
public:
	GetPosHandler(osg::Group* group);
	~GetPosHandler(){};

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	bool findHouseInOSG(double mx,double my,double mz);
	void MakeFloor3DBorder(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Vec4 color = osg::Vec4(1,1,0,1));
public:
	bool bLButtonDown;
	double mx;
	double my;
	double mz;

	std::string houseName;
	osg::ref_ptr< osg::Group > _group;
};

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

	CEdit* editInfo;
};

struct HOUSEINFO
{
	std::string ownerName;
	std::string sex;
	std::string houseID;
	std::string phone;
	std::string idCard;
	std::string sDG;
	std::string sDG2;
};