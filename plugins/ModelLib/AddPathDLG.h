#pragma once
#include "resource.h"

// CAddPathDLG 对话框

class CAddPathDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPathDLG)

public:
	CAddPathDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddPathDLG();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
