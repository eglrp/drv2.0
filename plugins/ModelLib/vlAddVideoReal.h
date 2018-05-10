#pragma once
#include "UIPlugins.h"
#include <osgGA/GUIEventHandler>
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"
#include "MovieEventHandler.h"
#include "vlAddVideo.h"

USING_NAMESPACE_EARTHMATRIX
class MyMouseEventHandlerVideoReal;
const char* const clsidvlAddVideoReal = "00000000-5100-0000-0050-000000000100";
class CvlAddVideoReal : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlAddVideoReal, clsidvlAddVideoReal)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CvlAddVideoReal(void);
	~CvlAddVideoReal(void);

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

	void activaLocalVideoHander(std::string s1,std::string s2);//激活本地视频播放事件

	std::string ToOSGNode(std::string imageFilePath, std::string inFilePath);
	std::vector<std::string> split(std::string str, std::string pattern);

	bool bInActive;

	osg::ref_ptr<MyMouseEventHandlerVideoReal> m_spEventHandler;
	osg::ref_ptr<MyMouseEventHandlerVideo> m_spEventHandler1;
	osg::ref_ptr<MovieEventHandler> m_spMovieHandler;

};

class MyMouseEventHandlerVideoReal  : public osgGA::GUIEventHandler
{
public:
	MyMouseEventHandlerVideoReal();
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);
	virtual osg::Vec3d getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);

	x3::Object<IViewer3D> spViewer3D;
	osg::ref_ptr<MovieEventHandler> m_spMovieHandler;
	CvlAddVideoReal* m_spAddModel;

	osg::Drawable* createVideoSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image=NULL);
	osg::Node* createVideoModel(osg::Vec3d v3d);
};