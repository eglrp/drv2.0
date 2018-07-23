#pragma once
#include "resource.h"
#include "afxcmn.h"
#include <map>
// DLGTab1 对话框
using namespace std;
class DLGTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(DLGTab1)

public:
	DLGTab1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DLGTab1();

// 对话框数据
	enum { IDD = IDD_DIALOG_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CObject* parent;
	CListCtrl mWndList;
	virtual BOOL OnInitDialog();
	void InsertRow(CString sName, CString sValue);
	void setLVCheck(WPARAM ItemIndex, BOOL bCheck);
	std::map<CString,bool> mRowMap;//记录行状态
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void changeListPos();
	void changeOneGeometry(CString name,bool bAdd);//更改一条记录图形
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
