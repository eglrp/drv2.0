#pragma once
#include "UIPlugins.h"
#include <osgGA/GUIEventHandler>
#include "IViewer3D.h"
#include "vlMeasureToolHandler.h"

const char* const clsidvlAddDynamicModel = "00000000-5100-0000-0050-000000000050";
class CvlAddDynamicModel : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlAddDynamicModel, clsidvlAddDynamicModel)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlAddDynamicModel(void);
	~CvlAddDynamicModel(void);

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

	//osg::ref_ptr<MyMouseEventHandlerVideo> m_spEventHandler;
	//osg::ref_ptr<CvlMeasureToolHandler> m_spMeasureEventHandler;
	osg::ref_ptr<CvlDynamicPathHandler> m_spMeasureEventHandler;
};