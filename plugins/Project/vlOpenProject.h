#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlOpenProject = "00000000-6000-0000-0000-000000000001";
class CvlOpenProject : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlOpenProject, clsidvlOpenProject)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlOpenProject(void);
	~CvlOpenProject(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUICommand
	virtual bool OnClick();

	void OpenProject();
};

