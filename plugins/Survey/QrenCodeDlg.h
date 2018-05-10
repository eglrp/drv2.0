#pragma once
#include "afxwin.h"
#include "resource.h"

// CQrenCodeDlg �Ի���

class CQrenCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQrenCodeDlg)

public:
	CQrenCodeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQrenCodeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_QRENCODE };

	CString sLocation;
	CString sHouseInfo;

protected:
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picLocation;
	CStatic m_picHouseInfo;
	afx_msg void OnPaint();
};
