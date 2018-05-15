#pragma once
#include "resource.h"

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
};
