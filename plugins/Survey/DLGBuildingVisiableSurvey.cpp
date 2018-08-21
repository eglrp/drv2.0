// DLGBuildingVisiableSurvey.cpp : implementation file
//

#include "stdafx.h"
#include "DLGBuildingVisiableSurvey.h"
#include "afxdialogex.h"
#include "DLGCreatorData.h"
#include "HouseVisiableSurvey.h"
// CDLGBuildingVisiableSurvey dialog

IMPLEMENT_DYNAMIC(CDLGBuildingVisiableSurvey, CDialogEx)

CDLGBuildingVisiableSurvey::CDLGBuildingVisiableSurvey(CString sDefLayer,CWnd* pParent /*=NULL*/)
	:mDefLayer(sDefLayer), 
	CDialogEx(CDLGBuildingVisiableSurvey::IDD, pParent)
{

}

CDLGBuildingVisiableSurvey::~CDLGBuildingVisiableSurvey()
{
}

void CDLGBuildingVisiableSurvey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLGBuildingVisiableSurvey, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDLGBuildingVisiableSurvey::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDLGBuildingVisiableSurvey message handlers

//创建数据
void CDLGBuildingVisiableSurvey::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDLGCreatorData dlg(mDefLayer);
	dlg.DoModal();
}


void CDLGBuildingVisiableSurvey::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CBuildingVisiableSurvey* p = (CBuildingVisiableSurvey*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}
