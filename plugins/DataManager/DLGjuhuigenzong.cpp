// DLGjuhuigenzong.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGjuhuigenzong.h"
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
// CDLGjuhuigenzong �Ի���

IMPLEMENT_DYNAMIC(CDLGjuhuigenzong, CDialogEx)

int CDLGjuhuigenzong::m_SortColumn = -1;
BOOL CDLGjuhuigenzong::m_bAsc = FALSE;

int CDLGjuhuigenzong::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

CDLGjuhuigenzong::CDLGjuhuigenzong(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGjuhuigenzong::IDD, pParent)
{

}

CDLGjuhuigenzong::~CDLGjuhuigenzong()
{
}

void CDLGjuhuigenzong::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_JHGZ, m_wndList);
	DDX_Control(pDX, IDC_STATIC_JHGZTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_JHGZEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGjuhuigenzong, CDialogEx)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_JHGZ, &CDLGjuhuigenzong::OnLvnColumnclickListJhgz)
	ON_BN_CLICKED(IDC_BUTTON_JHGZEXIT, &CDLGjuhuigenzong::OnBnClickedButtonJhgzexit)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDLGjuhuigenzong ��Ϣ�������


BOOL CDLGjuhuigenzong::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("�����"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (1, _T("�������"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (2, _T("��ѯʱ��"), LVCFMT_LEFT, 150);
	m_font1.CreatePointFont(110, _T("����"));
	GetDlgItem(IDC_STATIC_RESULT1)->SetFont(&m_font1);
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
	m_dlgTitle.SetWindowTextW(_T("�ۻ����"));

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

void CDLGjuhuigenzong::doSearch()
{
	std::vector<int> vTimes;
	std::vector<std::string> vsHouse;
	int nMax = 4;
	std::string sdate = "17-8��";
	std::string sQuery;//��ѯ

	sQuery = std::string("SELECT count(*) ,CONCAT(BUILDING_NAME,DOOR_NAME) as address \
						 FROM lxc.acs_access_card  where lxc.acs_access_card.ACCESS_TIME like '%") + sdate + \
						 "%'   group by PEOPLE_NAME  ;";

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
				sprintf(msg,"%s",sql_row[0]);
				vTimes.push_back(atoi(msg));
				sprintf(msg,"%s",sql_row[1]);
				vsHouse.push_back(std::string(msg));
			}
		}
		mysql_free_result(result);//�ͷŽ����Դ
	}
	mysql_close(&sqlCont);
	std::vector<COMFUNC::PAIR1> vP;
	for (int i = 0;i<vsHouse.size();++i)
	{
		bool isFind = false;
		for (int j = 0;j<vP.size();++j)
		{
			if (vP[j].str == vsHouse[i])
			{
				isFind = true;
				vP[j].n += vTimes[i];
				break;
			}
		}
		if (!isFind)
		{
			COMFUNC::PAIR1 p;
			p.n = vTimes[i];
			p.str = vsHouse[i];
			vP.push_back(p);
		}
	}
	
	for (int i = 0;i<vP.size();++i)
	{
		if (vP[i].n < nMax)
		{
			continue;
		}
		char msg[9];
		sprintf(msg,"%d",vP[i].n);
		m_wndList.InsertItem(0, _T("1"));
		
		m_wndList.SetItemText (0, 0, CString(vP[i].str.c_str()) );
		m_wndList.SetItemText (0, 1, CString(msg) );
		m_wndList.SetItemText (0, 2, CString(sdate.c_str()) );
		numResult++;
	}
	char smsg[99];
	sprintf_s(smsg,"�� %d ��",numResult);
	GetDlgItem(IDC_STATIC_RESULT1)->SetWindowTextW(CString(smsg));
	AfxGetMainWnd()->EndWaitCursor();

}

void CDLGjuhuigenzong::OnLvnColumnclickListJhgz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_SortColumn = pNMListView->iSubItem;
	m_bAsc=!m_bAsc;//�����ǽ���

	m_wndList.SortItems(MyCompareProc, (LPARAM) &m_wndList);

	int count = m_wndList.GetItemCount();   //����
	for (int i=0; i<count;  i++)  
	{  
		m_wndList.SetItemData(i,i);  
	}
	*pResult = 0;
}


void CDLGjuhuigenzong::OnBnClickedButtonJhgzexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}


BOOL CDLGjuhuigenzong::PreTranslateMessage(MSG* pMsg)
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


void CDLGjuhuigenzong::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


HBRUSH CDLGjuhuigenzong::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
