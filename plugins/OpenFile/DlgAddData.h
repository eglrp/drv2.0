#pragma once
#include "resource.h"
#include "afxwin.h"
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

// CDlgAddData �Ի���

class CDlgAddData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddData)

public:
	CDlgAddData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddData();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrlocal();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
