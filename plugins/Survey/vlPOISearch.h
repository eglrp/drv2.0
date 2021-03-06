#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlPOISearch = "00000000-0000-0000-0040-000000000090";
class CvlPOISearch : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlPOISearch, clsidvlPOISearch)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlPOISearch(void);
	~CvlPOISearch(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();
};

