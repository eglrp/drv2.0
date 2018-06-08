// FireControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "FireControlDlg.h"
#include "afxdialogex.h"
#include "functionEx.h"
#include <ogr_spatialref.h>

extern int g_RowHeight;//行高
extern HOUSEINFO gHouseInfo;
extern CDataManager* m_pDM;
// CFireControlDlg 对话框

IMPLEMENT_DYNAMIC(CFireControlDlg, CDialogEx)

CFireControlDlg::CFireControlDlg(int H,CWnd* pParent /*=NULL*/)
	: CDialogEx(CFireControlDlg::IDD, pParent)
{
	viewH = H;
	
	pDLG_BK = nullptr;
	pDLG_code = nullptr;
	pDLG_NEIBOR = nullptr;
	pDLG_PIC = nullptr;
	pDLG_INFO = nullptr;
	readPrjInfo();
}

CFireControlDlg::~CFireControlDlg()
{
}

void CFireControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_JQDW, m_jqdw);
	DDX_Control(pDX, IDC_BUTTON_FWDW, m_fwdw);
	DDX_Control(pDX, IDC_BUTTON_FWEWM, m_fwewm);
	DDX_Control(pDX, IDC_BUTTON_LDLJB, m_ldljb);
	DDX_Control(pDX, IDC_BUTTON_XFSFB, m_xfsfb);
}


BEGIN_MESSAGE_MAP(CFireControlDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LDLJB, &CFireControlDlg::OnBnClickedButtonLdljb)
	ON_BN_CLICKED(IDC_BUTTON_JQDW, &CFireControlDlg::OnBnClickedButtonJqdw)
	ON_BN_CLICKED(IDC_BUTTON_FWDW, &CFireControlDlg::OnBnClickedButtonFwdw)
	ON_BN_CLICKED(IDC_BUTTON_FWEWM, &CFireControlDlg::OnBnClickedButtonFwewm)
	ON_BN_CLICKED(IDC_BUTTON_XFSFB, &CFireControlDlg::OnBnClickedButtonXfsfb)
END_MESSAGE_MAP()


// CFireControlDlg 消息处理程序
bool CFireControlDlg::checkPos()
{
	if ( (m_pGetPosHandler->mx == 0 && m_pGetPosHandler->my == 0 && m_pGetPosHandler->mz == 0) || m_pGetPosHandler->houseName.empty())
	{
		return false;
	}
	return true;
}

void CFireControlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


BOOL CFireControlDlg::PreTranslateMessage(MSG* pMsg)
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


void CFireControlDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CfireControl* ca = (CfireControl*)parent;
	ca->OnClick();
	DestroyWindow();
}


void CFireControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int step =5;
	if (nIDEvent == 0)
	{
		if (count >= nElapse)
		{
			KillTimer(0);
			return;
		}
		count = count + step;
		RECT r;
		GetWindowRect(&r);
		r.left -= step;
		r.right -= step;
		MoveWindow(&r);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CFireControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);
	HBITMAP   hBitmap1,hBitmap2,hBitmap3,hBitmap4,hBitmap5;   
	hBitmap1 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\消防平面图.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap2 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\业主查询.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap3 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\房屋二维码.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap4 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\邻居查询.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	hBitmap5 = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\button\\消防设备.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE 
	int btnH = 48;
	int btnW = 130;

	RECT rect;
	GetWindowRect(&rect);
	rect.right = rect.left + btnW;
	rect.bottom = rect.top + btnH * 5 + 40;
	MoveWindow(&rect);

	RECT br;
	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 0;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_jqdw.MoveWindow(&br);
	m_jqdw.SetBitmap(hBitmap1);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 1*btnH + 10;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_fwdw.MoveWindow(&br);
	m_fwdw.SetBitmap(hBitmap2);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 2*btnH + 20;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_fwewm.MoveWindow(&br);
	m_fwewm.SetBitmap(hBitmap3);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 3*btnH + 30;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_ldljb.MoveWindow(&br);
	m_ldljb.SetBitmap(hBitmap4);

	br.left = rect.left;
	br.right = br.left + btnW;
	br.top = rect.top + 4*btnH + 40;
	br.bottom = br.top + btnH;
	ScreenToClient(&br);
	m_xfsfb.MoveWindow(&br);
	m_xfsfb.SetBitmap(hBitmap5);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	POINT *pt = new POINT[12];
	POINT p;
	p.x = btnW/8.0*3; p.y = 0;
	pt[0] = p;

	p.x = btnW-2 ;p.y = 0;
	pt[1] = p;

	p.x = btnW/8.0*7 ;p.y = btnH;
	pt[2] = p;

	p.x = btnW/80.0*65; p.y = btnH * 2;
	pt[3] = p;

	p.x = btnW/80.0*62 ;p.y = btnH * 3;
	pt[4] = p;

	p.x = btnW/80.0*65;p.y = btnH*4;
	pt[5] = p;

	p.x = btnW/8.0*7;p.y = btnH*5;
	pt[6] = p;

	p.x = btnW/16.0*3;p.y = btnH*5;
	pt[7] = p;

	p.x = btnW/16.0;p.y = btnH*4;
	pt[8] = p;

	p.x = 2;p.y = btnH*3;
	pt[9] = p;

	p.x = btnW/16.0;p.y = btnH*2;
	pt[10] = p;

	p.x = btnW/16.0*3;p.y = btnH*1;
	pt[11] = p;
	CRgn wndRgn;
	wndRgn.CreatePolygonRgn(pt,12,1);
	//SetWindowRgn((HRGN)wndRgn, TRUE);
	UpdateWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFireControlDlg::play()
{
	RECT r;
	GetWindowRect(&r);
	nElapse = r.right - r.left + 20;
	count = 0;
	SetTimer(0,1,NULL);
}

