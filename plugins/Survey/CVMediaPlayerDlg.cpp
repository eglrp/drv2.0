// CVMediaPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CVMediaPlayerDlg.h"
#include "afxdialogex.h"


// CCVMediaPlayerDlg �Ի���

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
	DDX_Text(pDX, IDC_STATIC_GH_V, m_gh);
	DDX_Text(pDX, IDC_STATIC_GSPQ_V, m_gspq);
	DDX_Text(pDX, IDC_STATIC_SBLX_V, m_sblx);
	DDX_Text(pDX, IDC_STATIC_SBCS_V, m_sbcs);
	DDX_Text(pDX, IDC_STATIC_WHRY_V, m_whry);
	DDX_Text(pDX, IDC_STATIC_WHTEL_V, m_whtel);
}


BEGIN_MESSAGE_MAP(CCVMediaPlayerDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCVMediaPlayerDlg ��Ϣ�������


BOOL CCVMediaPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,225,LWA_ALPHA);//0 ͸��  255 ��͸��  
	//SetTimer(1,20,NULL);
	m_gh = _T("D10B8911");
	m_gspq = _T("�������¹����־�");
	m_sblx = _T("ǹ��");
	m_sbcs = _T("����");
	m_whry = _T("����");
	m_whtel = _T("13987654321");
	UpdateData(FALSE); //�����ؼ������

	m_font.CreatePointFont(120,_T("��Բ"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCVMediaPlayerDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect crect;
	GetDlgItem(ID)->GetClientRect(&rect);
	SetRect( crect, rect.left, rect.top, rect.right, rect.bottom );
	CvvImage cimg;
	cimg.CopyOf( img ); // ����ͼƬ
	cimg.DrawToHDC( hDC, &crect ); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC( pDC );
}

void CCVMediaPlayerDlg::changePlayerSize(int w,int h)
{
	RECT r,r1;
	GetWindowRect(&r);
	r.right = r.left+w+20;
	r.bottom = r.top+h+20+30+150;
	MoveWindow(&r);

	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC_PLAYER);
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
	p = (CStatic*)GetDlgItem(IDC_STATIC_GH);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_GH_V);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_GSPQ);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_GSPQ_V);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 2*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBLX);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 2*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBLX_V);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 3*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBCS);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 3*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_SBCS_V);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 4*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHRY);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 4*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHRY_V);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 5*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+20;
	r1.right= r1.left+w/3;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHTEL);
	p->MoveWindow(&r1);

	r1.top = r.top + 10 + h + 10 + 5*30;
	r1.bottom = r1.top + 20;
	r1.left = r.left+10 + w/3;
	r1.right= r.right - 10;
	ScreenToClient(&r1);
	p = (CStatic*)GetDlgItem(IDC_STATIC_WHTEL_V);
	p->MoveWindow(&r1);
}

void CCVMediaPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent != 1)
	{
		return;
	}
	Mat frame;//����һ��Mat���������ڴ洢ÿһ֡��ͼ��

	capture>>frame;  //��ȡ��ǰ֡
	if (frame.empty())
	{
		capture.open(filePath.c_str());
		return;
	}
	DrawPicToHDC(&IplImage(frame),IDC_STATIC_PLAYER);
	CDialogEx::OnTimer(nIDEvent);
}


void CCVMediaPlayerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(73,141,205));
}


HBRUSH CCVMediaPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(nCtlColor == CTLCOLOR_STATIC)  
	{  
		// ��������Ϊ͸��  
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(0,0,0));    
		pDC->SelectObject(&m_font);
		// ����һ��Ҫ����Ϊ�ջ�ˢ���������廹���б�����ɫ��  
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}  
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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