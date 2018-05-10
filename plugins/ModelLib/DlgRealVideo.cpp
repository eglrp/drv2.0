// DlgRealVideo.cpp : 实现文件
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


// CDlgRealVideo 消息处理程序

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

	CString strFilter( GETSTRINGT(_T("CDlgRealVideo"), _T("FilterVideoRealConfig"), _T("实时视频配置文件(*.config)|*.config")) );

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
	g1->SetWindowTextW(_T("实时视频(激活)"));
	g2->SetWindowTextW(_T("本地视频"));
}


void CDlgRealVideo::OnBnClickedOk()
{
	USES_CONVERSION;
	//是实时视频
	if (isLocal == false)
	{
		CString sConfig,sReal;
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CONFIG);
		pEdit->GetWindowText(sConfig);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_REAL);
		pEdit->GetWindowText(sReal);

		if (sConfig.GetLength() == 0 || sReal.GetLength() == 0)
		{
			AfxMessageBox(_T("实时视频地址和配置文件路径不能为空!"));
			return;
		}

		g_sConfigPath = T2A(sConfig);
		g_sRealVideoPath = T2A(sReal);
	}
	CDialog::OnOK();
}

void CDlgRealVideo::OnBnClickedButtonAddlocalv()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;

	CStatic* g1 = (CStatic*)GetDlgItem(IDC_STATIC_GROUP_REAL);
	CStatic* g2 = (CStatic*)GetDlgItem(IDC_STATIC_GROUP_LOCAL);

	CString strFilter( _T("视频文件(*.mp4;*.avi)|*.mp4;*.avi"));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal() == IDOK )
	{
		isLocal = true;
		g1->SetWindowTextW(_T("实时视频"));
		g2->SetWindowTextW(_T("本地视频(激活)"));
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
		g1->SetWindowTextW(_T("实时视频(激活)"));
		g2->SetWindowTextW(_T("本地视频"));
	}
	return;
}
