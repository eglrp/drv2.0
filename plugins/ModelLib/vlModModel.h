#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlModModel = "00000000-0000-0000-0050-000000000010";
class CvlModModel : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlModModel, clsidvlModModel)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlModModel(void);
	~CvlModModel(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();
	virtual bool Checked();
};

