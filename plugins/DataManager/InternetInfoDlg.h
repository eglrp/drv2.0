#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "CMyListCtr.h"
#include <vector>
#include "afxwin.h"

struct PERSON_LOCATION
{
	std::string name;
	double x;
	double y;
	double z;
};
// CInternetInfoDlg �Ի���

class CInternetInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInternetInfoDlg)

public:
	CInternetInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInternetInfoDlg();

	void InsertRow(std::vector<CString> vName);
	void AutoAdjustColumnWidth(CListCtrl *pListCtrl) ;
	void AutoAdjustDlg();
	void showInfo(bool bshow);
	void updatePolicesPosition();//���¾�Աλ��

	void selfDefShowWindow(UINT i);
// �Ի�������
	enum { IDD = IDD_DIALOG_INFO_NET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_wndList;
	CFont m_font;
	CObject* parent;
	CObject* parentBk;
	int _type;//���Ϊ�����ȵ���ʾ��Ϣ
	bool isAble;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CButton m_EXIT;
	afx_msg void OnBnClickedButtonNetexit();
};
