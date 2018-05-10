// WalkRecDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WalkRecDlg.h"
#include "afxdialogex.h"
#include "vlWalkRec.h"


// CWalkRecDlg 对话框

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


// CWalkRecDlg 消息处理程序


void CWalkRecDlg::OnBnClickedButtonAddkey()
{
	// TODO: 在此添加控件通知处理程序代码
	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->AddKeyPt();
}


void CWalkRecDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnStart.EnableWindow(FALSE);
	m_btnAdd.EnableWindow(TRUE);
	m_btnStop.EnableWindow(TRUE);
}


void CWalkRecDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnStart.EnableWindow(TRUE);
	m_btnAdd.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);

	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->SaveKeyPts();
}


BOOL CWalkRecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_btnAdd.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CWalkRecDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->ClearPts();
}


void CWalkRecDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CWalkRecDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CvlWalkRec* p = (CvlWalkRec*)this->parent;
	p->OnClick();
	CDialogEx::OnClose();
}
