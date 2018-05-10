// CVMediaPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "CVMediaPlayerDlg.h"
#include "afxdialogex.h"


// CCVMediaPlayerDlg 对话框

IMPLEMENT_DYNAMIC(CCVMediaPlayerDlg, CDialogEx)

	CCVMediaPlayerDlg::CCVMediaPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCVMediaPlayerDlg::IDD, pParent)
	, m_gh(_T(""))
	, m_gspq(_T(""))
	, m_sblx(_T(""))
	, m_sbcs(_T(""))
	, m_whry(_T(""))
	, m_whtel(_T(""))
{
	rect_height = 100;
	rect_width = 100;
}

CCVMediaPlayerDlg::~CCVMediaPlayerDlg()
{
}

void CCVMediaPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_GHV, m_gh);
	DDX_Text(pDX, IDC_STATIC_GSPQV, m_gspq);
	DDX_Text(pDX, IDC_STATIC_SBLXV, m_sblx);
	DDX_Text(pDX, IDC_STATIC_SBCSV, m_sbcs);
	DDX_Text(pDX, IDC_STATIC_WHRYV, m_whry);
	DDX_Text(pDX, IDC_STATIC_WHTELV, m_whtel);
}


BEGIN_MESSAGE_MAP(CCVMediaPlayerDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCVMediaPlayerDlg 消息处理程序


BOOL CCVMediaPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,225,LWA_ALPHA);//0 透明  255 不透明  
	//SetTimer(1,20,NULL);
	m_gh = _T("D10B8911");
	m_gspq = _T("东湖高新公安分局");
	m_sblx = _T("枪机");
	m_sbcs = _T("海康");
	m_whry = _T("张三");
	m_whtel = _T("13987654321");
	UpdateData(FALSE); //关联控件与变量

	m_font.CreatePointFont(120,_T("幼圆"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCVMediaPlayerDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect crect;
	GetDlgItem(ID)->GetClientRect(&rect);
	SetRect( crect, rect.left, rect.top, rect.right, rect.bottom );
	CvvImage cimg;
	cimg.CopyOf( img ); // 复制图片
	cimg.DrawToHDC( hDC, &crect ); // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
}

void CCVMediaPlayerDlg::changePlayerSize(int w,int h)
{
	RECT r,r1;
	GetWindowRect(&r);
	r.right = r.left+w+20;
	r.bottom = r.top+h+20+30+150;
	MoveWindow(&r);

	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC_PLAYERINFO);
	p->GetWindowRect(&rect);
	rect.left = r.left+10;
	rect.top = r.top+10;
	rect.right= rect.left+w;
	rect.bottom = rect.top+h;
	ScreenToClient(&rect);
	p->MoveWindow(&rect);

	r1.top = r.top + 10 + h + 10;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_GH_);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_GHV);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_GSPQ_);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_GSPQV);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 2*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBLX_);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 2*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBLXV);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 3*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBCS_);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 3*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBCSV);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 4*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHRY_);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 4*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHRYV);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 5*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHTEL_);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 5*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHTELV);
	p->MoveWindow(&r1);
}

void CCVMediaPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent != 1)
	{
		return;
	}
	Mat frame;//定义一个Mat变量，用于存储每一帧的图像

	capture>>frame;  //读取当前帧
	if (frame.empty())
	{
		capture.open(filePath.c_str());
		return;
	}
	DrawPicToHDC(&IplImage(frame),IDC_STATIC_PLAYERINFO);
	CDialogEx::OnTimer(nIDEvent);
}


void CCVMediaPlayerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(73,141,205));
}


HBRUSH CCVMediaPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(nCtlColor == CTLCOLOR_STATIC)  
	{  
		// 将背景设为透明  
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(0,0,0));    
		pDC->SelectObject(&m_font);
		// 这里一定要设置为空画刷，否则字体还是有背景颜色。  
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}  
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CCVMediaPlayerDlg::setupPolygonRegion()
{
	CRgn wndRgn;
	RECT rect;
	GetWindowRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	POINT *pt = new POINT[8];
	POINT p;
	p.x = 10;p.y = 0;
	pt[0] = p;

	p.x = w - 10;p.y = 0;
	pt[1] = p;

	p.x = w;p.y = 10;
	pt[2] = p;

	p.x = w;p.y = h-10;
	pt[3] = p;

	p.x = w-10;p.y = h;
	pt[4] = p;

	p.x = 10;p.y = h;
	pt[5] = p;

	p.x = 0;p.y = h -10;
	pt[6] = p;

	p.x = 0;p.y = 10;
	pt[7] = p;



	wndRgn.CreatePolygonRgn(pt,8,1);
	SetWindowRgn((HRGN)wndRgn, TRUE);
	Invalidate();
}

void CCVMediaPlayerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->ShowWindow(SW_HIDE);
	CDataManager* p1 = dynamic_cast<CDataManager*>(parent);
	if (p1 != nullptr)
	{
		if (p1->gTemp != nullptr)
		{
			(p1->m_spViewer3D)->RemoveNode(p1->gTemp);
			p1->gTemp = nullptr;
		}
	}
}


BOOL CCVMediaPlayerDlg::PreTranslateMessage(MSG* pMsg)
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
