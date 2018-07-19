#pragma once
//#include "Resource.h"
#include "CMyListCtr.h"
#include "DlgAttInfoEx.h"
#include "ShowPictureDlg.h"
#include <vector>
#include "afxwin.h"

// CDlgAttInfo 对话框

class CDlgAttInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgAttInfo)

public:
	CDlgAttInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAttInfo();
// 对话框数据
	enum { IDD = IDD_DIALOG_ENTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	CFont m_font;

public:
	std::vector<CString> vName;
	std::vector<CString> vID;
	CObject* parent; 
	CDlgAttInfoEx* pChild;
	CShowPictureDlg* pChild1;
	CMyListCtrl	m_wndList;

	
	CBrush m_brBk;
	CObject* parentBk;

	int btnW;
	int btnH;
public:
	void InsertRow(CString sName, CString sValue);
	void RemoveAllRows();
	void setupPolygonRegion(SHOWMODE mode);
	void suitHeightAndWidth(int n,int maxW);
	void MovetoPoint(RECT viewRect);
	void changeDlgHeight(int h);
	//void analysisString(std::string inS,std::vector<CString>& vec);
	void selfDefShowWindow(UINT i);
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnNMClickDialogEntinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CButton m_quitBtn;
	afx_msg void OnBnClickedButtonexit1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
