#pragma once
#include "DataManager.h"
#include "IViewer3D.h"
#include "sniperToolHandle.h"
#include "afxwin.h"
#include "DLGQrencode.h"
// CSniperDlg 对话框

class CSniperDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSniperDlg)

public:
	CSniperDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSniperDlg();

public:
	void InitialHandle();
	void UnInitialHandle();
// 对话框数据
	enum { IDD = IDD_DIALOG_SNIPER };

	x3::Object<IViewer3D> m_spViewer3D;
	osg::ref_ptr<LineAnalysisEventHandler> sniperHandle;
	CDLGQrencode* p_QrencodeDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	CEdit m_sniperInfo;
	afx_msg void OnBnClickedButton1Extractqrcode();
	afx_msg void OnBnClickedButtonsniperexit();
	CStatic m_dlgTitle;
	CButton m_EXIT;
	virtual BOOL OnInitDialog();

	CFont m_font;
	CObject* parent;
	CObject* parent1;
	CBrush m_brBk;
	HBRUSH m_editBrush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
