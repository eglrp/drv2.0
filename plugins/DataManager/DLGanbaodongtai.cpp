// DLGanbaodongtai.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGanbaodongtai.h"
#include "afxdialogex.h"
#include "BackgroundDlg.h"
extern std::vector<PERSON_LOCATION> g_vPersonLoc;
// CDLGanbaodongtai 对话框

IMPLEMENT_DYNAMIC(CDLGanbaodongtai, CDialogEx)

CDLGanbaodongtai::CDLGanbaodongtai(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGanbaodongtai::IDD, pParent)
	, m_info(_T(""))
{

}

CDLGanbaodongtai::~CDLGanbaodongtai()
{
}

void CDLGanbaodongtai::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ABDT, m_wndList);
	DDX_Control(pDX, IDC_STATIC_ABDTTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_ABDTEXIT, m_EXIT);
	DDX_Text(pDX, IDC_STATIC_RESUALT3, m_info);
}


BEGIN_MESSAGE_MAP(CDLGanbaodongtai, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ABDTEXIT, &CDLGanbaodongtai::OnBnClickedButtonAbdtexit)
END_MESSAGE_MAP()


// CDLGanbaodongtai 消息处理程序


BOOL CDLGanbaodongtai::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("安保人员姓名"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (1, _T("位置"), LVCFMT_LEFT, 250);
	m_font1.CreatePointFont(110, _T("宋体"));
	GetDlgItem(IDC_STATIC_RESUALT3)->SetFont(&m_font1);
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
	m_dlgTitle.SetWindowTextW(_T("安保动态"));
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

void CDLGanbaodongtai::doSearch()
{
	if (g_vPersonLoc.empty())
	{
		AfxMessageBox(_T("未获得数据，请稍后再试！"));
		return;
	}
	for (int i = 0;i<g_vPersonLoc.size();++i)
	{
		m_wndList.InsertItem(0, _T("1"));
		m_wndList.SetItemText (0, 0, CString(g_vPersonLoc[i].name.c_str()));
		char msg[99];
		sprintf(msg,"%.2f,%.2f",g_vPersonLoc[i].x,g_vPersonLoc[i].y);
		m_wndList.SetItemText (0, 1, CString(msg));
	}
	char smsg[99];
	sprintf(smsg,"共 %d 条",g_vPersonLoc.size());
	GetDlgItem(IDC_STATIC_RESUALT3)->SetWindowText(CString(smsg));
}

void CDLGanbaodongtai::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


BOOL CDLGanbaodongtai::PreTranslateMessage(MSG* pMsg)
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


HBRUSH CDLGanbaodongtai::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDLGanbaodongtai::OnBnClickedButtonAbdtexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}
