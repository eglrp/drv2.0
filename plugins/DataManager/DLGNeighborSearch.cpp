// DLGNeighborSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGNeighborSearch.h"
#include "afxdialogex.h"
#include "DlgDB_Connect.h"
#include "BackgroundDlg.h"
//extern CDataManager* m_pDM;
extern CDlgDB_Connect* g_DBconnector;
extern CDataManager* m_pDM;

extern std::string IP_SERVER;
extern std::string USER_NAME;
extern std::string USER_PASSWORD;
extern std::string DATABASE_NAME;
extern UINT PORT_NUM;
// CDLGNeighborSearch 对话框

IMPLEMENT_DYNAMIC(CDLGNeighborSearch, CDialogEx)

int CDLGNeighborSearch::m_SortColumn = -1;
BOOL CDLGNeighborSearch::m_bAsc = FALSE;

int CDLGNeighborSearch::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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
	/*CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CString    strItem1 = pListCtrl->GetItemText(lParam1,m_SortColumn);
	CString    strItem2 = pListCtrl->GetItemText(lParam2,m_SortColumn);

	if (m_bAsc)
	{
	return strItem1.Compare(strItem2);
	}
	else
	{
	return strItem2.Compare(strItem1);

	}*/
}

std::string getHouseID(std::string sBuild,std::string sDoor)
{
	if (sBuild.find("栋") == -1)
	{
		return sDoor;
	}
	int i = sBuild.find("栋");
	std::string sD = sBuild.substr(0,i);
	if (i == 1)
	{
		sD = std::string("0") + sD;
	}
	std::string sU = sBuild.substr(sBuild.find("单元") - 1,1);

	return sD + "-0" + sU + "-" + sDoor; 
}

CDLGNeighborSearch::CDLGNeighborSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGNeighborSearch::IDD, pParent)
	, m_edit_floor1(_T(""))
	, m_resultInfo(_T(""))
{
}

CDLGNeighborSearch::~CDLGNeighborSearch()
{
}

void CDLGNeighborSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_BUILD1, m_Combo_build1);
	DDX_Control(pDX, IDC_COMBO_UNIT1, m_Combo_unit1);
	DDX_Text(pDX, IDC_EDIT_FLOOR1, m_edit_floor1);
	DDX_Text(pDX, IDC_STATIC_INFO1, m_resultInfo);
	DDX_Control(pDX, IDC_LIST_NEIGHBOR1, m_wndList);
	DDX_Control(pDX, IDC_STATIC_NEIGHBORTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_NEIGHBOREXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGNeighborSearch, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_BUILD1, &CDLGNeighborSearch::OnCbnSelchangeComboBuild1)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHNEIGHBOR1, &CDLGNeighborSearch::OnBnClickedButtonSearchneighbor1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_NEIGHBOR1, &CDLGNeighborSearch::OnLvnColumnclickList1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_NEIGHBOREXIT, &CDLGNeighborSearch::OnBnClickedButtonNeighborexit)
	ON_BN_CLICKED(IDC_BUTTON2_SHOWALL, &CDLGNeighborSearch::OnBnClickedButton2Showall)
	ON_WM_CLOSE()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CDLGNeighborSearch 消息处理程序


BOOL CDLGNeighborSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	isShowAll = false;
	m_Combo_unit1.SetCurSel(0);
	m_Combo_build1.SetCurSel(0);
	m_edit_floor1 = _T("0101");
	UpdateData(FALSE);

	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetTextColor(RGB(255,255,255));
	m_font1.CreatePointFont(110, _T("宋体"));
	GetDlgItem(IDC_STATIC_INFO1)->SetFont(&m_font1);
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
		_T("黑体"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("邻居查询"));

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


void CDLGNeighborSearch::OnCbnSelchangeComboBuild1()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	int num = m_Combo_unit1.GetCount();
	for (int i = 1;i<num;++i)
	{
		m_Combo_unit1.DeleteString(i);
	}

	CString s;
	m_Combo_build1.GetLBText(m_Combo_build1.GetCurSel(),s);
	std::string str(W2A(s));
	if (str == "01栋")
	{
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "02栋")
	{
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "03栋")
	{
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "04栋")
	{
		m_Combo_unit1.AddString(_T("2单元"));
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "05栋")
	{
		m_Combo_unit1.AddString(_T("2单元"));
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "06栋")
	{
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "07栋")
	{
		m_Combo_unit1.AddString(_T("2单元"));
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "08栋")
	{
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "09栋")
	{
		m_Combo_unit1.AddString(_T("2单元"));
		m_Combo_unit1.SetCurSel(0);
	}
	else if (str == "10栋")
	{
		m_Combo_unit1.AddString(_T("2单元"));
		m_Combo_unit1.SetCurSel(0);
	}
}


