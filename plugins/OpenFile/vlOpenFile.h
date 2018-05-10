#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlOpenFile = "00000000-1000-0000-0000-000000000001";
class CvlOpenFile : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlOpenFile, clsidvlOpenFile)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlOpenFile(void);
	~CvlOpenFile(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);
	virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename);

	// IUICommand
	virtual bool OnClick();

};

