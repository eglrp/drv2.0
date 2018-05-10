// DLGmenjinqingdan.cpp : ʵ���ļ�
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
// CDLGmenjinqingdan �Ի���

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


// CDLGmenjinqingdan ��Ϣ�������


void CDLGmenjinqingdan::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return m_brBk;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CDLGmenjinqingdan::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc�����¼� 
			OnClose();
			return true;  
		case VK_RETURN: //Enter�����¼� 
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("����"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn (2, _T("¥��"), LVCFMT_LEFT, 120);
	m_wndList.InsertColumn (3, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (4, _T("ˢ��ʱ��"), LVCFMT_LEFT, 275);
	m_wndList.InsertColumn (5, _T("��Ϊ"), LVCFMT_LEFT, 50);

	m_font1.CreatePointFont(110, _T("����"));
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
		_T("����"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("�Ž��嵥"));

	m_brBk.CreateSolidBrush(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // ͼƬȫ·��  
		IMAGE_BITMAP,                          // ͼƬ��ʽ  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // ע��LR_LOADFROMFILE  
	m_EXIT.SetBitmap(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDLGmenjinqingdan::OnBnClickedButtonMjqdexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}

void CDLGmenjinqingdan::doSearch()
{
	std::string sQuery;//��ѯ

	sQuery = std::string("SELECT lxc.acs_access_card.PEOPLE_NAME,lxc.acs_access_card.CODE,lxc.acs_access_card.BUILDING_NAME, \
						 lxc.acs_access_card.DOOR_NAME,lxc.acs_access_card.ACCESS_TIME, \
						 lxc.acs_access_card.OWNER_ACTION FROM lxc.acs_access_card order by \
						 lxc.acs_access_card.FID desc limit 100");

	MYSQL sqlCont;
	MYSQL_RES *result = NULL;
	MYSQL_ROW sql_row;
	int res;
	int numResult = 0;
	//�����������ӷ������ݿ�
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
		AfxMessageBox(_T("����������ʧ�ܣ�������"));
		return;
	}
	AfxGetMainWnd()->BeginWaitCursor();
	mysql_query(&sqlCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
	res = mysql_query(&sqlCont, sQuery.c_str());
	if(!res)
	{
		result=mysql_store_result(&sqlCont);//�����ѯ�������ݵ�result
		if(result)
		{
			while(sql_row=mysql_fetch_row(result))//��ȡ���������
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
		mysql_free_result(result);//�ͷŽ����Դ
	}
	mysql_close(&sqlCont);

	char smsg[99];
	sprintf_s(smsg,"�� %d ��",numResult);
	m_result.SetWindowTextW(CString(smsg));
	AfxGetMainWnd()->EndWaitCursor();
}