#include "StdAfx.h"
#include "vPointSurveyInfoWin.h"
#include "vlPointSurvey.h"

IMPLEMENT_DYNAMIC(CPointSurveyInfoWin, CDialogEx)

CPointSurveyInfoWin::CPointSurveyInfoWin(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPointSurveyInfoWin::IDD, pParent)

{
	pChild = nullptr;
}


CPointSurveyInfoWin::~CPointSurveyInfoWin(void)
{
}


void CPointSurveyInfoWin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_QC, m_Static);
}



void CPointSurveyInfoWin::SetText(std::string xtext, std::string ytext, std::string ztext, std::string lontext, std::string lattext)
{
	if (pChild != nullptr)
	{
		pChild->DestroyWindow();
		pChild = nullptr;
	}

    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_X);
    pEdit->SetWindowTextW(CString(xtext.c_str()));

    CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_Y);
    pEdit2->SetWindowTextW(CString(ytext.c_str()));


    CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_Z);
    pEdit3->SetWindowTextW(CString(ztext.c_str()));

	CEdit* pEditAll = (CEdit*)GetDlgItem(IDC_EDIT_ALL);

	CString sAll;
	sAll.Format(_T("%s,%s"), CString(lontext.c_str()), CString(lattext.c_str()));
	pEditAll->SetWindowTextW(sAll);

	slon = lontext;
	slat = lattext;
}


BEGIN_MESSAGE_MAP(CPointSurveyInfoWin, CDialogEx)
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDC_STATIC_QC, &CPointSurveyInfoWin::OnStnClickedStaticQc)
	ON_BN_CLICKED(IDC_BUTTON_QRENCODE1, &CPointSurveyInfoWin::OnBnClickedButtonQrencode1)
END_MESSAGE_MAP()


void CPointSurveyInfoWin::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CvlPointSurvey* p = (CvlPointSurvey*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}


BOOL CPointSurveyInfoWin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect;
	m_Static.GetWindowRect(&rect);
	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, _T("Data\\DRVdata\\qc.bmp"), IMAGE_BITMAP, rect.Width(), rect.Height(), LR_LOADFROMFILE);
	m_Static.ModifyStyle(NULL, SS_BITMAP);
	m_Static.SetBitmap(hBmp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPointSurveyInfoWin::OnStnClickedStaticQc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::string locStr = std::string("-o Data\\DRVdata\\pointLocation.png ") + \
		"http://api.map.baidu.com/marker?location=" + slat + "," + slon +   \
		"&coord_type=wgs84&title=" + "����Զ����λ��" + "&content=" + "����Զ����λ��" + "(" + slon + "," + slat + \
		")&output=html";
	DeleteFile(_T("Data\\DRVdata\\pointLocation.png"));

	BeginWaitCursor();
	ShellExecute(this->m_hWnd, _T("open"), _T("qrencode.exe"), CString(locStr.c_str()), _T(""), SW_HIDE);
	EndWaitCursor();
	while( !PathFileExists(_T("Data\\DRVdata\\pointLocation.png")) );

	
	if (pChild == nullptr)
	{
		pChild = new CQRencode2Dlg();
		pChild->Create(IDD_DIALOG_QRENCODE2);
	}
	pChild->ShowWindow(SW_SHOW);
	
}


void CPointSurveyInfoWin::OnBnClickedButtonQrencode1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnStnClickedStaticQc();
}
