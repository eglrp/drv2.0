#pragma once
#include "resource.h"

// CDialogLoadShp �Ի���

class CDialogLoadShp : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLoadShp)

public:
	CDialogLoadShp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogLoadShp();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOAD_SHP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
};
