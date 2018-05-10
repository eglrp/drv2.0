#pragma once
#include "resource.h"
#include "mybutton.h"
#include "afxwin.h"
#include "BackgroundDlg.h"
#include "SniperDlg.h"
#include "DLGmenjinqingdan.h"
#include "DLGNeighborSearch.h"
// CExplosiveDisposalDlg 对话框

class CExplosiveDisposalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExplosiveDisposalDlg)

public:
	CExplosiveDisposalDlg(int H,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExplosiveDisposalDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_Explosive };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	CObject* parent;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	CButton m_btn_neighbor;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1Neighbor();
	CButton m_btn_sniper;
	afx_msg void OnBnClickedButton2Sniper();
	CButton m_btn_doorRec;

	void play();
	void destroyAll();
private:
	int viewH;

protected:
	int nElapse;
	int count;
public:
	CBackgroundDlg* pDLG_BK;
	CSniperDlg* pDLG_sniper;
	CDLGmenjinqingdan* pDLG_MJQD;
	CDLGNeighborSearch* pDLG_neighbor;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonDoorrecord();
};
