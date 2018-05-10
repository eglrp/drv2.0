#pragma once
#include "resource.h"
#include "myButton.h"
#include "afxwin.h"

#include "DLGanbaodongtai.h"
#include "DLGrenqunguanai.h"
#include "DLGShowRealData.h"
#include "DLGzuzhubiangeng.h"
#include "DLGSpecials.h"
#include "DLGjuhuigenzong.h"
#include "BackgroundDlg.h"
// CCommunityDlg 对话框

class CCommunityDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCommunityDlg)

public:
	CCommunityDlg(int H,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommunityDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_Community };
protected:
	int nElapse;
	int count;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	CObject* parent;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRrga();
	CButton m_tsrq;
	afx_msg void OnBnClickedButtonTsrq();
	afx_msg void OnBnClickedButtonWlyc();
	afx_msg void OnBnClickedButtonZzbg();
	afx_msg void OnBnClickedButtonAbdt();

	void play();
	void destroyAll();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	int viewH;
public:
	CButton m_rrga;
	CButton m_wlyc;
	CButton m_zzbg;
	CButton m_jhgz;
	CButton m_abdt;
	afx_msg void OnBnClickedButtonJhgz();

public:
	CDLGrenqunguanai* pDLG_RQGA;
	CDLGShowRealData* pDLG_WLYC;
	CDLGzuzhubiangeng* pDLG_ZZBG;
	CDLGSpecials* pDLG_TSRQ;
	CDLGjuhuigenzong* pDLG_JHGZ;
	CDLGanbaodongtai* pDLG_ABDT;

	CBackgroundDlg* pDLG_BK;
};
