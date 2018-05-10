#pragma once
#include "comFunction.h"
#include "afxcmn.h"
#include "CMyListCtr.h"
#include "afxwin.h"

// CDLGzuzhubiangeng 对话框

class CDLGzuzhubiangeng : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGzuzhubiangeng)

public:
	CDLGzuzhubiangeng(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGzuzhubiangeng();

// 对话框数据
	enum { IDD = IDD_DIALOG_ZZBG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_wndList;
	virtual BOOL OnInitDialog();

	void doSearch();

private:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int m_SortColumn;
	static BOOL m_bAsc;
public:
	afx_msg void OnLvnColumnclickListZzbg(NMHDR *pNMHDR, LRESULT *pResult);

	CBrush m_brBk;
	CObject* parent;
	CFont m_font;
	CFont m_font1;
	CStatic m_dlgTitle;
	CButton m_EXIT;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonZzbgexit();
};
