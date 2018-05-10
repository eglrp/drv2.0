// AreaSurveyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AreaSurveyDlg.h"
#include "afxdialogex.h"
#include "vlLineSurveyArea.h"
#include "ComFun.h"
#include "IViewer3D.h"
#include "EarthMatrixFunctions.h"

// CAreaSurveyDlg �Ի���

IMPLEMENT_DYNAMIC(CAreaSurveyDlg, CDialogEx)

CAreaSurveyDlg::CAreaSurveyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAreaSurveyDlg::IDD, pParent)
{

}

CAreaSurveyDlg::~CAreaSurveyDlg()
{
}

void CAreaSurveyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAreaSurveyDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAreaSurveyDlg ��Ϣ�������


void CAreaSurveyDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CvlLineSurveyArea* p = (CvlLineSurveyArea*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}

void CAreaSurveyDlg::SetArea(double area1, double area2)
{
	std::wstringstream sstr;

	std::wstring app = x3::a2w("PowerMatrix");

	sstr << GETSTRINGW(app.c_str(), L"TotalArea:", L"�� �� ��:") << " " << RoundOffToString(area1, 2) << L"��ƽ���ף�\r\n";

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_AREA);

	pEdit->SetWindowTextW(CString(sstr.str().c_str()));
}