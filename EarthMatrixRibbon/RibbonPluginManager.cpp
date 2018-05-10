#include "StdAfx.h"
#include "RibbonPluginManager.h"
//#include "CbersCLSID.h"
#include "PluginPane.h"
#include "x3py/manager/iworkpath.h"
#include "x3py/manager/x3manager.h"
#include <math.h>
#include "IViewer3D.h"

CRibbonPluginManager* CRibbonPluginManager::s_pManager = nullptr;
CRibbonPluginManager::CRibbonPluginManager(void)
{
	m_spViewer = NULL;
	afxGlobalData.EnableRibbonImageScale(false);
}

CRibbonPluginManager::~CRibbonPluginManager(void)
{
	DestroyControl();
}

CRibbonPluginManager* CRibbonPluginManager::getInstance()
{
	if( s_pManager==nullptr )
		s_pManager = new CRibbonPluginManager();
	return s_pManager;
}

void CRibbonPluginManager::Destroy()
{
	if( s_pManager!=nullptr )
		delete s_pManager;
	s_pManager = nullptr;
}

bool CRibbonPluginManager::CreateControl(CMFCRibbonBar* pRibbonBar, CFrameWndEx* pFrameWnd)
{
	if( pRibbonBar==NULL && pFrameWnd==NULL )
		return false;

	if( !LoadPlugins() )
		return false;

	x3::LockRW lockcls(m_mapPlugin.locker);
	bool ret = false;
	if( lockcls.canRead() )
	{
		pRibbonBar->SetRedraw(FALSE);
		hash_map<int, x3::AnyObject>::iterator it = m_mapPlugin.begin();
		for( ; it!=m_mapPlugin.end(); it++ )
		{
			x3::Object<IUICommon> spCommon(it->second);
			if( spCommon==NULL )
				continue;

			TRACE("CreateControl: %s\n", it->second->getClassName());
			if( m_spViewer!=NULL )
				spCommon->SetBuddy(m_spViewer.p());

			x3::Object<IUICommand> spCommand(spCommon);		
			x3::Object<IUIWnd> spWnd(spCommon);
			if( spCommand!=NULL || spWnd!=NULL )
				AddRibbonButton(pRibbonBar, spCommon.p(), it->first);
			
			if( spWnd!=NULL )
				CreatePane(pFrameWnd, spWnd.p(), it->first);
		}
		TRACE("CreateControl End\n");

		/*
		CMFCRibbonPanel* pPanel = FindRibbonPanel(pRibbonBar, _T("主页"), _T("视图"));
		if( pPanel!=NULL )
		pPanel->Add(new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, _T("状态栏")));*/
		pRibbonBar->SetRedraw(TRUE);
	} 

	return false;
}

void CRibbonPluginManager::OnCommand(int id)
{
	x3::Object<IUICommon> spCommon = FindPlugin(id);
	x3::Object<IUICommand> spCommand(spCommon);
	if( !spCommand.valid() )
	{
		CDockablePane* pPane = FindPluginPane(id);
		if( pPane!=NULL )
			pPane->ShowPane(!(pPane->IsVisible()), FALSE, TRUE);
		return;
	}

	x3::Object<IUITool> spTool(spCommon);
	if( spTool.valid() )
	{
		x3::Object<IUITool> spLastTool(LastTool());
		if( spLastTool.valid() )
		{
			SetLastTool(-1);
			if( spTool!=spLastTool )
			{
				spLastTool->Deactivate();
				if( spTool->Activate() )
					SetLastTool(id);	
			}
			else
			{
				if( spCommand->Checked() )
					spTool->Deactivate();
				else if( spTool->Activate() )
					SetLastTool(id);
			}
		}
		else
		{
			if( spTool->Activate() )
				SetLastTool(id);
		}
	}
	else
		spCommand->OnClick();
}

void CRibbonPluginManager::OnUpdateCommand(CCmdUI* pCmdUI)
{
	x3::Object<IUICommand> spCommand(FindPlugin(pCmdUI->m_nID));
	if( spCommand==NULL )
	{
		CDockablePane* pPane = FindPluginPane(pCmdUI->m_nID);
		if( pPane!=NULL )
		{
			pCmdUI->Enable(TRUE);
			pCmdUI->SetCheck(pPane->IsVisible() ? 1 : 0);
			return;
		}

		pCmdUI->Enable(FALSE);
		pCmdUI->SetCheck(0);
		return;
	}

	pCmdUI->Enable(spCommand->Enable());
	pCmdUI->SetCheck(spCommand->Checked() ? 1 : 0);
}

