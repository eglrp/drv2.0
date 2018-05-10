#pragma once
#include "Resource.h"
#include "afxwin.h"

// CPathManaDlg �Ի���

class CPathManaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPathManaDlg)

public:
	CPathManaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPathManaDlg();

	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PATHMANA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_wndList;
	CObject* parent; 
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	CButton m_btnAdd;
	CButton m_btnDel;
	CButton m_btnActive;
	afx_msg void OnBnClickedButtonRecord();
//	afx_msg void OnNMClickListPath(NMHDR *pNMHDR, LRESULT *pResult);

	CString m_curPath;
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonActive();

	void InitListData(CStringArray* aPaths);
	afx_msg void OnClose();
	CButton m_btnRefresh;
	afx_msg void OnBnClickedButtonRefresh();

	CString m_sProjectPath;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnImpath();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnNMClickListPath(NMHDR *pNMHDR, LRESULT *pResult);

};
