#pragma once
#include "resource.h"

// CAddPathDLG �Ի���

class CAddPathDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPathDLG)

public:
	CAddPathDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddPathDLG();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
