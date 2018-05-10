// BackgroundDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManager.h"
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
	DDX_Control(pDX, IDC_STATIC_PICHEAD, m_pic);
}


BEGIN_MESSAGE_MAP(CBackgroundDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBackgroundDlg ��Ϣ�������


BOOL CBackgroundDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_brush.CreateSolidBrush(RGB(0,0,0));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),80,LWA_ALPHA);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBackgroundDlg::changeSize(int dlgW,int dlgH)
{
	RECT r;
	GetWindowRect(&r);
	r.right = r.left + dlgW;
	r.bottom = r.top + dlgH;
	MoveWindow(&r);
}

HBRUSH CBackgroundDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_brush;
}


void CBackgroundDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	UpdateWindow();
	int height, width;
	CRect rect;//���������
	CRect rect1;
	ATL::CImage image; //����ͼƬ��
	image.Load(_T("Data\\DRVdata\\head.bmp"));
	height = image.GetHeight();
	width = image.GetWidth();


	m_pic.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
	CDC *pDc = m_pic.GetDC();//���pictrue�ؼ���Dc
	SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
