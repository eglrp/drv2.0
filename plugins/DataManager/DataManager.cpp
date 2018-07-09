// DataManager.cpp : implementation file
//

#include "stdafx.h"
#include "winuser.h"
#include "DataManager.h"
#include "resource.h"
#include "EarthMatrixFunctions.h"
#include "IViewer3D.h"
#include <osg/Material>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osgText/Text3D>
#include "INodeEvents.h"
#include "x3py/observer/observerobject.h"
#include "osg/Group"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "osg/LineWidth"
#include "osg/osgUtil/PlaneIntersector"
#include "DlgSearchAdv.h"
#include <osg/Billboard>
#include <osgText/Text>
#include "osg/PositionAttitudeTransform"
#include "EncodeString.h"
#include "BackgroundDlg.h"
extern CDlgDB_Connect* g_DBconnector;
extern std::string latestUpdateTime;
extern DATACONTAINER vmanHole,vsmokeSensor,vdoorSensor,veleMeter;
int g_FontSize;//字体大小
int g_RowHeight;//行高

using namespace std;
// CDataManager
IMPLEMENT_DYNAMIC(CDataManager, CWnd)

CDataManager* m_pDM;

CDataManager::CDataManager()
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"图层窗口"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"图层管理"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"显示或隐藏图层窗口"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"显示或隐藏图层窗口"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"TreeWindow"));
}

CDataManager::~CDataManager()
{
    UnInitialize();
}

BEGIN_MESSAGE_MAP(CDataManager, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_NOTIFY(NM_DBLCLK, 4, &CDataManager::OnNMDblclk)
    ON_NOTIFY(NM_CLICK, 4, &CDataManager::OnNMClickDataManager)
    ON_NOTIFY(NM_RCLICK, 4, &CDataManager::OnRBClick)
    ON_WM_RBUTTONUP()
    ON_COMMAND(ID_DELLAYER, &CDataManager::OnDellayer)
    ON_COMMAND(ID_SET_DEFLAYER, &CDataManager::OnSetDefLayer)
    ON_EN_CHANGE(ID_KEY, &CDataManager::OnKeyChangeEditName)
    ON_NOTIFY(NM_CLICK, ID_SEARCH_LIST, &CDataManager::OnNMClickList)
    ON_BN_CLICKED(ID_SEARCH_BUTTON, &CDataManager::OnBnClickedButtonSearch)
    ON_BN_CLICKED(ID_SEARCH_BUTTON_ADV, &CDataManager::OnBnClickedButtonSearchAdv)
	ON_WM_TIMER()
	ON_COMMAND(ID_40007, &CDataManager::OnSetBuildLayer)
	ON_COMMAND(ID_40008, &CDataManager::OnSetHouseLayer)
END_MESSAGE_MAP()


// CDataManager 消息处理程序
bool CDataManager::Initialize()
{
    dStep = 1;
    bFindedStart = false;
    bFindedEnd = false;
    dStartHeight = 0;
    dEndHeight = 200;
    m_pAttInfoWin = NULL;
	m_pHtmlInfoWin = NULL;
	m_pShowPictureInfoWin = NULL;
	m_pCVMediaPlayerInfoWin = NULL;
	m_pInternetInfoWin = NULL;
	g_DBconnector = NULL;
	m_pVLCPlayerInfoWin = NULL;
	AFX_MANAGE_STATE_EX;
	g_DBconnector = new CDlgDB_Connect();
	g_DBconnector->Create(IDD_DIALOG_CONNECT,this);
	g_DBconnector->ShowWindow(SW_HIDE);

	if (m_pInternetInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pInternetInfoWin = new CInternetInfoDlg();
		m_pInternetInfoWin->parent = (CObject*)this;
		m_pInternetInfoWin->Create(IDD_DIALOG_INFO_NET,this);
		CBackgroundDlg* dlg = new CBackgroundDlg();
		dlg->Create(IDD_DIALOG_BACKGROUND);
		m_pInternetInfoWin->parentBk = (CObject*)dlg;
	}
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if(spViewer3D.valid() )
	{
		RECT r;
		r.left = spViewer3D->rectView3D.right - 50;
		r.top = spViewer3D->rectView3D.bottom - 50;
		r.right = r.left + 30;
		r.bottom = r.top + 30;
		g_DBconnector->MoveWindow(&r);
	}
	
    bInterDel = false;

	int ScreenH = GetSystemMetrics(SM_CYSCREEN); 
	if (ScreenH < 1024)
	{
		g_FontSize = 12;
		g_RowHeight = 24;
	}
	else
	{
		g_FontSize = 16;
		g_RowHeight = 30;
	}
    return true;
}

bool CDataManager::UnInitialize()
{
	USES_CONVERSION;
	//保存数据库的最后更新时间，下次程序只读之后更新的数据
	osgDB::ofstream _fout("Data\\DRVdata\\sqlTime.dat");
	if (_fout)
	{
		_fout<<latestUpdateTime;
		_fout.close();
	}
	
	osgDB::ofstream _fout1("Data\\DRVdata\\server.dat",std::ios_base::binary);
	if (_fout1)
	{
		for (int i = 0;i<g_DBconnector->m_wndList.GetItemCount();++i)
		{
			//对密码加密
			std::string str(W2A(g_DBconnector->vPassword[i]));
			
			_fout1<<W2A(g_DBconnector->m_wndList.GetItemText(i,0))<<" "<<W2A(g_DBconnector->m_wndList.GetItemText(i,1))
				  <<" "<<W2A(g_DBconnector->m_wndList.GetItemText(i,2))<<" ";
			ENCODESTRING::EncodeString(str,"visiontek",true);
			_fout1<<str;
			_fout1<<" ";
		}
	}
	_fout1.close();
    if( ::IsWindow(GetSafeHwnd()) )
        DestroyWindow();

    return true;
}

bool CDataManager::SetBuddy(x3::IObject* val)
{
    x3::Object<IViewer3D> spViewer3D(val);
    if( !spViewer3D.valid() )
        return false;

    if( m_spBuddy == spViewer3D )
        return true;

    m_spBuddy = spViewer3D;
    if( spViewer3D->getViewer() == NULL )
        return false;

    m_spViewer3D = (IViewer3D*)val;

    m_pDM = this;

    return true;
}

bool CDataManager::createWnd(size_t hParent, long id)
{
    if( hParent == NULL || !::IsWindow((HWND)hParent) )
        return false;

    CRect rect;
    rect.SetRectEmpty();
    return Create(NULL, _T(""), WS_CHILD | WS_VISIBLE, rect, CWnd::FromHandle((HWND)hParent), id);
}

bool CDataManager::setParent(size_t hWnd)
{
    if( hWnd == NULL || !::IsWindow((HWND)hWnd) )
        return false;

    CWnd::SetParent(CWnd::FromHandle((HWND)hWnd));
    return true;
}

int CDataManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    AFX_MANAGE_STATE_EX
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // 创建视图:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP |
                              TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT | TVS_CHECKBOXES | TVS_SHOWSELALWAYS;
    if (!m_wndTreeCtrl.Create(dwViewStyle, rectDummy, this, 4))
    {
        TRACE0("未能创建文件视图\n");
        return -1;      // 未能创建
    }
	m_cKey.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | WS_CLIPCHILDREN , CRect(10, 10, 100, 30), this, ID_KEY );
	m_cList.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS | WS_CLIPCHILDREN , CRect(10, 10, 100, 30), this, ID_SEARCH_LIST );
	m_cSearch.Create(_T("搜索"), WS_VISIBLE | WS_CLIPCHILDREN , CRect(10, 10, 100, 30), this, ID_SEARCH_BUTTON);
	m_cSearchAdv.Create(_T("高级"), WS_VISIBLE | WS_CLIPCHILDREN , CRect(10, 10, 100, 30), this, ID_SEARCH_BUTTON_ADV);

    m_cList.InsertColumn (0, _T("字段名"), LVCFMT_LEFT, 80);
    m_cList.InsertColumn (1, _T("字段值"), LVCFMT_LEFT, 150);

    m_cList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    HFONT hFont = (HFONT)::GetStockObject(SYSTEM_FIXED_FONT);
    CFont* pFont = CFont::FromHandle(hFont);
    m_cKey.SetFont(pFont);
    m_cSearch.SetFont(pFont);
    m_cSearchAdv.SetFont(pFont);

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TOOLBAR);
    m_wndToolBar.LoadToolBar(IDR_TOOLBAR, 0, 0, TRUE /* 已锁定*/);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // 所有命令将通过此控件路由，而不是通过主框架路由:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();

    registerHandlers();
    return 0;
}

void CDataManager::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    AdjustLayout();
}

void CDataManager::OnDestroy()
{
    unregisterHandlers();

    CWnd::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
}

void CDataManager::AdjustLayout()
{
    if ( !::IsWindow(GetSafeHwnd()) )
        return;

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	
    int nTreeHeight = 300;
    int nKeyHeight = 24;
	if (GetSystemMetrics(SM_CYSCREEN)<1024)
	{
		nTreeHeight = 200;
	}
	 
    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndTreeCtrl.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, nTreeHeight, SWP_NOACTIVATE | SWP_NOZORDER);
    m_cKey.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 4 + cyTlb + nTreeHeight, rectClient.Width() - 1 - 60 - 60 - 1, nKeyHeight, SWP_NOACTIVATE | SWP_NOZORDER);
    m_cList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 6 + cyTlb + nKeyHeight + nTreeHeight, rectClient.Width() - 1, rectClient.Height() - 8 - cyTlb - nKeyHeight - nTreeHeight, SWP_NOACTIVATE | SWP_NOZORDER);
    m_cSearch.SetWindowPos(NULL, rectClient.left + 4 + rectClient.Width() - 60 - 60, rectClient.top + 4 + cyTlb + nTreeHeight, 60 - 4, nKeyHeight, SWP_NOACTIVATE | SWP_NOZORDER);
    m_cSearchAdv.SetWindowPos(NULL, rectClient.left + 4 + rectClient.Width() - 60, rectClient.top + 4 + cyTlb + nTreeHeight, 60 - 4, nKeyHeight, SWP_NOACTIVATE | SWP_NOZORDER);
}


QMemDC::QMemDC(CDC* dc, CRect rc)
{
    dcSrc = dc;
    rect  = rc;
    // 创建内存DC
    CreateCompatibleDC(dc);
    bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
    SelectObject(bmp);
}

void QMemDC::Apply()
{
    // 将内存DC绘制到设备DC上
    dcSrc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), this, 0, 0, SRCCOPY);
}

