// AttInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AttInfoDlg.h"
#include "afxdialogex.h"
#include "vlPointSelect.h"
#include <shlwapi.h>
#include "afxwin.h"
#include <ogr_spatialref.h>
#include "BackgroundDlg.h"

extern int g_FontSize;//字体大小
extern int g_RowHeight;//行高
// CAttInfoDlg 对话框

IMPLEMENT_DYNAMIC(CAttInfoDlg, CDialogEx)

CAttInfoDlg::CAttInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAttInfoDlg::IDD, pParent)
{
	//readPrjInfo();
}

CAttInfoDlg::~CAttInfoDlg()
{
}

void CAttInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ATT, m_wndList);
	DDX_Control(pDX, IDC_BUTTON1_ENTIEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CAttInfoDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ATT, &CAttInfoDlg::OnNMClickListAtt)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1_ENTIEXIT, &CAttInfoDlg::OnBnClickedButton1Entiexit)
END_MESSAGE_MAP()


// CAttInfoDlg 消息处理程序


BOOL CAttInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pChild = new CDlgAttInfoEx();
	pChild->Create(IDD_DIALOG_POINTSELECTEX,this);
	pChild->ModifyStyle(WS_CHILD,0);
	pChild->_rect = _rect;
	pChild1 = new CShowPictureDlg();
	pChild1->Create(IDD_DIALOG_PICTURE);
	pChild1->ModifyStyle(WS_CHILD,0);
	if (pChild!= NULL)
	{
		pChild->ShowWindow(SW_HIDE);
	}
	if (pChild1!= NULL)
	{
		pChild1->ShowWindow(SW_HIDE);
		pChild1->setZoom(true);
	}

	pQren = nullptr;

	SetBackgroundColor(RGB(253,254,255));
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
	m_wndList.InsertColumn (1, _T("字段值"), LVCFMT_LEFT, 15*g_FontSize);
	
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT | /*LVS_EX_GRIDLINES|*/LVS_NOCOLUMNHEADER);
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetRowHeigt(g_RowHeight);
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetFont(&m_font);
	m_wndList.SetTextColor(RGB(255,255,255));
	
	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	m_EXIT.SetBitmap(hBitmap);
	RECT r;
	m_EXIT.GetWindowRect(&r);
	btnW = r.right - r.left;
	btnH = r.bottom - r.top;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAttInfoDlg::InsertRow(CString sName, CString sValue)
{
	int r = m_wndList.GetItemCount();
	r = 0;
	m_wndList.InsertItem(r, _T("1"));
	m_wndList.SetItemText (r, 0, sName);
	m_wndList.SetItemText (r, 1, sValue);
	
}

void CAttInfoDlg::RemoveAllRows()
{
	m_wndList.DeleteAllItems();
}

void CAttInfoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CvlPointSelect* p = (CvlPointSelect*)parent;
	if (p->pickhandler->gTemp != nullptr)
	{
		p->pickhandler->m_spViewer3D->RemoveNode(p->pickhandler->gTemp);
		p->pickhandler->gTemp = nullptr;
	}
	ShowWindow(SW_HIDE);
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->ShowWindow(SW_HIDE);
	}
	//p->OnClick();
	//CDialogEx::OnClose();
}


void CAttInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


HBRUSH CAttInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	
	return hbr;
}





BOOL CAttInfoDlg::PreTranslateMessage(MSG* pMsg)
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

void CAttInfoDlg::selfDefShowWindow(UINT i)
{
	// TODO: 在此添加控件通知处理程序代码
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->ShowWindow(i);
	}
	ShowWindow(i);
}


