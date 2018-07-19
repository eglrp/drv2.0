// DlgAttInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAttInfo.h"
#include "DataManager.h"
#include "DlgSearchAdv.h"
#include "BackgroundDlg.h"
// CDlgAttInfo 对话框


extern int g_FontSize;//字体大小
extern int g_RowHeight;//行高
IMPLEMENT_DYNAMIC(CDlgAttInfo, CDialog)

CDlgAttInfo::CDlgAttInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAttInfo::IDD, pParent)
{
}

CDlgAttInfo::~CDlgAttInfo()
{
}

void CDlgAttInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ENTINFO, m_wndList);
	DDX_Control(pDX, IDC_BUTTON_exit1, m_quitBtn);
}


BEGIN_MESSAGE_MAP(CDlgAttInfo, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENTINFO, &CDlgAttInfo::OnNMClickDialogEntinfo)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_exit1, &CDlgAttInfo::OnBnClickedButtonexit1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgAttInfo 消息处理程序


BOOL CDlgAttInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pChild = new CDlgAttInfoEx();
	pChild->Create(IDD_DIALOG_ENTINFOEX,this);
	pChild->ModifyStyle(WS_CHILD,0);
	if (pChild)
	{
		pChild->_rect = _rect;
		pChild->ShowWindow(SW_HIDE);
	}
	pChild1 = new CShowPictureDlg();
	pChild1->Create(IDD_DIALOG_PICTUREINFO,this);
	pChild1->ModifyStyle(WS_CHILD,0);
	if (pChild1)
	{
		pChild1->ShowWindow(SW_HIDE);
		pChild1->setZoom(true);
	}
	
	m_brBk.CreateSolidBrush(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

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
	m_wndList.InsertColumn (1, _T("字段值"), LVCFMT_LEFT, 10*g_FontSize);

	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/|LVS_NOCOLUMNHEADER);

	m_wndList.SetRowHeigt(25);
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetTextColor(RGB(255,255,255));
	m_wndList.SetFont(&m_font);
	
	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	m_quitBtn.SetBitmap(hBitmap);

	RECT r;
	m_quitBtn.GetWindowRect(&r);
	btnW = r.right - r.left;
	btnH = r.bottom - r.top;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgAttInfo::InsertRow(CString sName, CString sValue)
{
	int n = m_wndList.GetItemCount();
	n = 0;
	m_wndList.InsertItem(n, _T("1"));
	m_wndList.SetItemText (n, 0, sName);
	m_wndList.SetItemText (n, 1, sValue);
}

void CDlgAttInfo::RemoveAllRows()
{
	m_wndList.DeleteAllItems();
}

void CDlgAttInfo::OnClose()
{
	this->ShowWindow(SW_HIDE);
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->ShowWindow(SW_HIDE);
	}
	if (parent == nullptr)
	{
		return;
	}
	CDataManager* p1 = dynamic_cast<CDataManager*>(parent);
	CDlgSearchAdv* p2 = dynamic_cast<CDlgSearchAdv*>(parent);
	if (p1 != nullptr)
	{
		if (p1->gTemp != nullptr)
		{
			(p1->m_spViewer3D)->RemoveNode(p1->gTemp);
			p1->gTemp = nullptr;
		}
	}
	else if (p2 != nullptr)
	{
		if (p2->gTemp != nullptr)
		{
			(p2->m_spViewer3D)->RemoveNode(p2->gTemp);
			p2->gTemp = nullptr;
		}
	}
	
}


void CDlgAttInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}