void CDataManager::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 __super::OnPaint()

    CRect rc;
    GetClientRect(&rc);

    // 采用双缓存，防止闪烁
    QMemDC mdc(&dc, rc);
    COLORREF bkgnd = RGB(255, 255, 255);
    mdc.FillSolidRect(&rc, bkgnd);
    mdc.Apply();

    CRect rectTree;
    m_wndTreeCtrl.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

}

void CDataManager::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    // TODO: 在此处添加消息处理程序代码
    if( ::IsWindow(m_wndTreeCtrl.GetSafeHwnd()) )
        m_wndTreeCtrl.SetFocus();
}

//////////////////////////////////////////////////////////////////////////
// CDataManagerObserver
CDataManagerObserver::CDataManagerObserver()
{
    m_bRegister = false;
}

CDataManagerObserver::~CDataManagerObserver()
{
    unregisterHandlers();
}

void CDataManagerObserver::registerHandlers()
{
    if( m_bRegister )
        return;

    X3_REGISTER_OBSERVER_OBJECT(EventNodeAdd, &CDataManagerObserver::OnNodeAdd);
    X3_REGISTER_OBSERVER_OBJECT(EventNodeRemove, &CDataManagerObserver::OnNodeRemove);
    X3_REGISTER_OBSERVER_OBJECT(EventNodeChanged, &CDataManagerObserver::OnNodeChange);
    X3_REGISTER_OBSERVER_OBJECT(EventNodeSelected, &CDataManagerObserver::OnNodeSelected);
    X3_REGISTER_OBSERVER_OBJECT(EventSetDefLayer, &CDataManagerObserver::OnSetDefLayer2);
    X3_REGISTER_OBSERVER_OBJECT(EventProjectOpen, &CDataManagerObserver::OnProjectOpen);
    m_bRegister = true;
}

void CDataManagerObserver::unregisterHandlers()
{
    if( !m_bRegister )
        return;

    x3::unregisterObserver(this);
}
//////////////////////////////////////////////////////////////////////////

void CDataManager::OnNodeAdd(x3::IObject* pBuddy, osg::Node* pNode, osg::Node* pParentNode, std::string filename)
{
	AFX_MANAGE_STATE_EX
		USES_CONVERSION;
	if( pNode == nullptr || pBuddy == nullptr )
		return;

	bool bExsit = false;
	if (FindItem(m_wndTreeCtrl.GetRootItem(),CString(pNode->getName().c_str())))
	{
		bExsit = true;
	}

	CString Clayer = _T("自定义");
	std::string up = pNode->getName();
	for (size_t i = 0;i<up.size();++i)
	{
		if (islower(up[i]))
		{
			up[i] -= 32;
		}
	}

	if (up.find(".SHP") != -1 || up.find(".DXF") != -1)
	{
		Clayer = _T("矢量图层");
	}
	else if (up.find(".OSG") != -1)
	{
		Clayer = _T("三维场景");
	}
	else if (up == "线框" || up.find("兴趣点(")!= -1)
	{
		Clayer = _T("临时图层");
	}
	HTREEITEM root = m_wndTreeCtrl.GetRootItem();
	HTREEITEM htree;
	if (!m_wndTreeCtrl.ItemHasChildren(root))
	{
		htree = m_wndTreeCtrl.InsertItem(Clayer,root,TVI_LAST);
		m_wndTreeCtrl.SetItemColor(htree,RGB(0,0,128));
		m_wndTreeCtrl.SetCheck(htree,TRUE);
	}
	else
	{
		bool find = false;
		htree = m_wndTreeCtrl.GetChildItem(root);
		while (htree)
		{
			if (m_wndTreeCtrl.GetItemText(htree) == Clayer)
			{
				find = true;
				break;
			}
			htree= m_wndTreeCtrl.GetNextSiblingItem(htree);
		}
		if (!find)
		{
			htree = m_wndTreeCtrl.InsertItem(Clayer,root,TVI_LAST);
			m_wndTreeCtrl.SetItemColor(htree,RGB(0,0,128));
			m_wndTreeCtrl.SetCheck(htree,TRUE);
		}
	}
	
	HTREEITEM item = m_wndTreeCtrl.InsertItem(A2CT(pNode->getName().c_str()),htree);
	
	m_wndTreeCtrl.SetCheck(item);
	ExpandAll(m_wndTreeCtrl.GetRootItem());

	if (CString(pNode->getName().c_str()) == m_sDefLayer || CString(pNode->getName().c_str()) == m_sDefLayer + _T(".shp"))
	{
		m_wndTreeCtrl.SetItemBold(item, true);
	}


	x3::Object<IViewer3D> spViewer3D(pBuddy);
	if( !spViewer3D.valid() )
		return;

	//////////////////////////////////////////////////////////////////////////
	CStringArray* aLayers = spViewer3D->GetLayersArray();
	CStringArray* aLayersCheck = spViewer3D->GetLayerCheck();
	for (int i = 0; i < aLayers->GetCount(); i++)
	{
		CString sL = aLayers->GetAt(i);
		std::string fp = T2A(sL);
		std::string filename = osgDB::getStrippedName(fp);
		if (filename == pNode->getName())
		{
			CString sCheck = aLayersCheck->GetAt(i);
			if (sCheck == L"1")
			{
				m_wndTreeCtrl.SetCheck(item);
			}
			else
			{
				m_wndTreeCtrl.SetCheck(item, FALSE);
				pNode->setNodeMask(0);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	const osg::BoundingSphere& bs = pNode->computeBound();
	if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
		return;

	osg::Vec3d v3f = bs.center();
	osg::Vec3d v3dOut;

	std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
	std::string FileNameEx = filename.substr(filename.find_last_of( '\\' ) + 1);
	std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));

	const SpatialReference* epsg2382;
	std::string ProjFile = FilePath + "\\" + FileNameNoEx + ".prj";

	if (osgDB::fileExists(ProjFile))
	{
		std::string sContent = spViewer3D->GetFileContent(ProjFile);
		epsg2382 = SpatialReference::create(sContent);

	}
	else
		epsg2382 = SpatialReference::create("wgs84");

#ifdef _2D
#else
	const SpatialReference* wgs84 = spViewer3D->getMapNode()->getMapSRS();
	epsg2382->transform(v3f, wgs84, v3dOut);
#endif


	CString sData;
	sData.Format(L"%f,%f", v3dOut.x(), v3dOut.y());

	//m_wndTreeCtrl.SetItemData(item, (DWORD_PTR)sData.AllocSysString());
	m_wndTreeCtrl.SetItemData(item, (DWORD_PTR)pNode);
	
	

}

HTREEITEM CDataManager::FindItem(HTREEITEM item, CString strText)
{
    HTREEITEM hFind;
    if (item == NULL)
    {
        return NULL;
    }
    while(item != NULL)
    {
        if (m_wndTreeCtrl.GetItemText(item) == strText)
        {
            return item;
        }
        if (m_wndTreeCtrl.ItemHasChildren(item))
        {
            item = m_wndTreeCtrl.GetChildItem(item);
            hFind = FindItem(item, strText);
            if (hFind)
            {
                return hFind;
            }
            else
            {
                item = m_wndTreeCtrl.GetNextSiblingItem(m_wndTreeCtrl.GetParentItem(item));
            }
        }
        else
        {
            item = m_wndTreeCtrl.GetNextSiblingItem(item);
            if (item == NULL)
            {
                return NULL;
            }
        }
    }
    return item;
}

void CDataManager::OnNodeRemove(x3::IObject* pBuddy, osg::Node* pNode, osg::Node* pParentNode)
{
	if (bInterDel)
	{
		return;
	}

	if( pNode == NULL || pBuddy == NULL )
		return;


	HTREEITEM hRoot = m_wndTreeCtrl.GetRootItem();
	HTREEITEM hFind = FindItem(hRoot, CString(pNode->getName().c_str()));

	m_wndTreeCtrl.DeleteItem(hFind);

	hRoot = m_wndTreeCtrl.GetRootItem();
	if (m_wndTreeCtrl.ItemHasChildren(hRoot))
	{
		HTREEITEM hChild = m_wndTreeCtrl.GetChildItem(hRoot);
		while(hChild)
		{
			if ( !m_wndTreeCtrl.ItemHasChildren(hChild))
			{
				m_wndTreeCtrl.DeleteItem(hChild);
				break;
			}
			hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
		}
	}
}

void CDataManager::OnNodeChange(x3::IObject* pBuddy, osg::Node* pNode)
{
}

void CDataManager::OnNodeSelected(x3::IObject* pBuddy, osg::Node* pNode)
{
}

void CDataManager::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
    /*
    HTREEITEM hCurSel = m_wndTreeCtrl.GetSelectedItem();
    CString s = (BSTR)m_wndTreeCtrl.GetItemData(hCurSel);

    double lon = _tstof(s.Left(s.Find(L",")));
    double lat = _tstof(s.Right(s.GetLength() - s.Find(L",") - 1));

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if( !spViewer3D.valid() )
    	return;

    spViewer3D->FlyToViewport(lon,lat, 100);
    */
    HTREEITEM hCurSel = m_wndTreeCtrl.GetSelectedItem();
    osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hCurSel);
	if (!pNode)
	{
		return;
	}
    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if( !spViewer3D.valid() )
        return;

    const osg::BoundingSphere& bs = pNode->getBound();
    if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
        return;

    osg::Vec3d v3f = bs.center();
#ifdef _2D
    spViewer3D->CenterNode(pNode, bs.radius());
#else
    spViewer3D->FlyToViewport(v3f.x(), v3f.y(), 100);
