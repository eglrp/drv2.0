#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CMyListCtr.h"

// CDLGmenjinqingdan �Ի���

class CDLGmenjinqingdan : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGmenjinqingdan)

public:
	CDLGmenjinqingdan(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGmenjinqingdan();

// �Ի�������
	enum { IDD = IDD_DIALOG_MJQD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_dlgTitle;
	CStatic m_result;
	CMyListCtrl m_wndList;
	CButton m_EXIT;

	CObject* parent;
	CFont m_font;
	CFont m_font1;
	CBrush m_brBk;
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonMjqdexit();

	void doSearch();
};
