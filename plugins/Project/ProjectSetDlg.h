#pragma once
#include "resource.h"

// CProjectSetDlg �Ի���

class CProjectSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectSetDlg)

public:
	CProjectSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjectSetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETTING };

	CObject* parent; 
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();

	void InitSysInfo();
	afx_msg void OnBnClickedButtonSaveset();
	afx_msg void OnBnClickedButtonClose();

	int m_nOSG_MAX_PAGEDLOD;
	int m_nOSG_NUM_DATABASE_THREADS;
	int m_nScreenNum;
	int m_nCamera_Height_Limit;
	afx_msg void OnBnClickedButton2Gethouselayer();
	CString m_houseLayer;
	CString m_buildLayer;
	afx_msg void OnBnClickedButton3Getbuildlayer();
	afx_msg void OnBnClickedButtonSelprj();
	CString mPrjStr;

	std::string mPrj;//����ͶӰ��Ϣ
};
