#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "DLGTab1.h"
//#include "afxwin.h"
// DLGHouseVisiableSurvey dialog

class DLGHouseVisiableSurvey : public CDialogEx
{
	DECLARE_DYNAMIC(DLGHouseVisiableSurvey)

public:
	DLGHouseVisiableSurvey(CWnd* pParent = NULL);   // standard constructor
	virtual ~DLGHouseVisiableSurvey();

// Dialog Data
	enum { IDD = IDD_DIALOG_HOUSEVISIABLESURVER };

	CObject* parent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl mTab;
	int mCurSelTab;   
	DLGTab1 mPage1;   
	DLGTab1 mPage2;   
	CDialogEx* mpDialog[2];  //用来保存对话框对象指针  
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSelall();
	afx_msg void OnBnClickedButtonSelinv();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
