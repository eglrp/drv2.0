// DLGVLCPlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGVLCPlayer.h"
#include "afxdialogex.h"


// CDLGVLCPlayer 对话框

IMPLEMENT_DYNAMIC(CDLGVLCPlayer, CDialogEx)

CDLGVLCPlayer::CDLGVLCPlayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGVLCPlayer::IDD, pParent)
	, m_title(_T("西门"))
{
	m_vlcMedia = nullptr;
	m_vlcMplay = nullptr;
	m_vlcInst = nullptr;
}

CDLGVLCPlayer::~CDLGVLCPlayer()
{
}

void CDLGVLCPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VLCTITLE, m_title);
}


BEGIN_MESSAGE_MAP(CDLGVLCPlayer, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDLGVLCPlayer 消息处理程序
void CDLGVLCPlayer::connectRTSP()
{
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
	libvlc_media_player_set_hwnd (m_vlcMplay,GetDlgItem(IDC_STATIC_VLC)->m_hWnd);   
	/* play the media_player */   
	int x=libvlc_media_player_play (m_vlcMplay);  
}

BOOL CDLGVLCPlayer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_font.CreatePointFont(120,_T("幼圆"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDLGVLCPlayer::PreTranslateMessage(MSG* pMsg)
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


void CDLGVLCPlayer::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
	CDataManager* p1 = dynamic_cast<CDataManager*>(parent);
	if (p1 != nullptr)
	{
		if (p1->gTemp != nullptr)
		{
			(p1->m_spViewer3D)->RemoveNode(p1->gTemp);
			p1->gTemp = nullptr;
		}
	}
	CDialogEx::OnClose();
}


HBRUSH CDLGVLCPlayer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_VLCTITLE) 
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


void CDLGVLCPlayer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(73,141,205));
	// 不为绘图消息调用 CDialogEx::OnPaint()
}
