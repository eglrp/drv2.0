#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "AddAnnoDlg.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlAddAnno = "00000000-0000-0000-0050-000000000090";
class CvlAddAnno : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlAddAnno, clsidvlAddAnno)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlAddAnno(void);
	~CvlAddAnno(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	bool Activate();
	bool Deactivate();

	bool bShow;

	CAddAnnoDlg* m_pWin; 
	CString m_sProjectDir;

	// IUICommand
	virtual bool OnClick();
};