bool CRibbonPluginManager::AddRibbonButton(CMFCRibbonBar* pRibbonBar, IUICommon* pCommon, int id)
{
	USES_CONVERSION;
	if( pRibbonBar==NULL || pCommon==NULL )
		return false;

	CString strCategory = CW2CT(pCommon->Category());
	if( strCategory.IsEmpty() )
		strCategory = _T("插件");

	// 获取插件的标题
	CString strCaption = CW2CT(pCommon->Caption());
	if( strCaption.IsEmpty() )
		strCaption = strCategory;

	// 根据范畴名称, 查找插件所属的CMFCRibbonCategory
	CMFCRibbonCategory* pCategory = FindRibbonCategory(pRibbonBar, strCategory);
	// 如果不存在该范畴,则自动创建
	if( pCategory==NULL )
		pCategory = pRibbonBar->AddCategory(strCategory, 0, 0);

	// 根据标题,查找插件所属的CMFCRibbonPanel
	CMFCRibbonPanel* pPanel = FindRibbonPanel(pCategory, strCaption);	
	// 如果不存在,将自动创建
	if( pPanel==NULL )
		pPanel = pCategory->AddPanel(strCaption);

	// 获取插件的名称以及Tooltip提示
	CString strName = CW2CT(pCommon->Name());
	if( strName.IsEmpty() )
		strName.Format(_T("插件%d"), id - CPluginManager::MINID + 1);

	CString strToolTip = CW2CT(pCommon->Tooltip());	
	if( strToolTip.IsEmpty() )
	{
		CString strDescription = CW2CT(pCommon->Description());
		if( strDescription.IsEmpty() )
			strToolTip = strName;
		else
			strToolTip = strDescription;
	}

	// 获取插件的图标
	int nSmallImageIndex = -1;
	int nLargeImageIndex = -1;
	CString strBitmapName = CW2CT(pCommon->BitmapName());
	if( !strBitmapName.IsEmpty() )
	{
		CString strImagePath;
		x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
		if( spAppWorkPath!=NULL )
		{			
			std::wstring path = spAppWorkPath->getWorkPath();	
			strImagePath.Format(_T("%s\\Skins\\default\\%s.png"), CW2CT(path.c_str()), strBitmapName);
		}

		if( strImagePath.IsEmpty() )
			strImagePath.Format(_T("Skins\\default\\%s.png"), strBitmapName);

		CPngImage pngImage;
		if( pngImage.LoadFromFile(strImagePath) )		
		{
			BITMAP bmp;
			pngImage.GetBitmap(&bmp);

			HBITMAP hBitmap = (HBITMAP) pngImage.Detach();			
			CSize szSmall = pCategory->GetImageSize(FALSE);
			CSize szLarge = pCategory->GetImageSize(TRUE);
			int nSmallDis = __min(abs(bmp.bmWidth - szSmall.cx), abs(bmp.bmHeight - szSmall.cy));
			int nLargeDis = __min(abs(bmp.bmWidth - szLarge.cx), abs(bmp.bmHeight - szLarge.cy));
			if( nSmallDis<nLargeDis )
			{
				CMFCToolBarImages& smallImages = pCategory->GetSmallImages();
				nSmallImageIndex = smallImages.AddImage(hBitmap);
			}
			else
			{
				CMFCToolBarImages& largeImages = pCategory->GetLargeImages();
				if (bmp.bmBitsPixel >= 32)
				{
					CMFCToolBarImages::PreMultiplyAlpha(hBitmap, largeImages.IsPreMultiplyAutoCheck());
				}
				else if ((bmp.bmBitsPixel > 8 && largeImages.GetMapTo3DColors()) || afxGlobalData.m_bIsBlackHighContrast)
				{
					// LR_LOADMAP3DCOLORS don't support > 8bpp images,
					// we should convert it now:					
					// CMFCToolBarImages::MapBmpTo3dColors(hBitmap, FALSE);
				}
				nLargeImageIndex = largeImages.AddImage(hBitmap, largeImages.GetBitsPerPixel() < bmp.bmBitsPixel);
			}

			::DeleteObject(hBitmap);
		}
	}

	// 创建插件按钮
	CMFCRibbonButton* pButton = new CMFCRibbonButton(id, strName, nSmallImageIndex, nLargeImageIndex);
	pButton->SetToolTipText(strToolTip);
	pPanel->Add(pButton);		
	return true;
}

CMFCRibbonCategory* CRibbonPluginManager::FindRibbonCategory(CMFCRibbonBar* pRibbonBar, LPCTSTR lpszCategory)
{
	if( pRibbonBar==NULL || lpszCategory==NULL )
		return NULL;

	CString strCategory(lpszCategory);
	if( strCategory.IsEmpty() )
		return NULL;

	// 根据范畴名称, 查找插件所属的CMFCRibbonCategory
	CMFCRibbonCategory* pCategory = NULL;
	for( int i=0; i<pRibbonBar->GetCategoryCount(); i++ )
	{
		CMFCRibbonCategory* pTmp = pRibbonBar->GetCategory(i);
		if( pTmp==NULL )
			continue;

		if( strCategory.CompareNoCase(pTmp->GetName())==0 )
		{
			pCategory = pTmp;
			break;
		}
	}

	return pCategory;
}

