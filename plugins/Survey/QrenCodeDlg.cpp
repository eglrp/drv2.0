// QrenCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QrenCodeDlg.h"
#include "afxdialogex.h"


// CQrenCodeDlg 对话框

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


// CQrenCodeDlg 消息处理程序


void CQrenCodeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	int height, width;
    RECT r;
	m_picLocation.GetWindowRect(&r);
	height = r.bottom - r.top;
	width = r.right - r.left;
	CRect rect;//定义矩形类
	CRect rect1;
	CImage img1;
	CImage img2;
	img1.Load(_T("Data\\DRVdata\\location.png"));
	if (!img1.IsNull())
	{
		m_picLocation.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_picLocation.GetDC();//获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img1.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc
	}
	img2.Load(_T("Data\\DRVdata\\houseInfo.png"));
	if (!img2.IsNull())
	{
		m_picHouseInfo.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_picHouseInfo.GetDC();//获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img2.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc
	}
	CDialogEx::OnPaint();        
}