void CFireControlDlg::destroyAll()
{
	if (pDLG_NEIBOR != nullptr)
	{
		pDLG_NEIBOR->DestroyWindow();
		pDLG_NEIBOR = nullptr;
	}
	if (pDLG_BK != nullptr)
	{
		pDLG_BK->DestroyWindow();
		pDLG_BK = nullptr;
	}
	if (pDLG_code != nullptr)
	{
		pDLG_code->DestroyWindow();
		pDLG_code = nullptr;
	}
	if (pDLG_PIC != nullptr)
	{
		pDLG_PIC->DestroyWindow();
		pDLG_PIC = nullptr;
	}
	if (pDLG_INFO != nullptr)
	{
		pDLG_INFO->DestroyWindow();
		pDLG_INFO = nullptr;
	}
}

void CFireControlDlg::OnBnClickedButtonLdljb()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!checkPos())
	{
		return;
	}
	destroyAll();
	pDLG_NEIBOR = new CDLGNeighborSearch();
	pDLG_NEIBOR->Create(IDD_DIALOG_NEIGHBOR);
	//按房间号设置
	std::string houseName = m_pGetPosHandler->houseName;
	std::string sfloor =houseName.substr(houseName.find_last_of("-") + 1);
	pDLG_NEIBOR->m_edit_floor1 = CString(sfloor.c_str());
	pDLG_NEIBOR->UpdateData(FALSE);
	
	int iBuild = atoi((houseName.substr(0,2)).c_str()); 
	int iUnit = atoi((houseName.substr(3,2)).c_str()); 
	pDLG_NEIBOR->m_Combo_build1.SetCurSel(iBuild - 1);
	pDLG_NEIBOR->m_Combo_unit1.SetCurSel(iUnit - 1);

	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);
	pDLG_NEIBOR->parent = (CObject*)pDLG_BK;
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_NEIBOR->ShowWindow(SW_SHOW);
	pDLG_NEIBOR->OnBnClickedButtonSearchneighbor1();
}


void CFireControlDlg::OnBnClickedButtonJqdw()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	if (!checkPos())
	{
		return;
	}
	destroyAll();
	pDLG_PIC = new CShowPictureDlg();
	pDLG_PIC->Create(IDD_DIALOG_PICTUREINFO);
	std::string houseName = m_pGetPosHandler->houseName;
	int iBuild = atoi((houseName.substr(0,2)).c_str());
	CString sBuild;
	switch(iBuild)
	{
	case 1:sBuild = _T("一栋");break;
	case 2:sBuild = _T("二栋");break;
	case 3:sBuild = _T("三栋");break;
	case 4:sBuild = _T("四栋");break;
	case 5:sBuild = _T("五栋");break;
	case 6:sBuild = _T("六栋");break;
	case 7:sBuild = _T("七栋");break;
	case 8:sBuild = _T("八栋");break;
	case 9:sBuild = _T("九栋");break;
	case 10:sBuild = _T("十栋");break;
	default:sBuild = _T("0");break;
	}
	CString picPath = _T("Data\\DRVdata\\") + sBuild + _T(".jpg");
	int height = 0;
	int width = 0;
	if (!pDLG_PIC->getPicSize(picPath,width,height))
	{
		return ;
	}
	pDLG_PIC->fullScreen(width,height);
	pDLG_PIC->setPath(string(W2A(picPath)));
	pDLG_PIC->setColorType(0);
	pDLG_PIC->ShowWindow(SW_SHOW);
	pDLG_PIC->Invalidate();
}


