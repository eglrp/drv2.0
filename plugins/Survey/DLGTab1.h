#pragma once
#include "resource.h"
#include "afxcmn.h"

// DLGTab1 �Ի���

class DLGTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(DLGTab1)

public:
	DLGTab1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DLGTab1();

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl mWndList;
	virtual BOOL OnInitDialog();
	void InsertRow(CString sName, CString sValue);
};
