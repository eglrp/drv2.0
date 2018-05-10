#pragma once

#include "resource.h"
// CProject 对话框
class CProject : public CDialogEx
{
	DECLARE_DYNAMIC(CProject)

public:
	CProject(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProject();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CObject* parent; 
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOK();

	CString m_strFilePath;
	afx_msg void OnBnClickedButtonSave();
	void SaveProject();
};
