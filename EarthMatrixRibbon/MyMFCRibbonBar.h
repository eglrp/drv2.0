#pragma once


// CMyMFCRibbonBar

class CMyMFCRibbonBar : public CMFCRibbonBar
{
	DECLARE_DYNAMIC(CMyMFCRibbonBar)

public:
	CMyMFCRibbonBar();
	virtual ~CMyMFCRibbonBar();

	virtual BOOL OnShowRibbonQATMenu(CWnd* pWnd,int x,int y,CMFCRibbonBaseElement* pHit);
	virtual void OnEditContextMenu(CMFCRibbonRichEditCtrl* pEdit, CPoint point);
	virtual BOOL OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
protected:
	DECLARE_MESSAGE_MAP()
};


