#pragma once
#include "afxwin.h"


// CDLGQrencode2 对话框

class CDLGQrencode2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGQrencode2)

public:
	CDLGQrencode2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGQrencode2();

// 对话框数据
	enum { IDD = IDD_DIALOG_QRENCODE2INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonQrencode2exit();
	CButton m_EXIT;
	CStatic m_picHouseInfo;
	CStatic m_picLocation;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	CFont m_font;
	CObject* parent;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
