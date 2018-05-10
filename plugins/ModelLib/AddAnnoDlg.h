#pragma once
#include "Resource.h"

// CAddAnnoDlg 对话框

class CAddAnnoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAnnoDlg)

public:
	CAddAnnoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddAnnoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

	CObject* parent; 
	CString m_sProjectPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
