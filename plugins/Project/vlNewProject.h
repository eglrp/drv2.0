#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlOpenFile = "00000000-6000-0000-0000-000000000000";
class CvlNewProject : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlNewProject, clsidvlOpenFile)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlNewProject(void);
	~CvlNewProject(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();
};

