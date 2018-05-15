// DLGHouseVisiableSurvey.cpp : implementation file
//

#include "stdafx.h"
#include "DLGHouseVisiableSurvey.h"
#include "afxdialogex.h"
#include "HouseVisiableSurvey.h"

// DLGHouseVisiableSurvey dialog

IMPLEMENT_DYNAMIC(DLGHouseVisiableSurvey, CDialogEx)

DLGHouseVisiableSurvey::DLGHouseVisiableSurvey(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLGHouseVisiableSurvey::IDD, pParent)
{

}

DLGHouseVisiableSurvey::~DLGHouseVisiableSurvey()
{
}

void DLGHouseVisiableSurvey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
}


BEGIN_MESSAGE_MAP(DLGHouseVisiableSurvey, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &DLGHouseVisiableSurvey::OnTcnSelchangeTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// DLGHouseVisiableSurvey message handlers


BOOL DLGHouseVisiableSurvey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mTab.InsertItem(0, _T("可视"));  
	mTab.InsertItem(1, _T("不可视"));
	mPage1.Create(IDD_DIALOG_TAB1, &mTab);
	mPage2.Create(IDD_DIALOG_TAB1, &mTab);

	CRect rc;  
	mTab.GetClientRect(rc);  
	rc.top += 20;  
	rc.bottom -= 0;  
	rc.left += 0;  
	rc.right -= 0;  
	mPage1.MoveWindow(&rc);  
	mPage2.MoveWindow(&rc);  

	mPage1.InsertRow(L"01-01-0201",L"张三");
	mPage2.InsertRow(L"02-01-0201",L"李四");
	//把对话框对象指针保存起来  
	mpDialog[0] = &mPage1;  
	mpDialog[1] = &mPage2;  
	//显示初始页面  
	mpDialog[0]->ShowWindow(SW_SHOW);  
	mpDialog[1]->ShowWindow(SW_HIDE);  
	//保存当前选择  
	mCurSelTab = 0;  
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DLGHouseVisiableSurvey::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	mpDialog[mCurSelTab]->ShowWindow(SW_HIDE);  
	//得到新的页面索引  
	mCurSelTab = mTab.GetCurSel();  
	//把新的页面显示出来  
	mpDialog[mCurSelTab]->ShowWindow(SW_SHOW);  
	*pResult = 0;
}


void DLGHouseVisiableSurvey::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CHouseVisiableSurvey* p = (CHouseVisiableSurvey*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}



