// MyMFCRibbonBar.cpp : 实现文件
//

#include "stdafx.h"
#include "EarthMatrixRibbon.h"
#include "MyMFCRibbonBar.h"


// CMyMFCRibbonBar

IMPLEMENT_DYNAMIC(CMyMFCRibbonBar, CMFCRibbonBar)

CMyMFCRibbonBar::CMyMFCRibbonBar()
{
	SetQuickAccessToolbarOnTop(FALSE);
}

CMyMFCRibbonBar::~CMyMFCRibbonBar()
{
}

BOOL CMyMFCRibbonBar::OnShowRibbonQATMenu(CWnd* pWnd,int x,int y,CMFCRibbonBaseElement* pHit)
{
	return FALSE;
}

void CMyMFCRibbonBar::OnEditContextMenu(CMFCRibbonRichEditCtrl* pEdit, CPoint point)
{

}

void CMyMFCRibbonBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{

}

BOOL CMyMFCRibbonBar::OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit)
{
	return FALSE;
}

BEGIN_MESSAGE_MAP(CMyMFCRibbonBar, CMFCRibbonBar)
END_MESSAGE_MAP()



// CMyMFCRibbonBar 消息处理程序


