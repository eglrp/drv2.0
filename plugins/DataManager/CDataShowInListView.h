#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"
#include "Resource.h"
USING_NAMESPACE_EARTHMATRIX

const char* const clsidDataShowInListView = "00000000-4000-0000-0000-000000000000";
class CDataShowInListView : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CDataShowInListView, clsidDataShowInListView)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CDataShowInListView(void);
	~CDataShowInListView(void);
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);
	//virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename);
	// IUICommand
	virtual bool OnClick();
};
