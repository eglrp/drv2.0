#pragma once


// CDLGSettings �Ի���

class CDLGSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGSettings)

public:
	CDLGSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGSettings();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_old;
	CString m_child;

	CObject* parent;
	afx_msg void OnBnClickedOk();
};
