#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CMyListCtr.h"

// CDLGmenjinqingdan 对话框

class CDLGmenjinqingdan : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGmenjinqingdan)

public:
	CDLGmenjinqingdan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGmenjinqingdan();

// 对话框数据
	enum { IDD = IDD_DIALOG_MJQD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_dlgTitle;
	CStatic m_result;
	CMyListCtrl m_wndList;
	CButton m_EXIT;

	CObject* parent;
	CFont m_font;
	CFont m_font1;
	CBrush m_brBk;
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonMjqdexit();

	void doSearch();
};