#endif

    *pResult = 0;
}
void CDataManager::OnNMClickDataManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	UINT uFlag;
	bool bCheck = true;
	GetCursorPos(&point);
	m_wndTreeCtrl.ScreenToClient(&point);
	HTREEITEM hTree = m_wndTreeCtrl.HitTest(point, &uFlag);
	if((TVHT_NOWHERE & uFlag))
	{
		return;
	}

	if (hTree && (TVHT_ONITEMSTATEICON & uFlag))
	{
		if (m_wndTreeCtrl.ItemHasChildren(hTree))
		{	
			if (hTree)
			{
				bCheck = m_wndTreeCtrl.GetCheck(hTree);
				SetChildMask(hTree,!bCheck);
				/*HTREEITEM hChild = m_wndTreeCtrl.GetChildItem(hTree);
				while (hChild)
				{
					if (m_wndTreeCtrl.ItemHasChildren(hChild))
					{
						HTREEITEM hcc = m_wndTreeCtrl.GetChildItem(hChild);
						while (hcc)
						{
							osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hcc);
							if(pNode)
								pNode->setNodeMask(!bCheck);
							hcc = m_wndTreeCtrl.GetNextSiblingItem(hcc);
						}		
					}
					else
						while (hChild)
						{
							osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hChild);
							if(pNode)
								pNode->setNodeMask(!bCheck);
							hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
						}
					hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
				}*/
			}

			SetChildCheck(hTree, !bCheck);
			*pResult = 0;
			return;
		}
		m_wndTreeCtrl.SelectItem(hTree);
		CString temp = m_wndTreeCtrl.GetItemText(hTree) + L"\r\n";
		TRACE(temp);
		bCheck = m_wndTreeCtrl.GetCheck(hTree);

		if (bCheck)
		{
			TRACE("取消选择");
			osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hTree);
			if (pNode)
			{
				pNode->setNodeMask(0);
			}

			SetLayerCheck(m_wndTreeCtrl.GetItemText(hTree), false);
		}
		else
		{
			TRACE("选中");
			osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hTree);
			if (pNode)
			{
				pNode->setNodeMask(1);
			}
			SetLayerCheck(m_wndTreeCtrl.GetItemText(hTree), true);
		}

		SetChildCheck(hTree, !bCheck);
		SetParentCheck(hTree, bCheck);
	}

	*pResult = 0;
}

void CDataManager::SetLayerCheck(CString sName, bool bCheck)
{
    USES_CONVERSION;
    //////////////////////////////////////////////////////////////////////////
    CStringArray* aLayers = m_spViewer3D->GetLayersArray();
    CStringArray* aLayersCheck = m_spViewer3D->GetLayerCheck();
    for (int i = 0; i < aLayers->GetCount(); i++)
    {
        CString sL = aLayers->GetAt(i);
        std::string fp = T2A(sL);
        std::string filename = osgDB::getStrippedName(fp);
        if (CString(filename.c_str()) == sName)
        {
            if (bCheck)
            {
                aLayersCheck->SetAt(i, L"1");
            }
            else
                aLayersCheck->SetAt(i, L"0");
        }
    }
    //////////////////////////////////////////////////////////////////////////
}

void CDataManager::SetChildCheck(HTREEITEM hTree, BOOL bCheck)
{
    m_wndTreeCtrl.Expand(hTree, TVE_EXPAND);
    hTree = m_wndTreeCtrl.GetChildItem(hTree);
    while (hTree)
    {
        m_wndTreeCtrl.SetCheck(hTree, bCheck);
        SetChildCheck(hTree, bCheck);
        hTree = m_wndTreeCtrl.GetNextSiblingItem(hTree);
    }
}

void CDataManager::SetChildMask(HTREEITEM hTree,BOOL bCheck)
{
	if (!m_wndTreeCtrl.ItemHasChildren(hTree))
	{
		osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hTree);
		if(pNode)
			pNode->setNodeMask(bCheck);
		return;
	}
	HTREEITEM hChild = m_wndTreeCtrl.GetChildItem(hTree);
	while (hChild)
	{
		SetChildMask(hChild,bCheck);
		hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
	}
}

void CDataManager::RemoveAllNode(HTREEITEM hTree)
{
	if (!m_wndTreeCtrl.ItemHasChildren(hTree))
	{
		osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hTree);
		if (pNode)
		{
			m_spViewer3D->RemoveNode(pNode);
		}
		return;
	}
	HTREEITEM hChild = m_wndTreeCtrl.GetChildItem(hTree);
	while (hChild)
	{
		RemoveAllNode(hChild);
		hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
	}
}

void CDataManager::SetParentCheck(HTREEITEM hTree, BOOL bCheck)
{
    HTREEITEM hParent = m_wndTreeCtrl.GetParentItem(hTree);
    if (hParent)
    {
        HTREEITEM hChild = m_wndTreeCtrl.GetChildItem(hParent);
        while (hChild)
        {
            if (hChild == hTree)
            {
                hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
                continue;
            }
            BOOL bflag = m_wndTreeCtrl.GetCheck(hChild);

            if (bCheck == bflag)
            {
                return;
            }
            hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
        }
        m_wndTreeCtrl.SetCheck(hParent, !bCheck);
        SetParentCheck(hParent, bCheck);
    }
}

void CDataManager::ExpandAll(HTREEITEM hroot)
{
	if (!m_wndTreeCtrl.ItemHasChildren(hroot))
	{
		return;
	}
	HTREEITEM item = m_wndTreeCtrl.GetChildItem(hroot);
	while (item)
	{
		ExpandAll(item);	
		item = m_wndTreeCtrl.GetNextItem(item, TVGN_NEXT);
	}
	m_wndTreeCtrl.Expand(hroot,TVE_EXPAND);
}

CPoint point;
void CDataManager::OnRBClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    GetCursorPos(&point);
    CPoint pointInTree = point;
    m_wndTreeCtrl.ScreenToClient(&pointInTree);

    HTREEITEM item;
    UINT flag = TVHT_ONITEM;
    item = m_wndTreeCtrl.HitTest(pointInTree, &flag);
	if (m_wndTreeCtrl.ItemHasChildren(item))
	{
		return;
	}
    if(item != NULL)
    {
        CMenu m_Menu, *p_Menu = NULL ;
        {
            m_Menu.LoadMenu( IDR_MENU1) ;
            p_Menu = (CMenu*) m_Menu.GetSubMenu(0);
            if( p_Menu != NULL)
                p_Menu->TrackPopupMenu( TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
            p_Menu = NULL;
        }
    }
    *pResult = 0;
}

void CDataManager::OnDellayer()
{
    USES_CONVERSION;

    m_wndTreeCtrl.ScreenToClient(&point);

    HTREEITEM item;
    UINT flag = TVHT_ONITEM;
    item = m_wndTreeCtrl.HitTest(point, &flag);
    if(item != NULL)
    {
        CString temp = m_wndTreeCtrl.GetItemText(item);
		std::string stemp = T2A(temp);
		stemp = stemp.substr(0, stemp.find_last_of( '.' ));
        osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(item);
		if (!pNode)
		{
			return;
		}
		if (pNode->getName() == "线框" || pNode->getName().find("兴趣点(")!=-1)
		{
			MessageBox(_T("不允许删除该项！"));
			return;
		}
        x3::Object<IViewer3D> spViewer3D(m_spBuddy);
        if( !spViewer3D.valid() )
            return;
        spViewer3D->RemoveNode(pNode);
				
        CStringArray* aLayers = spViewer3D->GetLayersArray();
        CStringArray* aLayerCheck = spViewer3D->GetLayerCheck();
        for (int i = 0; i < aLayers->GetCount(); i++)
        {
            CString sL = aLayers->GetAt(i);
            std::string fp = T2A(sL);
            std::string filename = osgDB::getStrippedName(fp);
            if (filename == stemp)
            {
                aLayers->RemoveAt(i);
                aLayerCheck->RemoveAt(i);
            }
        }
    }
}

void CDataManager::OnSetDefLayer()
{
    USES_CONVERSION;

    m_wndTreeCtrl.ScreenToClient(&point);

    HTREEITEM item;
    UINT flag = TVHT_ONITEM;
    item = m_wndTreeCtrl.HitTest(point, &flag);
    if(item != NULL)
    {
        UnBold();
        m_wndTreeCtrl.SetItemBold(item, TRUE);
        m_sDefLayer = m_wndTreeCtrl.GetItemText(item);
        m_spViewer3D->SetDefLayer(m_sDefLayer);
    }
}

void CDataManager::UnBold()
{
    HTREEITEM h;

    HTREEITEM hRoot = m_wndTreeCtrl.GetRootItem();
    h = FindItem(hRoot, m_sDefLayer);

    if (h != NULL)
    {
        m_wndTreeCtrl.SetItemBold(h, FALSE);
		m_wndTreeCtrl.SetItemColor(hRoot,RGB(0,0,0));
    }
}

void CDataManager::OnKeyChangeEditName()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码

}

CString CDataManager::FindDefSHPFile(CString m_sDefLayer)
{
	USES_CONVERSION;
	CStringArray* a = m_spViewer3D->GetLayersArray();

	if (m_sDefLayer == _T(""))
	{
		return L"";
	}
	else
	{
		if (string(W2A(m_sDefLayer)).find(".shp") == -1 && string(W2A(m_sDefLayer)).find(".SHP") == -1)
		{
			return L"";
		}
		for (int i = 0; i < a->GetCount(); i++)
		{
			if (a->GetAt(i).Find(m_sDefLayer) > -1)
			{
				return a->GetAt(i);
			}
		}
		return L"";
	}
}

CString CDataManager::FindSHPFile()
{
	USES_CONVERSION;
    CStringArray* a = m_spViewer3D->GetLayersArray();
	
    if (m_sDefLayer == _T(""))
    {
        for (int i = 0; i < a->GetCount(); i++)
        {
            if (a->GetAt(i).Right(4) == L".shp")
            {
                return a->GetAt(i);
            }
        }
        return L"";
    }
    else
    {
		if (string(W2A(m_sDefLayer)).find(".shp") == -1)
		{
			return L"";
		}
        for (int i = 0; i < a->GetCount(); i++)
        {
            if (a->GetAt(i).Find(m_sDefLayer) > -1)
            {
                return a->GetAt(i);
            }
        }
        return L"";
    }
}

bool IsContainAlpha(CString str)
{
    bool bflag = false;
    int length = str.GetLength();
    for(int i = 0 ; i < length ; i++)
    {
        int num = str.GetAt(i);
        if((num >= 'a' && num <= 'z') || (num >= 'A' && num <= 'Z'))
        {
            bflag = true;
            break;
        }
    }

    return bflag;
}

void CDataManager::DoSearchKey(CString sShpFile,CString m_key)
{
	USES_CONVERSION;
    osgEarth::Drivers::OGRFeatureOptions ogrOpt;
    std::string path = W2A(sShpFile);
    if (path.length() == 0)
    {
        return;
    }

    if (int(path.find('/')) > -1)
    {
        CString sProjPath = m_spViewer3D->GetProjectFilePath();
        CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
        std::string FileNameEx = path.substr(path.find_last_of( '/' ) + 1);

        path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
    }
    //如果关键字为空，则返回
    if (m_key == _T(""))
    {
    	return;
    }
    
    ogrOpt.url() = path;

    m_cList.DeleteAllItems();

    features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
    features->initialize();

    osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();

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

                if ( sName == m_key )
                {
                    m_cList.InsertItem(0, _T("1"));
                    m_cList.SetItemText (0, 0, sName);
                    m_cList.SetItemText (0, 1, sValue);
                    vFeature.insert(vFeature.begin(), feature);
                    v.insert(v.begin(), part);
                    break;
                }
            }  
        }
    }
}

