#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "PathManaDlg.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidvlWalkMana = "00000000-2000-0000-0000-000000000001";
class CvlWalkMana : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CvlWalkMana, clsidvlWalkMana)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlWalkMana(void);
	~CvlWalkMana(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);
	bool AddPath(CString sName);
	bool ImportPath(CString sPath);
	bool RecordPath(CString sName);
	bool DoRecord(std::string path);
	bool StopRecord();
	bool DelPath(CString sName);

	virtual void OnPathActive(std::string filename);

	virtual bool OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename);

	// IUICommand
	virtual bool OnClick();
	virtual bool Checked();

	bool Activate();
	bool Deactivate();

	CPathManaDlg* m_pWin; 
	bool bShow;
	CString m_sProjectDir;

	std::string GetFileContent(std::string path);
	void BuildPath(std::string path, CStringArray* a);
};

