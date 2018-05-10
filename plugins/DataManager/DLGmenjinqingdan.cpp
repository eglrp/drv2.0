// DLGmenjinqingdan.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGmenjinqingdan.h"
#include "afxdialogex.h"
#include "BackgroundDlg.h"
#include "DataManager.h"
#include "DlgDB_Connect.h"

extern CDataManager* m_pDM;
extern CDlgDB_Connect* g_DBconnector;

extern std::string IP_SERVER;
extern std::string USER_NAME;
extern std::string USER_PASSWORD;
extern std::string DATABASE_NAME;
extern UINT PORT_NUM;
// CDLGmenjinqingdan 对话框

IMPLEMENT_DYNAMIC(CDLGmenjinqingdan, CDialogEx)

CDLGmenjinqingdan::CDLGmenjinqingdan(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGmenjinqingdan::IDD, pParent)
{

}

CDLGmenjinqingdan::~CDLGmenjinqingdan()
{
}

void CDLGmenjinqingdan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MJQDTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_STATIC_RESUALT5, m_result);
	DDX_Control(pDX, IDC_LIST_MJQD, m_wndList);
	DDX_Control(pDX, IDC_BUTTON_MJQDEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGmenjinqingdan, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_MJQDEXIT, &CDLGmenjinqingdan::OnBnClickedButtonMjqdexit)
END_MESSAGE_MAP()


// CDLGmenjinqingdan 消息处理程序


void CDLGmenjinqingdan::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
	CDialogEx::OnClose();
}


HBRUSH CDLGmenjinqingdan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDLGmenjinqingdan::PreTranslateMessage(MSG* pMsg)
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


BOOL CDLGmenjinqingdan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("姓名"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("卡号"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn (2, _T("楼栋"), LVCFMT_LEFT, 120);
	m_wndList.InsertColumn (3, _T("房间"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (4, _T("刷卡时间"), LVCFMT_LEFT, 275);
	m_wndList.InsertColumn (5, _T("行为"), LVCFMT_LEFT, 50);

	m_font1.CreatePointFont(110, _T("宋体"));
	m_result.SetFont(&m_font1);
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
	m_dlgTitle.SetWindowTextW(_T("门禁清单"));

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


void CDLGmenjinqingdan::OnBnClickedButtonMjqdexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}

void CDLGmenjinqingdan::doSearch()
{
	std::string sQuery;//查询

	sQuery = std::string("SELECT lxc.acs_access_card.PEOPLE_NAME,lxc.acs_access_card.CODE,lxc.acs_access_card.BUILDING_NAME, \
						 lxc.acs_access_card.DOOR_NAME,lxc.acs_access_card.ACCESS_TIME, \
						 lxc.acs_access_card.OWNER_ACTION FROM lxc.acs_access_card order by \
						 lxc.acs_access_card.FID desc limit 100");

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
				int n = m_wndList.GetItemCount();
				m_wndList.InsertItem(n, _T("1"));
				for (int i = 0;i<6;++i)
				{
					sprintf(msg,"%s",sql_row[i]);
					m_wndList.SetItemText (n, i, (CString(msg)));
				}
				numResult++;
			}
		}
		mysql_free_result(result);//释放结果资源
	}
	mysql_close(&sqlCont);

	char smsg[99];
	sprintf_s(smsg,"共 %d 条",numResult);
	m_result.SetWindowTextW(CString(smsg));
	AfxGetMainWnd()->EndWaitCursor();
}