void CDataManager::DoSearch(CString sShpFile)
{
    USES_CONVERSION;
    osgEarth::Drivers::OGRFeatureOptions ogrOpt;
    std::string path = W2A(sShpFile);
    if (path.length() == 0)
    {
        return;
    }

    if (int(path.find('/')) > -1)
    {
        CString sProjPath = m_spViewer3D->GetProjectFilePath();
        CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
        std::string FileNameEx = path.substr(path.find_last_of( '/' ) + 1);

        path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
    }


    /*
    //如果关键字为空，则显示所有数据
    if (m_sKey == _T(""))
    {
    	return;
    }
    */
    ogrOpt.url() = path;

    m_cList.DeleteAllItems();

    features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
    features->initialize();

    osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();

	
    while(cursor->hasMore())
    {
        //输出feature中的信息
        osgEarth::Features::Feature* feature = cursor->nextFeature();
        osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);


        //std::string sHeight = feature->getString("Elevation");
        //double height = atof(sHeight.data());

        while(parts.hasMore())
        {
            osgEarth::Symbology::Geometry* part = parts.next();

            if (m_sKey != _T(""))
            {
                const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
                for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
                {
                    CString sName = CString(i->first.c_str());
                    CString sValue = CString(i->second.getString().c_str());

                    if ( (sValue.Find(m_sKey) >= 0 && sValue != _T("")) )
                    {
                        m_cList.InsertItem(0, _T("1"));
                        m_cList.SetItemText (0, 0, sName);
                        m_cList.SetItemText (0, 1, sValue);
                        vFeature.insert(vFeature.begin(), feature);
                        v.insert(v.begin(), part);
                        break;
                        //m_cList.SetItemData(0, (DWORD_PTR)part);
                    }
                }
            }
            else
            {
                const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
                for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
                {
                    CString sName = CString(i->first.c_str());
                    CString sValue = CString(i->second.getString().c_str());

                    /*if (IsContainAlpha(sName) )
                    {
                        continue;
                    }*/
					std::string str = i->second.getString();
					int n = str.find(".");
					if ( n != -1)
					{
						str = str.substr(0,n+3<=str.length()?n+3:str.length());
					}
					
                    m_cList.InsertItem(0, _T("1"));
                    m_cList.SetItemText (0, 0, sName);
                    //m_cList.SetItemText (0, 1, CString(str.c_str()));
					m_cList.SetItemText (0, 1, _T(""));
                    //vFeature.insert(vFeature.begin(), feature);
                    //v.insert(v.begin(), part);
                    //break;
                }
				return;
            }

        }
    }
}

osg::ref_ptr<osg::Geode> CDataManager::AddPolygonNode(osg::Vec3Array* arr,osg::Vec4 color)
{
    USES_CONVERSION;

	//color.a() = 0.2;
 //   osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
 //   shared_colors->push_back(color);
	//osg::ref_ptr<osg::Vec4Array> shared_colors1 = new osg::Vec4Array;
	//shared_colors1->push_back(osg::Vec4(color.r(),color.g(),color.b(),1));

 //   // same trick for shared normal.
 //   osg::ref_ptr<osg::Vec3Array> shared_normals = new osg::Vec3Array;
 //   shared_normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

 //   osg::ref_ptr<osg::Geode> geode = new osg::Geode;
 //   osg::Geometry* polyGeom = new osg::Geometry();
	//osg::Geometry* polyGeom1 = new osg::Geometry();

 //   int numCoords = arr->getNumElements();
 //   polyGeom->setVertexArray(arr);
	//polyGeom1->setVertexArray(arr);

 //   polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, numCoords));
	//polyGeom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, numCoords));
 //   // use the shared color array.
 //   polyGeom->setColorArray(shared_colors1.get(), osg::Array::BIND_OVERALL);
	//polyGeom1->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
 //   // use the shared normal array.
 //   polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);
	//polyGeom1->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);
 //   geode->addDrawable(polyGeom);
	//geode->addDrawable(polyGeom1);

 //   CString s;
 //   s.Format(_T("线"));
 //   geode->setName(W2A(s.GetBuffer(0)));

 //   osg::StateSet* ss = geode->getOrCreateStateSet();
 //   ss->setMode(GL_BLEND, osg::StateAttribute::ON);
 //   ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
 //   ss->setAttributeAndModes( new osg::LineWidth(3.5f) );
	//ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);

	// same trick for shared normal.
	osg::ref_ptr<osg::Vec3Array> shared_normals = new osg::Vec3Array;
	shared_normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::Geometry* polyGeom = new osg::Geometry();

	int numCoords = arr->getNumElements();
	polyGeom->setVertexArray(arr);

	polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, numCoords));

	// use the shared color array.
	polyGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);

	// use the shared normal array.
	polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

	geode->addDrawable(polyGeom);

	CString s;
	s.Format(_T("线"));
	geode->setName(W2A(s.GetBuffer(0)));

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setMode(GL_BLEND, osg::StateAttribute::ON);
	ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	ss->setAttributeAndModes( new osg::LineWidth(3.5f) );
	
	return geode.get();
}

void CDataManager::ListByKey(CString m_key)
{
	CString sShpFile = FindSHPFile();
	if (sShpFile == _T(""))
	{
		return;
	}
	vFeature.clear();
	DoSearchKey(sShpFile,m_key);
}

