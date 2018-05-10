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
#include "osgText/Font"  
#include "osgText/Text"

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
	void updateHbyImpact(vector<myPoint>& vecPt,osg::Group* root);
};
