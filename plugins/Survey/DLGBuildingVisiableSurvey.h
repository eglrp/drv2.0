#pragma once
#include "resource.h"

// CDLGBuildingVisiableSurvey dialog

class CDLGBuildingVisiableSurvey : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGBuildingVisiableSurvey)

public:
	CDLGBuildingVisiableSurvey(CString sDefLayer,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGBuildingVisiableSurvey();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

	CObject* parent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CString mDefLayer;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};
