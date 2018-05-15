// DLGHouseVisiableSurvey.cpp : implementation file
//

#include "stdafx.h"
#include "DLGHouseVisiableSurvey.h"
#include "afxdialogex.h"
#include "HouseVisiableSurvey.h"

// DLGHouseVisiableSurvey dialog

IMPLEMENT_DYNAMIC(DLGHouseVisiableSurvey, CDialogEx)

DLGHouseVisiableSurvey::DLGHouseVisiableSurvey(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLGHouseVisiableSurvey::IDD, pParent)
{

}

DLGHouseVisiableSurvey::~DLGHouseVisiableSurvey()
{
}

void DLGHouseVisiableSurvey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
}


BEGIN_MESSAGE_MAP(DLGHouseVisiableSurvey, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &DLGHouseVisiableSurvey::OnTcnSelchangeTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// DLGHouseVisiableSurvey message handlers


BOOL DLGHouseVisiableSurvey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mTab.InsertItem(0, _T("����"));  
	mTab.InsertItem(1, _T("������"));
	mPage1.Create(IDD_DIALOG_TAB1, &mTab);
	mPage2.Create(IDD_DIALOG_TAB1, &mTab);

	CRect rc;  
	mTab.GetClientRect(rc);  
	rc.top += 20;  
	rc.bottom -= 0;  
	rc.left += 0;  
	rc.right -= 0;  
	mPage1.MoveWindow(&rc);  
	mPage2.MoveWindow(&rc);  

	mPage1.InsertRow(L"01-01-0201",L"����");
	mPage2.InsertRow(L"02-01-0201",L"����");
	//�ѶԻ������ָ�뱣������  
	mpDialog[0] = &mPage1;  
	mpDialog[1] = &mPage2;  
	//��ʾ��ʼҳ��  
	mpDialog[0]->ShowWindow(SW_SHOW);  
	mpDialog[1]->ShowWindow(SW_HIDE);  
	//���浱ǰѡ��  
	mCurSelTab = 0;  
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void DLGHouseVisiableSurvey::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mpDialog[mCurSelTab]->ShowWindow(SW_HIDE);  
	//�õ��µ�ҳ������  
	mCurSelTab = mTab.GetCurSel();  
	//���µ�ҳ����ʾ����  
	mpDialog[mCurSelTab]->ShowWindow(SW_SHOW);  
	*pResult = 0;
}


void DLGHouseVisiableSurvey::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CHouseVisiableSurvey* p = (CHouseVisiableSurvey*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}



