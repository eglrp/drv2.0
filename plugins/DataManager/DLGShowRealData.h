#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ColorListCtrl.h"
#include "comFunction.h"
#include "CMyListCtr.h"
// CDLGShowRealData 对话框

class CDLGShowRealData : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGShowRealData)

public:
	CDLGShowRealData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGShowRealData();

// 对话框数据
	enum { IDD = IDD_DIALOG_REALDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
	void insertRows(std::vector<std::string>& vec);
	CMyListCtrl m_wndList;

	void resetList(int type);
	CComboBox m_combox;
	virtual BOOL OnInitDialog();
	CComboBox m_combox_limit;

private:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int m_SortColumn;
	static BOOL m_bAsc;
public:
	afx_msg void OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombo2();

	CBrush m_brBk;
	CObject* parent;
	CFont m_font;
	CFont m_font1;
	afx_msg void OnBnClickedButtonRealdataexit();
	CStatic m_dlgTitle;
	CButton m_EXIT;
	CString m_info;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
