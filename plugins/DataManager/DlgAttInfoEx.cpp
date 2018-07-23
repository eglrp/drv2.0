// DlgAttInfoEx.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DlgAttInfoEx.h"
#include "afxdialogex.h"
#include "DlgAttInfo.h"

extern int g_FontSize;
extern int g_RowHeight;
// CDlgAttInfoEx 对话框

IMPLEMENT_DYNAMIC(CDlgAttInfoEx, CDialogEx)

CDlgAttInfoEx::CDlgAttInfoEx(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAttInfoEx::IDD, pParent)
{

}

CDlgAttInfoEx::~CDlgAttInfoEx()
{
}

void CDlgAttInfoEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ENTINFOEX, m_wndList);
}


BEGIN_MESSAGE_MAP(CDlgAttInfoEx, CDialogEx)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ENTINFOEX, &CDlgAttInfoEx::OnLvnItemchangedList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENTINFOEX, &CDlgAttInfoEx::OnNMClickList2)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgAttInfoEx 消息处理程序


BOOL CDlgAttInfoEx::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,130,LWA_ALPHA);//0 透明  255 不透明
	m_font.CreateFont(g_FontSize, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		FALSE, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体"));

	m_wndList.InsertColumn (0, _T("字段名"), LVCFMT_LEFT, 5*g_FontSize);
	m_wndList.InsertColumn (1, _T("字段值"), LVCFMT_LEFT, 11*g_FontSize);

	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/|LVS_NOCOLUMNHEADER);
	m_wndList.setColor(RGB(14,108,195));
	m_wndList.SetRowHeigt(g_RowHeight);
	m_wndList.SetTextBkColor(RGB(14,108,195));
	m_wndList.SetBkColor(RGB(14,108,195));
	m_wndList.SetFont(&m_font);
	m_wndList.SetTextColor(RGB(255,255,255));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAttInfoEx::InsertRow(CString sName, CString sValue)
{
	int n = m_wndList.GetItemCount();
	m_wndList.InsertItem(n, _T("1"));
	m_wndList.SetItemText (n, 0, sName);
	m_wndList.SetItemText (n, 1, sValue);
}

void CDlgAttInfoEx::RemoveAllRows()
{
	m_wndList.DeleteAllItems();
}


void CDlgAttInfoEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 

	dc.FillSolidRect ( &rcGlassArea, RGB(255,255,255));
}


void CDlgAttInfoEx::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgAttInfoEx::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	

	*pResult = 0;
}

void CDlgAttInfoEx::MovetoPoint()
{
	/*CWnd* wnd = GetParent();
	CDlgAttInfo* pWnd = dynamic_cast<CDlgAttInfo*>(wnd);
	RECT rp;
	pWnd->m_wndList.GetWindowRect(&rp);
	CPoint   point ;
	GetCursorPos(&point);

	int edgeW = 3;
	RECT r;
	GetWindowRect(&r);
	ClientToScreen(&r);
	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = rp.right + 2*edgeW ;
	r.bottom = point.y;
	r.top = r.bottom - nHeight;
	r.right = r.left + 256 + 2*edgeW + 20;
	MoveWindow(&r);
	Invalidate();

	CMyListCtrl* list = (CMyListCtrl*)GetDlgItem(IDC_LIST2);
	RECT pr;
	list->GetWindowRect(&pr);


	GetWindowRect(&r);
	pr.left = r.left + edgeW + 20;
	pr.top = r.top + edgeW;
	pr.right = r.right - edgeW;
	pr.bottom = r.bottom - edgeW;
	ScreenToClient(&pr);
	list->MoveWindow(&pr);
	list->Invalidate();

	setPolygonRgn();*/

	CWnd* wnd = GetParent();
	CDlgAttInfo* pWnd = dynamic_cast<CDlgAttInfo*>(wnd);
	RECT rp;
	pWnd->m_wndList.GetWindowRect(&rp);
	CPoint   point ;
	GetCursorPos(&point);

	int edgeW = 3;
	RECT r;
	GetWindowRect(&r);
	ClientToScreen(&r);
	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	if (rp.right + g_FontSize*16 + 4*edgeW + 20 <= pWnd->_rect.right)
	{
		r.left = rp.right + 2*edgeW;
		r.bottom = point.y;
		r.top = r.bottom - nHeight;
		r.right = r.left + g_FontSize*16 + 2*edgeW + 20;
		MoveWindow(&r);
		setPolygonRgn(LEFT);
	}
	else
	{
		r.right = rp.left - 2*edgeW;
		r.left = r.right - g_FontSize*16- 2*edgeW - 20;
		r.bottom = point.y;
		r.top = r.bottom - nHeight;
		MoveWindow(&r);
		setPolygonRgn(RIGHT);
	}
	UpdateWindow();
}