BOOL CDlgAttInfo::PreTranslateMessage(MSG* pMsg)
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgAttInfo::setupPolygonRegion(SHOWMODE mode)
{
	int edgeW = 3;
	CRgn wndRgn;
	RECT rect;
	GetWindowRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	POINT *pt = new POINT[7];
	POINT p;

	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST_ENTINFO);
	RECT r ,pr;
	list->GetWindowRect(&r);
	GetWindowRect(&pr);
	switch(mode)
	{
	case RIGHT_BOTTOM:
		{
			p.x = 0;p.y= 0;
			pt[0] = p;

			p.x = w*9/10;p.y= 0;
			pt[1] = p;

			p.x = w*9/10;p.y= h*9/10-20;
			pt[2] = p;

			p.x = w;p.y= h;
			pt[3] = p;

			p.x = w*9/10-20;p.y= h*9/10;
			pt[4] = p;

			p.x = 0;p.y= h*9/10;
			pt[5] = p;

			r.left = pr.left + edgeW;
			r.top = pr.top + edgeW;
			r.right = pr.right - w/10 - edgeW;
			r.bottom = pr.bottom - h/10 - edgeW;
			ScreenToClient(&r);
			list->MoveWindow(&r);
			break;
		}
	case LEFT_TOP:
		{
			p.x = 0;p.y= 0;
			pt[0] = p;

			p.x = w/10+20;p.y= h/10;
			pt[1] = p;

			p.x = w;p.y= h/10;
			pt[2] = p;

			p.x = w;p.y= h;
			pt[3] = p;

			p.x = w/10;p.y= h;
			pt[4] = p;

			p.x = w/10;p.y= h/10 + 20;
			pt[5] = p;

			r.left = pr.left + w/10 + edgeW;
			r.top = pr.top + h/10 + edgeW;
			r.right = pr.right - edgeW;
			r.bottom = pr.bottom - edgeW;
			ScreenToClient(&r);
			list->MoveWindow(&r);
			break;
		}
	case LEFT_BOTTOM:
		{
			p.x = w/10;p.y= 0;
			pt[0] = p;

			p.x = w;p.y= 0;
			pt[1] = p;

			p.x = w;p.y= h*9/10;
			pt[2] = p;

			p.x = w/10+20;p.y= h*9/10;
			pt[3] = p;

			p.x = 0;p.y= h;
			pt[4] = p;

			p.x = w/10;p.y= h*9/10 - 20;
			pt[5] = p;

			r.left = pr.left + w/10+edgeW;
			r.top = pr.top + edgeW;
			r.right = pr.right -edgeW;
			r.bottom = pr.bottom - h/10 - edgeW;
			ScreenToClient(&r);
			list->MoveWindow(&r);
			break;
		}
	case RIGHT_TOP:
		{
			p.x = 0;p.y= h/10;
			pt[0] = p;

			p.x = w*9/10 - 20;p.y= h/10;
			pt[1] = p;

			p.x = w;p.y= 0;
			pt[2] = p;

			p.x = w*9/10;p.y= h/10+20;
			pt[3] = p;

			p.x = w*9/10;p.y= h;
			pt[4] = p;

			p.x = 0;p.y= h;
			pt[5] = p;

			r.left = pr.left + edgeW;
			r.top = pr.top + h/10 + edgeW;
			r.right =  pr.right - w/10 - edgeW;
			r.bottom = pr.bottom - edgeW;
			ScreenToClient(&r);
			list->MoveWindow(&r);
			break;
		}
	case LEFT_CENTER:
		{
			p.x = w/10;p.y= 0;
			pt[0] = p;
			p.x = w;p.y= 0;
			pt[1] = p;
			p.x = w;p.y= h;
			pt[2] = p;
			p.x = w/10;p.y= h;
			pt[3] = p;
			p.x = w/10;p.y= h/2 + 10;
			pt[4] = p;
			p.x = 0;p.y= h/2;
			pt[5] = p;
			p.x = w/10;p.y= h/2 - 10;
			pt[6] = p;

			r.top = pr.top + edgeW;
			r.bottom = pr.bottom - edgeW;
			r.left = pr.left + w/10+ edgeW;
			r.right =  pr.right - edgeW;
			ScreenToClient(&r);
			list->MoveWindow(&r);

			wndRgn.CreatePolygonRgn(pt,7,1);
			SetWindowRgn((HRGN)wndRgn, TRUE);
			Invalidate();
			return;
		}
	default:break;
	}

	wndRgn.CreatePolygonRgn(pt,6,1);
	SetWindowRgn((HRGN)wndRgn, TRUE);
	Invalidate();
}

void CDlgAttInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y));  //或SendMessage(WM_SYSCOMMAND,0xF012,0);   //0xF012 = SC_MOVE | HTCAPTION
}

//自适应调整行距和列宽
void CDlgAttInfo::suitHeightAndWidth(int n,int maxW)
{
	maxW += maxW%2;
	RECT r,r1,r2;
	m_wndList.GetWindowRect(&r);
	int height = r.bottom - r.top;
	int width = r. right - r.left;

	/*if (n*30<height)
	{
		int newH = height/n;
		m_wndList.SetRowHeigt(newH);
	}
	else
		m_wndList.SetRowHeigt(30);*/

	if (maxW/2*g_FontSize+20 < width/2)
	{
		m_wndList.SetColumnWidth(0,width/2-2*g_FontSize);
		m_wndList.SetColumnWidth(1,width/2 + 2*g_FontSize);
	}
	else
	{
		m_wndList.SetColumnWidth(0,width/4.0 + 20);
		m_wndList.SetColumnWidth(1,width*3/4.0 - 20/*maxW/2*g_FontSize+20*/);
	}

	/*m_quitBtn.GetWindowRect(&r1);

	r1.top = r.top - btnH - 2;
	r1.bottom = r1.top + btnH;
	r1.right = r.right;
	r1.left = r1.right - btnW;


	ScreenToClient(&r1);
	m_quitBtn.MoveWindow(&r1);*/
	Invalidate();
}

//将窗口移动到鼠标位置
void CDlgAttInfo::MovetoPoint(RECT viewRect)
{
	pChild->_rect = viewRect;
	CPoint   point ;
	point.x = viewRect.left + (viewRect.right - viewRect.left)/2;
	point.y = viewRect.top + (viewRect.bottom - viewRect.top)/2;
	//GetCursorPos(&point);
	
	RECT r;
	GetWindowRect(&r);
	ClientToScreen(&r);
	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	if (point.x - nWidth >=viewRect.left && point.y - nHeight >= viewRect.top )
	{
		r.right = point.x;
		r.bottom = point.y;
		r.top = r.bottom - nHeight;
		r.left = r.right - nWidth;
		MoveWindow(&r);
		setupPolygonRegion(RIGHT_BOTTOM);

	}
	else if (point.x - nWidth >= viewRect.left && point.y + nHeight <= viewRect.bottom)
	{
		r.top = point.y;
		r.left = point.x - nWidth;
		r.bottom = point.y + nHeight;
		r.right = point.x;
		MoveWindow(&r);
		setupPolygonRegion(RIGHT_TOP);	
	}
	else if (point.y - nHeight>=viewRect.top && point.x + nWidth <= viewRect.right)
	{
		r.top = point.y - nHeight;
		r.left = point.x;
		r.right = point.x + nWidth;
		r.bottom = point.y;
		MoveWindow(&r);
		setupPolygonRegion(LEFT_BOTTOM);	
	}
	else if(point.x + nWidth <= viewRect.right && point.y + nHeight <= viewRect.bottom)
	{
		r.left = point.x;
		r.top = point.y;
		r.right = r.left + nWidth;
		r.bottom = r.top + nHeight;
		MoveWindow(&r);
		setupPolygonRegion(LEFT_TOP);	
	}
	else
	{
		nHeight /= 1.1;
		r.left = point.x;
		r.top = point.y - nHeight/2;
		r.bottom = r.top + nHeight;
		r.right = r.left + nWidth;
		MoveWindow(&r);
		setupPolygonRegion(LEFT_CENTER);
	}
	Invalidate();
}