CMFCRibbonPanel*  CRibbonPluginManager::FindRibbonPanel(CMFCRibbonCategory* pCategory, LPCTSTR lpszCaption)
{
	if( pCategory==NULL || lpszCaption==NULL )
		return NULL;

	CString strCaption(lpszCaption);
	if( strCaption.IsEmpty() )
		return NULL;

	// 根据标题,查找插件所属的CMFCRibbonPanel
	CMFCRibbonPanel* pPanel = NULL;
	for( int i=0; i<pCategory->GetPanelCount(); i++ )
	{
		CMFCRibbonPanel* pTmp = pCategory->GetPanel(i);
		if( pTmp==NULL )
			continue;

		if( strCaption.CompareNoCase(pTmp->GetName())==0 )
		{
			pPanel = pTmp;
			break;
		}
	}

	return pPanel;
}

CMFCRibbonPanel*  CRibbonPluginManager::FindRibbonPanel(CMFCRibbonBar* pRibbonBar, LPCTSTR lpszCategory, LPCTSTR lpszCaption)
{
	if( pRibbonBar==NULL || lpszCategory==NULL || lpszCaption==NULL )
		return NULL;

	// 根据范畴名称, 查找插件所属的CMFCRibbonCategory
	CMFCRibbonCategory* pCategory = FindRibbonCategory(pRibbonBar, lpszCategory);	
	if( pCategory==NULL )
		return NULL;

	// 根据标题,查找插件所属的CMFCRibbonPanel
	CMFCRibbonPanel* pPanel = FindRibbonPanel(pCategory, lpszCaption);	
	return pPanel;
}

bool CRibbonPluginManager::CreatePane(CFrameWndEx* pFrameWnd, IUIWnd* pUIWnd, int id)
{
	USES_CONVERSION;

	if( pFrameWnd==NULL || pUIWnd==NULL )
		return false;

	x3::Object<IUICommon> spCommon(pUIWnd);
	if( spCommon==NULL )
		return false;

	CPluginPane* pPane = new CPluginPane(pUIWnd);
	if( pPane==NULL )
		return false;

	if( !(pPane->Create(CW2CT(spCommon->Name()), pFrameWnd, CRect(0, 0, 200, 200), TRUE, id, pUIWnd->getStyle())) )
	{
		delete pPane;
		return false;
	}

	pPane->EnableDocking(CBRS_ALIGN_ANY);
	pFrameWnd->DockPane(pPane);

	x3::LockRW lockcls(m_mapPluginPane.locker, true);
	if( lockcls.canWrite() )
		m_mapPluginPane[id] = pPane;	
	return true;
}

void CRibbonPluginManager::DestroyControl()
{
	if(m_spViewer.valid())
	{
		x3::Object<IViewer3D> spViewer3D(m_spViewer);
		if(spViewer3D.valid())
			spViewer3D->StopRender();
	}
	m_spViewer = NULL;
	UnloadPlugins();

	x3::LockRW lockcls(m_mapPluginPane.locker, true);
	if( lockcls.canWrite() )
	{
		hash_map<int, CDockablePane*>::iterator it = m_mapPluginPane.begin();
		for( ; it!=m_mapPluginPane.end(); it++ )
		{
			CDockablePane* pPane = it->second;
			if( pPane!=NULL )
			{
				if( ::IsWindow(pPane->GetSafeHwnd()) )
					pPane->DestroyWindow();
				delete pPane;
			}
		}
		m_mapPluginPane.clear();		
	}
}

void CRibbonPluginManager::SetViewer(x3::IObject* val)
{
	if( m_spViewer==val )
		return;

	m_spViewer = val;
	x3::LockRW lockcls(m_mapPlugin.locker);
	bool ret = false;
	if( lockcls.canRead() )
	{
		hash_map<int, x3::AnyObject>::iterator it = m_mapPlugin.begin();
		for( ; it!=m_mapPlugin.end(); it++ )
		{
			x3::Object<IUICommon> spCommon(it->second);
			if( spCommon==NULL )
				continue;

			spCommon->SetBuddy(m_spViewer.p());
		}
	} 
}

CDockablePane* CRibbonPluginManager::FindPluginPane(int id)
{
	x3::LockRW lockcls(m_mapPluginPane.locker);
	CDockablePane* ret = NULL;
	if( lockcls.canRead() )
	{
		hash_map<int, CDockablePane*>::iterator it = m_mapPluginPane.find(id);
		if( it!=m_mapPluginPane.end() )
			ret = it->second;
	}

	return ret;
}