#pragma once
#include "UIPlugins.h"
#include <osgGA/GUIEventHandler>
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"

#include "osg/NodeCallback"
#include "osg/Group"
#include "osg/ShapeDrawable"
#include "AddPathDLG.h"
#include <vector>
#include <fstream>  
#include <string>  

using namespace std;
USING_NAMESPACE_EARTHMATRIX
class MyMouseEventHandler;
const char* const clsidvlAddModel = "00000000-4000-0000-0000-000000000002";
class CvlAddModel : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlAddModel, clsidvlAddModel)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlAddModel(void);
	~CvlAddModel(void);

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

	bool bInActive;

	osg::ref_ptr<MyMouseEventHandler> m_spEventHandler;

public:
	//功能函数
	void InsertPoint_1m(std::vector<osg::Vec3d>& vecIn,std::vector<osg::Vec3d>& vecOut); //路径插值
	void readPath(const char* filename,std::vector<osg::Vec3d>& vecKeyPt,std::string& ObjectName); //读路径
	osg::Geode* drawCicle(osg::Vec3d center,float radiu,osg::Vec4 color);
	
};

class MyMouseEventHandler  : public osgGA::GUIEventHandler
{
public:
	MyMouseEventHandler();
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);
	virtual osg::Vec3d MyMouseEventHandler::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);

	x3::Object<IViewer3D> spViewer3D;
	CvlAddModel* m_spAddModel;
};


class  CMyUpdateCallback : public osg::NodeCallback
{
public:
	CMyUpdateCallback(void);
	~CMyUpdateCallback(void);

public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
	osg::Geode* drawPath(std::vector<osg::Vec3d>& vec,osg::Vec4 color);
	osg::Geode* drawLine(osg::Vec3d p1,osg::Vec3d p2,osg::Vec4 color);

	std::vector<osg::Vec3d> vecKeyPath;
protected:
	OpenThreads::Mutex _addMutex;
	OpenThreads::Mutex _removeMutex;

	osg::Vec3d lastPosition;
	size_t Callback_n;
	size_t path_index;
};