void CDataManager::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	USES_CONVERSION;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nItem = m_cList.GetSelectionMark();
	if (nItem >= 0)
	{
		if (m_pAttInfoWin != NULL)
		{
			m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
		}
		int type = 1;
		CString str = m_cList.GetItemText(nItem, 0);
		CString strValue = m_cList.GetItemText(nItem, 1);
		if (strValue.IsEmpty())
		{
			ListByKey(str);
			return;
		}
		DWORD_PTR  dd = m_cList.GetItemData(nItem);
		//osgEarth::Symbology::Geometry* part = (osgEarth::Symbology::Geometry*)(m_cList.GetItemData(nItem));
		m_geoPart = v.at(nItem);

		std::string sGC = vFeature.at(nItem)->getString("高程");
		std::string sDG = vFeature.at(nItem)->getString("底高");
		std::string sDG2 = vFeature.at(nItem)->getString("顶高");
		std::string sTH = vFeature.at(nItem)->getString("TH");
		std::string sZ = vFeature.at(nItem)->getString("z");

		if (!sGC.empty() && !sDG.empty() && !sDG2.empty())
		{
			double dGC = atof(sGC.data());
			double dDG = atof(sDG.data());
			double dDG2 = atof(sDG2.data());

			if (gTemp != nullptr){
				m_spViewer3D->RemoveNode(gTemp);
				gTemp = nullptr;
			}
			osg::Vec4 color;
			std::string sTs = vFeature.at(nItem)->getString("特殊人群");
			if (!sTs.empty())
			{
				if (sTs == "社区校正人员")
				{
					color = osg::Vec4(0,0,1,1);
					MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2,color);
				}
				else if (sTs == "刑满释放人员")
				{
					color = osg::Vec4(0,0,0,1);
					MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2,color);
				}
				else if (sTs == "吸毒人员")
				{
					color = osg::Vec4(1,0,0,1);
					MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2,color);
				}
				else
					MakeFloor3DBorder(vFeature.at(nItem), dGC + dDG, dGC + dDG2);
			}
			else
				MakeFloor3DBorder(vFeature.at(nItem), dGC + dDG, dGC + dDG2);
			type = 2;
		}
		else if (!sTH.empty())
			MakeBuildingStruct(sTH,sZ);
		else
		{
			std::string sNm = vFeature.at(nItem)->getString("name");
			if (!sNm.empty())
			{
				hideAll();
				
				if (m_pInternetInfoWin != NULL)
				{
					CBackgroundDlg* dlg = (CBackgroundDlg*)m_pInternetInfoWin->parentBk;
					if (dlg)
					{
						dlg->DestroyWindow();
					}
					m_pInternetInfoWin->DestroyWindow();
				}
				AFX_MANAGE_STATE_EX;
				m_pInternetInfoWin = new CInternetInfoDlg();
				m_pInternetInfoWin->parent = (CObject*)this;
				m_pInternetInfoWin->Create(IDD_DIALOG_INFO_NET);
				CBackgroundDlg* bkDlg = new CBackgroundDlg();
				bkDlg->Create(IDD_DIALOG_BACKGROUND);
				m_pInternetInfoWin->parentBk = (CObject*)bkDlg;
				RECT r;
				m_pInternetInfoWin->GetWindowRect(&r);
				int nWidth = r.right - r.left;
				int nHeight = r.bottom - r.top;
				r.left = m_spViewer3D->rectView3D.left + 20;
				r.top = m_spViewer3D->rectView3D.top + 20;
				r.right = r.left + nWidth;
				r.bottom = r.top + nHeight; 
				m_pInternetInfoWin->MoveWindow(&r);
				bkDlg->MoveWindow(&r);
				double x = atof(vFeature.at(nItem)->getString("x").data());
				double y = atof(vFeature.at(nItem)->getString("y").data());
				double z = atof(vFeature.at(nItem)->getString("z").data());
				ShowPOI(x, y, z + 2, CString(sNm.c_str()));

				if (sNm == "物业服务中心" )
				{
					if (!g_DBconnector->bConnect)
					{
						if (m_pHtmlInfoWin == NULL)
						{
							AFX_MANAGE_STATE_EX;
							m_pHtmlInfoWin = new CHtmlDlg(); 
							m_pHtmlInfoWin->parent = (CObject*)this;
							m_pHtmlInfoWin->Create(IDD_DIALOG_HTMLINFO);
							m_pHtmlInfoWin->ShowWindow(SW_HIDE);
							RECT r;
							m_pHtmlInfoWin->GetWindowRect(&r);
							int nWidth = r.right - r.left;
							int nHeight = r.bottom - r.top;
							r.left = m_spViewer3D->rectView3D.left + 20;
							r.top = m_spViewer3D->rectView3D.top + 20;
							r.right = r.left + nWidth;
							r.bottom = r.top + nHeight; 
							m_pHtmlInfoWin->MoveWindow(&r);
						}
						m_pHtmlInfoWin->setHTML("Data\\DRVdata\\swiperecord\\index.html");
						std::string sW = vFeature.at(nItem)->getString("width");
						std::string sH = vFeature.at(nItem)->getString("height");
						int dW = atof(sW.data());
						int dH = atof(sH.data());
						m_pHtmlInfoWin->changeHTMLsize(dW,dH);
						m_pHtmlInfoWin->setupPolygonRegion();
						m_pHtmlInfoWin->ShowWindow(SW_SHOW);
						m_pHtmlInfoWin->Invalidate();
					}
					else
					{
						m_pInternetInfoWin->_type = 3;
						m_pInternetInfoWin->selfDefShowWindow(SW_SHOW);
						m_pInternetInfoWin->AutoAdjustDlg();
					}		
				}
				else if (sNm == "车辆出口")
				{
					if (!g_DBconnector->bConnect)
					{
						if (m_pShowPictureInfoWin == NULL)
						{
							AFX_MANAGE_STATE_EX;
							m_pShowPictureInfoWin = new CShowPictureDlg(); 
							m_pShowPictureInfoWin->parent = (CObject*)this;
							m_pShowPictureInfoWin->Create(IDD_DIALOG_PICTUREINFO);
							m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
							RECT r;
							m_pShowPictureInfoWin->GetWindowRect(&r);
							int nWidth = r.right - r.left;
							int nHeight = r.bottom - r.top;
							r.left = m_spViewer3D->rectView3D.left + 20;
							r.top = m_spViewer3D->rectView3D.top + 20;
							r.right = r.left + nWidth;
							r.bottom = r.top + nHeight; 
							m_pShowPictureInfoWin->MoveWindow(&r);
						}
						std::string sW = vFeature.at(nItem)->getString("width");
						std::string sH = vFeature.at(nItem)->getString("height");
						int dW = atof(sW.data());
						int dH = atof(sH.data());

						m_pShowPictureInfoWin->changePictureSize(dW,dH);
						m_pShowPictureInfoWin->setupPolygonRegion();
						m_pShowPictureInfoWin->setPath("Data\\DRVdata\\车辆卡口.png");
						m_pShowPictureInfoWin->setColorType(5);
						m_pShowPictureInfoWin->ShowWindow(SW_SHOW);
						m_pShowPictureInfoWin->Invalidate();
					}
					else
					{
						m_pInternetInfoWin->_type = 1;
						m_pInternetInfoWin->selfDefShowWindow(SW_SHOW);
						m_pInternetInfoWin->AutoAdjustDlg();
					}			
				}
				else if (sNm == "车辆入口" )
				{
					if (!g_DBconnector->bConnect)
					{
						if (m_pShowPictureInfoWin == NULL)
						{
							AFX_MANAGE_STATE_EX;
							m_pShowPictureInfoWin = new CShowPictureDlg(); 
							m_pShowPictureInfoWin->parent = (CObject*)this;
							m_pShowPictureInfoWin->Create(IDD_DIALOG_PICTUREINFO);
							m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
							RECT r;
							m_pShowPictureInfoWin->GetWindowRect(&r);
							int nWidth = r.right - r.left;
							int nHeight = r.bottom - r.top;
							r.left = m_spViewer3D->rectView3D.left + 20;
							r.top = m_spViewer3D->rectView3D.top + 20;
							r.right = r.left + nWidth;
							r.bottom = r.top + nHeight; 
							m_pShowPictureInfoWin->MoveWindow(&r);
						}
						std::string sW = vFeature.at(nItem)->getString("width");
						std::string sH = vFeature.at(nItem)->getString("height");
						int dW = atof(sW.data());
						int dH = atof(sH.data());

						m_pShowPictureInfoWin->changePictureSize(dW,dH);
						m_pShowPictureInfoWin->setupPolygonRegion();
						m_pShowPictureInfoWin->setPath("Data\\DRVdata\\车辆卡口.png");
						m_pShowPictureInfoWin->setColorType(5);
						m_pShowPictureInfoWin->ShowWindow(SW_SHOW);
						m_pShowPictureInfoWin->Invalidate();
					}
					else
					{
						m_pInternetInfoWin->_type = 2;
						m_pInternetInfoWin->selfDefShowWindow(SW_SHOW);
						m_pInternetInfoWin->AutoAdjustDlg();
					}	
				}
				else if (sNm == "井盖")
				{
					std::vector<CString> vec;
					if (vmanHole.LData.empty())
					{
						for (int i = 0;i<=10;++i)
						{
							vec.push_back(_T(" "));
						}
					}
					else
						COMFUNC::split(vmanHole.LData.back(),"&&",vec);
					ShowAttWin();
					m_pAttInfoWin->RemoveAllRows();
					std::string s;
					COMFUNC::dateTrans(W2A(vec[9]),s);
					m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
					m_pAttInfoWin->InsertRow(_T("水位状态"),vec[8]);
					m_pAttInfoWin->InsertRow(_T("终端电压"),vec[7]);
					m_pAttInfoWin->InsertRow(_T("报警频率"),vec[6]);
					m_pAttInfoWin->InsertRow(_T("上报频率"),vec[5]);
					m_pAttInfoWin->InsertRow(_T("当前角度"),vec[4]);
					m_pAttInfoWin->InsertRow(_T("报警角度"),vec[3]);
					m_pAttInfoWin->InsertRow(_T("功能码"),vec[2]);
					m_pAttInfoWin->InsertRow(_T("安装位置"),vec[1]);
					m_pAttInfoWin->InsertRow(_T("编号"),vec[0]);
					
					USES_CONVERSION;
					int w = s.size() + 3;
					w = w<5?5:w;
					m_pAttInfoWin->changeDlgHeight((10+1)*g_RowHeight);
					//m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					m_pAttInfoWin->suitHeightAndWidth(10,w);//调整高度和列宽
					m_pAttInfoWin->UpdateWindow();
					m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
					return;
				}
				else if (sNm == "烟感器")
				{
					std::vector<CString> vec;
					if (vsmokeSensor.LData.empty())
					{
						for (int i = 0;i<6;++i)
						{
							vec.push_back(_T(" "));
						}
					}
					else
						COMFUNC::split(vsmokeSensor.LData.back(),"&&",vec);
					ShowAttWin();
					m_pAttInfoWin->RemoveAllRows();

					std::string s;
					COMFUNC::dateTrans(W2A(vec[5]),s);
					m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
					m_pAttInfoWin->InsertRow(_T("测试键"),vec[4]);
					m_pAttInfoWin->InsertRow(_T("电池状态"),vec[3]);
					m_pAttInfoWin->InsertRow(_T("烟雾状态"),vec[2]);
					m_pAttInfoWin->InsertRow(_T("安装位置"),vec[1]);
					m_pAttInfoWin->InsertRow(_T("编号"),vec[0]);
					
					/*m_pAttInfoWin->InsertRow(_T("设备状态"),vec[5]);	
					m_pAttInfoWin->InsertRow(_T("设备位置"),vec[6] + _T(" ") + vec[7]);*/

					USES_CONVERSION;
					int w = s.size() + 3;
					w = w<5?5:w;
					m_pAttInfoWin->changeDlgHeight((6+1)*g_RowHeight);
					//m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					m_pAttInfoWin->suitHeightAndWidth(6,w);//调整高度和列宽
					m_pAttInfoWin->UpdateWindow();
					m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
					return;
				}
				else if (sNm == "门磁")
				{
					std::vector<CString> vec;
					if (vdoorSensor.LData.empty())
					{
						for (int i = 0;i<=4;++i)
						{
							vec.push_back(_T(" "));
						}
					}
					else
						COMFUNC::split(vdoorSensor.LData.back(),"&&",vec);
					ShowAttWin();
					m_pAttInfoWin->RemoveAllRows();
					std::string s;
					COMFUNC::dateTrans(W2A(vec[4]),s);
					m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
					m_pAttInfoWin->InsertRow(_T("RF类型"),vec[3]);
					m_pAttInfoWin->InsertRow(_T("门禁状态"),vec[2]);
					m_pAttInfoWin->InsertRow(_T("安装位置"),vec[1]);
					m_pAttInfoWin->InsertRow(_T("编号"),vec[0]);
					
					USES_CONVERSION;
					
					int w = s.size() + 3;
					w = w<5?5:w;
					m_pAttInfoWin->changeDlgHeight((4+1)*g_RowHeight);
					//m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					m_pAttInfoWin->suitHeightAndWidth(4,w);//调整高度和列宽
					m_pAttInfoWin->UpdateWindow();
					m_pAttInfoWin->ShowWindow(SW_SHOW);
					return;
				}
				else if (sNm == "门禁视频")
				{	
					if (m_pCVMediaPlayerInfoWin == NULL)
					{
						AFX_MANAGE_STATE_EX;
						m_pCVMediaPlayerInfoWin = new CCVMediaPlayerDlg(); 
						m_pCVMediaPlayerInfoWin->parent = (CObject*)this;
						m_pCVMediaPlayerInfoWin->Create(IDD_DIALOG_CVPLAYERINFO);
						m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
						RECT r;
						m_pCVMediaPlayerInfoWin->GetWindowRect(&r);
						int nWidth = r.right - r.left;
						int nHeight = r.bottom - r.top;
						r.left = m_spViewer3D->rectView3D.left + 20;
						r.top = m_spViewer3D->rectView3D.top + 20;
						r.right = r.left + nWidth;
						r.bottom = r.top + nHeight; 
						m_pCVMediaPlayerInfoWin->MoveWindow(&r);
					}
					std::string sW = vFeature.at(nItem)->getString("width");
					std::string sH = vFeature.at(nItem)->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());
					m_pCVMediaPlayerInfoWin->changePlayerSize(dW,dH);
					m_pCVMediaPlayerInfoWin->setupPolygonRegion();
					m_pCVMediaPlayerInfoWin->KillTimer(1);
					m_pCVMediaPlayerInfoWin->open("Data\\DRVdata\\door.mp4");
					m_pCVMediaPlayerInfoWin->ShowWindow(SW_SHOWNOACTIVATE);
					m_pCVMediaPlayerInfoWin->Invalidate();
				}
				else if (sNm == "西门门禁视频")
				{
					if (g_DBconnector->bConnect)
					{
						std::string source = vFeature.at(nItem)->getString("source");
						if (m_pVLCPlayerInfoWin == NULL)
						{
							AFX_MANAGE_STATE_EX;
							m_pVLCPlayerInfoWin = new CDLGVLCPlayer();
							m_pVLCPlayerInfoWin->parent = (CObject*)this;
							m_pVLCPlayerInfoWin->Create(IDD_DIALOG_VLC);
							RECT r;
							m_pVLCPlayerInfoWin->GetWindowRect(&r);
							int nWidth = r.right - r.left;
							int nHeight = r.bottom - r.top;
							r.left = m_spViewer3D->rectView3D.left + 20;
							r.top = m_spViewer3D->rectView3D.top + 20;
							r.right = r.left + nWidth;
							r.bottom = r.top + nHeight; 
							m_pVLCPlayerInfoWin->MoveWindow(&r);
							m_pVLCPlayerInfoWin->setRtspAddress(source);
							m_pVLCPlayerInfoWin->connectRTSP();
						}
						m_pVLCPlayerInfoWin->ShowWindow(SW_SHOW);
					}
				}
				else
				{
					if (m_pShowPictureInfoWin == NULL)
					{
						AFX_MANAGE_STATE_EX;
						m_pShowPictureInfoWin = new CShowPictureDlg(); 
						m_pShowPictureInfoWin->parent = (CObject*)this;
						m_pShowPictureInfoWin->Create(IDD_DIALOG_PICTUREINFO);
						m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
						RECT r;
						m_pShowPictureInfoWin->GetWindowRect(&r);
						int nWidth = r.right - r.left;
						int nHeight = r.bottom - r.top;
						r.left = m_spViewer3D->rectView3D.left + 20;
						r.top = m_spViewer3D->rectView3D.top + 20;
						r.right = r.left + nWidth;
						r.bottom = r.top + nHeight; 
						m_pShowPictureInfoWin->MoveWindow(&r);
					}
					std::string sW = vFeature.at(nItem)->getString("width");
					std::string sH = vFeature.at(nItem)->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());

					m_pShowPictureInfoWin->changePictureSize(dW,dH);
					m_pShowPictureInfoWin->setupPolygonRegion();
					m_pShowPictureInfoWin->setPath("Data\\DRVdata\\1024商家信息.PNG");
					m_pShowPictureInfoWin->setColorType(5);
					m_pShowPictureInfoWin->ShowWindow(SW_SHOWNOACTIVATE);
					m_pShowPictureInfoWin->Invalidate();
				}
			}
			return;
			/*std::string sJD = vFeature.at(nItem)->getString("x");
			std::string sWD = vFeature.at(nItem)->getString("y");
			std::string sH = vFeature.at(nItem)->getString("z");
			double dJD = atof(sJD.data());
			double dWD = atof(sWD.data());
			double dH = atof(sH.data());

			ShowPOI(dJD, dWD, dH, strValue);*/
		}

		ShowAttWin();

		
		m_pAttInfoWin->RemoveAllRows();
		const osgEarth::Features::AttributeTable& attrs = vFeature.at(nItem)->getAttrs();
		int len = 0,maxW = 0;
		bool bexpand = false;

		if (type == 1 && vdoorSensor.LData.size()>0)//栋
		{
			std::vector<CString> vecs;
			COMFUNC::split(vdoorSensor.LData.back(),"&&",vecs);
			std::string s;
			COMFUNC::dateTrans(W2A(vecs[4]),s);
			m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
			m_pAttInfoWin->InsertRow(_T("门磁状态"),vecs[2]);
			maxW = s.size();
		}
		else if (type == 2 && veleMeter.LData.size()>0)
		{
			std::vector<CString> vecs;
			COMFUNC::split(veleMeter.LData.back(),"&&",vecs);
			std::string s;
			COMFUNC::dateTrans(W2A(vecs[6]),s);
			m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
			m_pAttInfoWin->InsertRow(_T("电能"),vecs[3] + _T("Kwh"));
			maxW = s.size();
		}

		for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
		{
			//如果字段为全英文，跳过不显示
			if (COMFUNC::allAlpha(i->first) || i->first == "高程" || i->first == "顶高" || i->first == "底高" )
			{
				continue;
			}
			std::string str = i->second.getString();
			if (CString(i->first.c_str()) == _T("家庭成员")) 
			{
				COMFUNC::split(str,std::string(","),m_pAttInfoWin->vName );
				//m_pAttInfoWin->analysisString( str,m_pAttInfoWin->vName );
				bexpand = true;
			}
			if (CString(i->first.c_str()) == _T("成员身份证"))
			{
				COMFUNC::split(str,std::string(","),m_pAttInfoWin->vID );
				//m_pAttInfoWin->analysisString( str,m_pAttInfoWin->vID );
				continue;
			}
			
			int n = str.find(".");
			if ( n != -1 && isdigit(str[str.size() - 1]))
			{
				str = str.substr(0,n+3<=str.length()?n+3:str.length());
			}
			if (bexpand)
			{
				str += "(详细)";
				CString cs = CString(str.c_str());
				m_pAttInfoWin->InsertRow(CString(i->first.c_str()),cs);
				bexpand = false;
			}
			else
				m_pAttInfoWin->InsertRow(CString(i->first.c_str()),CString(str.c_str()));
			len++;

			//记录最长的字段值长度
			if (str.size()>maxW)
			{
				maxW = str.size();
			}
		}
		
		m_pAttInfoWin->changeDlgHeight((len+3)*g_RowHeight);
		//m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
		m_pAttInfoWin->suitHeightAndWidth(len,maxW);//调整高度和列宽
		m_pAttInfoWin->UpdateWindow();
		m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

