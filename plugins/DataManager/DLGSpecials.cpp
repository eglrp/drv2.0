// DLGSpecials.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGSpecials.h"
#include "afxdialogex.h"
#include "DlgDB_Connect.h"
#include "BackgroundDlg.h"

extern CDataManager* m_pDM;

// CDLGSpecials 对话框

IMPLEMENT_DYNAMIC(CDLGSpecials, CDialogEx)

int CDLGSpecials::m_SortColumn = -1;
BOOL CDLGSpecials::m_bAsc = FALSE;

int CDLGSpecials::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

CDLGSpecials::CDLGSpecials(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGSpecials::IDD, pParent)
	, m_Info(_T("共 0 条"))
{

}

CDLGSpecials::~CDLGSpecials()
{
}

void CDLGSpecials::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SPECIALTYPE, m_combox_specialType);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Text(pDX, IDC_STATIC_INFO, m_Info);
	DDX_Control(pDX, IDC_STATIC_TSRQTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_TSRQEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGSpecials, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_1, &CDLGSpecials::OnBnClickedButtonSearch1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDLGSpecials::OnNMDblclkList1)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CDLGSpecials::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON_TSRQEXIT, &CDLGSpecials::OnBnClickedButtonTsrqexit)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CDLGSpecials 消息处理程序


BOOL CDLGSpecials::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_combox_specialType.SetCurSel(0);
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("姓名"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("性别"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("楼栋"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (3, _T("房间"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (4, _T("身份证"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (5, _T("电话"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (6, _T("备注"), LVCFMT_LEFT, 130);
	m_font1.CreatePointFont(110, _T("宋体"));
	GetDlgItem(IDC_STATIC_INFO)->SetFont(&m_font1);
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
	m_dlgTitle.SetWindowTextW(_T("特殊人群"));

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


void CDLGSpecials::OnBnClickedButtonSearch1()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CString sType;
	m_combox_specialType.GetLBText(m_combox_specialType.GetCurSel(),sType);
	std::string typeStr(W2A(sType));

	CString sDefLayer = m_pDM->FindDefSHPFile(m_pDM->m_HouseDefLayer);
	if (sDefLayer.IsEmpty())
	{
		AfxMessageBox(_T("请设置默认矢量图层！"));
		return;
	}

	m_wndList.DeleteAllItems();
	int num = 0;
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(sDefLayer);
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
		//输出feature中的信息
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		std::string s = feature->getString("特殊人群");

		if ((s == typeStr && typeStr != "所有") || (!s.empty() && typeStr == "所有"))
		{
			m_wndList.InsertItem(0, _T("1"));
			
			m_wndList.SetItemText (0, 0, CString((feature->getString("姓名")).c_str()));
			m_wndList.SetItemText (0, 1, CString((feature->getString("性别")).c_str()));
			m_wndList.SetItemText (0, 2, CString((feature->getString("单元")).c_str()));
			m_wndList.SetItemText (0, 3, CString((feature->getString("房间号")).c_str()));
			//隐藏身份证
			std::string sID = feature->getString("身份证号");
			if (sID.size() != 18)
			{
				sID = " ";
			}
			else
			{
				for (int j = 3;j<=14;++j)
				{
					sID[j] = '*';
				}
			}
			m_wndList.SetItemText (0, 4, CString(sID.c_str()));
			std::string s1 = feature->getString("手机号码");
			if (s1.size() == 11)
			{
				for (int i = 4;i<=7;++i)
				{
					s1[i] = '*';
				}
			}
			m_wndList.SetItemText (0, 5, CString(s1.c_str()));
			m_wndList.SetItemText (0, 6, CString(s.c_str()));
			num++;
		}
	}
	m_Info.Format(_T("共 %d 条"),num);
	//UpdateData(FALSE);
	CStatic* cs = (CStatic*)GetDlgItem(IDC_STATIC_INFO);
	cs->SetWindowTextW(m_Info);
	CRect rc;
	GetDlgItem(IDC_STATIC_INFO)->GetWindowRect(&rc);      
	ScreenToClient(&rc);        
	InvalidateRect(rc);
	if (num == 0)
	{
		AfxMessageBox(_T("未找到数据,请检查默认图层！"));
		return;
	}
}


void CDLGSpecials::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	int nItem = m_wndList.GetSelectionMark();
	if (nItem < 0)
	{
		return;
	}
	CString sIDCard;
	
	sIDCard = m_wndList.GetItemText(nItem, 4);
	CString mDefLayer = m_pDM->FindSHPFile();
	if (mDefLayer == _T(""))
	{
		AfxMessageBox(_T("请设置默认矢量图层！"));
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
		//输出feature中的信息
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		if (feature->getString("身份证号") == std::string(W2A(sIDCard)))
		{
			std::string sGC = feature->getString("高程");
			std::string sDG = feature->getString("底高");
			std::string sDG2 = feature->getString("顶高");
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
	*pResult = 0;
}


BOOL CDLGSpecials::PreTranslateMessage(MSG* pMsg)
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


void CDLGSpecials::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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


HBRUSH CDLGSpecials::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDLGSpecials::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
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


void CDLGSpecials::OnBnClickedButtonTsrqexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


void CDLGSpecials::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDCtl==IDC_BUTTON_SEARCH_1)         //checking for the button 
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
