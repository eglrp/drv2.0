// CvZLineSurveyInfoWin.cpp : 实现文件
//

#include "stdafx.h"
#include "vZLineSurveyInfoWin.h"
#include "afxdialogex.h"
#include "vlLineSurveyTri.h"
#include "IViewer3D.h"
#include "EarthMatrixFunctions.h"
#include "ComFun.h"

// CvZLineSurveyInfoWin 对话框

IMPLEMENT_DYNAMIC(CvZLineSurveyInfoWin, CDialogEx)

CvZLineSurveyInfoWin::CvZLineSurveyInfoWin(CWnd* pParent /*=NULL*/)
    : CDialogEx(CvZLineSurveyInfoWin::IDD, pParent)
	, m_bPath(false)
{

}

CvZLineSurveyInfoWin::~CvZLineSurveyInfoWin()
{
}

void CvZLineSurveyInfoWin::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CvZLineSurveyInfoWin, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_PATHLINESURVEY, &CvZLineSurveyInfoWin::OnBnClickedCheckPathlinesurvey)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

void CvZLineSurveyInfoWin::SetDist(double distance, double distance1, double distance2)
{
	std::wstringstream sstr;

	std::wstring app = x3::a2w("PowerMatrix");
	if( distance1>std::numeric_limits<double>::epsilon() && fabs(distance1 - distance)>std::numeric_limits<double>::epsilon() )
		sstr << GETSTRINGW(app.c_str(), L"PreDistance:", L"前段距离:") << " " << RoundOffToString(distance1, 2) << L"（米）\r\n";
	if( distance2>std::numeric_limits<double>::epsilon() && fabs(distance2 - distance)>std::numeric_limits<double>::epsilon() )
		sstr << GETSTRINGW(app.c_str(), L"CurDistance:", L"当前距离:") << " " << RoundOffToString(distance2, 2) << L"（米）\r\n";
	if( distance>0 )
		sstr << GETSTRINGW(app.c_str(), L"TotalDistance:", L"总 距 离:") << " " << RoundOffToString(distance, 2) << L"（米）\r\n";

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_LINESURVEY_DIST);

	pEdit->SetWindowTextW(CString(sstr.str().c_str()));
}

// CvZLineSurveyInfoWin 消息处理程序


void CvZLineSurveyInfoWin::OnBnClickedCheckPathlinesurvey()
{
	CButton *p=(CButton*)GetDlgItem(IDC_CHECK_PATHLINESURVEY);
	int x=p->GetCheck();

	CvlLineSurveyTri* lst = (CvlLineSurveyTri*)parent;
	lst->SetPathMode(x);
}


void CvZLineSurveyInfoWin::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CvlLineSurveyTri* p = (CvlLineSurveyTri*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}
