#pragma once
#include "resource.h"

// CBackgroundDlg �Ի���

class CBackgroundDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBackgroundDlg)

public:
	CBackgroundDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBackgroundDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BACKGROUND1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
