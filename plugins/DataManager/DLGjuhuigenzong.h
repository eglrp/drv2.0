#pragma once
#include "afxcmn.h"
#include "comFunction.h"
#include "CMyListCtr.h"
#include "afxwin.h"
// CDLGjuhuigenzong 对话框

class CDLGjuhuigenzong : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGjuhuigenzong)

public:
	CDLGjuhuigenzong(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGjuhuigenzong();

// 对话框数据
	enum { IDD = IDD_DIALOG_JHGZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_wndList;
	virtual BOOL OnInitDialog();
	void doSearch();
	afx_msg void OnLvnColumnclickListJhgz(NMHDR *pNMHDR, LRESULT *pResult);

private:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int m_SortColumn;
	static BOOL m_bAsc;
public:
	CStatic m_dlgTitle;
	CButton m_EXIT;
	CBrush m_brBk;
	CObject* parent;
	CFont m_font;
	CFont m_font1;
	afx_msg void OnBnClickedButtonJhgzexit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
