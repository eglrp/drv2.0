#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CMyListCtr.h"

// CDLGNeighborSearch 对话框

class CDLGNeighborSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGNeighborSearch)

public:
	CDLGNeighborSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGNeighborSearch();

// 对话框数据
	enum { IDD = IDD_DIALOG_NEIGHBOR };

	CBrush m_brBk;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_wndList;
	CComboBox m_Combo_build1;
	CComboBox m_Combo_unit1;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboBuild1();
	afx_msg void OnBnClickedButtonSearchneighbor1();
	
	CObject* parent;
private:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int m_SortColumn;
	static BOOL m_bAsc;
public:
	CString m_edit_floor1;
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_resultInfo;
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_dlgTitle;
	CButton m_EXIT;
	afx_msg void OnBnClickedButtonNeighborexit();
	afx_msg void OnBnClickedButton2Showall();

	CFont m_font;
	CFont m_font1;

	bool isShowAll;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
