// DLGQrencode2.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGQrencode2.h"
#include "afxdialogex.h"
#include "BackgroundDlg.h"

// CDLGQrencode2 对话框

IMPLEMENT_DYNAMIC(CDLGQrencode2, CDialogEx)

CDLGQrencode2::CDLGQrencode2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGQrencode2::IDD, pParent)
{

}

CDLGQrencode2::~CDLGQrencode2()
{
}

void CDLGQrencode2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_QRENCODE2EXIT, m_EXIT);
	DDX_Control(pDX, IDC_STATIC_QRENCODEPIC1, m_picHouseInfo);
	DDX_Control(pDX, IDC_STATIC_QRENCODEPIC2, m_picLocation);
}


BEGIN_MESSAGE_MAP(CDLGQrencode2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QRENCODE2EXIT, &CDLGQrencode2::OnBnClickedButtonQrencode2exit)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDLGQrencode2 消息处理程序


void CDLGQrencode2::OnBnClickedButtonQrencode2exit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


void CDLGQrencode2::OnPaint()
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
	ATL::CImage img1;
	ATL::CImage img2;
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
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CDLGQrencode2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

	m_font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		TRUE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("黑体"));                 // 
	GetDlgItem(IDC_STATIC_QRENCODE_1)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_QRENCODE_2)->SetFont(&m_font);

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


BOOL CDLGQrencode2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc按键事件 
			OnClose();
			return true;  
		case VK_RETURN: //Enter按键事件 
			OnClose();
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDLGQrencode2::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBackgroundDlg* dlg = (CBackgroundDlg*)parent;
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


HBRUSH CDLGQrencode2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
