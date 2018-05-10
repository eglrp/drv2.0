#pragma once
#include "resource.h"
#include "CMyListCtr.h"
#include "afxcmn.h"
#include <vector>
#include "afxwin.h"
// CDLGInternetInfo �Ի���

class CDLGInternetInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGInternetInfo)

public:
	CDLGInternetInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGInternetInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_NETINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void AutoAdjustDlg();
	void showInfo(bool bshow);
	void analysisData(std::string s,std::vector<CString>& vec);
protected:
	void InsertRow(std::vector<CString> vName);
	void AutoAdjustColumnWidth(CListCtrl *pListCtrl) ;
	
	void readFromDB();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();

public:
	CMyListCtrl m_wndList;
	CFont m_font;
	CObject* parent;
	CObject* parentBk;
	int _type;//���Ϊ�����ȵ���ʾ��Ϣ
	afx_msg void OnBnClickedButton1Netexit();
	CButton m_EXIT;
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	void selfDefShowWindow(UINT i);
};
