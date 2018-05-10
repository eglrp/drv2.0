#pragma once
#include "UIPlugins.h"
#include <osgGA/GUIEventHandler>
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"
#include "MovieEventHandler.h"

USING_NAMESPACE_EARTHMATRIX
class MyMouseEventHandlerVideo;
const char* const clsidvlAddVideo = "00000000-5100-0000-0050-000000000040";
class CvlAddVideo : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlAddVideo, clsidvlAddVideo)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlAddVideo(void);
	~CvlAddVideo(void);

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

	void EnsurePathExists(LPCTSTR lpszResourcePath);
	std::vector<std::string> split(std::string str, std::string pattern);
	std::string ToOSGNode(std::string imageFilePath, std::string inFilePath);

	bool bInActive;

	osg::ref_ptr<MyMouseEventHandlerVideo> m_spEventHandler;
	osg::ref_ptr<MovieEventHandler> m_spMovieHandler;

};

class MyMouseEventHandlerVideo  : public osgGA::GUIEventHandler
{
public:
	MyMouseEventHandlerVideo();
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);
	virtual osg::Vec3d getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);

	x3::Object<IViewer3D> spViewer3D;
	osg::ref_ptr<MovieEventHandler> m_spMovieHandler;
	CvlAddVideo* m_spAddModel;

	osg::Drawable* createVideoSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image=NULL);
	osg::Node* createVideoModel(osg::Vec3d v3d);	
};