void CFireControlDlg::OnBnClickedButtonFwdw()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!checkPos())
	{
		return;
	}
	destroyAll();
	pDLG_INFO = new CDlgAttInfo();
	pDLG_INFO->Create(IDD_DIALOG_ENTINFO);
	pDLG_INFO->InsertRow(_T("户主"),CString(gHouseInfo.ownerName.c_str()));
	pDLG_INFO->InsertRow(_T("性别"),CString(gHouseInfo.sex.c_str()));
	pDLG_INFO->InsertRow(_T("房间号"),CString(gHouseInfo.houseID.c_str()));
	std::string s = gHouseInfo.idCard;
	if (s.size() == 18)
	{
		for (int i = 3;i<=14;++i)
		{
			s[i] = '*';
		}
	}
	pDLG_INFO->InsertRow(_T("身份证号"),CString(s.c_str()));
	s = gHouseInfo.phone;
	if (s.size() == 11)
	{
		for (int i = 4;i<=7;++i)
		{
			s[i] = '*';
		}
	}
	pDLG_INFO->InsertRow(_T("手机号码"),CString(s.c_str()));
	pDLG_INFO->InsertRow(_T("房间底高"),CString(gHouseInfo.sDG.c_str()));
	pDLG_INFO->InsertRow(_T("房间顶高"),CString(gHouseInfo.sDG2.c_str()));
	RECT r,r1;
	r.left = m_pDM->m_spViewer3D->rectView3D.left + 20;
	r.right = r.left + (m_pDM->m_spViewer3D->rectView3D.right - m_pDM->m_spViewer3D->rectView3D.left)/5.0;
	r.top = m_pDM->m_spViewer3D->rectView3D.top + 20;
	r.bottom = r.top + 20 + 7*g_RowHeight;
	pDLG_INFO->MoveWindow(&r);
	
	r1.left = r.left + 2;
	r1.right = r.right - 2;
	r1.top = r.top + 2;
	r1.bottom = r.bottom -2;
	pDLG_INFO->ScreenToClient(&r1);
	((CStatic*)pDLG_INFO->GetDlgItem(IDC_STATIC_edge))->MoveWindow(&r1);

	r1.left = r.left + 3;
	r1.right = r.right - 2;
	r1.top = r.top + 12;
	r1.bottom = r.bottom -10;
	pDLG_INFO->ScreenToClient(&r1);
	pDLG_INFO->m_wndList.MoveWindow(&r1);

	pDLG_BK = new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);
	pDLG_INFO->parentBk = (CObject*)pDLG_BK;
	pDLG_INFO->parent = nullptr;
	pDLG_BK->MoveWindow(&r);

	pDLG_INFO->selfDefShowWindow(SW_SHOW);
}


void CFireControlDlg::OnBnClickedButtonFwewm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!checkPos())
	{
		return;
	}
	destroyAll();
	DeleteFile(_T("Data\\DRVdata\\location.png"));
	DeleteFile(_T("Data\\DRVdata\\houseInfo.png"));
	std::string sloc = getLocationStr(m_pGetPosHandler->mx,m_pGetPosHandler->my,"定位点-航天远景",m_pGetPosHandler->houseName);
	std::string strlocation = std::string("-o Data\\DRVdata\\location.png ") + sloc;

	std::string sHouse = std::string("-o Data\\DRVdata\\houseInfo.png ") +  getHouseInfo(gHouseInfo);

	BeginWaitCursor();
	ShellExecute(this->m_hWnd, _T("open"), _T("qrencode.exe"), CString(strlocation.c_str()), _T(""), SW_HIDE);
	ShellExecute(this->m_hWnd, _T("open"), _T("qrencode.exe"), CString(sHouse.c_str()), _T(""), SW_HIDE);
	EndWaitCursor();
	while( !PathFileExists(_T("Data\\DRVdata\\location.png")) );
	while( !PathFileExists(_T("Data\\DRVdata\\houseInfo.png")));
	pDLG_code = new CDLGQrencode2();
	pDLG_code->Create(IDD_DIALOG_QRENCODE2INFO);
	pDLG_BK =new CBackgroundDlg();
	pDLG_BK->Create(IDD_DIALOG_BACKGROUND);
	pDLG_code->parent = (CObject*)pDLG_BK;
	RECT r;
	pDLG_code->GetWindowRect(&r);
	pDLG_BK->MoveWindow(&r);
	pDLG_BK->ShowWindow(SW_SHOW);
	pDLG_code->ShowWindow(SW_SHOW);

}


