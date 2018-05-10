// DialogLoadShp.cpp : 实现文件
//

#include "stdafx.h"
#include "DialogLoadShp.h"
#include "afxdialogex.h"

CString g_sShpPath;
// CDialogLoadShp 对话框

IMPLEMENT_DYNAMIC(CDialogLoadShp, CDialogEx)

CDialogLoadShp::CDialogLoadShp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogLoadShp::IDD, pParent)
{

}

CDialogLoadShp::~CDialogLoadShp()
{
}

void CDialogLoadShp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogLoadShp, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogLoadShp::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CDialogLoadShp::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogLoadShp 消息处理程序


void CDialogLoadShp::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("Shp文件(*.shp)|*.shp||"));
	if( dlg.DoModal() != IDOK )
	{
		return;
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	pEdit->SetWindowTextW(dlg.GetPathName());
}


void CDialogLoadShp::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sData;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	pEdit->GetWindowText(sData);
	if (sData.GetLength() == 0)
	{
		AfxMessageBox(_T("Shp文件路径不能为空!"));
		return;
	}

	g_sShpPath = (sData);
	CDialogEx::OnOK();
}
