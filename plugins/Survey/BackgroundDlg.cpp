// BackgroundDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BackgroundDlg.h"
#include "afxdialogex.h"


// CBackgroundDlg �Ի���

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


// CBackgroundDlg ��Ϣ�������


BOOL CBackgroundDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(RGB(0,0,0));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),80,LWA_ALPHA);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
