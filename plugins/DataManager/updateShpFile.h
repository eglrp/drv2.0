#pragma once

#include <osgEarthSymbology/Geometry>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <vector>

using namespace std;

class CUpdateShpFile
{
public:
	CUpdateShpFile(){}
	~CUpdateShpFile(){}
	void refetchShp(std::string sfield,int inout,int maxAge,int familyMem);
protected:
	void FindSHPFile();
private:
	vector<CString> vecShp;
	osg::ref_ptr<osgEarth::Features::FeatureSource> features;
};
 