void CDataManager::MakeFloor3DBorder(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Vec4 color,bool showAtt)
{
    osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);

    while(parts.hasMore())
    {
        osgEarth::Symbology::Geometry* part = parts.next();
        osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

        allPoints = part->toVec3Array();

        osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

        for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
        {
            osg::Vec3d v(*i);
            v.z() = v.z() + 1;
            v3->push_back(v);
        }

        gTemp = new osg::Group();
        for (int i = 0; i < v3->size(); i++)
        {
            osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
            osg::Vec3d ptVert, ptVertNext;

            ptVert = osg::Vec3d((*v3)[i]);
            if (i == v3->size() - 1)
            {
                ptVertNext = osg::Vec3d((*v3)[0]);
            }
            else
                ptVertNext = osg::Vec3d((*v3)[i + 1]);

            ptVert.z() = dFloor;
            ptVertNext.z() = dFloor;
            v3Vert->push_back(ptVert);
            v3Vert->push_back(ptVertNext);

            ptVertNext.z() = dCeil;
            v3Vert->push_back(ptVertNext);

            ptVert.z() = dCeil;
            v3Vert->push_back(ptVert);

            gTemp->addChild(AddPolygonNode(v3Vert.get(),color));
        }

        gTemp->setName("线框");
        m_spViewer3D->AddNode(gTemp);
    }

#ifdef _2D
    const osg::BoundingSphere& bs = gTemp->getBound();
    if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
        return;
	if (m_pAttInfoWin!= NULL)
	{
		m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
		m_spViewer3D->CenterNode(gTemp, 150);
		if (showAtt)
		{
			m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
		}
	}
	else
		m_spViewer3D->CenterNode(gTemp, 150);
#else
    m_spViewer3D->FlyToViewport(v3f.x(), v3f.y(), 100);
#endif
}

void CDataManager::MakeFloor3DBorderAll(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Group* group)
{
	osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
	while(parts.hasMore())
	{
		osgEarth::Symbology::Geometry* part = parts.next();
		osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

		allPoints = part->toVec3Array();

		osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

		for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
		{
			osg::Vec3d v(*i);
			v.z() = v.z() + 1;
			v3->push_back(v);
		}

		
		for (int i = 0; i < v3->size(); i++)
		{
			osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
			osg::Vec3d ptVert, ptVertNext;

			ptVert = osg::Vec3d((*v3)[i]);
			if (i == v3->size() - 1)
			{
				ptVertNext = osg::Vec3d((*v3)[0]);
			}
			else
				ptVertNext = osg::Vec3d((*v3)[i + 1]);

			ptVert.z() = dFloor;
			ptVertNext.z() = dFloor;
			v3Vert->push_back(ptVert);
			v3Vert->push_back(ptVertNext);

			ptVertNext.z() = dCeil;
			v3Vert->push_back(ptVertNext);

			ptVert.z() = dCeil;
			v3Vert->push_back(ptVert);

			group->addChild(AddPolygonNode(v3Vert.get()));
		}
	}
}

// create text which sits in 3D space such as would be inserted into a normal model
osg::PositionAttitudeTransform* CDataManager::create3DText(const osg::Vec3& center, float radius, CString sText)
{
    USES_CONVERSION;

	//osg::Geode* geode  = new osg::Geode;

	osg::Billboard* geode = new osg::Billboard();
	geode->setMode(osg::Billboard::POINT_ROT_EYE);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Examples of how to set up axis/orientation alignments
    //

    float characterSize = radius * 0.2f;
    float characterDepth = characterSize * 0.2f;

    osg::Vec3 pos(center.x(), center.y(), center.z());

	osgText::Text3D* text1 = new osgText::Text3D;
	text1->setFont("fonts/simhei.ttf");
	text1->setColor(osg::Vec4(1,0,0,1));
    text1->setCharacterSize(characterSize);
    text1->setCharacterDepth(characterDepth);
    //text1->setPosition(pos);
    text1->setDrawMode(osgText::Text3D::TEXT);
    text1->setAxisAlignment(osgText::Text3D::XZ_PLANE);
	//std::string str =  sText.GetBuffer(0);
    text1->setText(T2A(sText));
	//text1->setText(str.c_str());
	
	osg::ref_ptr<osgText::Style> style = new osgText::Style;
	osg::ref_ptr<osgText::Bevel> bevel = new osgText::Bevel;
	bevel->roundedBevel2(0.25);
	style->setBevel(bevel.get());
	style->setWidthRatio(0.4f);

	text1->setStyle(style.get());

    geode->addDrawable(text1);

	osg::PositionAttitudeTransform* rootNode = new osg::PositionAttitudeTransform;
	rootNode->addChild(geode);
	rootNode->setPosition(pos);


    osg::Material* front = new osg::Material;
    front->setAlpha(osg::Material::FRONT_AND_BACK, 1);
    front->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2, 0.2, 0.2, 1.0));
    front->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(.0, .0, 1.0, 1.0));
	
    rootNode->getOrCreateStateSet()->setAttributeAndModes(front);


    return rootNode;
}

