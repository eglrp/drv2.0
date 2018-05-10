#pragma once
#include "UIPlugins.h"
#include "osgGA/StateSetManipulator"
#include "x3py/module/classmacro.h"
#include "vZCompass.h"
#include "vZThumb.h"
#include <osgWidget/Util>

USING_NAMESPACE_EARTHMATRIX

class CStatesetCommand : public IUICommon, public IUICommand
{
public:
	CStatesetCommand(void);
	virtual ~CStatesetCommand(void);

public:
	osgGA::StateSetManipulator* getStateSetManipulator();

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand	
	virtual bool Enable();

	std::string sWorkDir;
};

const char* const clsidStatesetBackface = "00000000-0000-0000-0020-000000000010";
class CStatesetBackface : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetBackface, clsidStatesetBackface)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CStatesetBackface();
	virtual ~CStatesetBackface();

	// IUICommand	
	virtual bool Checked();
	virtual bool OnClick();
};

const char* const clsidStatesetLighting = "00000000-0000-0000-0020-000000000020";
class CStatesetLighting : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetLighting, clsidStatesetLighting)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CStatesetLighting();
	virtual ~CStatesetLighting();

	// IUICommand	
	virtual bool Checked();
	virtual bool OnClick();
};

const char* const clsidStatesetTexture = "00000000-0000-0000-0020-000000000030";
class CStatesetTexture : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetTexture, clsidStatesetTexture)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CStatesetTexture();
	virtual ~CStatesetTexture();

	// IUICommand	
	virtual bool Checked();
	virtual bool OnClick();
};

const char* const clsidStatesetPolygonModePoint = "00000000-0000-0000-0020-000000000040";
class CStatesetPolygonModePoint : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetPolygonModePoint, clsidStatesetPolygonModePoint)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CStatesetPolygonModePoint();
	virtual ~CStatesetPolygonModePoint();

	// IUICommand	
	virtual bool Checked();
	virtual bool OnClick();
};

const char* const clsidStatesetPolygonModeLine = "00000000-0000-0000-0020-000000000050";
class CStatesetPolygonModeLine : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetPolygonModeLine, clsidStatesetPolygonModeLine)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CStatesetPolygonModeLine();
	virtual ~CStatesetPolygonModeLine();

	// IUICommand	
	virtual bool Checked();
	virtual bool OnClick();
};

const char* const clsidStatesetPolygonModeFill = "00000000-0000-0000-0020-000000000060";
class CStatesetPolygonModeFill : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetPolygonModeFill, clsidStatesetPolygonModeFill)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CStatesetPolygonModeFill();
	virtual ~CStatesetPolygonModeFill();

	// IUICommand	
	virtual bool Checked();
	virtual bool OnClick();
};

const char* const clsidStatesetNorthPointer = "00000000-0000-0000-0020-000000000070";
class CStatesetNorthPointer : public CStatesetCommand, public osgGA::GUIEventHandler
{
	X3BEGIN_CLASS_DECLARE(CStatesetNorthPointer, clsidStatesetNorthPointer)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CStatesetNorthPointer();
	virtual ~CStatesetNorthPointer();

	// IUICommand
	virtual bool UnInitialize();
	virtual bool Checked();
	virtual bool OnClick();
	virtual bool SetBuddy(x3::IObject* val);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void setWindowSize(osg::Camera* pCamera, int width, int height);

protected:
	osg::ref_ptr<CvZCompass> m_spCompass;
	osg::ref_ptr<osg::Camera> m_spSubtitles;
};

const char* const clsidStatesetViewportInfo = "00000000-0000-0000-0020-000000000080";
class CStatesetViewportInfo : public CStatesetCommand, public osgGA::GUIEventHandler
{
	X3BEGIN_CLASS_DECLARE(CStatesetViewportInfo, clsidStatesetViewportInfo)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CStatesetViewportInfo();
	virtual ~CStatesetViewportInfo();

	// IUICommand
	virtual bool UnInitialize();
	virtual bool Checked();
	virtual bool OnClick();
	virtual bool SetBuddy(x3::IObject* val);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	void CreateViewportInfoWin();

protected:
	osg::ref_ptr<osg::Camera> m_spViewportInfo;
	//std::string m_sViewportInfo;
	osg::ref_ptr<osgText::Text> m_spText;
};

const char* const clsidStatesetThumb = "00000000-0000-0000-0020-000000000090";
class CStatesetThumb : public CStatesetCommand, public osgGA::GUIEventHandler
{
	X3BEGIN_CLASS_DECLARE(CStatesetThumb, clsidStatesetThumb)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CStatesetThumb();
	virtual ~CStatesetThumb();

	// IUICommand
	virtual bool UnInitialize();
	virtual bool Checked();
	virtual bool OnClick();
	virtual bool SetBuddy(x3::IObject* val);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void setWindowSize(osg::Camera* pCamera, int width, int height);

protected:
	osg::ref_ptr<CvZThumb> m_spThumb;
};

const char* const clsidStatesetSkyBox = "00000000-0000-0000-0020-000000000100";
class CStatesetSkyBox : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetSkyBox, clsidStatesetSkyBox)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CStatesetSkyBox();
	virtual ~CStatesetSkyBox();

	// IUICommand
	virtual bool UnInitialize();
	virtual bool Checked();
	virtual bool OnClick();
	virtual bool SetBuddy(x3::IObject* val);
	
	bool m_bShow;
	std::string m_sWorkDir;
	osg::ref_ptr<osg::Node> skynode;
};

const char* const clsidStatesetManipulator = "00000000-0000-0000-0020-000000000110";
class CStatesetManipulator : public CStatesetCommand
{
	X3BEGIN_CLASS_DECLARE(CStatesetManipulator, clsidStatesetManipulator)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CStatesetManipulator();
	virtual ~CStatesetManipulator();

	// IUICommand
	virtual bool UnInitialize();
	virtual bool Checked();
	virtual bool OnClick();
	virtual bool SetBuddy(x3::IObject* val);

	bool m_bShow;
	std::string m_sWorkDir;
};