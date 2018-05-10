// DLGrenqunguanai.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGrenqunguanai.h"
#include "afxdialogex.h"
#include "DlgDB_Connect.h"
#include "DataManager.h"
#include "CommunityDlg.h"
#include "DLGSettings.h"
#include "BackgroundDlg.h"
#include <string>
#include <fstream>


extern CDataManager* m_pDM;
extern CDlgDB_Connect* g_DBconnector;

extern std::string IP_SERVER;
extern std::string USER_NAME;
extern std::string USER_PASSWORD;
extern std::string DATABASE_NAME;
extern UINT PORT_NUM;

// CDLGrenqunguanai �Ի���

IMPLEMENT_DYNAMIC(CDLGrenqunguanai, CDialogEx)

int CDLGrenqunguanai::m_SortColumn = -1;
BOOL CDLGrenqunguanai::m_bAsc = FALSE;

int CDLGrenqunguanai::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

CDLGrenqunguanai::CDLGrenqunguanai(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGrenqunguanai::IDD, pParent)
	, m_searchContext(_T(""))
	, m_resultInfo(_T(""))
{
	pQren = NULL;
}

CDLGrenqunguanai::~CDLGrenqunguanai()
{
}

void CDLGrenqunguanai::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SEARCHTYPE, m_combox_searchType);
	DDX_Text(pDX, IDC_EDIT_SEARCHCONTEXT, m_searchContext);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Text(pDX, IDC_STATIC_result, m_resultInfo);
	DDX_Control(pDX, IDC_COMBO_BUILD, m_ComboBox_Build);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_ComboBox_Unit);
	DDX_Control(pDX, IDC_STATIC_RYGATITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_RYGAEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGrenqunguanai, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDLGrenqunguanai::OnBnClickedButtonSearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDLGrenqunguanai::OnNMClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDLGrenqunguanai::OnNMDblclkList1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_OLDER, &CDLGrenqunguanai::OnBnClickedButtonOlder)
	ON_BN_CLICKED(IDC_BUTTON_ALL, &CDLGrenqunguanai::OnBnClickedButtonAll)
	ON_CBN_SELCHANGE(IDC_COMBO_BUILD, &CDLGrenqunguanai::OnCbnSelchangeComboBuild)
	ON_BN_CLICKED(IDC_BUTTON_QRENCODE, &CDLGrenqunguanai::OnBnClickedButtonQrencode)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CDLGrenqunguanai::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CDLGrenqunguanai::OnBnClickedButtonSetting)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CHILDREN, &CDLGrenqunguanai::OnBnClickedButtonChildren)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_RYGAEXIT, &CDLGrenqunguanai::OnBnClickedButtonRygaexit)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CDLGrenqunguanai ��Ϣ�������


BOOL CDLGrenqunguanai::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	minAge = 60;
	maxAge = 7;
	isShowAll = false;
	m_combox_searchType.SetCurSel(0);
	m_ComboBox_Build.SetCurSel(1);
	m_ComboBox_Unit.SetCurSel(0);
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetTextColor(RGB(255,255,255));
	m_font1.CreatePointFont(110, _T("����"));
	GetDlgItem(IDC_STATIC_result)->SetFont(&m_font1);
	m_wndList.SetFont(&m_font1);
	m_wndList.SetRowHeigt(25);
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
	m_dlgTitle.SetWindowTextW(_T("��Ⱥ�ذ�"));

	//SetBackgroundColor(RGB(253,254,255));
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


