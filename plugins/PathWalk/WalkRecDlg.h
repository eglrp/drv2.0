#pragma once
#include "resource.h"
#include "afxwin.h"

// CWalkRecDlg �Ի���

class CWalkRecDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWalkRecDlg)

public:
	CWalkRecDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWalkRecDlg();

	CObject* parent; 

// �Ի�������
	enum { IDD = IDD_DIALOG_RECPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddkey();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CButton m_btnStart;
	CButton m_btnAdd;
	CButton m_btnStop;
	virtual BOOL OnInitDialog();
	CEdit m_editNum;
	CButton m_btnClear;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
};
