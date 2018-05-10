#if !defined(AFX_MYHTMLDLGDLG_H__8F553BA2_C70A_40D1_94A0_446965E045DA__INCLUDED_)
#define AFX_MYHTMLDLGDLG_H__8F553BA2_C70A_40D1_94A0_446965E045DA__INCLUDED_

#include "MyHtmlCtrl.h"	// Added by ClassView
#include "resource.h"
#if _MSC_VER > 1000
#pragma once
#endif 


// CHtmlDlg 对话框

class CHtmlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHtmlDlg)

public:
	CHtmlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHtmlDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_HTMLINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void setHTML(std::string name);
	void changeHTMLsize(int w,int h);
	void setupPolygonRegion();
protected:
	CHtmlCtrl* nObjOutputHTML;
	CBrush m_brush;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CObject* parent;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
#endif