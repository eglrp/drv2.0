
#include <string>
#include <iostream>
#include <strstream>
#include <osg/Group>


using namespace std;

#ifndef DLL_API
#define DLL_API __declspec(dllexport)
#endif
namespace toShpAndDxf
{
	DLL_API void convertToShp(vector<vector<osg::Vec3d> > &vec,string filename,string str);
	DLL_API void convertToDxf(vector<vector<osg::Vec3d> > &vec,string filename,string str);
}
