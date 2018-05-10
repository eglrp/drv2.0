#pragma once
#include "afxwin.h"


// CDLGQrencode 对话框

class CDLGQrencode : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGQrencode)

public:
	CDLGQrencode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGQrencode();

// 对话框数据
	enum { IDD = IDD_DIALOG_QRENCODEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picQrencode;
	afx_msg void OnPaint();
	CButton m_EXIT;
	afx_msg void OnBnClickedButton1Qrencodeexit();
	virtual BOOL OnInitDialog();
};
