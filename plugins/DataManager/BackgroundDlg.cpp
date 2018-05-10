// BackgroundDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "BackgroundDlg.h"
#include "afxdialogex.h"


// CBackgroundDlg 对话框

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


// CBackgroundDlg 消息处理程序


BOOL CBackgroundDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_brush.CreateSolidBrush(RGB(0,0,0));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),80,LWA_ALPHA);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
	// TODO: 在此处添加消息处理程序代码
	UpdateWindow();
	int height, width;
	CRect rect;//定义矩形类
	CRect rect1;
	ATL::CImage image; //创建图片类
	image.Load(_T("Data\\DRVdata\\head.bmp"));
	height = image.GetHeight();
	width = image.GetWidth();


	m_pic.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	CDC *pDc = m_pic.GetDC();//获得pictrue控件的Dc
	SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
	image.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
	ReleaseDC(pDc);//释放picture控件的Dc
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
