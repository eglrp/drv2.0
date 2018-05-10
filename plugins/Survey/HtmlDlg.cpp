// HtmlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HtmlDlg.h"
#include "afxdialogex.h"


// CHtmlDlg 对话框
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


// CHtmlDlg 消息处理程序
BOOL CHtmlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//设置对话框背景色及透明度
	m_brush.CreateSolidBrush(RGB(95,97,198)); 
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,180,LWA_ALPHA);//0 透明  255 不透明
	//加载本地html文件
	nObjOutputHTML->CreateFromStatic(IDC_STATIC_HTML,this);
	nObjOutputHTML->Navigate(_T("about:blank")); 
	nObjOutputHTML->Navigate(_T("file://D:\\门禁系统.html"));
	return TRUE;
}

void CHtmlDlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文 
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(14,108,195)); 
}


void CHtmlDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	int n = GetModuleFileNameA(0, szModuleFilePath, MAX_PATH); //获得当前执行文件的路径  
	szModuleFilePath[ strrchr(szModuleFilePath, '\\') - szModuleFilePath + 1 ] = 0;//将最后一个"\\"后的字符置为0  
	strcpy(SaveResult,szModuleFilePath);  
	strcat(SaveResult,name.c_str());//在当前路径后添加子路径 

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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y)); 
	CDialogEx::OnLButtonDown(nFlags, point);
}
