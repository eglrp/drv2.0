// DLGInternetInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DLGInternetInfo.h"
#include "afxdialogex.h"
#include "vlPointSelect.h"
#include <list>
#include "sqlite3/include/sqlite3.h"
#include "BackgroundDlg.h"
using namespace std;

extern int g_FontSize;//字体大小
extern int g_RowHeight;//行高

//data
std::list<string> vlxcclck;
std::list<string> vlxcclrk;
std::list<string> vdoor6;

string s_manHole;
string s_smokeSensor;
string s_doorSensor;
string s_eleMeter;
//function
//打开本地sqlite3数据库
sqlite3 * OpenLocalDatabase(char* dataBaseFileName)
{
	sqlite3 *pDB = NULL;
	int result = sqlite3_open(dataBaseFileName, &pDB);
	return pDB;
}
void StartBegin(sqlite3 *pDB)
{
	sqlite3_exec(pDB, "begin;", 0, 0, 0);
}
void Commit(sqlite3 *pDB)
{
	sqlite3_exec(pDB, "commit;", 0, 0, 0);
}
//关闭数据库
void CloseDatabase(sqlite3 *pDB)
{
	sqlite3_close(pDB);
}

// CDLGInternetInfo 对话框

IMPLEMENT_DYNAMIC(CDLGInternetInfo, CDialogEx)

CDLGInternetInfo::CDLGInternetInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGInternetInfo::IDD, pParent)
{

}

CDLGInternetInfo::~CDLGInternetInfo()
{
}

void CDLGInternetInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Control(pDX, IDC_BUTTON1_NETEXIT, m_EXIT);
}

BEGIN_MESSAGE_MAP(CDLGInternetInfo, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDLGInternetInfo::OnBnClickedButtonUpdate)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1_NETEXIT, &CDLGInternetInfo::OnBnClickedButton1Netexit)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CDLGInternetInfo 消息处理程序


BOOL CDLGInternetInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
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
	SetBackgroundColor(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);
	m_wndList.SetTextColor(RGB(255,255,255));
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetRowHeigt(g_RowHeight);
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.InsertColumn (0, _T("名称"), LVCFMT_CENTER, 150);
	m_wndList.InsertColumn (1, _T("字段值"), LVCFMT_CENTER, 500);
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_wndList.SetFont(&m_font);
	readFromDB();
	SetTimer(1,10000,NULL);

	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	m_EXIT.SetBitmap(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDLGInternetInfo::selfDefShowWindow(UINT i)
{
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->ShowWindow(i);
	}
	ShowWindow(i);
}

void CDLGInternetInfo::readFromDB()
{
	sqlite3* db = OpenLocalDatabase("DRV.db");
	char *errmsg = NULL;
	const char* strSql1 = "select * from lxcclrk";
	char **dbResult = NULL;
	int nRow;
	int nCol;
	int result = sqlite3_get_table(db, strSql1, &dbResult, &nRow, &nCol, &errmsg);
	if (nRow > 0)
	{
		vlxcclrk.clear();
		for (int i = 0;i<nRow;++i)
		{
			vlxcclrk.push_back(string(dbResult[i+1]));
		}
	}
		
	const char* strSql2 = "select * from lxcclck";
	dbResult = NULL;
	result = sqlite3_get_table(db, strSql2, &dbResult, &nRow, &nCol, &errmsg);
	if (nRow > 0)
	{
		vlxcclck.clear();
		for (int i = 0;i<nRow;++i)
		{
			vlxcclck.push_back(string(dbResult[i+1]));
		}
	}
	
	const char* strSql3 = "select * from door6";
	dbResult = NULL;
	result = sqlite3_get_table(db, strSql3, &dbResult, &nRow, &nCol, &errmsg);
	if (nRow > 0)
	{
		vdoor6.clear();
		for (int i = 0;i<nRow;++i)
		{
			vdoor6.push_back(string(dbResult[i+1]));
		}
	}
	
	const char* strSql4 = "select * from manHole";
	dbResult = NULL;
	result = sqlite3_get_table(db, strSql4, &dbResult, &nRow, &nCol, &errmsg);
	if (nRow > 0)
	{
		s_manHole = string(dbResult[nRow]);
	}

	const char* strSql5 = "select * from smokeSensor";
	dbResult = NULL;
	result = sqlite3_get_table(db, strSql5, &dbResult, &nRow, &nCol, &errmsg);
	if (nRow > 0)
	{
		s_smokeSensor = string(dbResult[nRow]);
	}

	const char* strSql6 = "select * from doorSensor";
	dbResult = NULL;
	result = sqlite3_get_table(db, strSql6, &dbResult, &nRow, &nCol, &errmsg);
	if (nRow > 0)
	{
		s_doorSensor = string(dbResult[nRow]);
	}

	const char* strSql7 = "select * from eleMeter";
	dbResult = NULL;
	result = sqlite3_get_table(db, strSql7, &dbResult, &nRow, &nCol, &errmsg);
	if (nRow > 0)
	{
		s_eleMeter = string(dbResult[nRow]);
	}

	Commit(db);
	CloseDatabase(db);
}

