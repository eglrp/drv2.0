#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "ProjectSetDlg.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlSetProject = "00000000-6000-0000-0000-000000000003";
class CvlSetProject : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlSetProject, clsidvlSetProject)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlSetProject(void);
	~CvlSetProject(void);

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

	CProjectSetDlg* m_pWin; 
	bool bShow;

	void SetOSG_MAX_PAGEDLOD(int nMax);
	void SetOSG_NUM_DATABASE_THREADS(int nThreads);
	void SetScreenNum(int nNum);
	void SetCameraLimitHeight(int h);

	CString GetHouseLayer();
	CString GetBuildLayer();

	void SetHouseLayer(CString s);
	void SetBuildLayer(CString s);
};

