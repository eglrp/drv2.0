// DLGSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGSettings.h"
#include "afxdialogex.h"


// CDLGSettings 对话框

IMPLEMENT_DYNAMIC(CDLGSettings, CDialogEx)

CDLGSettings::CDLGSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGSettings::IDD, pParent)
	, m_old(_T("60"))
	, m_child(_T("7"))
{

}

CDLGSettings::~CDLGSettings()
{
}

void CDLGSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD, m_old);
	DDX_Text(pDX, IDC_EDIT_CHILD, m_child);
}


BEGIN_MESSAGE_MAP(CDLGSettings, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDLGSettings::OnBnClickedOk)
END_MESSAGE_MAP()


// CDLGSettings 消息处理程序


void CDLGSettings::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}
