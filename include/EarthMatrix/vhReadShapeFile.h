#pragma once 
#include <string>
#include <fstream>
#include <vector>
#include "osgUtil/IntersectVisitor"
#include "osg/LineSegment"
#include "osg/Group"
#include "osgUtil/LineSegmentIntersector"
#include "osg/LOD"
#include "osg\DrawPixels"
#include "osg/ShapeDrawable"
#include "osgText/Font"  
#include "osgText/Text"
#include "osg/PositionAttitudeTransform"
#include <osg/Billboard>
#include "osg/LineWidth"
#include <osgText/Text3D>
#include <osg/Material>

using namespace std;

class myPoint
{
public:
	myPoint(double a=0,double b = 0,double c = 0)
	{
		x = a;
		y = b;
		z = c;
	}

	double x;
	double y;
	double z;
};

class CKeyValue
{
public:
	CKeyValue(string s = " ",short v = 0)
	{
		key = s;
		val = v;
	}

	string key;
	short val;
};

class __declspec(dllexport) CReadShapeFile
{
public:
	unsigned long OnChangeByteOrder (int indata);
	string OnReadShp(std::string ShpFileName,vector<myPoint>& vecPt);
	void OnReadDbf(std::string DbfFileName,vector<string>& vecLab,vector<myPoint>& vecPt);
	void updateHbyImpact(vector<osg::Vec3d>& vecPt,osg::Group* root);
	string multiLine(std::string s,int k = 8);
	std::vector<int>& vacuate(vector<myPoint>& vecPt,double dis = 50);//³éÏ¡

	//osg::ref_ptr<osgText::Text> createText(osg::Vec3d pos,osg::Vec4 color,osgText::Font* font,std::string title,float size);
	osg::PositionAttitudeTransform* create3DText(osg::Vec3d pos,osg::Vec4 color, float radius, std::string title);
	osg::Billboard* createText(osg::Vec3d pos,osg::Vec4 color,osg::Vec4 outlineColor, float radius, std::string title);
};
