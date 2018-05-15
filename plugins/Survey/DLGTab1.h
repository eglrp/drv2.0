#pragma once
#include "resource.h"
#include "afxcmn.h"

// DLGTab1 对话框

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
	CListCtrl mWndList;
	virtual BOOL OnInitDialog();
	void InsertRow(CString sName, CString sValue);
};
