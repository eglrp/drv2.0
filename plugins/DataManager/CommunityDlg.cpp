// CommunityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "CommunityDlg.h"
#include "afxdialogex.h"
#include "functionEx.h"
#include "BackgroundDlg.h"
#include "DLGrenqunguanai.h"
#include "DLGShowRealData.h"
#include "DLGzuzhubiangeng.h"
#include "DLGanbaodongtai.h"
#include "DLGjuhuigenzong.h"
// CCommunityDlg 对话框

IMPLEMENT_DYNAMIC(CCommunityDlg, CDialogEx)

CCommunityDlg::CCommunityDlg(int H,CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommunityDlg::IDD, pParent)
{
	viewH = H;

	pDLG_RQGA = nullptr;
	pDLG_WLYC = nullptr;
	pDLG_ZZBG = nullptr;
	pDLG_TSRQ = nullptr;
	pDLG_ABDT = nullptr;
	pDLG_JHGZ = nullptr;

	pDLG_BK = nullptr;
}

CCommunityDlg::~CCommunityDlg()
{
}

void CCommunityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_RRGA, m_rrga);
	DDX_Control(pDX, IDC_BUTTON_TSRQ, m_tsrq);
	DDX_Control(pDX, IDC_BUTTON_WLYC, m_wlyc);
	DDX_Control(pDX, IDC_BUTTON_ZZBG, m_zzbg);
	DDX_Control(pDX, IDC_BUTTON_JHGZ, m_jhgz);
	DDX_Control(pDX, IDC_BUTTON_ABDT, m_abdt);
}


BEGIN_MESSAGE_MAP(CCommunityDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_RRGA, &CCommunityDlg::OnBnClickedButtonRrga)
	ON_BN_CLICKED(IDC_BUTTON_TSRQ, &CCommunityDlg::OnBnClickedButtonTsrq)
	ON_BN_CLICKED(IDC_BUTTON_WLYC, &CCommunityDlg::OnBnClickedButtonWlyc)
	ON_BN_CLICKED(IDC_BUTTON_ZZBG, &CCommunityDlg::OnBnClickedButtonZzbg)
	ON_BN_CLICKED(IDC_BUTTON_ABDT, &CCommunityDlg::OnBnClickedButtonAbdt)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_JHGZ, &CCommunityDlg::OnBnClickedButtonJhgz)
END_MESSAGE_MAP()


// CCommunityDlg 消息处理程序


void CCommunityDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	//CRect rc;  
	//GetWindowRect(&rc);
	//ATL::CImage mImage;  
	//if(mImage.Load(_T("Data\\DRVdata\\社区综合治理.jpg")) == S_OK)  
	//{  
	//	//这里让图片绘制区域和窗口大小一致  
	//	mImage.Draw(GetDC()->GetSafeHdc(), CRect(0,0,rc.Width(),rc.Height()));  
	//}  

	/*CRect rect;

	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(255,255,255));*/
	// 不为绘图消息调用 CDialogEx::OnPaint()
}



BOOL CCommunityDlg::PreTranslateMessage(MSG* pMsg)
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


void CCommunityDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CdataAnalysis* ca = (CdataAnalysis*)parent;
	ca->OnClick();
	DestroyWindow();
}


