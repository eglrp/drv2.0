// DLGVLCPlayer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGVLCPlayer.h"
#include "afxdialogex.h"


// CDLGVLCPlayer �Ի���

IMPLEMENT_DYNAMIC(CDLGVLCPlayer, CDialogEx)

CDLGVLCPlayer::CDLGVLCPlayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGVLCPlayer::IDD, pParent)
	, m_title(_T("����"))
{
	m_vlcMedia = nullptr;
	m_vlcMplay = nullptr;
	m_vlcInst = nullptr;
}

CDLGVLCPlayer::~CDLGVLCPlayer()
{
}

void CDLGVLCPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VLCTITLE, m_title);
}


BEGIN_MESSAGE_MAP(CDLGVLCPlayer, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDLGVLCPlayer ��Ϣ�������
void CDLGVLCPlayer::connectRTSP()
{
	m_vlcInst = libvlc_new(0, NULL);
	//Create a new item  
	//Method 1: 
	if (rtspAddress.empty())
	{
		m_vlcMedia = libvlc_media_new_location(m_vlcInst, "rtsp://admin:admin@192.168.0.246/11");
	}
	else
	{
		m_vlcMedia = libvlc_media_new_location(m_vlcInst, rtspAddress.c_str());
	}
	
	/* Create a media player playing environement */
	m_vlcMplay = libvlc_media_player_new_from_media(m_vlcMedia);
	libvlc_media_release (m_vlcMedia);   
	//on windows   
	libvlc_media_player_set_hwnd (m_vlcMplay,GetDlgItem(IDC_STATIC_VLC)->m_hWnd);   
	/* play the media_player */   
	int x=libvlc_media_player_play (m_vlcMplay);  
}

BOOL CDLGVLCPlayer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_font.CreatePointFont(120,_T("��Բ"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CDLGVLCPlayer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc�����¼� 
			OnClose();
			return true;  
		case VK_RETURN: //Enter�����¼� 
			OnClose();
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDLGVLCPlayer::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowWindow(SW_HIDE);
	CDataManager* p1 = dynamic_cast<CDataManager*>(parent);
	if (p1 != nullptr)
	{
		if (p1->gTemp != nullptr)
		{
			(p1->m_spViewer3D)->RemoveNode(p1->gTemp);
			p1->gTemp = nullptr;
		}
	}
	CDialogEx::OnClose();
}


HBRUSH CDLGVLCPlayer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_VLCTITLE) 
	{  
		// ��������Ϊ͸��  
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(255,255,255));    
		//pDC->SelectObject(&m_font);
		// ����һ��Ҫ����Ϊ�ջ�ˢ���������廹���б�����ɫ��  
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}  
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CDLGVLCPlayer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(73,141,205));
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
