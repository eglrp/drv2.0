#pragma once
#include "afxwin.h"


// CBackgroundDlg 对话框

class CBackgroundDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBackgroundDlg)

public:
	CBackgroundDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBackgroundDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BACKGROUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void changeSize(int dlgW,int dlgH);

	CBrush m_brush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_pic;
	afx_msg void OnPaint();
};
