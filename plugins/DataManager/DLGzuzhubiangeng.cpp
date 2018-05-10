// DLGzuzhubiangeng.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGzuzhubiangeng.h"
#include "afxdialogex.h"
#include "DlgDB_Connect.h"
#include "DataManager.h"
#include "CommunityDlg.h"
#include <string>
#include <fstream>
#include "BackgroundDlg.h"

extern CDataManager* m_pDM;
extern CDlgDB_Connect* g_DBconnector;

extern std::string IP_SERVER;
extern std::string USER_NAME;
extern std::string USER_PASSWORD;
extern std::string DATABASE_NAME;
extern UINT PORT_NUM;

// CDLGzuzhubiangeng 对话框

IMPLEMENT_DYNAMIC(CDLGzuzhubiangeng, CDialogEx)

int CDLGzuzhubiangeng::m_SortColumn = -1;
BOOL CDLGzuzhubiangeng::m_bAsc = FALSE;

int CDLGzuzhubiangeng::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	USES_CONVERSION;
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	std::string    strItem1 = std::string(W2A(pListCtrl->GetItemText(lParam1,m_SortColumn)));
	std::string    strItem2 = std::string(W2A(pListCtrl->GetItemText(lParam2,m_SortColumn)));

	double d1 = atof(strItem1.c_str());
	double d2 = atof(strItem2.c_str());
	if (d1 != 0.0 && d2 != 0.0)
	{
		if (m_bAsc)
		{
			return d1<= d2;
		}
		else
			return d1 > d2;
	}

	if (m_bAsc)
	{
		return strItem1 <= strItem2;
	}
	else
	{
		return strItem2 < strItem1;
	}
}

CDLGzuzhubiangeng::CDLGzuzhubiangeng(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGzuzhubiangeng::IDD, pParent)
{

}

CDLGzuzhubiangeng::~CDLGzuzhubiangeng()
{
}

void CDLGzuzhubiangeng::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ZZBG, m_wndList);
	DDX_Control(pDX, IDC_STATIC_ZZBGTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_ZZBGEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGzuzhubiangeng, CDialogEx)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ZZBG, &CDLGzuzhubiangeng::OnLvnColumnclickListZzbg)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_ZZBGEXIT, &CDLGzuzhubiangeng::OnBnClickedButtonZzbgexit)
END_MESSAGE_MAP()


// CDLGzuzhubiangeng 消息处理程序


BOOL CDLGzuzhubiangeng::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("姓名"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("性别"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("楼栋"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (3, _T("房间"), LVCFMT_LEFT, 60);
	m_wndList.InsertColumn (4, _T("身份证"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (5, _T("电话"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (6, _T("居住时间"), LVCFMT_LEFT, 100);
	m_font1.CreatePointFont(110, _T("宋体"));
	GetDlgItem(IDC_STATIC_RESULT2)->SetFont(&m_font1);
	m_wndList.SetFont(&m_font1);
	m_wndList.SetRowHeigt(25);
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetTextColor(RGB(255,255,255));
	m_font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		TRUE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("黑体"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("租住变更"));

	m_brBk.CreateSolidBrush(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

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

void CDLGzuzhubiangeng::doSearch()
{
	std::string sQuery;//查询
	
	sQuery = "SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
				lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
				lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE,lxc.acs_resident.LIVE_TYPE \
				FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
				where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
				and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
				and (lxc.acs_resident.LIVE_TYPE = '一个月' \
				or lxc.acs_resident.LIVE_TYPE = '两个月' \
				or lxc.acs_resident.LIVE_TYPE = '三个月') \
				group by lxc.acs_resident.PEOPLE_IDCARD";

	MYSQL sqlCont;
	MYSQL_RES *result = NULL;
	MYSQL_ROW sql_row;
	int res;
	int numResult = 0;
	//单独建立连接访问数据库
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
		AfxMessageBox(_T("服务器访问失败！请重试"));
		return;
	}
	AfxGetMainWnd()->BeginWaitCursor();
	mysql_query(&sqlCont, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文
	res = mysql_query(&sqlCont, sQuery.c_str());
	if(!res)
	{
		result=mysql_store_result(&sqlCont);//保存查询到的数据到result
		if(result)
		{
			while(sql_row=mysql_fetch_row(result))//获取具体的数据
			{
				char msg[255];
				m_wndList.InsertItem(0, _T("1"));
				for (int i = 0;i<7;++i)
				{
					sprintf(msg,"%s",sql_row[i]);
					//隐藏身份证
					//隐藏身份证
					if (i == 4)
					{
						for(int j = 3;j<=14;++j)
						{
							msg[j] = '*';
						}
					}
					//隐藏电话
					if (i == 5)
					{
						for(int j = 4;j<=7;++j)
						{
							msg[j] = '*';
						}
					}
					m_wndList.SetItemText (0, i, COMFUNC::getChineseSex(CString(msg)));
				}
				numResult++;
			}
		}
		mysql_free_result(result);//释放结果资源
	} 
	mysql_close(&sqlCont);
	AfxGetMainWnd()->EndWaitCursor();
	CString s;
	s.Format(_T("共 %d 条"),numResult);
	((CStatic*)GetDlgItem(IDC_STATIC_RESULT2))->SetWindowTextW(s);
	CRect rc;
	GetDlgItem(IDC_STATIC_RESULT2)->GetWindowRect(&rc);      
	ScreenToClient(&rc);        
	InvalidateRect(rc);
}

void CDLGzuzhubiangeng::OnLvnColumnclickListZzbg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_SortColumn = pNMListView->iSubItem;
	m_bAsc=!m_bAsc;//升序还是降序

	m_wndList.SortItems(MyCompareProc, (LPARAM) &m_wndList);

	int count = m_wndList.GetItemCount();   //行数
	for (int i=0; i<count;  i++)  
	{  
		m_wndList.SetItemData(i,i);  
	}
	*pResult = 0;
}


HBRUSH CDLGzuzhubiangeng::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return m_brBk;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDLGzuzhubiangeng::PreTranslateMessage(MSG* pMsg)
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


void CDLGzuzhubiangeng::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


void CDLGzuzhubiangeng::OnBnClickedButtonZzbgexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}
