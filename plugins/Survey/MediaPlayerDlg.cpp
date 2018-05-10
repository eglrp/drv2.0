// MediaPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MediaPlayerDlg.h"
#include "afxdialogex.h"


// CMediaPlayerDlg 对话框

IMPLEMENT_DYNAMIC(CMediaPlayerDlg, CDialogEx)

CMediaPlayerDlg::CMediaPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMediaPlayerDlg::IDD, pParent)
{
	m_title = "D10B8911";
}

CMediaPlayerDlg::~CMediaPlayerDlg()
{
}

void CMediaPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_OCX1,m_player);
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_INFO, m_title);
}

BEGIN_MESSAGE_MAP(CMediaPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_CLOSE, &CMediaPlayerDlg::OnStnClickedStaticClose)
	//ON_WM_CLOSE()
//	ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMediaPlayerDlg 消息处理程序
BOOL CMediaPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	 
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,220,LWA_ALPHA);
	// TODO:  在此添加额外的初始化
	m_player.SetUrl(_T("Data\\DRVdata\\door.mp4"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMediaPlayerDlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文 
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(249,105,14) ); 
}

HBRUSH CMediaPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)  
	{  
		// 将背景设为透明  
		pDC->SetBkMode(TRANSPARENT);  
		// 这里一定要设置为空画刷，否则字体还是有背景颜色。  
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}  
	return hbr;
}


//BOOL CMediaPlayerDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	//if (pMsg->message == WM_KEYDOWN)  
//	//{  
//	//	switch(pMsg->wParam)  
//	//	{  
//	//	case VK_ESCAPE: //Esc按键事件 
//	//		ShowWindow(SW_HIDE);
//	//		
//	//		return true;
//	//	case VK_RETURN: //Enter按键事件 
//	//		ShowWindow(SW_HIDE);
//	//		
//	//		return true;  
//	//	default:  
//	//		;  
//	//	}  
//	//}  
//	return CDialogEx::PreTranslateMessage(pMsg);
//}



void CMediaPlayerDlg::OnStnClickedStaticClose()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);
	Invalidate();
}


//void CMediaPlayerDlg::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	/*ShowWindow(SW_HIDE);
//	Invalidate();*/
//	CDialogEx::OnClose();
//}






//void CMediaPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	CDialogEx::OnLButtonUp(nFlags, point);
//}


void CMediaPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y)); 
	CDialogEx::OnLButtonDown(nFlags, point);
}
