// AreaSurveyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AreaSurveyDlg.h"
#include "afxdialogex.h"
#include "vlLineSurveyArea.h"
#include "ComFun.h"
#include "IViewer3D.h"
#include "EarthMatrixFunctions.h"

// CAreaSurveyDlg 对话框

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


// CAreaSurveyDlg 消息处理程序


void CAreaSurveyDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CvlLineSurveyArea* p = (CvlLineSurveyArea*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}

void CAreaSurveyDlg::SetArea(double area1, double area2)
{
	std::wstringstream sstr;

	std::wstring app = x3::a2w("PowerMatrix");

	sstr << GETSTRINGW(app.c_str(), L"TotalArea:", L"总 面 积:") << " " << RoundOffToString(area1, 2) << L"（平方米）\r\n";

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_AREA);

	pEdit->SetWindowTextW(CString(sstr.str().c_str()));
}