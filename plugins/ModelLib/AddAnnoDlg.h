#pragma once
#include "Resource.h"

// CAddAnnoDlg �Ի���

class CAddAnnoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAnnoDlg)

public:
	CAddAnnoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddAnnoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

	CObject* parent; 
	CString m_sProjectPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
