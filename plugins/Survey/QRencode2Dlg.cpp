// QRencode2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QRencode2Dlg.h"
#include "afxdialogex.h"


// CQRencode2Dlg 对话框

IMPLEMENT_DYNAMIC(CQRencode2Dlg, CDialogEx)

CQRencode2Dlg::CQRencode2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQRencode2Dlg::IDD, pParent)
{

}

CQRencode2Dlg::~CQRencode2Dlg()
{
}

void CQRencode2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_QRENCODE2, m_pic);
}


BEGIN_MESSAGE_MAP(CQRencode2Dlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CQRencode2Dlg 消息处理程序


void CQRencode2Dlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	int height, width;
	RECT r;
	m_pic.GetWindowRect(&r);
	height = r.bottom - r.top;
	width = r.right - r.left;
	CRect rect;//定义矩形类
	ATL::CImage img1;
	img1.Load(_T("Data\\DRVdata\\pointLocation.png"));
	if (!img1.IsNull())
	{
		m_pic.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_pic.GetDC();//获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img1.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc
	}
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
