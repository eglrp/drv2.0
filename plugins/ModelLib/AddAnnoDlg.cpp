// AddAnnoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AddAnnoDlg.h"
#include "afxdialogex.h"


// CAddAnnoDlg 对话框

IMPLEMENT_DYNAMIC(CAddAnnoDlg, CDialogEx)

CAddAnnoDlg::CAddAnnoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddAnnoDlg::IDD, pParent)
{

}

CAddAnnoDlg::~CAddAnnoDlg()
{
}

void CAddAnnoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddAnnoDlg, CDialogEx)
END_MESSAGE_MAP()


// CAddAnnoDlg 消息处理程序
