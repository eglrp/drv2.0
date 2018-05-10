#pragma once
#include "resource.h"

// CAreaSurveyDlg 对话框

class CAreaSurveyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAreaSurveyDlg)

public:
	CAreaSurveyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAreaSurveyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	void SetArea(double , double);
	CObject* parent;
};
