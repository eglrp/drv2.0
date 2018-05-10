#pragma once
#include "resource.h"

// CDialogLoadShp 对话框

class CDialogLoadShp : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLoadShp)

public:
	CDialogLoadShp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogLoadShp();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOAD_SHP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
