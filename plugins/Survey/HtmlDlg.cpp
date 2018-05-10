// HtmlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HtmlDlg.h"
#include "afxdialogex.h"


// CHtmlDlg �Ի���
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNAMIC(CHtmlDlg, CDialogEx)

CHtmlDlg::CHtmlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlDlg::IDD, pParent)
{
	nObjOutputHTML = new CHtmlCtrl();
}

CHtmlDlg::~CHtmlDlg()
{
}

void CHtmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHtmlDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CHtmlDlg ��Ϣ�������
BOOL CHtmlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//���öԻ��򱳾�ɫ��͸����
	m_brush.CreateSolidBrush(RGB(95,97,198)); 
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,180,LWA_ALPHA);//0 ͸��  255 ��͸��
	//���ر���html�ļ�
	nObjOutputHTML->CreateFromStatic(IDC_STATIC_HTML,this);
	nObjOutputHTML->Navigate(_T("about:blank")); 
	nObjOutputHTML->Navigate(_T("file://D:\\�Ž�ϵͳ.html"));
	return TRUE;
}

void CHtmlDlg::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������ 
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(14,108,195)); 
}


void CHtmlDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


HBRUSH CHtmlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_brush;
}

void CHtmlDlg::setHTML(std::string name)
{
	char szModuleFilePath[MAX_PATH];  
	char SaveResult[MAX_PATH];  
	int n = GetModuleFileNameA(0, szModuleFilePath, MAX_PATH); //��õ�ǰִ���ļ���·��  
	szModuleFilePath[ strrchr(szModuleFilePath, '\\') - szModuleFilePath + 1 ] = 0;//�����һ��"\\"����ַ���Ϊ0  
	strcpy(SaveResult,szModuleFilePath);  
	strcat(SaveResult,name.c_str());//�ڵ�ǰ·���������·�� 

	nObjOutputHTML->Navigate(CString(SaveResult));
	Invalidate();
}

void CHtmlDlg::changeHTMLsize(int w,int h)
{
	CStatic* pStatic = NULL;
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_HTML);
	RECT r,r1;
	this->GetWindowRect(&r1);
	
	r1.right = r1.left + w + 20;
	r1.bottom = r1.top + h + 30;
	this->MoveWindow(&r1);
	this->Invalidate();
	pStatic->GetWindowRect(&r);
	r.left = r1.left+10;
	r.top = r1.top + 10;
	r.right = r.left+w;
	r.bottom = r.top + h;
	ScreenToClient(&r);
	pStatic->MoveWindow(&r);
	this->Invalidate(); 	
}

void CHtmlDlg::setupPolygonRegion()
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

void CHtmlDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y)); 
	CDialogEx::OnLButtonDown(nFlags, point);
}
