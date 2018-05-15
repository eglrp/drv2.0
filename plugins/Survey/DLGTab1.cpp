// DLGTab1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DLGTab1.h"
#include "afxdialogex.h"


// DLGTab1 �Ի���

IMPLEMENT_DYNAMIC(DLGTab1, CDialogEx)

DLGTab1::DLGTab1(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLGTab1::IDD, pParent)
{

}

DLGTab1::~DLGTab1()
{
}

void DLGTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mWndList);
}


BEGIN_MESSAGE_MAP(DLGTab1, CDialogEx)
END_MESSAGE_MAP()


// DLGTab1 ��Ϣ�������


BOOL DLGTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rc;  
	GetClientRect(rc);  
	//ScreenToClient(&rc);
	mWndList.MoveWindow(&rc);  
	
	mWndList.InsertColumn (0, _T("�ֶ���"), LVCFMT_LEFT, 200);
	mWndList.InsertColumn (1, _T("�ֶ�ֵ"), LVCFMT_LEFT, 300);

	mWndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/|LVS_NOCOLUMNHEADER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void DLGTab1::InsertRow(CString sName, CString sValue)
{
	int n = mWndList.GetItemCount();
	mWndList.InsertItem(n, _T("1"));
	mWndList.SetItemText (n, 0, sName);
	mWndList.SetItemText (n, 1, sValue);
}