// AddPathDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "AddPathDLG.h"
#include "afxdialogex.h"


// CAddPathDLG 对话框

IMPLEMENT_DYNAMIC(CAddPathDLG, CDialogEx)

CAddPathDLG::CAddPathDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddPathDLG::IDD, pParent)
{

}

CAddPathDLG::~CAddPathDLG()
{
}

void CAddPathDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddPathDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddPathDLG::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAddPathDLG 消息处理程序


void CAddPathDLG::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("文本文件(*.txt)|*.txt||"));
	if( dlg.DoModal() != IDOK )
	{
		return;
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	pEdit->SetWindowTextW(dlg.GetPathName());
}
