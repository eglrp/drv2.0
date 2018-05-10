// VLCPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VLCPlayerDlg.h"
#include "afxdialogex.h"


// CVLCPlayerDlg �Ի���

IMPLEMENT_DYNAMIC(CVLCPlayerDlg, CDialogEx)

CVLCPlayerDlg::CVLCPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVLCPlayerDlg::IDD, pParent)
	, m_info(_T("����"))
{
	m_vlcMedia = nullptr;
	m_vlcMplay = nullptr;
	m_vlcInst = nullptr;
}

CVLCPlayerDlg::~CVLCPlayerDlg()
{
}

void CVLCPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VLCINFO, m_info);
}


BEGIN_MESSAGE_MAP(CVLCPlayerDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVLCPlayerDlg ��Ϣ�������


BOOL CVLCPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	bpathSetted = false;
	m_font.CreatePointFont(120,_T("��Բ"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CVLCPlayerDlg::connectRTSP()
{
	if (!bpathSetted)
	{
		bpathSetted = true;
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
		libvlc_media_player_set_hwnd (m_vlcMplay,GetDlgItem(IDC_STATIC_CVPLAYER)->m_hWnd);   
		/* play the media_player */   
		int x=libvlc_media_player_play (m_vlcMplay);  
	}
	
}

BOOL CVLCPlayerDlg::PreTranslateMessage(MSG* pMsg)
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


void CVLCPlayerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


void CVLCPlayerDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(73,141,205));
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


HBRUSH CVLCPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_VLCINFO) 
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
