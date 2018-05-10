// WalkRecDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WalkRecDlg.h"
#include "afxdialogex.h"
#include "vlWalkRec.h"


// CWalkRecDlg �Ի���

IMPLEMENT_DYNAMIC(CWalkRecDlg, CDialogEx)

CWalkRecDlg::CWalkRecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWalkRecDlg::IDD, pParent)
{

}

CWalkRecDlg::~CWalkRecDlg()
{
}

void CWalkRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_ADDKEY, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_EDIT_PTNUM, m_editNum);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
}


BEGIN_MESSAGE_MAP(CWalkRecDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDKEY, &CWalkRecDlg::OnBnClickedButtonAddkey)
	ON_BN_CLICKED(IDC_BUTTON_START, &CWalkRecDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CWalkRecDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CWalkRecDlg::OnBnClickedButtonClear)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CWalkRecDlg ��Ϣ�������


void CWalkRecDlg::OnBnClickedButtonAddkey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->AddKeyPt();
}


void CWalkRecDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnStart.EnableWindow(FALSE);
	m_btnAdd.EnableWindow(TRUE);
	m_btnStop.EnableWindow(TRUE);
}


void CWalkRecDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnStart.EnableWindow(TRUE);
	m_btnAdd.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);

	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->SaveKeyPts();
}


BOOL CWalkRecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_btnAdd.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CWalkRecDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->ClearPts();
}


void CWalkRecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CWalkRecDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->OnClick();
	CDialogEx::OnClose();
}
