// BackgroundDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BackgroundDlg.h"
#include "afxdialogex.h"


// CBackgroundDlg 对话框

IMPLEMENT_DYNAMIC(CBackgroundDlg, CDialogEx)

CBackgroundDlg::CBackgroundDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBackgroundDlg::IDD, pParent)
{

}

CBackgroundDlg::~CBackgroundDlg()
{
}

void CBackgroundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBackgroundDlg, CDialogEx)
END_MESSAGE_MAP()


// CBackgroundDlg 消息处理程序


BOOL CBackgroundDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(0,0,0));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),80,LWA_ALPHA);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