osg::Group* CDataManager::createText(const osg::Vec3& center, float radius, CString sText)
{
	USES_CONVERSION;

	//osg::Geode* geode  = new osg::Geode;

	osg::Billboard* geode = new osg::Billboard();
	geode->setMode(osg::Billboard::AXIAL_ROT);

	osg::Vec4 layoutColor(1.0f,1.0f,0.0f,1.0f);
	float layoutCharacterSize = 20.0f;    

	{
		osgText::Text* text = new osgText::Text;
		osgText::Font* font = osgText::readFontFile("fonts/arial.ttf");
		text->setFont(font);
		text->setColor(layoutColor);
		text->setCharacterSize(layoutCharacterSize);
		//text->setPosition(center);

		// the default layout is left to right, typically used in languages
		// originating from europe such as English, French, German, Spanish etc..
		text->setLayout(osgText::Text::LEFT_TO_RIGHT);

		text->setText("text->setLayout(osgText::Text::LEFT_TO_RIGHT);");
		geode->addDrawable(text);
	}

	osg::Group* rootNode = new osg::Group;
	rootNode->addChild(geode);


	return rootNode;
}

void CDataManager::ShowPOI(double dJD, double dWD, double dH, CString sText)
{
	USES_CONVERSION;
    osg::Vec3d ptVert;
    osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;

    ptVert.x() = dJD + 10;
    ptVert.y() = dWD + 10;
    ptVert.z() = dH;
    v3Vert->push_back(ptVert);

    ptVert.x() = dJD - 10;
    ptVert.y() = dWD + 10;
    ptVert.z() = dH;
    v3Vert->push_back(ptVert);

    ptVert.x() = dJD - 10;
    ptVert.y() = dWD - 10;
    ptVert.z() = dH;
    v3Vert->push_back(ptVert);

    ptVert.x() = dJD + 10;
    ptVert.y() = dWD - 10;
    ptVert.z() = dH;
    v3Vert->push_back(ptVert);

    ptVert.x() = dJD + 10;
    ptVert.y() = dWD + 10;
    ptVert.z() = dH;
    v3Vert->push_back(ptVert);

    m_spViewer3D->Lock();

    if (gTemp != nullptr)
    {
        m_spViewer3D->RemoveNode(gTemp.get());
    }

    gTemp = new osg::Group();

    gTemp->addChild(AddPolygonNode(v3Vert.get()));

    osg::Vec3 center(dJD, dWD, dH + 10);
    float radius = 50.0f;

	osg::PositionAttitudeTransform * g = create3DText(center, radius, sText);
    gTemp->addChild(g);

    gTemp->setName(std::string("兴趣点(")+T2A(sText) + std::string(")"));
    m_spViewer3D->AddNode(gTemp);

    m_spViewer3D->Unlock();

#ifdef _2D
    const osg::BoundingSphere& bs = gTemp->getBound();
    if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
        return;

    m_spViewer3D->CenterNode(gTemp, 150);
#else
    m_spViewer3D->FlyToViewport(v3f.x(), v3f.y(), 100);
#endif
}

void CDataManager::MakeBuildingStruct(std::string sTH,std::string sZ)
{
    if (gTemp != nullptr){
        m_spViewer3D->RemoveNode(gTemp);
		gTemp = nullptr;
	}
    osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
    allPoints = m_geoPart->toVec3Array();

    double dH = 0;

    if (!sTH.empty())
    {
        dH = atof(sTH.data());
    }
    else
        dH = GetBuildingHeight();

    osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

    for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
    {
        osg::Vec3d v(*i);
        v.z() = v.z() + 1;
        v3->push_back(v);
    }

    gTemp = new osg::Group();
	double newZ = 0;
	if (!sZ.empty())
	{
		newZ = atof(sZ.data()) - 0.5;
	}
    for (int i = 0; i < v3->size(); i++)
    {
        osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
        osg::Vec3d ptVert, ptVertNext;

        ptVert = osg::Vec3d((*v3)[i]);
		ptVert.z() = newZ;
        if (i == v3->size() - 1)
        {
            ptVertNext = osg::Vec3d((*v3)[0]);
        }
        else
            ptVertNext = osg::Vec3d((*v3)[i + 1]);
		ptVertNext.z() = newZ;
        v3Vert->push_back(ptVert);

        v3Vert->push_back(ptVertNext);

        ptVertNext.z() = dH + dStep;
        v3Vert->push_back(ptVertNext);

        ptVert.z() = dH + dStep;
        v3Vert->push_back(ptVert);

        gTemp->addChild(AddPolygonNode(v3Vert.get()));
    }

    osg::ref_ptr< osg::Vec3Array > v3Top = new osg::Vec3Array;

    for (int i = 0; i < v3->size(); i++)
    {
        osg::Vec3d ptTop;

        ptTop = osg::Vec3d((*v3)[i]);
        ptTop.z() = dH + dStep;
        v3Top->push_back(ptTop);
    }
    gTemp->addChild(AddPolygonNode(v3Top.get()));

    gTemp->setName("线框");
    m_spViewer3D->AddNode(gTemp);

#ifdef _2D
    const osg::BoundingSphere& bs = gTemp->getBound();
    if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
        return;
	if (m_pAttInfoWin!= NULL)
	{
		m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
		m_spViewer3D->CenterNode(gTemp, 250);
		m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
	}
	else
		m_spViewer3D->CenterNode(gTemp, 250);
#else
    m_spViewer3D->FlyToViewport(v3f.x(), v3f.y(), 100);
#endif
}

void CDataManager::MakeBuildingStructAll(std::string sTH,std::string sZ,osg::Group* group)
{
	osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
	allPoints = m_geoPart->toVec3Array();

	double dH = 0;

	if (!sTH.empty())
	{
		dH = atof(sTH.data());
	}
	else
		dH = GetBuildingHeight();

	osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

	for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
	{
		osg::Vec3d v(*i);
		v.z() = v.z() + 1;
		v3->push_back(v);
	}
	double newZ = 0;
	if (!sZ.empty())
	{
		newZ = atof(sZ.data()) - 0.5;
	}
	for (int i = 0; i < v3->size(); i++)
	{
		osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
		osg::Vec3d ptVert, ptVertNext;

		ptVert = osg::Vec3d((*v3)[i]);
		ptVert.z() = newZ;
		if (i == v3->size() - 1)
		{
			ptVertNext = osg::Vec3d((*v3)[0]);
		}
		else
			ptVertNext = osg::Vec3d((*v3)[i + 1]);
		ptVertNext.z() = newZ;
		v3Vert->push_back(ptVert);

		v3Vert->push_back(ptVertNext);

		ptVertNext.z() = dH + dStep;
		v3Vert->push_back(ptVertNext);

		ptVert.z() = dH + dStep;
		v3Vert->push_back(ptVert);

		group->addChild(AddPolygonNode(v3Vert.get()));
	}
}

double CDataManager::GetBuildingHeight()
{
    double dHeight = 0.0;
    double ds, de;
    ds = dStartHeight;
    de = dEndHeight;

    //从0米开始，每0.5米计算一次范围内的横切点的Z值，一直计算到100米
    //得到200个点，取200个点中的最大值
    double aHeight[1024] = {0};
    int n = 0;
    for (double h = ds; h < de; h = h + dStep)
    {
        int nCrossNum = 0;
        if (BuildingInThisHeight(h, nCrossNum))
        {
            aHeight[n] = h;
        }
        aPtsInHeight[n] = nCrossNum;

        if (!bFindedStart)
        {
            if (nCrossNum > 1000 && dStartHeight < 10)
            {
                dStartHeight = h;
                bFindedStart = true;
            }
        }

        if (!bFindedEnd && bFindedStart)
        {
            if (nCrossNum < 161)
            {
                dEndHeight = h + 2;
                bFindedEnd = true;
            }
        }

        n++;
    }

    for (int i = 0; i < 1024; i++)
    {
        if (aHeight[i] > dHeight)
        {
            dHeight = aHeight[i];
        }
    }

    return dHeight;
}

bool CDataManager::BuildingInThisHeight(double height, int & nCrossNum)
{
    //建筑物的范围坐标
    osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
    allPoints = m_geoPart->toVec3Array();

    Vec3dVector* vv = new Vec3dVector();
    for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
    {
        osg::Vec3d v(*i);
        vv->push_back(v);
    }

    osgEarth::Symbology::Ring r(vv);

    osg::Plane plane(osg::Vec4(0, 0, 1, -1 * height));
    osg::ref_ptr<osgUtil::PlaneIntersector> pi = new osgUtil::PlaneIntersector(plane);
    osgUtil::IntersectionVisitor iv(pi);
    m_spViewer3D->getRootNode()->accept(iv);
    if(pi->containsIntersections())
    {
        for(osgUtil::PlaneIntersector::Intersections::iterator it = pi->getIntersections().begin();
                it != pi->getIntersections().end(); it++)
        {
            //交点坐标
            std::vector<osg::Vec3d> intersectPoints = it->polyline;

            //osg::Geode* geode=new osg::Geode;
            //osg::Geometry* geom=new osg::Geometry;
            //osg::Vec3Array* v3a=new osg::Vec3Array;
            for (int i = 0; i < intersectPoints.size(); i++)
            {
                nCrossNum++;

                //v3a->push_back(intersectPoints.at(i));
                osg::Vec3d v3d = intersectPoints.at(i);
                osg::Vec3d v3dNew = it->matrix->preMult(v3d);
                if (r.contains2D(v3dNew.x(), v3dNew.y()))
                {
                    return true;
                }
            }
            //geom->setVertexArray(v3a);
            //geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,intersectPoints.size()));
            //geode->addDrawable(geom);

            //osg::ref_ptr<osg::MatrixTransform> spMatrixTransfrom = new osg::MatrixTransform(*(it->matrix.get()));
            //spMatrixTransfrom->addChild(geode);
            //spMatrixTransfrom->setName(("相交面"));
            //m_spViewer3D->AddNode(spMatrixTransfrom);
        }
    }

    return false;
}