//设置窗口多边形
void CAttInfoDlg::setupPolygonRegion(SHOWMODE mode)
{
	int edgeW = 3;
	CRgn wndRgn;
	RECT rect;
	GetWindowRect(&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	POINT *pt = new POINT[7];
	POINT p;
	
	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_LIST_ATT);
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
	case RIGHT_CENTER:
		{
			p.x = 0;p.y= 0;
			pt[0] = p;
			p.x = w*9/10;p.y= 0;
			pt[1] = p;
			p.x = w*9/10;p.y= h/2 - 10;
			pt[2] = p;
			p.x = w;p.y= h/2;
			pt[3] = p;
			p.x = w*9/10;p.y= h/2 + 10;
			pt[4] = p;
			p.x = w*9/10;p.y= h;
			pt[5] = p;
			p.x = 0;p.y= h;
			pt[6] = p;

			r.top = pr.top + edgeW;
			r.bottom = pr.bottom - edgeW;
			r.left = pr.left + edgeW;
			r.right =  pr.right - w/10 - edgeW;
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

//将窗口移动到鼠标位置
void CAttInfoDlg::MovetoPoint(RECT viewRect)
{
	pChild->_rect = viewRect;
	CPoint   point;
	GetCursorPos(&point);
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
	else if (point.x + nWidth <= viewRect.right)
	{
		nHeight /= 1.1;
		r.left = point.x;
		r.right = point.x + nWidth;
		r.top = point.y - nHeight/2;
		r.bottom = point.y + nHeight/2;
		MoveWindow(&r);
		setupPolygonRegion(LEFT_CENTER);	
	}
	else
	{
		nHeight /= 1.1;
		r.left = point.x - nWidth;
		r.right = point.x;
		r.top = point.y - nHeight/2;
		r.bottom = point.y + nHeight/2;
		MoveWindow(&r);
		setupPolygonRegion(RIGHT_CENTER);	
	}
	pChild->ShowWindow(SW_HIDE);
	Invalidate();
}

//无边框对话框实现可拖动
void CAttInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonDown(nFlags, point);

	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y));  //或SendMessage(WM_SYSCOMMAND,0xF012,0);   //0xF012 = SC_MOVE | HTCAPTION
}

//自适应调整行距和列宽
void CAttInfoDlg::suitHeightAndWidth(int n,int maxW)
{
	maxW += maxW%2;
	RECT r;
	m_wndList.GetWindowRect(&r);
	int height = r.bottom - r.top;
	int width = r. right - r.left;

	if (maxW/2*g_FontSize+20 < width/2)
	{
		m_wndList.SetColumnWidth(0,width/2-2*g_FontSize);
		m_wndList.SetColumnWidth(1,width/2);
	}
	else
	{
		m_wndList.SetColumnWidth(0,width/4 + 20);
		m_wndList.SetColumnWidth(1,maxW/2*g_FontSize+20);
	}
	m_wndList.SetRedraw(TRUE);
	m_wndList.Invalidate();
	m_wndList.UpdateWindow();
	Invalidate();
}

void CAttInfoDlg::changeDlgHeight(int h)
{
	int edgeW = 3;
	RECT r,r1,r2;
	this->GetWindowRect(&r1);
	CMyListCtrl* list = (CMyListCtrl*)GetDlgItem(IDC_LIST_ATT);
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

	r.top = r1.top + 1;
	r.bottom = r1.bottom - 1;
	r.right = r1.right -1;
	r.left = r1.left + 1;
	ScreenToClient(&r);
	((CStatic*)GetDlgItem(IDC_STATIC_edge2))->MoveWindow(&r);
	UpdateWindow();
}

void CAttInfoDlg::analysisString(std::string inS,std::vector<CString>& vec)
{
	vec.clear();
	if (inS.empty())
	{
		return;
	}
	std::string str;
	int n;
	while(!inS.empty() && (n=inS.find_first_of(',')) != -1)
	{
		vec.push_back(CString(inS.substr(0,n).c_str()));
		inS = inS.substr(n+1);
	}
	if (!inS.empty())
	{
		vec.push_back(CString(inS.c_str()));
	}
}

