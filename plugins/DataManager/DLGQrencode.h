#pragma once
#include "afxwin.h"


// CDLGQrencode �Ի���

class CDLGQrencode : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGQrencode)

public:
	CDLGQrencode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGQrencode();

// �Ի�������
	enum { IDD = IDD_DIALOG_QRENCODEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picQrencode;
	afx_msg void OnPaint();
	CButton m_EXIT;
	afx_msg void OnBnClickedButton1Qrencodeexit();
	virtual BOOL OnInitDialog();
};
