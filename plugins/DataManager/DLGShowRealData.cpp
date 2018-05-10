// DLGShowRealData.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGShowRealData.h"
#include "afxdialogex.h"
#include "DlgDb_Connect.h"
#include "BackgroundDlg.h"
extern DATACONTAINER vlxcclck,vlxcclrk,vdoor6,vpolices,vmanHole,vsmokeSensor,vdoorSensor,veleMeter;
// CDLGShowRealData 对话框

IMPLEMENT_DYNAMIC(CDLGShowRealData, CDialogEx)

int CDLGShowRealData::m_SortColumn = -1;
BOOL CDLGShowRealData::m_bAsc = FALSE;

int CDLGShowRealData::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

CDLGShowRealData::CDLGShowRealData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGShowRealData::IDD, pParent)
	, m_info(_T(""))
{

}

CDLGShowRealData::~CDLGShowRealData()
{
}

void CDLGShowRealData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST2, m_wndList);
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_COMBO2, m_combox_limit);
	DDX_Control(pDX, IDC_STATIC_SSSJTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_REALDATAEXIT, m_EXIT);
	DDX_Text(pDX, IDC_STATIC_RESULT4, m_info);
}


BEGIN_MESSAGE_MAP(CDLGShowRealData, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDLGShowRealData::OnBnClickedButtonSearch)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CDLGShowRealData::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDC_BUTTON_REALDATAEXIT, &CDLGShowRealData::OnBnClickedButtonRealdataexit)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CDLGShowRealData 消息处理程序


void CDLGShowRealData::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	int n = m_combox.GetCurSel();
	if(n == -1)
	{
		return;
	}
	CString ss;
	m_combox.GetLBText(n,ss);
	std::string s(W2A(ss));
	if(s == "车辆入口")
	{
		resetList(2);
	}
	else if(s == "车辆出口")
	{
		resetList(1);
	}
	else if(s == "门禁")
	{
		resetList(3);
	}
	else if(s == "井盖")
	{
		resetList(4);
	}
	else if(s == "烟感")
	{
		resetList(5);
	}
	else if(s == "门磁")
	{
		resetList(6);
	}
	else if(s == "电表")
	{
		resetList(7);
	}
	else if(s == "所有")
	{
		resetList(0);
	}
}