void CAttInfoDlg::OnNMClickListAtt(NMHDR *pNMHDR, LRESULT *pResult)
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
		else if (str == _T("生成二维码"))
		{
			USES_CONVERSION;
			DeleteFile(_T("Data\\DRVdata\\location.png"));
			DeleteFile(_T("Data\\DRVdata\\houseInfo.png"));
			//定位
			std::string spos;
			int n =  m_wndList.GetItemCount();
			CString sitem(_T("单元"));
			for (int i = 0;i<n;++i)
			{
				if(m_wndList.GetItemText(i, 0) == sitem)
				{
					spos = std::string(W2A(m_wndList.GetItemText(i, 1)));
					break;
				}
			}
			std::string sloc = getLocationStr(lon,lat,"定位点-航天远景",spos);
			std::string strlocation = std::string("-o Data\\DRVdata\\location.png ") + sloc;

			std::string sHouse = std::string("-o Data\\DRVdata\\houseInfo.png ") +  getHouseInfoStr();

			BeginWaitCursor();
			ShellExecute(this->m_hWnd, _T("open"), _T("qrencode.exe"), CString(strlocation.c_str()), _T(""), SW_HIDE);
			ShellExecute(this->m_hWnd, _T("open"), _T("qrencode.exe"), CString(sHouse.c_str()), _T(""), SW_HIDE);
			EndWaitCursor();
			while( !PathFileExists(_T("Data\\DRVdata\\location.png")) );
			while( !PathFileExists(_T("Data\\DRVdata\\houseInfo.png")));
	
			if (pQren != nullptr)
			{
				pQren->DestroyWindow();
			}
			pQren = new CQrenCodeDlg();
			pQren->Create(IDD_DIALOG_QRENCODE);
			CRect rc;  
			pQren->GetWindowRect(&rc);
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;
			rc.left = x/2 - w/2;
			rc.right = rc.left + w;
			rc.top = y/2 - h/2;
			rc.bottom = rc.top + h;
			pQren->MoveWindow(&rc);
			pQren->ShowWindow(SW_SHOW);
			pQren->OnPaint();
		}
		else
		{
			pChild->ShowWindow(SW_HIDE);
			pChild1->ShowWindow(SW_HIDE);
		}		
	}
	*pResult = 0;
}

void CAttInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow == FALSE)
	{
		pChild->ShowWindow(bShow);
		pChild1->ShowWindow(bShow);
	}
}

std::string CAttInfoDlg::getLocationStr(double x,double y,std::string str1 ,std::string str2 )
{
	double lon = 0,lat = 0;
	PointToWGS84(x,y,lon,lat);
	char c1[20];
	char c2[20];
	sprintf_s(c1,"%f",lat);
	sprintf_s(c2,"%f",lon);

	std::string locStr = "http://api.map.baidu.com/marker?location=" + std::string(c1) + "," + std::string(c2) +   \
		"&coord_type=wgs84&title=" + str1 + "&content=" + str2 + "(" + std::string(c2) + "," + std::string(c1) + \
		")&output=html";
	return locStr;
}

void CAttInfoDlg::readPrjInfo()
{
	/*TCHAR szFilePath[1024 + 1];
	GetModuleFileName(NULL, szFilePath, 1024);

	CString str_url = szFilePath;
	USES_CONVERSION;
	std::string curFilePath( W2A(str_url) );

	curFilePath = replaceFileName(curFilePath, "Data\\DRVdata\\total.prj");

	FILE* fpPrj = fopen(curFilePath.c_str(), "r");
	char str[1024];
	fgets(str, 1024, fpPrj);
	fclose(fpPrj);

	prjInfo = str;*/
	CvlPointSelect* p = (CvlPointSelect*)parent;
	prjInfo = p->getPrj();
}

