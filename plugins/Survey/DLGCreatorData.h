#pragma once
#include "resource.h"
#include "afxcmn.h"

// CDLGCreatorData dialog

class CDLGCreatorData : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGCreatorData)

public:
	CDLGCreatorData(CString sDefLayer,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGCreatorData();
	
// Dialog Data
	enum { IDD = IDD_DIALOG_CREATE_QUERY_DATA };
	CObject* parent;
	CString mBuildingLayer;
	CString mPathFile;
	CString mSpan;
	CProgressCtrl mProgress;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonSelpath();
	afx_msg void OnBnClickedButtonSelvector();
	afx_msg void OnBnClickedOk();
	void finish()
	{
		AfxMessageBox(_T("���"));
		OnClose();
	}
};
