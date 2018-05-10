#pragma once


// CDLGSettings 对话框

class CDLGSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGSettings)

public:
	CDLGSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDLGSettings();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_old;
	CString m_child;

	CObject* parent;
	afx_msg void OnBnClickedOk();
};
