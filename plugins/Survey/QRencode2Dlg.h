#pragma once
#include "afxwin.h"
#include "resource.h"

// CQRencode2Dlg �Ի���

class CQRencode2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQRencode2Dlg)

public:
	CQRencode2Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQRencode2Dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_QRENCODE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CStatic m_pic;
};
