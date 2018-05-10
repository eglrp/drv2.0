// DLGanbaodongtai.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGanbaodongtai.h"
#include "afxdialogex.h"
#include "BackgroundDlg.h"
extern std::vector<PERSON_LOCATION> g_vPersonLoc;
// CDLGanbaodongtai �Ի���

IMPLEMENT_DYNAMIC(CDLGanbaodongtai, CDialogEx)

CDLGanbaodongtai::CDLGanbaodongtai(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGanbaodongtai::IDD, pParent)
	, m_info(_T(""))
{

}

CDLGanbaodongtai::~CDLGanbaodongtai()
{
}

void CDLGanbaodongtai::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ABDT, m_wndList);
	DDX_Control(pDX, IDC_STATIC_ABDTTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_ABDTEXIT, m_EXIT);
	DDX_Text(pDX, IDC_STATIC_RESUALT3, m_info);
}


BEGIN_MESSAGE_MAP(CDLGanbaodongtai, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ABDTEXIT, &CDLGanbaodongtai::OnBnClickedButtonAbdtexit)
END_MESSAGE_MAP()


// CDLGanbaodongtai ��Ϣ�������


BOOL CDLGanbaodongtai::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("������Ա����"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (1, _T("λ��"), LVCFMT_LEFT, 250);
	m_font1.CreatePointFont(110, _T("����"));
	GetDlgItem(IDC_STATIC_RESUALT3)->SetFont(&m_font1);
	m_wndList.SetFont(&m_font1);
	m_wndList.SetRowHeigt(25);

	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetTextColor(RGB(255,255,255));
	m_font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		TRUE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("����"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("������̬"));
	m_brBk.CreateSolidBrush(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // ͼƬȫ·��  
		IMAGE_BITMAP,                          // ͼƬ��ʽ  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // ע��LR_LOADFROMFILE  
	m_EXIT.SetBitmap(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDLGanbaodongtai::doSearch()
{
	if (g_vPersonLoc.empty())
	{
		AfxMessageBox(_T("δ������ݣ����Ժ����ԣ�"));
		return;
	}
	for (int i = 0;i<g_vPersonLoc.size();++i)
	{
		m_wndList.InsertItem(0, _T("1"));
		m_wndList.SetItemText (0, 0, CString(g_vPersonLoc[i].name.c_str()));
		char msg[99];
		sprintf(msg,"%.2f,%.2f",g_vPersonLoc[i].x,g_vPersonLoc[i].y);
		m_wndList.SetItemText (0, 1, CString(msg));
	}
	char smsg[99];
	sprintf(smsg,"�� %d ��",g_vPersonLoc.size());
	GetDlgItem(IDC_STATIC_RESUALT3)->SetWindowText(CString(smsg));
}

void CDLGanbaodongtai::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


BOOL CDLGanbaodongtai::PreTranslateMessage(MSG* pMsg)
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


HBRUSH CDLGanbaodongtai::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return m_brBk;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CDLGanbaodongtai::OnBnClickedButtonAbdtexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}
