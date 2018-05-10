#pragma once
#include "afxwin.h"


// CBackgroundDlg �Ի���

class CBackgroundDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBackgroundDlg)

public:
	CBackgroundDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBackgroundDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BACKGROUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void changeSize(int dlgW,int dlgH);

	CBrush m_brush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_pic;
	afx_msg void OnPaint();
};
