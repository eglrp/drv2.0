#pragma once
#include "resource.h"
#include "afxwin.h"
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"

// CDlgRealVideo 对话框

class CDlgRealVideo : public CDialog
{
	DECLARE_DYNAMIC(CDlgRealVideo)

public:
	CDlgRealVideo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRealVideo();
	virtual BOOL OnInitDialog();


// 对话框数据
	enum { IDD = IDD_DIALOG_REALV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBr();
	afx_msg void OnBnClickedOk();
	CEdit m_cConfigPath;
	CEdit m_cDemo;
	CEdit m_cVideoPath;
	afx_msg void OnBnClickedButtonAddlocalv();

	bool isLocal;
	std::string slocalVideo;
	std::string slocalVideoConfig;
};
