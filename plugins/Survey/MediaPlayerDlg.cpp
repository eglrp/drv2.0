// MediaPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MediaPlayerDlg.h"
#include "afxdialogex.h"


// CMediaPlayerDlg �Ի���

IMPLEMENT_DYNAMIC(CMediaPlayerDlg, CDialogEx)

CMediaPlayerDlg::CMediaPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMediaPlayerDlg::IDD, pParent)
{
	m_title = "D10B8911";
}

CMediaPlayerDlg::~CMediaPlayerDlg()
{
}

void CMediaPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_OCX1,m_player);
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_INFO, m_title);
}

BEGIN_MESSAGE_MAP(CMediaPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_CLOSE, &CMediaPlayerDlg::OnStnClickedStaticClose)
	//ON_WM_CLOSE()
//	ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMediaPlayerDlg ��Ϣ�������
BOOL CMediaPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	 
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0,220,LWA_ALPHA);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_player.SetUrl(_T("Data\\DRVdata\\door.mp4"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMediaPlayerDlg::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������ 
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(249,105,14) ); 
}

HBRUSH CMediaPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)  
	{  
		// ��������Ϊ͸��  
		pDC->SetBkMode(TRANSPARENT);  
		// ����һ��Ҫ����Ϊ�ջ�ˢ���������廹���б�����ɫ��  
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}  
	return hbr;
}


//BOOL CMediaPlayerDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//	//if (pMsg->message == WM_KEYDOWN)  
//	//{  
//	//	switch(pMsg->wParam)  
//	//	{  
//	//	case VK_ESCAPE: //Esc�����¼� 
//	//		ShowWindow(SW_HIDE);
//	//		
//	//		return true;
//	//	case VK_RETURN: //Enter�����¼� 
//	//		ShowWindow(SW_HIDE);
//	//		
//	//		return true;  
//	//	default:  
//	//		;  
//	//	}  
//	//}  
//	return CDialogEx::PreTranslateMessage(pMsg);
//}



void CMediaPlayerDlg::OnStnClickedStaticClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
	Invalidate();
}


//void CMediaPlayerDlg::OnClose()
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	/*ShowWindow(SW_HIDE);
//	Invalidate();*/
//	CDialogEx::OnClose();
//}






//void CMediaPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CDialogEx::OnLButtonUp(nFlags, point);
//}


void CMediaPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y)); 
	CDialogEx::OnLButtonDown(nFlags, point);
}
