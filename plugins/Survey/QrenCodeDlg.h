#pragma once
#include "afxwin.h"
#include "resource.h"

// CQrenCodeDlg 对话框

class CQrenCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQrenCodeDlg)

public:
	CQrenCodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQrenCodeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_QRENCODE };

	CString sLocation;
	CString sHouseInfo;

protected:
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picLocation;
	CStatic m_picHouseInfo;
	afx_msg void OnPaint();
};