void CDataManager::ShowAttWin()
{
    AFX_MANAGE_STATE_EX;

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	CBackgroundDlg* dlg = nullptr;
    if (m_pAttInfoWin == NULL)
    {
        m_pAttInfoWin = new CDlgAttInfo();
        m_pAttInfoWin->parent = (CObject*) this;
        if (!m_pAttInfoWin->Create(IDD_DIALOG_ENTINFO))
        {
            AfxMessageBox(_T("创建窗口失败"));
        }
		dlg =  new CBackgroundDlg();
		dlg->Create(IDD_DIALOG_BACKGROUND);
		m_pAttInfoWin->parentBk = (CObject*)dlg;
    }

   
	RECT r,r1;
	m_pAttInfoWin->GetWindowRect(&r);

	/*int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;*/
	int nWidth = (spViewer3D->rectView3D.right - spViewer3D->rectView3D.left)/5.0;
	int nHeight = r.bottom - r.top;
	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	m_pAttInfoWin->MoveWindow(&r);
	((CBackgroundDlg*)m_pAttInfoWin->parentBk)->MoveWindow(&r);
	
	m_pAttInfoWin->selfDefShowWindow(SW_SHOW);

	r1.top = r.top + m_pAttInfoWin->btnH + 5;
	r1.bottom = r.bottom - 10;
	r1.left = r.left + 3;
	r1.right = r.right - 3;
	m_pAttInfoWin->ScreenToClient(&r1);
	m_pAttInfoWin->m_wndList.MoveWindow(&r1);
	r1.top = r.top + 4;
	r1.bottom = r1.top + m_pAttInfoWin->btnH;
	r1.right = r.right - 3;
	r1.left = r1.right - m_pAttInfoWin->btnW;
	m_pAttInfoWin->ScreenToClient(&r1);
	m_pAttInfoWin->m_quitBtn.MoveWindow(&r1);
	//m_pAttInfoWin->setupPolygonRegion();

}

void CDataManager::OnBnClickedButtonSearch()
{
	m_cKey.GetWindowText(m_sKey);
	CString sShpFile = FindSHPFile();
	if (sShpFile == _T(""))
	{
		return;
	}
	vFeature.clear();
	DoSearch(sShpFile);
}

void CDataManager::OnBnClickedButtonSearchAdv()
{
    m_pSearchAdv = new CDlgSearchAdv();
    m_pSearchAdv->Create(IDD_DIALOG_SEARCH_ADV);
	CBackgroundDlg* bdlg = new CBackgroundDlg();
	bdlg->Create(IDD_DIALOG_BACKGROUND);

	m_pSearchAdv->parent = this;
	m_pSearchAdv->parent1 = bdlg;
	bdlg->ShowWindow(SW_SHOW);
    m_pSearchAdv->ShowWindow(SW_SHOW);

    RECT r;
    m_pSearchAdv->GetWindowRect(&r);

    int nWidth = r.right - r.left;
    int nHeight = r.bottom - r.top;

    r.left = m_spViewer3D->rectView3D.right - nWidth - 6;
    r.top = m_spViewer3D->rectView3D.bottom - nHeight - 6;
    r.right = r.left + nWidth;
    r.bottom = r.top + nHeight;

    m_pSearchAdv->MoveWindow(&r);
	bdlg->MoveWindow(&r);
    CString sShpFile = FindSHPFile();
    if (sShpFile == _T(""))
    {
		m_pSearchAdv->m_spViewer3D = m_spViewer3D;
        return;
    }
    FillFieldsToDlg(sShpFile);
    m_pSearchAdv->m_sShp = sShpFile;
    m_pSearchAdv->m_spViewer3D = m_spViewer3D;
}

HBRUSH CDataManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here
    if (nCtlColor == CTLCOLOR_DLG)
        return (HBRUSH)::GetStockObject(BLACK_BRUSH);
    else
        pDC->SetTextColor(RGB(255, 0, 0));

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void CDataManager::OnSetDefLayer2(std::string filename)
{
    m_sDefLayer = CString(filename.c_str());
}

void CDataManager::OnProjectOpen(std::string filename)
{
	if (m_pAttInfoWin !=  NULL)
	{
		m_pAttInfoWin->DestroyWindow();
		m_pAttInfoWin  = NULL;
	}
	if (m_pCVMediaPlayerInfoWin != NULL)
	{
		m_pCVMediaPlayerInfoWin->DestroyWindow();
		m_pCVMediaPlayerInfoWin = NULL;
	}
	if (m_pShowPictureInfoWin != NULL)
	{
		m_pShowPictureInfoWin->DestroyWindow();
		m_pShowPictureInfoWin = NULL;
	}
	if (m_pHtmlInfoWin != NULL)
	{
		m_pHtmlInfoWin->DestroyWindow();
		m_pHtmlInfoWin = NULL;
	}
	gTemp = NULL;
	bInterDel = true;
	HTREEITEM hroot = m_wndTreeCtrl.GetRootItem();
	if (hroot)
	{
		RemoveAllNode(hroot);
	}
	/*while (hroot)
	{
		if (m_wndTreeCtrl.ItemHasChildren(hroot))
		{
			HTREEITEM item = m_wndTreeCtrl.GetChildItem(hroot);
			while(item != NULL)
			{
				if (m_wndTreeCtrl.ItemHasChildren(item))
				{
					HTREEITEM hChild = m_wndTreeCtrl.GetChildItem(item);
					while (hChild)
					{
						osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(hChild);
						if (pNode)
						{
							m_spViewer3D->RemoveNode(pNode);
						}
						hChild = m_wndTreeCtrl.GetNextSiblingItem(hChild);
					}
				}
				else
				{
					osg::Node* pNode = (osg::Node*)m_wndTreeCtrl.GetItemData(item);
					if (pNode)
					{
						m_spViewer3D->RemoveNode(pNode);
					}
				}
				item = m_wndTreeCtrl.GetNextSiblingItem(item);
			}
		}
		hroot = m_wndTreeCtrl.GetNextSiblingItem(hroot);
	}*/



	bInterDel = false;

	m_wndTreeCtrl.DeleteAllItems();
	std::string FileNameEx = filename.substr(filename.find_last_of( '\\' )+1);
	std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));
	HTREEITEM root = m_wndTreeCtrl.InsertItem(CString(FileNameNoEx.c_str()),TVI_ROOT);
	m_wndTreeCtrl.Expand(root,TVE_EXPAND);
	m_wndTreeCtrl.SetCheck(root,true);
	m_wndTreeCtrl.SetItemColor(root,RGB(128,0,0));
	m_wndTreeCtrl.SetItemBold(root,TRUE);
}

void CDataManager::FillFieldsToDlg(CString sShpFile)
{
    USES_CONVERSION;
    osgEarth::Drivers::OGRFeatureOptions ogrOpt;
    std::string path = W2A(sShpFile);
    if (path.length() == 0 )
    {
        return;
    }

    if (int(path.find('/')) > -1)
    {
        CString sProjPath = m_spViewer3D->GetProjectFilePath();
        CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
        std::string FileNameEx = path.substr(path.find_last_of( '/' ) + 1);

        path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
    }

    ogrOpt.url() = path;

    m_pSearchAdv->m_cFields.ResetContent();
	m_pSearchAdv->m_cFields2.ResetContent();

    features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
    features->initialize();

    osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();

    while(cursor->hasMore())
    {
        //输出feature中的信息
        osgEarth::Features::Feature* feature = cursor->nextFeature();

        int nIndex = 0;
        const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
        for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
        {
            CString sName = CString(i->first.c_str());
            m_pSearchAdv->m_cFields.AddString(sName);
			m_pSearchAdv->m_cFields2.AddString(sName);
            m_pSearchAdv->m_cList.InsertColumn (nIndex++, sName, LVCFMT_LEFT, 80);
        }

        break;
    }
}

void CDataManager::hideAll()
{
	if (m_pAttInfoWin != nullptr)
	{
		m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
	}
	if (m_pHtmlInfoWin != nullptr)
	{
		m_pHtmlInfoWin->ShowWindow(SW_HIDE);
	}
	if (m_pShowPictureInfoWin != nullptr)
	{
		m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
	}
	if (m_pCVMediaPlayerInfoWin != nullptr)
	{
		m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
	}
	if (m_pInternetInfoWin != nullptr)
	{
		m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
	}
	if (m_pVLCPlayerInfoWin != nullptr)
	{
		m_pVLCPlayerInfoWin->ShowWindow(SW_HIDE);
	}
}

void CDataManager::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if(spViewer3D.valid() )
	{
		if (spViewer3D->getRootNode()->asGroup()->getNumChildren()>0)
		{
			if (m_pInternetInfoWin == NULL)
			{
				m_pInternetInfoWin = new CInternetInfoDlg();
				m_pInternetInfoWin->parent = (CObject*)this;
				m_pInternetInfoWin->Create(IDD_DIALOG_INFO_NET);

				RECT r;
				m_pInternetInfoWin->GetWindowRect(&r);
				int width = r.right - r.left;
				int height = r.bottom - r.top;
				r.left = spViewer3D->rectView3D.left + 20;
				r.top = spViewer3D->rectView3D.top + 20;
				r.right = r.left + width;
				r.bottom = r.top + height;
				m_pInternetInfoWin->MoveWindow(&r);
			}
		}
	}
	__super::OnTimer(nIDEvent);
}


void CDataManager::OnSetHouseLayer()
{
	// TODO: 在此添加命令处理程序代码
	USES_CONVERSION;
	if (!m_HouseDefLayer.IsEmpty())
	{
		cancelDefLayer(m_HouseDefLayer);
	}
	m_wndTreeCtrl.ScreenToClient(&point);

	HTREEITEM item;
	UINT flag = TVHT_ONITEM;
	item = m_wndTreeCtrl.HitTest(point, &flag);
	if(item != NULL)
	{
		UnBold();
		m_wndTreeCtrl.SetItemColor(item,RGB(0,128,0));
		m_HouseDefLayer = m_wndTreeCtrl.GetItemText(item);
	}
}


void CDataManager::OnSetBuildLayer()
{
	// TODO: 在此添加命令处理程序代码
	USES_CONVERSION;
	if (!m_BuildDefLayer.IsEmpty())
	{
		cancelDefLayer(m_BuildDefLayer);
	}

	m_wndTreeCtrl.ScreenToClient(&point);

	HTREEITEM item;
	UINT flag = TVHT_ONITEM;
	item = m_wndTreeCtrl.HitTest(point, &flag);
	if(item != NULL)
	{
		UnBold();
		m_wndTreeCtrl.SetItemColor(item,RGB(128,128,0));
		m_BuildDefLayer = m_wndTreeCtrl.GetItemText(item);
	}
}

void CDataManager::cancelDefLayer(CString m_BuildDefLayer)
{
	HTREEITEM item = FindItem(m_wndTreeCtrl.GetRootItem(), m_BuildDefLayer);
	if (item != NULL)
	{
		CString s = m_wndTreeCtrl.GetItemText(item);
		m_wndTreeCtrl.SetItemColor(item,RGB(0,0,0));
	}
}