#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CMyListCtr.h"

// CDLGSpecials 对话框

class CDLGSpecials : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGSpecials)

public:
	CDLGSpecials(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGSpecials();

// 对话框数据
	enum { IDD = IDD_DIALOG_SPECIALS };

	CBrush m_brBk;
	CObject* parent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combox_specialType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch1();
	CMyListCtrl m_wndList;
	CString m_Info;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int m_SortColumn;
	static BOOL m_bAsc;
public:
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTsrqexit();
	CStatic m_dlgTitle;
	CButton m_EXIT;
	CFont m_font;
	CFont m_font1;
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