void CDLGInternetInfo::InsertRow(std::vector<CString> vName)
{
	int m = m_wndList.GetItemCount();
	m = 0;
	m_wndList.InsertItem(m, _T("1"));
	for (int i = 0;i<vName.size();++i)
	{
		m_wndList.SetItemText (m, i, vName[i]);
	}
}

void CDLGInternetInfo::analysisData(std::string s,std::vector<CString>& vec)
{
	size_t pos;
	vec.clear();
	while( (pos=s.find_first_of("&&")) != -1)
	{
		vec.push_back(CString((s.substr(0,pos)).c_str()));
		s = s.substr(pos+2);
	}
	if (!s.empty())
	{
		vec.push_back(CString(s.c_str()));
	}
}

void CDLGInternetInfo::AutoAdjustDlg()
{
	CHeaderCtrl *pHeader = m_wndList.GetHeaderCtrl();  
	int nColumnCount = pHeader->GetItemCount();  
	int len = 0;
	for(int i = 0; i < nColumnCount; i++)  
	{  
		len +=  m_wndList.GetColumnWidth(i); //计算list的宽度
	}  
	RECT r1,r2,r3;
	((CButton*)GetDlgItem(IDC_BUTTON1_NETEXIT))->GetWindowRect(&r3);
	int btnW = r3.right - r3.left;
	int btnH = r3.bottom - r3.top;
	GetWindowRect(&r1);
	r1.right = r1.left + len + 60;
	r1.bottom = r1.top + 25 + g_RowHeight * 6 + 35 ;

	CBackgroundDlg* dlg = dynamic_cast<CBackgroundDlg*>(parentBk);
	if (dlg)
	{
		dlg->MoveWindow(&r1);
	}
	MoveWindow(&r1);
	r2.left = r1.left+2;
	r2.top = r1.top;
	r2.bottom = r1.bottom - 2;
	r2.right = r1.right - 2;
	ScreenToClient(&r2);
	((CStatic*)GetDlgItem(IDC_STATIC_edge2))->MoveWindow(&r2);
	m_wndList.GetWindowRect(&r2);
	r2.left = r1.left + 25;
	r2.right = r2.left + len;
	r2.top = r1.top + 25;
	r2.bottom = r1.bottom - 35;
	ScreenToClient(&r2);
	m_wndList.MoveWindow(&r2);
	GetWindowRect(&r1);
	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON_UPDATE);
	btn->GetWindowRect(&r2);
	r2.top = r1.bottom - 30;
	r2.bottom = r2.top + 25;
	r2.left = r1.right - 30 - 75;
	r2.right = r2.left + 75;
	ScreenToClient(&r2);
	btn->MoveWindow(&r2);

	r3.top = r1.top + 3;
	r3.bottom = r3.top + btnH;
	r3.right = r1.right - 3;
	r3.left = r3.right - btnW;
	ScreenToClient(&r3);
	((CButton*)GetDlgItem(IDC_BUTTON1_NETEXIT))->MoveWindow(&r3);
	this->UpdateWindow();
}

void CDLGInternetInfo::AutoAdjustColumnWidth(CListCtrl *pListCtrl)  
{  
	pListCtrl->SetRedraw(FALSE);  
	CHeaderCtrl *pHeader = pListCtrl->GetHeaderCtrl();  
	int nColumnCount = pHeader->GetItemCount();  

	for(int i = 0; i < nColumnCount; i++)  
	{  
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE);  
		int nColumnWidth = pListCtrl->GetColumnWidth(i);  
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  
		int nHeaderWidth = pListCtrl->GetColumnWidth(i);  

		pListCtrl->SetColumnWidth(i, max(nColumnWidth, nHeaderWidth) );  
		TRACE(max(nColumnWidth, nHeaderWidth));
	}  
	pListCtrl->SetRedraw(TRUE);  
}  

BOOL CDLGInternetInfo::PreTranslateMessage(MSG* pMsg)
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

void CDLGInternetInfo::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
	CBackgroundDlg* dlg = dynamic_cast<CBackgroundDlg*>(parentBk);
	if (dlg)
	{
		dlg->ShowWindow(SW_HIDE);
	}
}

void CDLGInternetInfo::OnBnClickedButtonUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON_UPDATE);
	btn->EnableWindow(FALSE);
	readFromDB();
	showInfo(true);
	btn->EnableWindow(TRUE);
}