BOOL CCommunityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HBITMAP   hBitmap1,hBitmap2,hBitmap3,hBitmap4,hBitmap5,hBitmap6;   
	hBitmap1 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\人群关爱.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap2 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\物联异常.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap3 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\租住变更.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap4 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\特殊人群.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap5 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\聚会跟踪.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap6 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\安保动态.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	/*CRect r,rc;
	GetWindowRect(&rc);
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	rc.left = x/2 - 400;
	rc.right = rc.left + 800;
	rc.top = y/2 - 300;
	rc.bottom = rc.top + 600;
	MoveWindow(&rc);
	m_rrga.GetWindowRect(&r);
	r.left = rc.left + 142;
	r.top = rc.top + 316;
	r.right = r.left + 63;
	r.bottom = r.top + 63;
	ScreenToClient(&r);
	m_rrga.MoveWindow(&r);

	m_tsrq.GetWindowRect(&r);
	r.left = rc.left + 425;
	r.top = rc.top + 172;
	r.right = r.left + 63;
	r.bottom = r.top + 63;
	ScreenToClient(&r);
	m_tsrq.MoveWindow(&r);*/

	/*COLORREF maskColor = RGB(255,255,255);
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	HINSTANCE hInst=LoadLibrary(_T("User32.DLL"));
	if(hInst)
	{
		typedef BOOL (WINAPI * MYFUNC)(HWND , COLORREF,BYTE,DWORD);
		MYFUNC fun=NULL;
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),maskColor,100,1);
		FreeLibrary(hInst);
	}*/
	SetBackgroundColor(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

	int btnH = 48/*viewH/2/6*/;
	int btnW = 130;

	RECT rect;
	GetWindowRect(&rect);
	rect.right = rect.left + btnW;
	rect.bottom = rect.top + btnH * 6 + 50;
	MoveWindow(&rect);

	RECT br;
	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 0;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_rrga.MoveWindow(&br);
	m_rrga.SetBitmap(hBitmap1);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 1*btnH + 10;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_wlyc.MoveWindow(&br);
	m_wlyc.SetBitmap(hBitmap2);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 2*btnH + 20;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_zzbg.MoveWindow(&br);
	m_zzbg.SetBitmap(hBitmap3);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 3*btnH+ 30;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_tsrq.MoveWindow(&br);
	m_tsrq.SetBitmap(hBitmap4);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 4*btnH + 40;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_jhgz.MoveWindow(&br);
	m_jhgz.SetBitmap(hBitmap5);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 5*btnH + 50;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_abdt.MoveWindow(&br);
	m_abdt.SetBitmap(hBitmap6);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	POINT *pt = new POINT[14];
	POINT p;
	p.x = btnW/8.0*3; p.y = 0;
	pt[0] = p;

	p.x = btnW-2 ;p.y = 0;
	pt[1] = p;

	p.x = btnW/8.0*7 ;p.y = btnH;
	pt[2] = p;

	p.x = btnW/80.0*65; p.y = btnH * 2;
	pt[3] = p;

	p.x = btnW/80.0*62 ;p.y = btnH * 3;
	pt[4] = p;

	p.x = btnW/80.0*65;p.y = btnH*4;
	pt[5] = p;

	p.x = btnW/8.0*7;p.y = btnH*5;
	pt[6] = p;

	p.x = btnW-2;p.y = btnH*6;
	pt[7] = p;

	p.x = btnW/8.0*3;p.y = btnH*6;
	pt[8] = p;

	p.x = btnW/16.0*3;p.y = btnH*5;
	pt[9] = p;

	p.x = btnW/16.0;p.y = btnH*4;
	pt[10] = p;

	p.x = 2;p.y = btnH*3;
	pt[11] = p;

	p.x = btnW/16.0;p.y = btnH*2;
	pt[12] = p;

	p.x = btnW/16.0*3;p.y = btnH*1;
	pt[13] = p;
	CRgn wndRgn;
	wndRgn.CreatePolygonRgn(pt,14,1);
	//SetWindowRgn((HRGN)wndRgn, TRUE);
	UpdateWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCommunityDlg::play()
{
	RECT r;
	GetWindowRect(&r);
	nElapse = r.right - r.left + 20;
	count = 0;
	SetTimer(0,1,NULL);
}

void CCommunityDlg::OnBnClickedButtonRrga()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_RQGA = new CDLGrenqunguanai();
	pDLG_RQGA->Create(IDD_DIALOG_RYGA);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);

	pDLG_RQGA->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_RQGA->ShowWindow(SW_SHOW);
	pDLG_RQGA->OnBnClickedButtonOlder();
}


void CCommunityDlg::OnBnClickedButtonTsrq()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_TSRQ = new CDLGSpecials();
	pDLG_TSRQ->Create(IDD_DIALOG_SPECIALS);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);

	pDLG_TSRQ->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_TSRQ->ShowWindow(SW_SHOW);
	pDLG_TSRQ->OnBnClickedButtonSearch1();
}


void CCommunityDlg::OnBnClickedButtonWlyc()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_WLYC = new CDLGShowRealData();
	pDLG_WLYC->Create(IDD_DIALOG_REALDATA);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);

	pDLG_WLYC->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_WLYC->ShowWindow(SW_SHOW);
}


void CCommunityDlg::OnBnClickedButtonZzbg()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_ZZBG = new CDLGzuzhubiangeng();
	pDLG_ZZBG->Create(IDD_DIALOG_ZZBG);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);

	pDLG_ZZBG->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_ZZBG->ShowWindow(SW_SHOW);
	pDLG_ZZBG->doSearch();
}


void CCommunityDlg::OnBnClickedButtonAbdt()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_ABDT = new CDLGanbaodongtai();
	pDLG_ABDT->Create(IDD_DIALOG_ABDT);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);
	pDLG_ABDT->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_ABDT->ShowWindow(SW_SHOW);
	pDLG_ABDT->doSearch();
}


void CCommunityDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int step =5;
	if (nIDEvent == 0)
	{
		if (count >= nElapse)
		{
			KillTimer(0);
			return;
		}
		count = count + step;
		RECT r;
		GetWindowRect(&r);
		r.left -= step;
		r.right -= step;
		MoveWindow(&r);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CCommunityDlg::OnBnClickedButtonJhgz()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_JHGZ = new CDLGjuhuigenzong();
	pDLG_JHGZ->Create(IDD_DIALOG_JHGZ);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);

	pDLG_JHGZ->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_JHGZ->ShowWindow(SW_SHOW);
	pDLG_JHGZ->doSearch();
}

void CCommunityDlg::destroyAll()
{
	if (pDLG_JHGZ != nullptr)
	{
		pDLG_JHGZ->DestroyWindow();
		pDLG_JHGZ = nullptr;
	}
	if (pDLG_ABDT != nullptr)
	{
		pDLG_ABDT->DestroyWindow();
		pDLG_ABDT = nullptr;
	}
	if (pDLG_ZZBG != nullptr)
	{
		pDLG_ZZBG->DestroyWindow();
		pDLG_ZZBG = nullptr;
	}
	if (pDLG_WLYC != nullptr)
	{
		pDLG_WLYC->DestroyWindow();
		pDLG_WLYC = nullptr;
	}
	if (pDLG_TSRQ != nullptr)
	{
		pDLG_TSRQ->DestroyWindow();
		pDLG_TSRQ = nullptr;
	}
	if (pDLG_RQGA != nullptr)
	{
		pDLG_RQGA->DestroyWindow();
		pDLG_RQGA = nullptr;
	}
	if (pDLG_BK != nullptr)
	{
		pDLG_BK->DestroyWindow();
		pDLG_BK = nullptr;
	}
}