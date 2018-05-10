#pragma once
#include "afxwin.h"
#include "resource.h"

// CQRencode2Dlg 对话框

class CQRencode2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQRencode2Dlg)

public:
	CQRencode2Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQRencode2Dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_QRENCODE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CStatic m_pic;
};
