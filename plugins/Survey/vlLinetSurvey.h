#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <osgGA/GUIEventHandler>

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlLineSurvey = "00000000-4900-2000-0000-000000000001";
class CvlLineSurvey : public IUICommon, public IUICommand, public osgGA::GUIEventHandler
{
	X3BEGIN_CLASS_DECLARE(CvlLineSurvey, clsidvlLineSurvey)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlLineSurvey(void);
	~CvlLineSurvey(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();

	double GetDis(osg::Vec3 from, osg::Vec3 to);
	double GetDis(osg::Vec3Array * vec);

public:

	//
	bool isTestJu;

	//0代表并未双击开始测试距离
	//1已经设置了起点
	//2已经设置了终点
	//
	int jieDuan;

	osg::Vec3 startPoint;
	osg::Vec3 endPoint;
	osg::ref_ptr<osg::Vec3Array> vectex;

};

