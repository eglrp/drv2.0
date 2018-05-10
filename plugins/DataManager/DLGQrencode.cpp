// DLGQrencode.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGQrencode.h"
#include "afxdialogex.h"


// CDLGQrencode 对话框

IMPLEMENT_DYNAMIC(CDLGQrencode, CDialogEx)

CDLGQrencode::CDLGQrencode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGQrencode::IDD, pParent)
{

}

CDLGQrencode::~CDLGQrencode()
{
}

void CDLGQrencode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICINFO, m_picQrencode);
	DDX_Control(pDX, IDC_BUTTON1_QRENCODEEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGQrencode, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1_QRENCODEEXIT, &CDLGQrencode::OnBnClickedButton1Qrencodeexit)
END_MESSAGE_MAP()


// CDLGQrencode 消息处理程序


void CDLGQrencode::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	int height, width;
	RECT r;
	m_picQrencode.GetWindowRect(&r);
	height = r.bottom - r.top;
	width = r.right - r.left;
	CRect rect;//定义矩形类
	ATL::CImage img1;
	img1.Load(_T("Data\\DRVdata\\listInfo.png"));
	if (!img1.IsNull())
	{
		m_picQrencode.GetClientRect(&rect); //获得pictrue控件所在的矩形区域
		CDC *pDc = m_picQrencode.GetDC();//获得pictrue控件的Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img1.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//释放picture控件的Dc
	}
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CDLGQrencode::OnBnClickedButton1Qrencodeexit()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);
}


BOOL CDLGQrencode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(200,200,200));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);
	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	m_EXIT.SetBitmap(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
