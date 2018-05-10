// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// EarthMatrixRibbonView.cpp : CEarthMatrixRibbonView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CEarthMatrixRibbonView ����/����

CEarthMatrixRibbonView::CEarthMatrixRibbonView()
{
	// TODO: �ڴ˴���ӹ������
	m_pViewer = NULL;
}

CEarthMatrixRibbonView::~CEarthMatrixRibbonView()
{
	m_pViewer = NULL;
}

BOOL CEarthMatrixRibbonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CEarthMatrixRibbonView ����

void CEarthMatrixRibbonView::OnDraw(CDC* /*pDC*/)
{
	CEarthMatrixRibbonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CEarthMatrixRibbonView ��ӡ


void CEarthMatrixRibbonView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEarthMatrixRibbonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CEarthMatrixRibbonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CEarthMatrixRibbonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CEarthMatrixRibbonView ���

#ifdef _DEBUG
void CEarthMatrixRibbonView::AssertValid() const
{
	CView::AssertValid();
}

void CEarthMatrixRibbonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEarthMatrixRibbonDoc* CEarthMatrixRibbonView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEarthMatrixRibbonDoc)));
	return (CEarthMatrixRibbonDoc*)m_pDocument;
}
#endif //_DEBUG


// CEarthMatrixRibbonView ��Ϣ�������


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

	// TODO: �ڴ˴������Ϣ����������
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
