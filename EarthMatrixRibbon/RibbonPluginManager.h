#pragma once
#include "pluginmanager.h"

class CRibbonPluginManager : public CPluginManager
{
public:
	CRibbonPluginManager(void);
	~CRibbonPluginManager(void);

public:
	static CRibbonPluginManager* getInstance();
	static void Destroy();

private:
	static CRibbonPluginManager* s_pManager;

public:
	bool CreateControl(CMFCRibbonBar* pRibbonBar, CFrameWndEx* pFrameWnd);
	void OnCommand(int id);
	void OnUpdateCommand(CCmdUI* pCmdUI);
	void SetViewer(x3::IObject* val);
	void DestroyControl();

protected:
	bool AddRibbonButton(CMFCRibbonBar* pRibbonBar, IUICommon* pCommon, int id);	
	CMFCRibbonCategory* FindRibbonCategory(CMFCRibbonBar* pRibbonBar, LPCTSTR lpszCategory);
	CMFCRibbonPanel*  FindRibbonPanel(CMFCRibbonCategory* pCategory, LPCTSTR lpszCaption);
	CMFCRibbonPanel*  FindRibbonPanel(CMFCRibbonBar* pRibbonBar, LPCTSTR lpszCategory, LPCTSTR lpszCaption);
	bool CreatePane(CFrameWndEx* pFrameWnd, IUIWnd* pUIWnd, int id);	
	CDockablePane* FindPluginPane(int id);

protected:
	x3::AnyObject m_spViewer;
	x3::LockRW_<hash_map<int, CDockablePane*>> m_mapPluginPane;
};

