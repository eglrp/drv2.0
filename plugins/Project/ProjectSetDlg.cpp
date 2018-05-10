// ProjectSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EarthMatrixFunctions.h"
#include "Project.h"
#include "ProjectSetDlg.h"
#include "afxdialogex.h"
#include "vlSetProject.h"


// CProjectSetDlg �Ի���

IMPLEMENT_DYNAMIC(CProjectSetDlg, CDialogEx)

CProjectSetDlg::CProjectSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectSetDlg::IDD, pParent)
	, m_houseLayer(_T(""))
	, m_buildLayer(_T(""))
{
	m_nScreenNum = 1;
}

CProjectSetDlg::~CProjectSetDlg()
{
}

void CProjectSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2_HOUSELAYER, m_houseLayer);
	DDX_Text(pDX, IDC_EDIT3_BUILDLAYER, m_buildLayer);
}


BEGIN_MESSAGE_MAP(CProjectSetDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SAVESET, &CProjectSetDlg::OnBnClickedButtonSaveset)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CProjectSetDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON2_GETHOUSELAYER, &CProjectSetDlg::OnBnClickedButton2Gethouselayer)
	ON_BN_CLICKED(IDC_BUTTON3_GETBUILDLAYER, &CProjectSetDlg::OnBnClickedButton3Getbuildlayer)
END_MESSAGE_MAP()


// CProjectSetDlg ��Ϣ�������
BOOL CProjectSetDlg::OnInitDialog()
{
	AFX_MANAGE_STATE_EX;
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitSysInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProjectSetDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CvlSetProject* p = (CvlSetProject*)this->parent;
	p->OnClick();
	CDialogEx::OnClose();
}

void CProjectSetDlg::InitSysInfo()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	CString s;
	s.Format(L"%d", statex.dwMemoryLoad);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MACHINE_MUP);
	pEdit->SetWindowTextW(s);

	s.Format(L"%d", statex.ullAvailPhys/1024/1024);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MACHINE_MREST);
	pEdit->SetWindowTextW(s);

	s.Format(L"%d", m_nOSG_MAX_PAGEDLOD);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OSG_MAX_PAGEDLOD);
	pEdit->SetWindowTextW(s);

	s.Format(L"%d", m_nOSG_NUM_DATABASE_THREADS);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OSG_NUM_DATABASE_THREADS);
	pEdit->SetWindowTextW(s);

	s.Format(L"%d", m_nScreenNum);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SCREEN_NUM);
	pEdit->SetWindowTextW(s);

	s.Format(L"%d", -1);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CAMERA_HEGHT_LIMIT);
	pEdit->SetWindowTextW(s);
}


void CProjectSetDlg::OnBnClickedButtonSaveset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString s;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OSG_MAX_PAGEDLOD);
	pEdit->GetWindowTextW(s);
	m_nOSG_MAX_PAGEDLOD = _tstoi(s.GetBuffer(0));

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OSG_NUM_DATABASE_THREADS);
	pEdit->GetWindowTextW(s);
	m_nOSG_NUM_DATABASE_THREADS = _tstoi(s.GetBuffer(0));

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SCREEN_NUM);
	pEdit->GetWindowTextW(s);
	m_nScreenNum = _tstoi(s.GetBuffer(0));

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CAMERA_HEGHT_LIMIT);
	pEdit->GetWindowTextW(s);
	m_nCamera_Height_Limit = _tstoi(s.GetBuffer(0));

	CvlSetProject* p = (CvlSetProject*)this->parent;
	p->SetOSG_MAX_PAGEDLOD(m_nOSG_MAX_PAGEDLOD);
	p->SetOSG_NUM_DATABASE_THREADS(m_nOSG_NUM_DATABASE_THREADS);
	p->SetScreenNum(m_nScreenNum);
	p->SetCameraLimitHeight(m_nCamera_Height_Limit);

	UpdateData(TRUE);
	p->SetBuildLayer(m_buildLayer);
	p->SetHouseLayer(m_houseLayer);
	AfxMessageBox(L"�ѱ��棡");
}


void CProjectSetDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->OnClose();
}


void CProjectSetDlg::OnBnClickedButton2Gethouselayer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CvlSetProject* ps = (CvlSetProject*)parent;
	m_houseLayer = ps->GetHouseLayer();
	
	UpdateData(FALSE);
}


void CProjectSetDlg::OnBnClickedButton3Getbuildlayer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CvlSetProject* ps = (CvlSetProject*)parent;
	m_buildLayer = ps->GetBuildLayer();

	UpdateData(FALSE);
}
