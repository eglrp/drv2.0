#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DLGQrencode.h"
#include "comFunction.h"
#include "CMyListCtr.h"
#include "ColorListCtrl.h"

// CDLGrenqunguanai 对话框

class CDLGrenqunguanai : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGrenqunguanai)

public:
	CDLGrenqunguanai(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGrenqunguanai();

// 对话框数据
	enum { IDD = IDD_DIALOG_RYGA };

	CBrush m_brBk;
	CFont m_font;
	CFont m_font1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CObject* parent;
	CComboBox m_combox_searchType;
	virtual BOOL OnInitDialog();
	CString m_searchContext;
	afx_msg void OnBnClickedButtonSearch();
	CMyListCtrl m_wndList;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

public:
	std::string getHouseID(std::string ,std::string);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonOlder();
	CString m_resultInfo;
	afx_msg void OnBnClickedButtonAll();

private:
	bool isShowAll;
	CDLGQrencode* pQren;
	int minAge;
	int maxAge;
public:
	CComboBox m_ComboBox_Build;
	CComboBox m_ComboBox_Unit;
	afx_msg void OnCbnSelchangeComboBuild();
	afx_msg void OnBnClickedButtonQrencode();
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);

private:
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int m_SortColumn;
	static BOOL m_bAsc;
public:
	afx_msg void OnBnClickedButtonSetting();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonChildren();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CStatic m_dlgTitle;
	CButton m_EXIT;
	afx_msg void OnBnClickedButtonRygaexit();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
