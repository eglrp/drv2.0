// QRencode2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QRencode2Dlg.h"
#include "afxdialogex.h"


// CQRencode2Dlg �Ի���

IMPLEMENT_DYNAMIC(CQRencode2Dlg, CDialogEx)

CQRencode2Dlg::CQRencode2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQRencode2Dlg::IDD, pParent)
{

}

CQRencode2Dlg::~CQRencode2Dlg()
{
}

void CQRencode2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_QRENCODE2, m_pic);
}


BEGIN_MESSAGE_MAP(CQRencode2Dlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CQRencode2Dlg ��Ϣ�������


void CQRencode2Dlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	int height, width;
	RECT r;
	m_pic.GetWindowRect(&r);
	height = r.bottom - r.top;
	width = r.right - r.left;
	CRect rect;//���������
	ATL::CImage img1;
	img1.Load(_T("Data\\DRVdata\\pointLocation.png"));
	if (!img1.IsNull())
	{
		m_pic.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		CDC *pDc = m_pic.GetDC();//���pictrue�ؼ���Dc
		SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE); 
		img1.StretchBlt(pDc->m_hDC,rect,SRCCOPY);
		ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	}
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
