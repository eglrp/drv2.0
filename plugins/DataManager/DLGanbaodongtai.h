#pragma once
#include "afxcmn.h"
#include "InternetInfoDlg.h"
#include "afxwin.h"
#include "CMyListCtr.h"
// CDLGanbaodongtai 对话框

class CDLGanbaodongtai : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGanbaodongtai)

public:
	CDLGanbaodongtai(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGanbaodongtai();

// 对话框数据
	enum { IDD = IDD_DIALOG_ABDT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_wndList;
	virtual BOOL OnInitDialog();
	void doSearch();
	CStatic m_dlgTitle;
	CButton m_EXIT;
	CString m_info;

	CBrush m_brBk;
	CObject* parent;
	CFont m_font;
	CFont m_font1;
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAbdtexit();
};
