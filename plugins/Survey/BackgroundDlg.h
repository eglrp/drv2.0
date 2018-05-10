#pragma once
#include "resource.h"

// CBackgroundDlg 对话框

class CBackgroundDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBackgroundDlg)

public:
	CBackgroundDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBackgroundDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BACKGROUND1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
