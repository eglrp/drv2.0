#pragma once
#include "resource.h"
#include "afxwin.h"
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

// CDlgAddData 对话框

class CDlgAddData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddData)

public:
	CDlgAddData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddData();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrlocal();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
