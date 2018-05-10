#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlModelLib = "00000000-0000-0000-0050-000000000020";
class CvlModelLib : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlModelLib, clsidvlModelLib)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlModelLib(void);
	~CvlModelLib(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();
};

