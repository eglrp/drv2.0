#pragma once
#include "resource.h"
#include "afxcmn.h"
#include <map>
// DLGTab1 �Ի���
using namespace std;
class DLGTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(DLGTab1)

public:
	DLGTab1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DLGTab1();

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CObject* parent;
	CListCtrl mWndList;
	virtual BOOL OnInitDialog();
	void InsertRow(CString sName, CString sValue);
	void setLVCheck(WPARAM ItemIndex, BOOL bCheck);
	std::map<CString,bool> mRowMap;//��¼��״̬
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void changeListPos();
	void changeOneGeometry(CString name,bool bAdd);//����һ����¼ͼ��
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
