// DlgRealVideo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgRealVideo.h"
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "EarthMatrixFunctions.h"

#include <sstream>
#include <io.h>
#include <atlconv.h>
#include <atlstr.h>

USING_NAMESPACE_EARTHMATRIX

	using namespace std;

std::string g_sRealVideoPath;
std::string g_sConfigPath;

IMPLEMENT_DYNAMIC(CDlgRealVideo, CDialog)

CDlgRealVideo::CDlgRealVideo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRealVideo::IDD, pParent)
{

}

CDlgRealVideo::~CDlgRealVideo()
{
}

void CDlgRealVideo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_cConfigPath);
	DDX_Control(pDX, IDC_EDIT3, m_cDemo);
	DDX_Control(pDX, IDC_EDIT1, m_cVideoPath);
}


BEGIN_MESSAGE_MAP(CDlgRealVideo, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BR, &CDlgRealVideo::OnBnClickedButtonBr)
	ON_BN_CLICKED(IDOK, &CDlgRealVideo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADDLOCALV, &CDlgRealVideo::OnBnClickedButtonAddlocalv)
END_MESSAGE_MAP()


// CDlgRealVideo ��Ϣ�������

BOOL CDlgRealVideo::OnInitDialog()
{
	isLocal = false;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	pEdit->SetWindowTextW(_T("rtsp://admin:admin123456@27.17.16.6:554/h264/1/main/av_stream"));

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_REAL);
	pEdit->SetWindowTextW(_T("rtsp://admin:admin123456@27.17.16.6:554/h264/1/main/av_stream"));	
	return FALSE;
}

void CDlgRealVideo::OnBnClickedButtonBr()
{
	USES_CONVERSION;
	CStatic* g1 = (CStatic*)GetDlgItem(IDC_STATIC_GROUP_REAL);
	CStatic* g2 = (CStatic*)GetDlgItem(IDC_STATIC_GROUP_LOCAL);

	CString strFilter( GETSTRINGT(_T("CDlgRealVideo"), _T("FilterVideoRealConfig"), _T("ʵʱ��Ƶ�����ļ�(*.config)|*.config")) );

	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal() != IDOK )
	{
		return;
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CONFIG);
	pEdit->SetWindowTextW(dlg.GetPathName());

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
	pEdit->SetWindowTextW(_T("rtsp://admin:admin123456@27.17.16.6:554/h264/1/main/av_stream"));

	isLocal = false;
	g1->SetWindowTextW(_T("ʵʱ��Ƶ(����)"));
	g2->SetWindowTextW(_T("������Ƶ"));
}


void CDlgRealVideo::OnBnClickedOk()
{
	USES_CONVERSION;
	//��ʵʱ��Ƶ
	if (isLocal == false)
	{
		CString sConfig,sReal;
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CONFIG);
		pEdit->GetWindowText(sConfig);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_REAL);
		pEdit->GetWindowText(sReal);

		if (sConfig.GetLength() == 0 || sReal.GetLength() == 0)
		{
			AfxMessageBox(_T("ʵʱ��Ƶ��ַ�������ļ�·������Ϊ��!"));
			return;
		}

		g_sConfigPath = T2A(sConfig);
		g_sRealVideoPath = T2A(sReal);
	}
	CDialog::OnOK();
}

void CDlgRealVideo::OnBnClickedButtonAddlocalv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;

	CStatic* g1 = (CStatic*)GetDlgItem(IDC_STATIC_GROUP_REAL);
	CStatic* g2 = (CStatic*)GetDlgItem(IDC_STATIC_GROUP_LOCAL);

	CString strFilter( _T("��Ƶ�ļ�(*.mp4;*.avi)|*.mp4;*.avi"));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal() == IDOK )
	{
		isLocal = true;
		g1->SetWindowTextW(_T("ʵʱ��Ƶ"));
		g2->SetWindowTextW(_T("������Ƶ(����)"));
		slocalVideo = T2A(dlg.GetPathName());
		slocalVideoConfig = slocalVideo.substr(0,slocalVideo.find_last_of(".")+1);
		slocalVideoConfig += "config";
		CStatic* cedit1 = (CStatic*)GetDlgItem(IDC_EDIT_LOCAL);
		cedit1->SetWindowTextW(CString(slocalVideo.c_str()));
		CStatic* cedit2 = (CStatic*)GetDlgItem(IDC_EDIT5);
		cedit2->SetWindowTextW(CString(slocalVideoConfig.c_str()));
	}
	else
	{
		isLocal = false;
		g1->SetWindowTextW(_T("ʵʱ��Ƶ(����)"));
		g2->SetWindowTextW(_T("������Ƶ"));
	}
	return;
}
