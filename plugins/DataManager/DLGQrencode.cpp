// DLGQrencode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGQrencode.h"
#include "afxdialogex.h"


// CDLGQrencode �Ի���

IMPLEMENT_DYNAMIC(CDLGQrencode, CDialogEx)

CDLGQrencode::CDLGQrencode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGQrencode::IDD, pParent)
{

}

CDLGQrencode::~CDLGQrencode()
{
}

void CDLGQrencode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICINFO, m_picQrencode);
	DDX_Control(pDX, IDC_BUTTON1_QRENCODEEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGQrencode, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1_QRENCODEEXIT, &CDLGQrencode::OnBnClickedButton1Qrencodeexit)
END_MESSAGE_MAP()


// CDLGQrencode ��Ϣ�������


void CDLGQrencode::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	int height, width;
	RECT r;
	m_picQrencode.GetWindowRect(&r);
	height = r.bottom - r.top;
	width = r.right - r.left;
	CRect rect;//���������
	ATL::CImage img1;
	img1.Load(_T("Data\\DRVdata\\listInfo.png"));
	if (!img1.IsNull())
	{
		m_picQrencode.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_picQrencode.GetDC();//���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img1.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	}
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CDLGQrencode::OnBnClickedButton1Qrencodeexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
}


BOOL CDLGQrencode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(RGB(200,200,200));
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
