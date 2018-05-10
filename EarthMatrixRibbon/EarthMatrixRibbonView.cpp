// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// EarthMatrixRibbonView.cpp : CEarthMatrixRibbonView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "EarthMatrixRibbon.h"
#endif

#include "EarthMatrixRibbonDoc.h"
#include "EarthMatrixRibbonView.h"
#include "RibbonPluginManager.h"
#include "IViewer3D.h"
USING_NAMESPACE_EARTHMATRIX

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEarthMatrixRibbonView

IMPLEMENT_DYNCREATE(CEarthMatrixRibbonView, CView)

BEGIN_MESSAGE_MAP(CEarthMatrixRibbonView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEarthMatrixRibbonView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CEarthMatrixRibbonView 构造/析构

CEarthMatrixRibbonView::CEarthMatrixRibbonView()
{
	// TODO: 在此处添加构造代码
	m_pViewer = NULL;
}

CEarthMatrixRibbonView::~CEarthMatrixRibbonView()
{
	m_pViewer = NULL;
}

BOOL CEarthMatrixRibbonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CEarthMatrixRibbonView 绘制

void CEarthMatrixRibbonView::OnDraw(CDC* /*pDC*/)
{
	CEarthMatrixRibbonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CEarthMatrixRibbonView 打印


void CEarthMatrixRibbonView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEarthMatrixRibbonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CEarthMatrixRibbonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CEarthMatrixRibbonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CEarthMatrixRibbonView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CEarthMatrixRibbonView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEarthMatrixRibbonView 诊断

#ifdef _DEBUG
void CEarthMatrixRibbonView::AssertValid() const
{
	CView::AssertValid();
}

void CEarthMatrixRibbonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEarthMatrixRibbonDoc* CEarthMatrixRibbonView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEarthMatrixRibbonDoc)));
	return (CEarthMatrixRibbonDoc*)m_pDocument;
}
#endif //_DEBUG


// CEarthMatrixRibbonView 消息处理程序


int CEarthMatrixRibbonView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	x3::Object<IViewer3D> spViewer3d(clsidViewer3D);
	if( !spViewer3d.valid() )
	{
		//AfxMessageBox(_T("1"));
		return -1;
	}

	m_pViewer = spViewer3d;
	if( !spViewer3d->Create((size_t)GetSafeHwnd()) )
	{		
		m_pViewer = NULL;
		return -1;
	}

	CRibbonPluginManager::getInstance()->SetViewer(spViewer3d.p());		
	return 0;
}

BOOL CEarthMatrixRibbonView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CEarthMatrixRibbonView::OnDestroy()
{
	m_pViewer = NULL;
	CView::OnDestroy();

	// TODO: Add your message handler code here
}

void CEarthMatrixRibbonView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	x3::Object<IViewer3D> spViewer3D(m_pViewer);
	if( !spViewer3D.valid() )
		return;

	spViewer3D->ChangePosition(cx,cy);

	this->GetWindowRect(&(spViewer3D->rectView3D));

	// TODO: 在此处添加消息处理程序代码
	/*
	if( m_pViewer==NULL )
		return;

	x3::Object<IUIWnd> spUIWnd(m_pViewer);
	if( spUIWnd==NULL || !::IsWindow((HWND)(spUIWnd->getHandle())) )
		return;

	::MoveWindow((HWND)(spUIWnd->getHandle()), 0, 0, cx, cy, TRUE);	
	*/
}

void CEarthMatrixRibbonView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	/*
	x3::Object<IUIWnd> spUIWnd(m_pViewer);
	if( spUIWnd==NULL || !::IsWindow((HWND)(spUIWnd->getHandle())) )
		return;

	::SetFocus((HWND)(spUIWnd->getHandle()));
	*/
}
