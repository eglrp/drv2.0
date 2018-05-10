// DLGzuzhubiangeng.cpp : ʵ���ļ�
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

// CDLGzuzhubiangeng �Ի���

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


// CDLGzuzhubiangeng ��Ϣ�������


BOOL CDLGzuzhubiangeng::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("�Ա�"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("¥��"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (3, _T("����"), LVCFMT_LEFT, 60);
	m_wndList.InsertColumn (4, _T("���֤"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (5, _T("�绰"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (6, _T("��סʱ��"), LVCFMT_LEFT, 100);
	m_font1.CreatePointFont(110, _T("����"));
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
		_T("����"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("��ס���"));

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

void CDLGzuzhubiangeng::doSearch()
{
	std::string sQuery;//��ѯ
	
	sQuery = "SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
				lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
				lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE,lxc.acs_resident.LIVE_TYPE \
				FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
				where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
				and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
				and (lxc.acs_resident.LIVE_TYPE = 'һ����' \
				or lxc.acs_resident.LIVE_TYPE = '������' \
				or lxc.acs_resident.LIVE_TYPE = '������') \
				group by lxc.acs_resident.PEOPLE_IDCARD";

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
				m_wndList.InsertItem(0, _T("1"));
				for (int i = 0;i<7;++i)
				{
					sprintf(msg,"%s",sql_row[i]);
					//�������֤
					//�������֤
					if (i == 4)
					{
						for(int j = 3;j<=14;++j)
						{
							msg[j] = '*';
						}
					}
					//���ص绰
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
		mysql_free_result(result);//�ͷŽ����Դ
	} 
	mysql_close(&sqlCont);
	AfxGetMainWnd()->EndWaitCursor();
	CString s;
	s.Format(_T("�� %d ��"),numResult);
	((CStatic*)GetDlgItem(IDC_STATIC_RESULT2))->SetWindowTextW(s);
	CRect rc;
	GetDlgItem(IDC_STATIC_RESULT2)->GetWindowRect(&rc);      
	ScreenToClient(&rc);        
	InvalidateRect(rc);
}

void CDLGzuzhubiangeng::OnLvnColumnclickListZzbg(NMHDR *pNMHDR, LRESULT *pResult)
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


HBRUSH CDLGzuzhubiangeng::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDLGzuzhubiangeng::PreTranslateMessage(MSG* pMsg)
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


void CDLGzuzhubiangeng::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


void CDLGzuzhubiangeng::OnBnClickedButtonZzbgexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}