void CDLGNeighborSearch::OnBnClickedButtonSearchneighbor1()
{
	// TODO: 在此添加控件通知处理程序代码
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
	m_wndList.InsertColumn (0, _T("姓名"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("性别"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("楼栋"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn (3, _T("房间"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (4, _T("身份证"), LVCFMT_LEFT, 200);
	m_wndList.InsertColumn (5, _T("电话"), LVCFMT_LEFT, 110);
	/*m_wndList.InsertColumn (6, _T("底高"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (7, _T("顶高"), LVCFMT_LEFT, 50);*/
	UpdateData(TRUE);


	int houseID = atoi(std::string(W2A(m_edit_floor1)).c_str());
	CString s;
	char msg[20];
	sprintf_s(msg,"%04d",houseID);
	std::string sHouse1(msg);
	sprintf_s(msg,"%04d",houseID - 1);
	std::string sHouse2(msg);
	sprintf_s(msg,"%04d",houseID + 1);
	std::string sHouse3(msg);
	sprintf_s(msg,"%04d",houseID - 100);
	std::string sHouse4(msg);
	sprintf_s(msg,"%04d",houseID + 100);
	std::string sHouse5(msg);

	CString sBuild,sUnit;
	m_Combo_build1.GetLBText(m_Combo_build1.GetCurSel(),sBuild);
	m_Combo_unit1.GetLBText(m_Combo_unit1.GetCurSel(),sUnit);
	std::string sBuildID = std::string(W2A(sBuild)) + std::string(W2A(sUnit)); 

	std::string sQuery = " \
		SELECT lxc.acs_resident.PEOPLE_NAME,lxc.acs_resident.PEOPLE_SEX, \
		lxc.acs_house.BUILDING_NAME,lxc.acs_house.DOOR_NAME, \
		lxc.acs_resident.PEOPLE_IDCARD,lxc.acs_resident.PEOPLE_PHONE \
		FROM lxc.acs_resident_house_rel,lxc.acs_resident,lxc.acs_house \
		where lxc.acs_resident_house_rel.RESIDENT_ID=lxc.acs_resident.ID \
	    and lxc.acs_house.id=lxc.acs_resident_house_rel.HOUSE_ID \
		and lxc.acs_house.BUILDING_NAME = '" + sBuildID +"' ";
	sQuery += 
		"and (lxc.acs_house.DOOR_NAME = '" + sHouse1 + "' or lxc.acs_house.DOOR_NAME = '" + \
		 sHouse2 + "' or lxc.acs_house.DOOR_NAME = '" + sHouse3 + "' or lxc.acs_house.DOOR_NAME = '" + \
		 sHouse4 + "' or lxc.acs_house.DOOR_NAME = '" + sHouse5 + "') group by lxc.acs_resident.PEOPLE_IDCARD";

	MYSQL sqlCont;
	MYSQL_RES *result = NULL;
	MYSQL_ROW sql_row;
	int res;
	//单独建立连接访问数据库
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
		AfxMessageBox(_T("服务器异常！"));
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
				for (int i = 0;i<6;++i)
				{
					sprintf(msg,"%s",sql_row[i]);
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
	char smsg[99];
	sprintf_s(smsg,"共 %d 条",numResult);
	GetDlgItem(IDC_STATIC_INFO1)->SetWindowTextW(CString(smsg));
	AfxGetMainWnd()->EndWaitCursor();
	CRect rc;
	GetDlgItem(IDC_STATIC_INFO1)->GetWindowRect(&rc);      
	ScreenToClient(&rc);        
	InvalidateRect(rc);  
}


void CDLGNeighborSearch::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
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


HBRUSH CDLGNeighborSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDLGNeighborSearch::OnBnClickedButtonNeighborexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


void CDLGNeighborSearch::OnBnClickedButton2Showall()
{
	// TODO: 在此添加控件通知处理程序代码
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
	CString mDefLayer = m_pDM->FindDefSHPFile(m_pDM->m_HouseDefLayer);
	if (mDefLayer == _T(""))
	{
		AfxMessageBox(_T("请选择矢量图层！"));
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
			if (strColumnName.Compare(_T("楼栋")) ==0 )
			{
				sBuild = m_wndList.GetItemText(i, j);
			}
			else if (strColumnName.Compare(_T("房间")) ==0)
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
		//输出feature中的信息
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

				if ( sName.Compare(_T("完整房间号")) == 0 && COMFUNC::isStringInVector(std::string(W2A(sValue)),vecHouseID) )
				{
					std::string sGC = feature->getString("高程");
					std::string sDG = feature->getString("底高");
					std::string sDG2 = feature->getString("顶高");
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
	if (group->getNumChildren() > 0)
	{
		m_pDM->gTemp->addChild(group);
		m_pDM->gTemp->setName("线框");
		m_pDM->m_spViewer3D->AddNode(m_pDM->gTemp);
		isShowAll = true;
	}
	else
	{
		AfxMessageBox(_T("未找到数据，请检查图层！"));
	}
}


BOOL CDLGNeighborSearch::PreTranslateMessage(MSG* pMsg)
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


void CDLGNeighborSearch::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	if(m_pDM->gTemp!=nullptr)
	{
		m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
		m_pDM->gTemp = nullptr;
	}
	DestroyWindow();
}


void CDLGNeighborSearch::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDCtl==IDC_BUTTON_SEARCHNEIGHBOR1 || nIDCtl==IDC_BUTTON2_SHOWALL)         //checking for the button 
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
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
