#pragma once

#include "resource.h"
// CProject �Ի���
class CProject : public CDialogEx
{
	DECLARE_DYNAMIC(CProject)

public:
	CProject(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProject();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CObject* parent; 
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOK();

	CString m_strFilePath;
	afx_msg void OnBnClickedButtonSave();
	void SaveProject();
};