void CDlgAttInfoEx::SuitHeight(int len)
{
	RECT r;
	int edgeW = 3;
	GetWindowRect(&r);
	r.bottom = r.top + (len+2) * g_RowHeight + 10 + 2*edgeW;
	MoveWindow(&r);
}

void CDlgAttInfoEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y));  //或SendMessage(WM_SYSCOMMAND,0xF012,0);   //0xF012 = SC_MOVE | HTCAPTION
}

void CDlgAttInfoEx::setPolygonRgn(SHOWMODE mode)
{
	/*int edgeW = 3;
	CRgn wndRgn;
	RECT rect;
	GetWindowRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	POINT *pt = new POINT[5];
	POINT p;

	p.x = 20;p.y= 0;
	pt[0] = p;

	p.x = w;p.y= 0;
	pt[1] = p;

	p.x = w;p.y= h;
	pt[2] = p;

	p.x = 0;p.y= h;
	pt[3] = p;

	p.x = 20;p.y= h-15;
	pt[4] = p;

	wndRgn.CreatePolygonRgn(pt,5,1);
	SetWindowRgn((HRGN)wndRgn, TRUE);
	Invalidate();*/
	int edgeW = 3;
	CRgn wndRgn;
	RECT rect;
	GetWindowRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	CMyListCtrl* list = (CMyListCtrl*)GetDlgItem(IDC_LIST_ENTINFOEX);
	RECT pr;
	list->GetWindowRect(&pr);



	POINT *pt = new POINT[5];
	POINT p;
	switch(mode)
	{
	case LEFT:
		{
			p.x = 20;p.y= 0;
			pt[0] = p;

			p.x = w;p.y= 0;
			pt[1] = p;

			p.x = w;p.y= h;
			pt[2] = p;

			p.x = 0;p.y= h;
			pt[3] = p;

			p.x = 20;p.y= h-15;
			pt[4] = p;

			pr.left = rect.left + 20 + edgeW;
			pr.right = rect.right - edgeW;
			pr.top = rect.top + edgeW;
			pr.bottom = rect.bottom - edgeW;
			ScreenToClient(&pr);
			list->MoveWindow(&pr);
			break;
		}
	case RIGHT:
		{
			p.x = 0;p.y= 0;
			pt[0] = p;
			p.x = w - 20;p.y= 0;
			pt[1] = p;
			p.x = w - 20;p.y= h - 15;
			pt[2] = p;
			p.x = w;p.y= h;
			pt[3] = p;
			p.x = 0;p.y= h;
			pt[4] = p;

			pr.right = rect.right - 20 - edgeW;
			pr.left = rect.left + edgeW;
			pr.top = rect.top + edgeW;
			pr. bottom = rect.bottom - edgeW;
			ScreenToClient(&pr);
			list->MoveWindow(&pr);
			break;
		}
	default:break;
	}

	wndRgn.CreatePolygonRgn(pt,5,1);
	SetWindowRgn((HRGN)wndRgn, TRUE);
	Invalidate();
}