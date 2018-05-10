// ShowPictureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShowPictureDlg.h"
#include "afxdialogex.h"
#include <atlimage.h>

// CShowPictureDlg �Ի���

IMPLEMENT_DYNAMIC(CShowPictureDlg, CDialogEx)

CShowPictureDlg::CShowPictureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowPictureDlg::IDD, pParent)
{
	bZoom = false;
}

CShowPictureDlg::~CShowPictureDlg()
{
}

void CShowPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picCtr);
}

BEGIN_MESSAGE_MAP(CShowPictureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOVE()
END_MESSAGE_MAP()

// CShowPictureDlg ��Ϣ�������
bool CShowPictureDlg::getPicSize(CString name,int& w,int& h)
{
	CImage pic;
	pic.Load(name);
	if (pic.IsNull())
	{
		return false;
	}
	w= pic.GetWidth();
	h = pic.GetHeight();
	return true;
}

void CShowPictureDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	if (colorType == 0)
	{
		dc.FillSolidRect ( &rcGlassArea, RGB(10,10,10) ); //��
	}
	else if (colorType ==1)
	{
		dc.FillSolidRect ( &rcGlassArea, RGB(249,105,14) ); //�ٺ�
	}
	else if (colorType == 2)
	{
		dc.FillSolidRect ( &rcGlassArea, RGB(207,0,15) ); //��
	}
	else if (colorType == 3)
	{
		dc.FillSolidRect ( &rcGlassArea, RGB(3,166,120) ); //��
	}
	else if (colorType == 4)
	{
		dc.FillSolidRect ( &rcGlassArea, RGB(58,83,155) ); //��
	}
	else if (colorType == 5)
	{
		dc.FillSolidRect ( &rcGlassArea, RGB(8,147,213) ); //��
	}
       
	UpdateWindow();
	int height, width;
	CRect rect;//���������
	CRect rect1;
	if (!img.IsNull())
	{
		img.Destroy();
	}
	img.Load(sPath);
	height = img.GetHeight();
	width = img.GetWidth();

	m_picCtr.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
	CDC *pDc = m_picCtr.GetDC();//���pictrue�ؼ���Dc
	SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
	img.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	CDialogEx::OnPaint();        
}

BOOL CShowPictureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,180,LWA_ALPHA);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CShowPictureDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(nCtlColor == CTLCOLOR_STATIC)  
	{  
		// ��������Ϊ͸��  
		pDC->SetBkMode(TRANSPARENT);  
		// ����һ��Ҫ����Ϊ�ջ�ˢ���������廹���б�����ɫ��  
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}  
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CShowPictureDlg::setPath(std::string path)
{
	sPath = CString(path.c_str());
}

void CShowPictureDlg::setColorType(int c)
{
	colorType = c;
}

void CShowPictureDlg::changePictureSize(int w,int h)
{
	CStatic* pStatic = NULL;
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_PICTURE);
	RECT r,r1;
	this->GetWindowRect(&r1);

	r1.right = r1.left + w;
	r1.bottom = r1.top + h;
	this->MoveWindow(&r1);
	this->Invalidate();
	pStatic->GetWindowRect(&r);
	r.left = r1.left;
	r.top = r1.top;
	r.right = r.left+w;
	r.bottom = r.top + h;
	ScreenToClient(&r);
	pStatic->MoveWindow(&r);
	this->Invalidate(); 	
}

void CShowPictureDlg::setupPolygonRegion()
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

void CShowPictureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y)); 
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CShowPictureDlg::fullDlg()
{
	RECT r1,r2;
	GetWindowRect(&r1);
	int w = r1.right - r1.left;
	int h = r1.bottom - r1.top;
	m_picCtr.GetWindowRect(&r2);
	r2.left = r1.left;
	r2.right = r2.left + w;
	r2.top = r1.top;
	r2.bottom = r2.top + h;
	ScreenToClient(&r2);
	m_picCtr.MoveWindow(&r2);
	Invalidate(TRUE);
}

void CShowPictureDlg::setZoom(bool flag)
{
	bZoom = flag;
}

BOOL CShowPictureDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!bZoom)
	{
		return FALSE;
	}
	RECT r;
	GetWindowRect(&r);
	int h = r.bottom - r.top;
	int w = r.right - r.left;
	int w1,h1;
	double d;
	d = zDelta/120 * 0.1;
	w1 = w*(1+d);
	h1 = h*(1+d);	
	r.top = centerY - h1/2;
	r.bottom = centerY + h1/2;
	r.left = centerX - w1/2;
	r.right = centerX + w1/2;
	if (d<=0)
	{
		if (w<=100 || h<=100)
		{
			return FALSE;
		}
	}
	else
	{
		if (r.top <= 0 && r.left <= 0 && r.right >= GetSystemMetrics(SM_CXSCREEN) && r.bottom >= GetSystemMetrics(SM_CYSCREEN))
		{
			return FALSE;
		}
		if ((r.top<0 && r.left<0) || (r.top<0 && r.right>GetSystemMetrics(SM_CXSCREEN)) ||
			(r.bottom > GetSystemMetrics(SM_CYSCREEN) && r.left<0) || (r.bottom > GetSystemMetrics(SM_CYSCREEN) && r.right>GetSystemMetrics(SM_CXSCREEN)))
		{
			fullScreen(img.GetWidth(),img.GetHeight());
			fullDlg();
			centerX = GetSystemMetrics(SM_CXSCREEN)/2;
			centerY = GetSystemMetrics(SM_CYSCREEN)/2;
			return TRUE;
		}
	}
	MoveWindow(&r);
	fullDlg();
	m_picCtr.UpdateData(FALSE);
	Invalidate(true);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CShowPictureDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: �ڴ˴������Ϣ����������
	RECT r;
	GetWindowRect(&r);
	int h = r.bottom - r.top;
	int w = r.right - r.left;
	centerX = r.left + w/2;
	centerY = r.top + h/2;
}

void CShowPictureDlg::fullScreen(int width,int height)
{
	int X = GetSystemMetrics(SM_CXSCREEN);
	int Y = GetSystemMetrics(SM_CYSCREEN);

	double dx = X/1.0/width;
	double dy = Y/1.0/height;

	int newW,newH;
	RECT r;
	if (dx>dy)
	{
		newH = Y;
		newW = width * dy;
		r.top = 0;
		r.bottom = newH;
		r.left = X/2 - newW/2;
		r.right = r.left + newW;
		MoveWindow(&r);
		fullDlg();
	}
	else
	{
		newW = X;
		newH = height * dx;
		r.left = 0;
		r.right = newW;
		r.top = Y/2 - newH/2;
		r.bottom = r.top + newH;
		MoveWindow(&r);
		fullDlg();
	}

	centerX = X/2;
	centerY = Y/2;
}