void CDLGInternetInfo::showInfo(bool bshow)
{
	m_wndList.DeleteAllItems();
	if (bshow)
	{
		//删除表头
		CHeaderCtrl* pHeaderCtrl = m_wndList.GetHeaderCtrl();
		if(pHeaderCtrl)
		{
			int n = pHeaderCtrl->GetItemCount();
			for (int i = n-1;i>=0;--i)
			{
				m_wndList.DeleteColumn(i);
			}
		}
		std::vector<CString> vec;
		switch(_type)
		{
		case 1://车辆出口
			{
				m_wndList.InsertColumn (0, _T("车牌号码"), LVCFMT_CENTER, 90);
				m_wndList.InsertColumn (1, _T("车辆类型"), LVCFMT_CENTER, 90);
				m_wndList.InsertColumn (2, _T("出场时间"), LVCFMT_CENTER, 190);
				m_wndList.InsertColumn (3, _T("停留时间"), LVCFMT_CENTER, 120);
				for (list<string>::iterator ite = vlxcclck.begin();ite != vlxcclck.end();++ite)
				{
					analysisData(*ite,vec);
					InsertRow(vec);
				}
				//插入表头
				vector<CString> cvec;
				cvec.push_back(_T("车牌号码"));
				cvec.push_back(_T("车辆类型"));
				cvec.push_back(_T("出场时间"));
				cvec.push_back(_T("停留时间"));
				InsertRow(cvec);
				
				//AutoAdjustColumnWidth(&m_wndList);
				break;
			}
		case 2://车辆入口
			{
				m_wndList.InsertColumn (0, _T("车牌号码"), LVCFMT_CENTER, 90);
				m_wndList.InsertColumn (1, _T("车辆类型"), LVCFMT_CENTER, 90);
				m_wndList.InsertColumn (2, _T("入场时间"), LVCFMT_CENTER, 190);
				for (list<string>::iterator ite = vlxcclrk.begin();ite != vlxcclrk.end();++ite)
				{
					analysisData(*ite,vec);
					InsertRow(vec);
				}
				//插入表头
				vector<CString> cvec;
				cvec.push_back(_T("车牌号码"));
				cvec.push_back(_T("车辆类型"));
				cvec.push_back(_T("入场时间"));
				InsertRow(cvec);
				
				//AutoAdjustColumnWidth(&m_wndList);
				break;
			}
		case 3://6号门禁
			{
				m_wndList.InsertColumn (0, _T("ID"), LVCFMT_CENTER, 100);
				m_wndList.InsertColumn (1, _T("姓名"), LVCFMT_CENTER, 80);
				m_wndList.InsertColumn (2, _T("身份证"), LVCFMT_CENTER, 180);
				m_wndList.InsertColumn (3, _T("单元"), LVCFMT_CENTER, 100);
				m_wndList.InsertColumn (4, _T("楼层"), LVCFMT_CENTER, 50);
				m_wndList.InsertColumn (5, _T("房间号"), LVCFMT_CENTER, 70);
				m_wndList.InsertColumn (6, _T("刷卡时间"), LVCFMT_CENTER, 300);
				m_wndList.InsertColumn (7, _T("in/out"), LVCFMT_CENTER, 70);
				for (list<string>::iterator ite = vdoor6.begin();ite != vdoor6.end();++ite)
				{
					analysisData(*ite,vec);
					InsertRow(vec);
				}
				vector<CString> cvec;
				cvec.push_back(_T("ID"));
				cvec.push_back(_T("姓名"));
				cvec.push_back(_T("身份证"));
				cvec.push_back(_T("单元"));
				cvec.push_back(_T("楼层"));
				cvec.push_back(_T("房间号"));
				cvec.push_back(_T("刷卡时间"));
				cvec.push_back(_T("in/out"));
				InsertRow(cvec);
				
				//AutoAdjustColumnWidth(&m_wndList);
				break;
			}
		default:{break;}
		}
	}
}

void CDLGInternetInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		readFromDB();
		showInfo(true);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDLGInternetInfo::OnShowWindow(BOOL bShow, UINT nStatus)
{
	//if (bShow == TRUE)
	{
		showInfo(true);
	}
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}

void CDLGInternetInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	/*CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(14,108,195));*/
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CDLGInternetInfo::OnBnClickedButton1Netexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


void CDLGInternetInfo::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDCtl==IDC_BUTTON_UPDATE)         //checking for the button 
	{
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct ->hDC);   // Get the Button DC to CDC

		rect = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.

		dc.Draw3dRect(&rect,RGB(0,0,0),RGB(0,0,0)); 

		dc.FillSolidRect(&rect,RGB(100,100,100));//Here you can define the required color to appear on the Button.

		UINT state=lpDrawItemStruct->itemState; //This defines the state of the Push button either pressed or not. 

		if((state & ODS_SELECTED))
		{
			dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

		}
		else
		{
			dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
		}

		dc.SetBkColor(RGB(100,100,100));   //Setting the Text Background color
		dc.SetTextColor(RGB(255,255,255));     //Setting the Text Color


		TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
		ZeroMemory(buffer,MAX_PATH );     //Intializing the buffer to zero
		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //Get the Caption of Button Window 

		dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//Redraw the Caption of Button Window 

		dc.Detach(); // Detach the Button DC
	}                
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
