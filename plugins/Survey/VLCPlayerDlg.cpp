// VLCPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VLCPlayerDlg.h"
#include "afxdialogex.h"


// CVLCPlayerDlg 对话框

IMPLEMENT_DYNAMIC(CVLCPlayerDlg, CDialogEx)

CVLCPlayerDlg::CVLCPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVLCPlayerDlg::IDD, pParent)
	, m_info(_T("西门"))
{
	m_vlcMedia = nullptr;
	m_vlcMplay = nullptr;
	m_vlcInst = nullptr;
}

CVLCPlayerDlg::~CVLCPlayerDlg()
{
}

void CVLCPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VLCINFO, m_info);
}


BEGIN_MESSAGE_MAP(CVLCPlayerDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVLCPlayerDlg 消息处理程序


BOOL CVLCPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	bpathSetted = false;
	m_font.CreatePointFont(120,_T("幼圆"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CVLCPlayerDlg::connectRTSP()
{
	if (!bpathSetted)
	{
		bpathSetted = true;
		m_vlcInst = libvlc_new(0, NULL);
		//Create a new item  
		//Method 1: 
		if (rtspAddress.empty())
		{
			m_vlcMedia = libvlc_media_new_location(m_vlcInst, "rtsp://admin:admin@192.168.0.246/11");
		}
		else
		{
			m_vlcMedia = libvlc_media_new_location(m_vlcInst, rtspAddress.c_str());
		}
		/* Create a media player playing environement */
		m_vlcMplay = libvlc_media_player_new_from_media(m_vlcMedia);
		libvlc_media_release (m_vlcMedia);   
		//on windows   
		libvlc_media_player_set_hwnd (m_vlcMplay,GetDlgItem(IDC_STATIC_CVPLAYER)->m_hWnd);   
		/* play the media_player */   
		int x=libvlc_media_player_play (m_vlcMplay);  
	}
	
}

BOOL CVLCPlayerDlg::PreTranslateMessage(MSG* pMsg)
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


void CVLCPlayerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


void CVLCPlayerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(73,141,205));
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


HBRUSH CVLCPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_VLCINFO) 
	{  
		// 将背景设为透明  
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(255,255,255));    
		//pDC->SelectObject(&m_font);
		// 这里一定要设置为空画刷，否则字体还是有背景颜色。  
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}  
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