void CDLGrenqunguanai::OnBnClickedButtonSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int numResult = 0;
	USES_CONVERSION;
	if (!g_DBconnector->bConnect)
	{
		return;
	}
	m_wndList.DeleteAllItems();
	CHeaderCtrl* pHeaderCtrl = m_wndList.GetHeaderCtrl();
	if(pHeaderCtrl)
	{
		int n = pHeaderCtrl->GetItemCount();
		for (int i = n-1;i>=0;--i)
		{
			m_wndList.DeleteColumn(i);
		}
	}
	m_wndList.InsertColumn (0, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("�Ա�"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("¥��"), LVCFMT_LEFT, 120);
	m_wndList.InsertColumn (3, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (4, _T("���֤"), LVCFMT_LEFT, 200);
	m_wndList.InsertColumn (5, _T("�绰"), LVCFMT_LEFT, 110);
	/*m_wndList.InsertColumn (6, _T("�׸�"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (7, _T("����"), LVCFMT_LEFT, 50);*/
	UpdateData(TRUE);
	if (m_searchContext == _T(""))
	{
		return;
	}

	std::string sQuery;//��ѯ
	if (m_combox_searchType.GetCurSel() == 0)//����
	{
		sQuery = "SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
				 lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
				 lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
				 FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
				 where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
				 and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
				 and lxc.acs_resident.PEOPLE_NAME like '%" + \
				 std::string(W2A(m_searchContext)) + "%'";
			     
	}
	else if (m_combox_searchType.GetCurSel() == 1)//���֤
	{
		sQuery = "SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
				  lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
				  lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
				  FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
		          where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
		          and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
			      and lxc.acs_resident.PEOPLE_IDCARD like '%" + \
				  std::string(W2A(m_searchContext)) + "%'";
	}
	CString sBuild,sUnit;
	m_ComboBox_Build.GetLBText(m_ComboBox_Build.GetCurSel(),sBuild);
	m_ComboBox_Unit.GetLBText(m_ComboBox_Unit.GetCurSel(),sUnit);
	std::string sBuildID = std::string(W2A(sBuild)) + std::string(W2A(sUnit)); 
	
	if (sBuild.Compare(_T("����")) != 0)
	{
		std::string sConstraint;
		if (sUnit.Compare(_T("����")) == 0)
		{	
			sConstraint = std::string(" and lxc.acs_house.BUILDING_NAME like '") + std::string(W2A(sBuild)) + std::string("%'");
			sQuery += sConstraint;
		}
		else
		{
			sConstraint = std::string(" and lxc.acs_house.BUILDING_NAME = '") + sBuildID + std::string("'");
			sQuery += sConstraint;
		}	
	}

	MYSQL sqlCont;
	MYSQL_RES *result = NULL;
	MYSQL_ROW sql_row;
	int res;
	//�����������ӷ������ݿ�
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
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
				for (int i = 0;i<6;++i)
				{
					sprintf(msg,"%s",sql_row[i]);
					m_wndList.SetItemText (0, i, COMFUNC::getChineseSex(CString(msg)));
				}
				numResult++;
			}
		}
		mysql_free_result(result);//�ͷŽ����Դ
	} 
	mysql_close(&sqlCont);
	char smsg[99];
	sprintf_s(smsg,"�� %d ��",numResult);
	m_resultInfo = CString(smsg);
	UpdateData(FALSE);
	AfxGetMainWnd()->EndWaitCursor();
}


void CDLGrenqunguanai::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	*pResult = 0;
}

void CDLGrenqunguanai::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	int nItem = m_wndList.GetSelectionMark();
	if (nItem < 0)
	{
		return;
	}
	CString sBuild ,sDoor  ;
	CHeaderCtrl *pHeader = m_wndList.GetHeaderCtrl();  
	int nColumnCount = pHeader->GetItemCount();  
	HDITEM   hdi;  
	TCHAR     lpBuffer[256];   
	hdi.mask   =   HDI_TEXT;  
	hdi.pszText   =   lpBuffer;  
	hdi.cchTextMax   =   256; 
	for (int i = 0;i<nColumnCount;++i)
	{
		pHeader->GetItem( i, &hdi );
		CString strColumnName = hdi.pszText;
		if (strColumnName.Compare(_T("¥��")) ==0 )
		{
			sBuild = m_wndList.GetItemText(nItem, i);
		}
		else if (strColumnName.Compare(_T("����")) ==0)
		{
			sDoor = m_wndList.GetItemText(nItem, i);
		}
	}
	if (sBuild.IsEmpty() || sDoor.IsEmpty())
	{
		return;
	}
	std::string houseID = getHouseID(W2A(sBuild),W2A(sDoor));

	CString mDefLayer = m_pDM->FindDefSHPFile(m_pDM->m_HouseDefLayer);
	if (mDefLayer == _T(""))
	{
		AfxMessageBox(_T("������Ĭ��ʸ��ͼ�㣡"));
		return;
	}

	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(mDefLayer);
	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_pDM->m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' ) + 1);
		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}
	ogrOpt.url() = path;
	m_pDM->features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	m_pDM->features->initialize();
	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = m_pDM->features->createFeatureCursor();
	while(cursor->hasMore())
	{
		//���feature�е���Ϣ
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
		while(parts.hasMore())
		{
			osgEarth::Symbology::Geometry* part = parts.next();
			const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
			for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
			{
				CString sName = CString(i->first.c_str());
				CString sValue = CString(i->second.getString().c_str());

				if ( sName.Compare(_T("���������")) == 0 && sValue.Compare(CString(houseID.c_str())) == 0 )
				{
					std::string sGC = feature->getString("�߳�");
					std::string sDG = feature->getString("�׸�");
					std::string sDG2 = feature->getString("����");
					if (!sGC.empty() && !sDG.empty() && !sDG2.empty())
					{
						double dGC = atof(sGC.data());
						double dDG = atof(sDG.data());
						double dDG2 = atof(sDG2.data());

						if (m_pDM->gTemp != nullptr){
							m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
							m_pDM->gTemp = nullptr;
						}
						m_pDM->MakeFloor3DBorder(feature, dGC+dDG, dGC + dDG2,osg::Vec4(1,1,0,1),false);
						return;
					}
				}
			}
		}
	}
	AfxMessageBox(_T("δ�ҵ�����,����Ĭ��ͼ�㣡"));
	*pResult = 0;
}