void CDlgAttInfo::changeDlgHeight(int h)
{
	int edgeW = 3;
	RECT r,r1,r2;
	this->GetWindowRect(&r1);
	CMyListCtrl* list = (CMyListCtrl*)GetDlgItem(IDC_LIST_ENTINFO);
	list->GetWindowRect(&r);
	r1.bottom = r1.top + 1.1*h + 2*edgeW + btnH;
	this->MoveWindow(&r1);
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->MoveWindow(&r1);
	}
	this->Invalidate();

	r.top = r1.top + btnH + 2;
	r.bottom = r1.bottom - btnH - edgeW;
	r.left = r1.left + 3;
	r.right = r1.right - 3;
	ScreenToClient(&r);
	list->MoveWindow(&r);
	list->Invalidate();

	r.top = r1.top;
	r.bottom = r1.bottom - 1;
	r.right = r1.right -1;
	r.left = r1.left + 1;
	ScreenToClient(&r);
	((CStatic*)GetDlgItem(IDC_STATIC_edge))->MoveWindow(&r);

}

void CDlgAttInfo::OnNMClickDialogEntinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	int nItem = m_wndList.GetSelectionMark();
	if (nItem >= 0)
	{
		CString str = m_wndList.GetItemText(nItem, 0);
		if (str == _T("家庭成员"))
		{
			pChild->RemoveAllRows();
			pChild->InsertRow(_T("成员清单"),_T(" "));
			pChild->InsertRow(_T("姓名"),_T("身份证号"));
			if (vName.size()>0 && vName.size() <= vID.size())
			{
				for (int i = 0;i<vName.size();++i)
				{
					pChild->InsertRow(vName[i],vID[i]);
				}
			}
			pChild->SuitHeight(vName.size());
			pChild->MovetoPoint();
			pChild->ShowWindow(SW_SHOW);
			UpdateWindow();
		}
		else if (str == _T("栋号"))
		{
			USES_CONVERSION;
			CString str1 = m_wndList.GetItemText(nItem, 1);
			CString str2 = _T("Data\\DRVdata\\") + str1 + _T(".jpg");

			int height = 0;
			int width = 0;
			if (!pChild1->getPicSize(str2,width,height))
			{
				return ;
			}
			pChild1->fullScreen(width,height);
			pChild1->setPath(string(W2A(str2)));
			pChild1->setColorType(0);
			pChild1->ShowWindow(SW_SHOW);
			pChild1->Invalidate();
		}
		else
		{
			pChild->ShowWindow(SW_HIDE);
			pChild1->ShowWindow(SW_HIDE);
		}		
	}
	
	*pResult = 0;
}

//void CDlgAttInfo::analysisString(std::string inS,std::vector<CString>& vec)
//{
//	vec.clear();
//	if (inS.empty())
//	{
//		return;
//	}
//	std::string str;
//	int n;
//	while(!inS.empty() && (n=inS.find_first_of(',')) != -1)
//	{
//		vec.push_back(CString(inS.substr(0,n).c_str()));
//		inS = inS.substr(n+1);
//	}
//	if (!inS.empty())
//	{
//		vec.push_back(CString(inS.c_str()));
//	}
//}

void CDlgAttInfo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: 在此处添加消息处理程序代码
	if (bShow == FALSE)
	{
		pChild->ShowWindow(bShow);
	}
}



void CDlgAttInfo::OnBnClickedButtonexit1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}
void CDlgAttInfo::selfDefShowWindow(UINT i)
{
	// TODO: 在此添加控件通知处理程序代码
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->ShowWindow(i);
	}
	ShowWindow(i);
}


HBRUSH CDlgAttInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	return m_brBk;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


LRESULT CDlgAttInfo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	switch(message)
	{
	case WM_MOVING:
		{
			CRect rtWnd;
			GetClientRect(&rtWnd);
			ClientToScreen(&rtWnd);
			CDialogEx* dlg = dynamic_cast<CDialogEx*>(parentBk);
			::SetWindowPos(dlg->GetSafeHwnd(), NULL, rtWnd.left, rtWnd.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