void CFireControlDlg::OnBnClickedButtonXfsfb()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	if (!checkPos())
	{
		return;
	}
	destroyAll();
	pDLG_PIC = new CShowPictureDlg(false);
	pDLG_PIC->Create(IDD_DIALOG_PICTUREINFO);
	
	CString picPath = _T("Data\\DRVdata\\消防栓分布图.jpg");
	int height = 0;
	int width = 0;
	if (!pDLG_PIC->getPicSize(picPath,width,height))
	{
		return ;
	}
	pDLG_PIC->fullScreen(width,height);
	pDLG_PIC->setPath(string(W2A(picPath)));
	pDLG_PIC->setColorType(0);
	pDLG_PIC->ShowWindow(SW_SHOW);
	pDLG_PIC->Invalidate();
}

std::string CFireControlDlg::getLocationStr(double x,double y,std::string str1 ,std::string str2 )
{
	PointToWGS84(x,y,lon,lat);
	char c1[20];
	char c2[20];
	sprintf_s(c1,"%f",lat);
	sprintf_s(c2,"%f",lon);

	std::string locStr = "http://api.map.baidu.com/marker?location=" + std::string(c1) + "," + std::string(c2) +   \
		"&coord_type=wgs84&title=" + str1 + "&content=" + str2 + "(" + std::string(c2) + "," + std::string(c1) + \
		")&output=html";
	return locStr;
}

void CFireControlDlg::PointToWGS84(double x, double y, double& lon, double& lat)
{
	lon = x;
	lat = y;
#ifdef _DEBUG
	//return;
#endif

	OGRCoordinateTransformation *poCT = NULL;

	if (poCT == nullptr)
	{
		OGRSpatialReference* oSourceSRS;
		oSourceSRS = (OGRSpatialReference*)OSRNewSpatialReference(prjInfo.c_str());

		if (oSourceSRS == NULL)
			return;

		OGRSpatialReference* oTargetSRS;
		oTargetSRS = (OGRSpatialReference*)OSRNewSpatialReference("GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]");
		if (oTargetSRS == NULL)
		{
			OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
			return;
		}

		poCT = OGRCreateCoordinateTransformation(oSourceSRS, oTargetSRS);

		if (poCT == nullptr)
		{
			OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
			OSRDestroySpatialReference((OGRSpatialReferenceH)oTargetSRS);
			return;
		}
	}

	poCT->Transform(1, &lon, &lat);

#if 0
	lon = x;
	lat = y;
	OGRSpatialReference* oSourceSRS;
	OGRSpatialReference oTargetSRS;
	OGRCoordinateTransformation *poCT;
	oSourceSRS = (OGRSpatialReference*)OSRNewSpatialReference(mSRS.c_str());
	if (oSourceSRS == NULL)
		return;

	oTargetSRS.importFromEPSG(4326);
	const char* err = CPLGetLastErrorMsg();
	poCT = OGRCreateCoordinateTransformation(oSourceSRS, &oTargetSRS);
	if (poCT == nullptr)
	{
		OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
		return;
	}

	poCT->Transform(1, &lon, &lat);
	OGRCoordinateTransformation::DestroyCT(poCT);
	OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
#endif


	if (poCT != nullptr)
		OGRCoordinateTransformation::DestroyCT((OGRCoordinateTransformation*)poCT);
	poCT = nullptr;
}

void CFireControlDlg::readPrjInfo()
{
	/*TCHAR szFilePath[1024 + 1];
	GetModuleFileName(NULL, szFilePath, 1024);

	CString str_url = szFilePath;
	USES_CONVERSION;
	std::string curFilePath( W2A(str_url) );

	curFilePath = replaceFileName(curFilePath, "Data\\DRVdata\\total.prj");

	FILE* fpPrj = fopen(curFilePath.c_str(), "r");
	char str[1024];
	fgets(str, 1024, fpPrj);
	fclose(fpPrj);*/
	CfireControl* p = (CfireControl*)(parent);
	prjInfo = p->getPrj();
}

std::string CFireControlDlg::replaceFileName(std::string srcStr, std::string replaceStr)
{
	int pos = srcStr.rfind("\\") + 1;
	int length = srcStr.length() - 1;
	return srcStr.replace(pos, length, replaceStr);
}

std::string CFireControlDlg::getHouseInfo(HOUSEINFO& hi)
{
	std::string s;
	s = "姓名："+hi.ownerName+"\r\n"+"性别："+hi.sex +"\r\n"+"房间号："+hi.houseID+"\r\n"+ "身份证号："+ hi.idCard + "\r\n"+"电话："+hi.phone +"\r\n"+ "底高：" + hi.sDG + "\r\n"+"顶高："+ hi.sDG2;
	return s;
}