std::string CDLGrenqunguanai::getHouseID(std::string sBuild,std::string sDoor)
{
	if (sBuild.find("��Ԫ") == -1)
	{
		return sDoor;
	}
	std::string sD = sBuild.substr(0,2);
	std::string sU = sBuild.substr(sBuild.find("��Ԫ") - 1,1);

	return sD + "-0" + sU + "-" + sDoor; 
}

void CDLGrenqunguanai::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pDM->gTemp != nullptr )
	{
		m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
		m_pDM->gTemp = nullptr;
	}
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


BOOL CDLGrenqunguanai::PreTranslateMessage(MSG* pMsg)
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


void CDLGrenqunguanai::OnBnClickedButtonOlder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int numResult = 0;
	USES_CONVERSION;
	if (!g_DBconnector->bConnect)
	{
		return;
	}
	CTime t = CTime::GetCurrentTime(); //��ȡϵͳ����
	int thisYear =t.GetYear(); //��ȡ���
	m_wndList.DeleteAllItems();
	CHeaderCtrl* pHeaderCtrl = m_wndList.GetHeaderCtrl();
	if(pHeaderCtrl)
	{
		int n = pHeaderCtrl->GetItemCount();
		for (int i = n-1;i>=0;--i)
		{
			m_wndList.DeleteColumn(i);
		}
	}
	m_wndList.InsertColumn (0, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("�Ա�"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("¥��"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn (3, _T("����"), LVCFMT_LEFT, 60);
	m_wndList.InsertColumn (4, _T("���֤"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (5, _T("�绰"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (6, _T("����"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (7, _T("�������"), LVCFMT_LEFT, 80);

	std::string sQuery;//��ѯ
	
	sQuery = "SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
				lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
				lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
				FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
				where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
				and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
				group by lxc.acs_resident.PEOPLE_IDCARD";
	
	CString sBuild,sUnit;
	m_ComboBox_Build.GetLBText(m_ComboBox_Build.GetCurSel(),sBuild);
	m_ComboBox_Unit.GetLBText(m_ComboBox_Unit.GetCurSel(),sUnit);
	std::string sBuildID = std::string(W2A(sBuild)) + std::string(W2A(sUnit)); 

	if (sBuild.Compare(_T("����")) != 0)
	{
		if (sUnit.Compare(_T("����")) == 0)
		{	
			sQuery = std::string("SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
								 lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
								 lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
								 FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
								 where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
								 and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
								 and  lxc.acs_house.BUILDING_NAME like '") + std::string(W2A(sBuild)) + std::string("%'") + \
								 std::string(" group by lxc.acs_resident.PEOPLE_IDCARD");
		}
		else
		{
			sQuery = std::string("SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
								 lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
								 lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
								 FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
								 where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
								 and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
								 and  lxc.acs_house.BUILDING_NAME = '") + sBuildID + std::string("'") + \
								 std::string(" group by lxc.acs_resident.PEOPLE_IDCARD");
		}	
	}

	MYSQL sqlCont;
	MYSQL_RES *result = NULL;
	MYSQL_RES *result1 = NULL;
	MYSQL_ROW sql_row;
	int res;
	//�����������ӷ������ݿ�
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
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
				sprintf(msg,"%s",sql_row[4]);
				std::string sID = std::string(msg);
				if (sID.size()!= 18)
				{
					continue;
				}
				int sYear = atoi( (sID.substr(6,4)).c_str());  
				if (thisYear - sYear < minAge)
				{
					continue;
				}
				m_wndList.InsertItem(0, _T("1"));
				for (int i = 0;i<6;++i)
				{
					sprintf(msg,"%s",sql_row[i]);
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
				sprintf(msg,"%d",thisYear - sYear);
				m_wndList.SetItemText (0, 6, CString(msg));
				//std::string sq = std::string("select count(*) from lxc.acs_access_card where lxc.acs_access_card.CODE = (select ACCESS_CODE FROM lxc.acs_resident where PEOPLE_IDCARD = '") + sID + std::string("');");
				std::string sq = std::string("select count(*) from lxc.acs_access_card where lxc.acs_access_card.people_idcard = '" + sID + std::string("'"));
				res = mysql_query(&sqlCont, sq.c_str());
				if (!res)
				{
					result1=mysql_store_result(&sqlCont);//�����ѯ�������ݵ�result
					CString str;
					str.Format(_T("%03d"),atoi(mysql_fetch_row(result1)[0]));
					m_wndList.SetItemText (0, 7, str);
					mysql_free_result(result1);//�ͷŽ����Դ
				}
				numResult++;
			}
		}
		mysql_free_result(result);//�ͷŽ����Դ
	} 
	mysql_close(&sqlCont);
	char smsg[99];
	sprintf_s(smsg,"�� %d ��",numResult);
	GetDlgItem(IDC_STATIC_result)->SetWindowTextW(CString(smsg));
	AfxGetMainWnd()->EndWaitCursor();
	CRect rc;
	GetDlgItem(IDC_STATIC_result)->GetWindowRect(&rc);      
	ScreenToClient(&rc);        
	InvalidateRect(rc);  
	UpdateWindow();
	if (numResult == 0)
	{
		AfxMessageBox(_T("δ�ҵ�����,����Ĭ��ͼ�㣡"));
	}
}


void CDLGrenqunguanai::OnBnClickedButtonAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (isShowAll)
	{
		if(m_pDM->gTemp!=nullptr)
		{
			m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
			m_pDM->gTemp = nullptr;
		}
		isShowAll = false;
		return;
	}
	USES_CONVERSION;
	CString mDefLayer = m_pDM->FindSHPFile();
	if (mDefLayer == _T(""))
	{
		AfxMessageBox(_T("��ѡ��ʸ��ͼ�㣡"));
		return;
	}
	std::string path = W2A(mDefLayer);
	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_pDM->m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' ) + 1);
		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}
	
	std::vector<std::string> vecHouseID;
	osg::ref_ptr<osg::Group> group = new osg::Group;
	int nItem = m_wndList.GetItemCount();
	CHeaderCtrl *pHeader = m_wndList.GetHeaderCtrl();  
	int nColumnCount = pHeader->GetItemCount(); 
	HDITEM   hdi;  
	TCHAR     lpBuffer[256];   
	hdi.mask   =   HDI_TEXT;  
	hdi.pszText   =   lpBuffer;  
	hdi.cchTextMax   =   256; 
	for (int i = 0;i<nItem ;++i)
	{
		CString sBuild, sDoor;
		
		for (int j = 0;j<nColumnCount;++j)
		{
			pHeader->GetItem( j, &hdi );
			CString strColumnName = hdi.pszText;
			if (strColumnName.Compare(_T("¥��")) ==0 )
			{
				sBuild = m_wndList.GetItemText(i, j);
			}
			else if (strColumnName.Compare(_T("����")) ==0)
			{
				sDoor = m_wndList.GetItemText(i, j);
			}
		}
		if (sBuild.IsEmpty() || sDoor.IsEmpty())
		{
			continue;
		}
		vecHouseID.push_back(getHouseID(W2A(sBuild),W2A(sDoor))) ;
	}
		
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;	
	ogrOpt.url() = path;
	m_pDM->features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	m_pDM->features->initialize();
	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = m_pDM->features->createFeatureCursor();
	while(cursor->hasMore())
	{
		//���feature�е���Ϣ
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
		while(parts.hasMore())
		{
			osgEarth::Symbology::Geometry* part = parts.next();
			const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
			for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
			{
				CString sName = CString(i->first.c_str());
				CString sValue = CString(i->second.getString().c_str());

				if ( sName.Compare(_T("���������")) == 0 && COMFUNC::isStringInVector(std::string(W2A(sValue)),vecHouseID) )
				{
					std::string sGC = feature->getString("�߳�");
					std::string sDG = feature->getString("�׸�");
					std::string sDG2 = feature->getString("����");
					if (!sGC.empty() && !sDG.empty() && !sDG2.empty())
					{
						double dGC = atof(sGC.data());
						double dDG = atof(sDG.data());
						double dDG2 = atof(sDG2.data());
	
						m_pDM->MakeFloor3DBorderAll(feature, dGC+dDG, dGC + dDG2,group);
						break;
					}
				}
			}
		}
	}
	if(m_pDM->gTemp!=nullptr)
	{
		m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
		m_pDM->gTemp = nullptr;
	}
	m_pDM->gTemp = new osg::Group();
	m_pDM->gTemp->addChild(group);
	m_pDM->gTemp->setName("�߿�");
	m_pDM->m_spViewer3D->AddNode(m_pDM->gTemp);
	isShowAll = true;
}


void CDLGrenqunguanai::OnCbnSelchangeComboBuild()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	int num = m_ComboBox_Unit.GetCount();
	for (int i = 1;i<num;++i)
	{
		m_ComboBox_Unit.DeleteString(i);
	}
	
	CString s;
	m_ComboBox_Build.GetLBText(m_ComboBox_Build.GetCurSel(),s);
	std::string str(W2A(s));
	if (str == "����")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "01��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "02��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "03��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "04��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.AddString(_T("2��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "05��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.AddString(_T("2��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "06��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "07��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.AddString(_T("2��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "08��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "09��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.AddString(_T("2��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
	else if (str == "10��")
	{
		//m_ComboBox_Unit.AddString(_T("����"));
		m_ComboBox_Unit.AddString(_T("1��Ԫ"));
		m_ComboBox_Unit.AddString(_T("2��Ԫ"));
		m_ComboBox_Unit.SetCurSel(0);
	}
}


void CDLGrenqunguanai::OnBnClickedButtonQrencode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	std::string msg;
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("δѡ�м�¼��"));
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_wndList.GetNextSelectedItem(pos);
			msg += std::string(W2A(m_wndList.GetItemText(nItem, 0))) + std::string(",") +
				std::string(W2A(m_wndList.GetItemText(nItem, 1))) + std::string(",") +
				std::string(W2A(m_wndList.GetItemText(nItem, 2))) + std::string(",") +
				std::string(W2A(m_wndList.GetItemText(nItem, 3))) + std::string(",") +
				std::string(W2A(m_wndList.GetItemText(nItem, 5))) + std::string(";\r\n");
		}
	}
	if (!msg.empty())
	{
		DeleteFile(_T("Data\\DRVdata\\listInfo.png"));

		std::string slist = std::string("-o Data\\DRVdata\\listInfo.png ") +  msg;
		BeginWaitCursor();
		ShellExecute(this->m_hWnd, _T("open"), _T("qrencode.exe"), CString(slist.c_str()), _T(""), SW_HIDE);
		EndWaitCursor();
		while( !PathFileExists(_T("Data\\DRVdata\\listInfo.png")) );
		if (pQren != nullptr)
		{
			pQren->DestroyWindow();
		}
		pQren = new CDLGQrencode();
		pQren->Create(IDD_DIALOG_QRENCODEINFO);
		pQren->ShowWindow(SW_SHOW);
	}
}


void CDLGrenqunguanai::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
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


void CDLGrenqunguanai::OnBnClickedButtonSetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	CDLGSettings* dlg = new CDLGSettings();
	dlg->parent = (CObject*)this;
	
	if (dlg->DoModal() == IDOK)
	{
		minAge = atoi(std::string(W2A(dlg->m_old)).c_str());
		maxAge = atoi(std::string(W2A(dlg->m_child)).c_str());
	}
}

HBRUSH CDLGrenqunguanai::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//if (pWnd->GetDlgCtrlID() == IDC_STATIC_UNIT0 || pWnd->GetDlgCtrlID() == IDC_STATIC_BUILD0 ||
	//	pWnd->GetDlgCtrlID() == IDC_STATIC_RYGATITLE || pWnd->GetDlgCtrlID() == IDC_STATIC_result)
	//{
	//	//pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetBkColor(RGB(253,254,255));
	//	pDC->SetTextColor(RGB(200,200,200));
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return m_brBk;
	return hbr;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
}


void CDLGrenqunguanai::OnBnClickedButtonChildren()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int numResult = 0;
	USES_CONVERSION;
	if (!g_DBconnector->bConnect)
	{
		return;
	}
	CTime t = CTime::GetCurrentTime(); //��ȡϵͳ����
	int thisYear =t.GetYear(); //��ȡ���
	m_wndList.DeleteAllItems();
	CHeaderCtrl* pHeaderCtrl = m_wndList.GetHeaderCtrl();
	if(pHeaderCtrl)
	{
		int n = pHeaderCtrl->GetItemCount();
		for (int i = n-1;i>=0;--i)
		{
			m_wndList.DeleteColumn(i);
		}
	}
	m_wndList.InsertColumn (0, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("�Ա�"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("¥��"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (3, _T("����"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (4, _T("���֤"), LVCFMT_LEFT, 130);
	m_wndList.InsertColumn (5, _T("�绰"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn (6, _T("����"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (7, _T("�������"), LVCFMT_LEFT, 80);

	std::string sQuery;//��ѯ

	sQuery = "SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
			 lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
			 lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
			 FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
			 where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
			 and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
			 group by lxc.acs_resident.PEOPLE_IDCARD";

	CString sBuild,sUnit;
	m_ComboBox_Build.GetLBText(m_ComboBox_Build.GetCurSel(),sBuild);
	m_ComboBox_Unit.GetLBText(m_ComboBox_Unit.GetCurSel(),sUnit);
	std::string sBuildID = std::string(W2A(sBuild)) + std::string(W2A(sUnit)); 

	if (sBuild.Compare(_T("����")) != 0)
	{
		if (sUnit.Compare(_T("����")) == 0)
		{	
			sQuery = std::string("SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
								 lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
								 lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
								 FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
								 where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
								 and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
								 and  lxc.acs_house.BUILDING_NAME like '") + std::string(W2A(sBuild)) + std::string("%'") + \
								 std::string(" group by lxc.acs_resident.PEOPLE_IDCARD");
		}
		else
		{
			sQuery = std::string("SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
								 lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
								 lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
								 FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
								 where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
								 and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
								 and  lxc.acs_house.BUILDING_NAME = '") + sBuildID + std::string("'") + \
								 std::string(" group by lxc.acs_resident.PEOPLE_IDCARD");
		}	
	}

	MYSQL sqlCont;
	MYSQL_RES *result = NULL;
	MYSQL_RES *result1 = NULL;
	MYSQL_ROW sql_row;
	int res;
	//�����������ӷ������ݿ�
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
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
				sprintf(msg,"%s",sql_row[4]);
				std::string sID = std::string(msg);
				if (sID.size()!= 18)
				{
					continue;
				}
				int sYear = atoi( (sID.substr(6,4)).c_str());  
				if (thisYear - sYear > maxAge)
				{
					continue;
				}
				m_wndList.InsertItem(0, _T("1"));
				for (int i = 0;i<6;++i)
				{
					sprintf(msg,"%s",sql_row[i]);
					m_wndList.SetItemText (0, i, COMFUNC::getChineseSex(CString(msg)));
				}
				sprintf(msg,"%d",thisYear - sYear);
				m_wndList.SetItemText (0, 6, CString(msg));
				//std::string sq = std::string("select count(*) from lxc.acs_access_card where lxc.acs_access_card.CODE = (select ACCESS_CODE FROM lxc.acs_resident where PEOPLE_IDCARD = '") + sID + std::string("');");
				std::string sq = std::string("select count(*) from lxc.acs_access_card where lxc.acs_access_card.people_idcard = '" + sID + std::string("'"));
				res = mysql_query(&sqlCont, sq.c_str());
				if (!res)
				{
					result1=mysql_store_result(&sqlCont);//�����ѯ�������ݵ�result
					CString str;
					str.Format(_T("%03d"),atoi(mysql_fetch_row(result1)[0]));
					m_wndList.SetItemText (0, 7, str);
					mysql_free_result(result1);//�ͷŽ����Դ
				}
				numResult++;
			}
		}
		mysql_free_result(result);//�ͷŽ����Դ
	} 
	mysql_close(&sqlCont);
	char smsg[99];
	sprintf_s(smsg,"�� %d ��",numResult);
	m_resultInfo = CString(smsg);
	UpdateData(FALSE);
	AfxGetMainWnd()->EndWaitCursor();
}


BOOL CDLGrenqunguanai::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	
	return CDialogEx::OnEraseBkgnd(pDC); 
	 
}


void CDLGrenqunguanai::OnBnClickedButtonRygaexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}


void CDLGrenqunguanai::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDCtl==IDC_BUTTON_OLDER || nIDCtl==IDC_BUTTON_SETTING || nIDCtl==IDC_BUTTON_QRENCODE || nIDCtl==IDC_BUTTON_ALL)         //checking for the button 
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
