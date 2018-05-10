// QrenCodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QrenCodeDlg.h"
#include "afxdialogex.h"


// CQrenCodeDlg �Ի���

IMPLEMENT_DYNAMIC(CQrenCodeDlg, CDialogEx)

CQrenCodeDlg::CQrenCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQrenCodeDlg::IDD, pParent)
{

}

CQrenCodeDlg::~CQrenCodeDlg()
{
}

void CQrenCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOCATION, m_picLocation);
	DDX_Control(pDX, IDC_STATIC_HOUSEINFO, m_picHouseInfo);
}


BEGIN_MESSAGE_MAP(CQrenCodeDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CQrenCodeDlg ��Ϣ�������


void CQrenCodeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	int height, width;
    RECT r;
	m_picLocation.GetWindowRect(&r);
	height = r.bottom - r.top;
	width = r.right - r.left;
	CRect rect;//���������
	CRect rect1;
	CImage img1;
	CImage img2;
	img1.Load(_T("Data\\DRVdata\\location.png"));
	if (!img1.IsNull())
	{
		m_picLocation.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_picLocation.GetDC();//���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img1.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	}
	img2.Load(_T("Data\\DRVdata\\houseInfo.png"));
	if (!img2.IsNull())
	{
		m_picHouseInfo.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_picHouseInfo.GetDC();//���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img2.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	}
	CDialogEx::OnPaint();        
}

