#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "WalkRecDlg.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlWalkRec = "00000000-2000-0000-0000-000000000002";
class CvlWalkRec : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlWalkRec, clsidvlWalkRec)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlWalkRec(void);
	~CvlWalkRec(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename);

	// IUICommand
	virtual bool OnClick();

	bool Activate();
	bool Deactivate();

	void AddKeyPt();
	void SaveKeyPts();
	void ClearPts();

	CWalkRecDlg* m_pWin; 
	bool bShow;

	CStringArray aPts;
};