void CDLGShowRealData::resetList(int type)
{
	USES_CONVERSION;
	//获得显示条数
	int n = m_combox_limit.GetCurSel();
	if(n == -1)
	{
		n = 2;
	}
	CString s;
	m_combox_limit.GetLBText(n,s);
	string s1 = W2A(s);
	int k = atoi(s1.c_str());

	m_wndList.DeleteAllItems();
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
	m_wndList.indexVector.clear();

	std::vector<std::string> vec;
	switch(type)
	{
	case 0:
		{
			m_wndList.InsertColumn (0, _T("记录类别"), LVCFMT_CENTER, 120);
			m_wndList.InsertColumn (1, _T("上次更新时间"), LVCFMT_CENTER, 180);
			m_wndList.InsertColumn (2, _T("记录内容"), LVCFMT_CENTER, 600);
	
			if(vdoor6.LData.size()>0)
			{
				vec.clear();
				vec.push_back("门禁");
				vec.push_back(vdoor6.lastUpdateTime);
				vec.push_back(vdoor6.LData.back());
				insertRows(vec);
			}
			if(vmanHole.LData.size()>0)
			{
				vec.clear();
				vec.push_back("井盖");
				vec.push_back(vmanHole.lastUpdateTime);
				vec.push_back(vmanHole.LData.back());
				insertRows(vec);
			}
			if(vsmokeSensor.LData.size()>0)
			{
				vec.clear();
				vec.push_back("烟感");
				vec.push_back(vsmokeSensor.lastUpdateTime);
				vec.push_back(vsmokeSensor.LData.back());
				insertRows(vec);
			}
			if(vpolices.LData.size()>0)
			{
				vec.clear();
				vec.push_back("人员定位");
				vec.push_back(vpolices.lastUpdateTime);
				vec.push_back(vpolices.LData.back());
				insertRows(vec);
			}
			if(vlxcclck.LData.size()>0)
			{
				vec.clear();
				vec.push_back("车辆出口");
				vec.push_back(vlxcclck.lastUpdateTime);
				vec.push_back(vlxcclck.LData.back());
				insertRows(vec);
			}
			if(vlxcclrk.LData.size()>0)
			{
				vec.clear();
				vec.push_back("车辆入口");
				vec.push_back(vlxcclrk.lastUpdateTime);
				vec.push_back(vlxcclrk.LData.back());
				insertRows(vec);
			}
			if(vdoorSensor.LData.size()>0)
			{
				vec.clear();
				vec.push_back("门磁");
				vec.push_back(vdoorSensor.lastUpdateTime);
				vec.push_back(vdoorSensor.LData.back());
				insertRows(vec);
			}
			if(veleMeter.LData.size()>0)
			{
				vec.clear();
				vec.push_back("电表");
				vec.push_back(veleMeter.lastUpdateTime);
				vec.push_back(veleMeter.LData.back());
				insertRows(vec);
			}
			break;
		}
	case 1:
		{
			m_wndList.InsertColumn (0, _T("车牌号码"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (1, _T("车辆类型"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (2, _T("出场时间"), LVCFMT_CENTER, 180);
			m_wndList.InsertColumn (3, _T("停留时间"), LVCFMT_CENTER, 100);
			vec.clear();
			std::vector<CString> vs;
			std::list<std::string> slist;
			COMFUNC::getLastNRecord(vlxcclck.LData,slist,k);
			for (std::list<std::string>::iterator ite = slist.begin();ite != slist.end();++ite)
			{
				COMFUNC::split(*ite,std::string("&&"),vs);
				for(int i = 0;i<vs.size();++i)
				{
					vec.push_back(W2A(vs[i]));
				}
				insertRows(vec);
				vec.clear();
			}
			break;
		}
	case 2:
		{
			m_wndList.InsertColumn (0, _T("车牌号码"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (1, _T("车辆类型"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (2, _T("入场时间"), LVCFMT_CENTER, 180);
			vec.clear();
			std::vector<CString> vs;
			std::list<std::string> slist;
			COMFUNC::getLastNRecord(vlxcclrk.LData,slist,k);
			for (std::list<std::string>::iterator ite = slist.begin();ite != slist.end();++ite)
			{
				COMFUNC::split(*ite,std::string("&&"),vs);
				for(int i = 0;i<vs.size();++i)
				{
					vec.push_back(W2A(vs[i]));
				}
				insertRows(vec);
				vec.clear();
			}
			break;
		}
	case 3:
		{
			m_wndList.InsertColumn (0, _T("ID"), LVCFMT_CENTER, 70);
			m_wndList.InsertColumn (1, _T("姓名"), LVCFMT_CENTER, 70);
			m_wndList.InsertColumn (2, _T("身份证"), LVCFMT_CENTER, 180);
			m_wndList.InsertColumn (3, _T("单元"), LVCFMT_CENTER, 80);
			m_wndList.InsertColumn (4, _T("楼层"), LVCFMT_CENTER, 80);
			m_wndList.InsertColumn (5, _T("房间号"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (6, _T("刷卡时间"), LVCFMT_CENTER, 200);
			m_wndList.InsertColumn (7, _T("行为"), LVCFMT_CENTER, 70);
			vec.clear();
			std::vector<CString> vs;
			std::list<std::string> slist;
			COMFUNC::getLastNRecord(vdoor6.LData,slist,k);
			for (std::list<std::string>::iterator ite = slist.begin();ite != slist.end();++ite)
			{
				COMFUNC::split(*ite,std::string("&&"),vs);
				for(int i = 0;i<vs.size();++i)
				{
					vec.push_back(W2A(vs[i]));
				}
				insertRows(vec);
				vec.clear();
			}
			break;
		}
	case 4://井盖
		{
			m_wndList.InsertColumn (0, _T("编号"), LVCFMT_CENTER, 70);
			m_wndList.InsertColumn (1, _T("安装位置"), LVCFMT_CENTER, 140);
			m_wndList.InsertColumn (2, _T("功能码"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (3, _T("报警角度"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (4, _T("当前角度"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (5, _T("上报频率"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (6, _T("报警频率"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (7, _T("终端电压"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (8, _T("水位状态"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (9, _T("更新时间"), LVCFMT_CENTER, 200);
			vec.clear();
			std::vector<CString> vs;
			std::list<std::string> slist;
			COMFUNC::getLastNRecord(vmanHole.LData,slist,k);
			for (std::list<std::string>::iterator ite = slist.begin();ite != slist.end();++ite)
			{
				COMFUNC::split(*ite,std::string("&&"),vs);
				for(int i = 0;i<vs.size()-1;++i)
				{
					vec.push_back(W2A(vs[i]));
				}
				std::string s;
				COMFUNC::dateTrans(W2A(vs.back()),s);
				vec.push_back(s);
				insertRows(vec);
				vec.clear();
			}
			break;
		}
	case 5://烟感
		{
			m_wndList.InsertColumn (0, _T("编号"), LVCFMT_CENTER, 70);
			m_wndList.InsertColumn (1, _T("安装位置"), LVCFMT_CENTER, 140);
			m_wndList.InsertColumn (2, _T("烟雾状态"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (3, _T("电池状态"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (4, _T("测试键"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (5, _T("更新时间"), LVCFMT_CENTER, 200);
			vec.clear();
			std::vector<CString> vs;
			std::list<std::string> slist;
			COMFUNC::getLastNRecord(vsmokeSensor.LData,slist,k);
			for (std::list<std::string>::iterator ite = slist.begin();ite != slist.end();++ite)
			{
				COMFUNC::split(*ite,std::string("&&"),vs);
				for(int i = 0;i<vs.size()-1;++i)
				{
					vec.push_back(W2A(vs[i]));
				}
				std::string s;
				COMFUNC::dateTrans(W2A(vs.back()),s);
				vec.push_back(s);
				insertRows(vec);
				vec.clear();
			}
			break;
		}
	case 6://门磁
		{
			m_wndList.InsertColumn (0, _T("编号"), LVCFMT_CENTER, 70);
			m_wndList.InsertColumn (1, _T("安装位置"), LVCFMT_CENTER, 140);
			m_wndList.InsertColumn (2, _T("门禁状态"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (3, _T("RF类型"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (4, _T("更新时间"), LVCFMT_CENTER, 200);
			vec.clear();
			std::vector<CString> vs;
			std::list<std::string> slist;
			COMFUNC::getLastNRecord(vdoorSensor.LData,slist,k);
			for (std::list<std::string>::iterator ite = slist.begin();ite != slist.end();++ite)
			{
				COMFUNC::split(*ite,std::string("&&"),vs);
				for(int i = 0;i<vs.size()-1;++i)
				{
					vec.push_back(W2A(vs[i]));
				}
				std::string s;
				COMFUNC::dateTrans(W2A(vs.back()),s);
				vec.push_back(s);
				insertRows(vec);
				vec.clear();
			}
			break;
		}
	case 7://电表
		{
			m_wndList.InsertColumn (0, _T("电表号"), LVCFMT_CENTER, 70);
			m_wndList.InsertColumn (1, _T("安装位置"), LVCFMT_CENTER, 180);
			m_wndList.InsertColumn (2, _T("电表类型"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (3, _T("电能(Kwh)"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (4, _T("电压(V)"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (5, _T("电流(A)"), LVCFMT_CENTER, 100);
			m_wndList.InsertColumn (6, _T("更新时间"), LVCFMT_CENTER, 200);
			vec.clear();
			std::vector<CString> vs;
			std::list<std::string> slist;
			COMFUNC::getLastNRecord(veleMeter.LData,slist,k);
			for (std::list<std::string>::iterator ite = slist.begin();ite != slist.end();++ite)
			{
				COMFUNC::split(*ite,std::string("&&"),vs);
				for(int i = 0;i<vs.size()-1;++i)
				{
					vec.push_back(W2A(vs[i]));
				}
				std::string s;
				COMFUNC::dateTrans(W2A(vs.back()),s);
				vec.push_back(s);
				insertRows(vec);
				vec.clear();
			}
			break;
		}
	default:break;
	}
	for (int i = 0;i<m_wndList.indexVector.size();++i)
	{
		m_wndList.indexVector[i] = m_wndList.GetItemCount() - 1 - m_wndList.indexVector[i];
	}
}

void CDLGShowRealData::insertRows(std::vector<std::string>& vec)
{
	int n = -1;
	int m = m_wndList.GetItemCount();
	m_wndList.InsertItem(0, _T("1"));
	for(int i = 0;i<vec.size();++i)
	{
		if (vec[i].find("异常") != -1)
		{
			n = m;
		}
		m_wndList.SetItemText (0, i, CString(vec[i].c_str()));
	}
	if (n != -1)
	{
		m_wndList.indexVector.push_back(UINT(n));
	}
}

BOOL CDLGShowRealData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_combox.SetCurSel(8);//所有
	m_combox_limit.SetCurSel(2);//10
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetTextColor(RGB(255,255,255));
	m_font1.CreatePointFont(110, _T("宋体"));
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
	m_dlgTitle.SetWindowTextW(_T("实时数据"));

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

	OnBnClickedButtonSearch();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDLGShowRealData::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
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
	m_wndList.refreshIndex();
	*pResult = 0;
}


void CDLGShowRealData::OnBnClickedButtonRealdataexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


HBRUSH CDLGShowRealData::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDLGShowRealData::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


BOOL CDLGShowRealData::PreTranslateMessage(MSG* pMsg)
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


void CDLGShowRealData::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDCtl==IDC_BUTTON_SEARCH)         //checking for the button 
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
