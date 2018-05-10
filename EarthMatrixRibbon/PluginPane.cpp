// PluginPane.cpp : 实现文件
//

#include "stdafx.h"
//#include "Cbers3D.h"
#include "PluginPane.h"


// CPluginPane

IMPLEMENT_DYNAMIC(CPluginPane, CDockablePane)

CPluginPane::CPluginPane(IUIWnd* pPlugin)
{
	m_spPlugin = pPlugin;
}

CPluginPane::~CPluginPane()
{
	if( ::IsWindow(GetSafeHwnd()) )
		DestroyWindow();
	m_spPlugin = NULL;
}


BEGIN_MESSAGE_MAP(CPluginPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CPluginPane 消息处理程序
int CPluginPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if( m_spPlugin==NULL )
		return -1;

	if( !m_spPlugin->createWnd((size_t)GetSafeHwnd(), 1) )
		return -1;
	
	return 0;
}


void CPluginPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( m_spPlugin==NULL || !::IsWindow((HWND)(m_spPlugin->getHandle())) )
		return;

	::MoveWindow((HWND)(m_spPlugin->getHandle()), 0, 0, cx, cy, TRUE);			
}


void CPluginPane::OnDestroy()
{
	m_spPlugin = NULL;
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CPluginPane::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}


void CPluginPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	if( m_spPlugin==NULL || !::IsWindow((HWND)(m_spPlugin->getHandle())) )
		return;

	::SetFocus((HWND)(m_spPlugin->getHandle()));
}