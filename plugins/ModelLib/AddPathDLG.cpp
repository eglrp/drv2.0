// AddPathDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AddPathDLG.h"
#include "afxdialogex.h"


// CAddPathDLG �Ի���

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


// CAddPathDLG ��Ϣ�������


void CAddPathDLG::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("�ı��ļ�(*.txt)|*.txt||"));
	if( dlg.DoModal() != IDOK )
	{
		return;
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	pEdit->SetWindowTextW(dlg.GetPathName());
}
