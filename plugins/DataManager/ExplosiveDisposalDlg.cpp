// ExplosiveDisposalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "ExplosiveDisposalDlg.h"
#include "afxdialogex.h"
#include "functionEx.h"
#include "DLGNeighborSearch.h"
#include "SniperDlg.h"

extern CDataManager* m_pDM;
// CExplosiveDisposalDlg 对话框

IMPLEMENT_DYNAMIC(CExplosiveDisposalDlg, CDialogEx)

CExplosiveDisposalDlg::CExplosiveDisposalDlg(int H,CWnd* pParent /*=NULL*/)
	: CDialogEx(CExplosiveDisposalDlg::IDD, pParent)
{
	viewH = H;
	pDLG_BK = nullptr;
	pDLG_MJQD = nullptr;
	pDLG_sniper = nullptr;
	pDLG_neighbor = nullptr;
}

CExplosiveDisposalDlg::~CExplosiveDisposalDlg()
{
}

void CExplosiveDisposalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1_NEIGHBOR, m_btn_neighbor);
	DDX_Control(pDX, IDC_BUTTON2_SNIPER, m_btn_sniper);
	DDX_Control(pDX, IDC_BUTTON_DOORRECORD, m_btn_doorRec);
}


BEGIN_MESSAGE_MAP(CExplosiveDisposalDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1_NEIGHBOR, &CExplosiveDisposalDlg::OnBnClickedButton1Neighbor)
	ON_BN_CLICKED(IDC_BUTTON2_SNIPER, &CExplosiveDisposalDlg::OnBnClickedButton2Sniper)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DOORRECORD, &CExplosiveDisposalDlg::OnBnClickedButtonDoorrecord)
END_MESSAGE_MAP()


// CExplosiveDisposalDlg 消息处理程序


void CExplosiveDisposalDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CExplosiveDisposalDlg::PreTranslateMessage(MSG* pMsg)
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


void CExplosiveDisposalDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CviolenceDisposal* ca = (CviolenceDisposal*)parent;
	ca->OnClick();
	DestroyWindow();
}


BOOL CExplosiveDisposalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HBITMAP   hBitmap1,hBitmap2,hBitmap3;   
	hBitmap1 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\门禁记录.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap2 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\邻居查询.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap3 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\狙击分析.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	SetBackgroundColor(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

	int btnH = 48;
	int btnW = 130;

	RECT rect;
	GetWindowRect(&rect);
	rect.right = rect.left + btnW;
	rect.bottom = rect.top + btnH * 3 + 20;
	MoveWindow(&rect);

	RECT br;
	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 0;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_btn_doorRec.MoveWindow(&br);
	m_btn_doorRec.SetBitmap(hBitmap1);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 1*btnH + 10;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_btn_neighbor.MoveWindow(&br);
	m_btn_neighbor.SetBitmap(hBitmap2);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 2*btnH + 20;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_btn_sniper.MoveWindow(&br);
	m_btn_sniper.SetBitmap(hBitmap3);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	POINT *pt = new POINT[8];
	POINT p;
	p.x = btnW/4.0; p.y = 0;
	pt[0] = p;

	p.x = btnW - 2; p.y = 0;
	pt[1] = p;

	p.x = btnW/8.0 * 7; p.y = btnH;
	pt[2] = p;

	p.x = btnW/8.0 * 7; p.y = btnH * 2;
	pt[3] = p;

	p.x = btnW - 2; p.y = btnH * 3;
	pt[4] = p;

	p.x = btnW/4.0; p.y = btnH * 3;
	pt[5] = p;

	p.x = 2; p.y = btnH*2;
	pt[6] = p;

	p.x = 2; p.y = btnH;
	pt[7] = p;
	
	CRgn wndRgn;
	wndRgn.CreatePolygonRgn(pt,8,1);
	//SetWindowRgn((HRGN)wndRgn, TRUE);
	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CExplosiveDisposalDlg::OnBnClickedButton1Neighbor()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_neighbor = new CDLGNeighborSearch();
	pDLG_neighbor->Create(IDD_DIALOG_NEIGHBOR);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);
	pDLG_neighbor->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_neighbor->ShowWindow(SW_SHOW);
	pDLG_neighbor->OnBnClickedButtonSearchneighbor1();
}


void CExplosiveDisposalDlg::OnBnClickedButton2Sniper()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	CStringArray* aLayers = m_pDM->m_spViewer3D->GetLayersArray();
	if (aLayers->GetCount() == 0)
	{
		AfxMessageBox(_T("请先加载场景！"));
		//da->OnClick();
		return;
	}
	pDLG_sniper = new CSniperDlg();
	pDLG_sniper->Create(IDD_DIALOG_SNIPER);
	pDLG_sniper->m_spViewer3D = m_pDM->m_spViewer3D;
	RECT r;
	pDLG_sniper->GetWindowRect(&r);
	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = m_pDM->m_spViewer3D->rectView3D.left + 20;
	r.top = m_pDM->m_spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;
	pDLG_sniper->MoveWindow(&r);
	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);
	pDLG_sniper->parent = (CObject*)pDLG_BK;
	pDLG_sniper->parent1 = (CObject*)this;
	pDLG_BK->MoveWindow(&r);
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_sniper->ShowWindow(SW_SHOW);
	
	pDLG_sniper->InitialHandle();
}

void CExplosiveDisposalDlg::play()
{
	RECT r;
	GetWindowRect(&r);
	nElapse = r.right - r.left + 20;
	count = 0;
	SetTimer(0,1,NULL);
}

void CExplosiveDisposalDlg::OnTimer(UINT_PTR nIDEvent)
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

void CExplosiveDisposalDlg::destroyAll()
{
	if (pDLG_sniper != nullptr)
	{
		pDLG_sniper->OnClose();
		pDLG_sniper = nullptr;
	}
	if (pDLG_BK != nullptr)
	{
		pDLG_BK->DestroyWindow();
		pDLG_BK = nullptr;
	}
	if (pDLG_MJQD != nullptr)
	{
		pDLG_MJQD->DestroyWindow();
		pDLG_MJQD = nullptr;
	}
	if (pDLG_neighbor != nullptr)
	{
		pDLG_neighbor->DestroyWindow();
		pDLG_neighbor = nullptr;
	}
}

void CExplosiveDisposalDlg::OnBnClickedButtonDoorrecord()
{
	// TODO: 在此添加控件通知处理程序代码
	destroyAll();
	pDLG_MJQD = new CDLGmenjinqingdan();
	pDLG_MJQD->Create(IDD_DIALOG_MJQD);

	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);
	pDLG_MJQD->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_MJQD->ShowWindow(SW_SHOW);
	pDLG_MJQD->doSearch();
}