void CAttInfoDlg::PointToWGS84(double x, double y, double& lon, double& lat)
{
	lon = x;
	lat = y;
#ifdef _DEBUG
	//return;
#endif

	OGRCoordinateTransformation *poCT = NULL;

	if (poCT == nullptr)
	{
		OGRSpatialReference* oSourceSRS;
		oSourceSRS = (OGRSpatialReference*)OSRNewSpatialReference(prjInfo.c_str());

		if (oSourceSRS == NULL)
			return;

		OGRSpatialReference* oTargetSRS;
		oTargetSRS = (OGRSpatialReference*)OSRNewSpatialReference("GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]");
		if (oTargetSRS == NULL)
		{
			OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
			return;
		}

		poCT = OGRCreateCoordinateTransformation(oSourceSRS, oTargetSRS);

		if (poCT == nullptr)
		{
			OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
			OSRDestroySpatialReference((OGRSpatialReferenceH)oTargetSRS);
			return;
		}
	}

	poCT->Transform(1, &lon, &lat);

#if 0
	lon = x;
	lat = y;
	OGRSpatialReference* oSourceSRS;
	OGRSpatialReference oTargetSRS;
	OGRCoordinateTransformation *poCT;
	oSourceSRS = (OGRSpatialReference*)OSRNewSpatialReference(mSRS.c_str());
	if (oSourceSRS == NULL)
		return;

	oTargetSRS.importFromEPSG(4326);
	const char* err = CPLGetLastErrorMsg();
	poCT = OGRCreateCoordinateTransformation(oSourceSRS, &oTargetSRS);
	if (poCT == nullptr)
	{
		OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
		return;
	}

	poCT->Transform(1, &lon, &lat);
	OGRCoordinateTransformation::DestroyCT(poCT);
	OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
#endif


	if (poCT != nullptr)
		OGRCoordinateTransformation::DestroyCT((OGRCoordinateTransformation*)poCT);
	poCT = nullptr;
}

std::string CAttInfoDlg::replaceFileName(std::string srcStr, std::string replaceStr)
{
	int pos = srcStr.rfind("\\") + 1;
	int length = srcStr.length() - 1;
	return srcStr.replace(pos, length, replaceStr);
}

std::string CAttInfoDlg::getHouseInfoStr()
{
	USES_CONVERSION;
	std::string sres;
	int n =  m_wndList.GetItemCount();

	CString sitem(_T("姓名"));
	for (int i = 0;i<n;++i)
	{
		if(m_wndList.GetItemText(i, 0) == sitem)
		{
			sres += "姓名:" + std::string(W2A(m_wndList.GetItemText(i, 1))) + "\r\n";
			break;
		}
	}
	sitem = _T("身份证号");
	for (int i = 0;i<n;++i)
	{
		if(m_wndList.GetItemText(i, 0) == sitem)
		{
			sres += "身份证号:" + std::string(W2A(m_wndList.GetItemText(i, 1))) + "\r\n";
			break;
		}
	}
	sitem = _T("手机号码");
	for (int i = 0;i<n;++i)
	{
		if(m_wndList.GetItemText(i, 0) == sitem)
		{
			sres += "手机号码:" + std::string(W2A(m_wndList.GetItemText(i, 1))) + "\r\n";
			break;
		}
	}
	sitem = _T("完整房间号");
	for (int i = 0;i<n;++i)
	{
		if(m_wndList.GetItemText(i, 0) == sitem)
		{
			sres += "完整房间号:" + std::string(W2A(m_wndList.GetItemText(i, 1))) + "\r\n";
			break;
		}
	}
	sitem = (_T("社区"));
	for (int i = 0;i<n;++i)
	{
		if(m_wndList.GetItemText(i, 0) == sitem)
		{
			sres += "社区:" + std::string(W2A(m_wndList.GetItemText(i, 1))) + "\r\n";
			break;
		}
	}
	sitem = (_T("街道"));
	for (int i = 0;i<n;++i)
	{
		if(m_wndList.GetItemText(i, 0) == sitem)
		{
			sres += "街道:" + std::string(W2A(m_wndList.GetItemText(i, 1))) + " ";
			break;
		}
	}
	return sres;
}

void CAttInfoDlg::OnBnClickedButton1Entiexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


LRESULT CAttInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
	return CDialogEx::WindowProc(message, wParam, lParam);
}
