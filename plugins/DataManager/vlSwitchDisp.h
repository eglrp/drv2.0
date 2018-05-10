#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlSwitchDisp = "8CF51F4A-67B6-49BA-B6CD-2C825B6A07F8";
class CvlSwitchDisp : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlSwitchDisp, clsidvlSwitchDisp)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlSwitchDisp(void);
	~CvlSwitchDisp(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);
	virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename);

	x3::Object<IViewer3D> m_spViewer3D;

	// IUICommand
	virtual bool OnClick();

	void SetLayerCheck(CString sName, bool bCheck);

};

