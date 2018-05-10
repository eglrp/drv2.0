#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlOpenFileDem = "8CF51F4A-67B6-49BA-B6CD-2C825B6A07F4";
class CvlOpenFileDem : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlOpenFileDem, clsidvlOpenFileDem)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlOpenFileDem(void);
	~CvlOpenFileDem(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);
	virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename);

	// IUICommand
	virtual bool OnClick();
};

