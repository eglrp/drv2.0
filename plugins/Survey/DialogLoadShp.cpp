// DialogLoadShp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DialogLoadShp.h"
#include "afxdialogex.h"

CString g_sShpPath;
// CDialogLoadShp �Ի���

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


// CDialogLoadShp ��Ϣ�������


void CDialogLoadShp::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("Shp�ļ�(*.shp)|*.shp||"));
	if( dlg.DoModal() != IDOK )
	{
		return;
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	pEdit->SetWindowTextW(dlg.GetPathName());
}


void CDialogLoadShp::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sData;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	pEdit->GetWindowText(sData);
	if (sData.GetLength() == 0)
	{
		AfxMessageBox(_T("Shp�ļ�·������Ϊ��!"));
		return;
	}

	g_sShpPath = (sData);
	CDialogEx::OnOK();
}
