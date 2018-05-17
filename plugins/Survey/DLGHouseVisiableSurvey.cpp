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
	ON_BN_CLICKED(IDC_BUTTON_SELALL, &DLGHouseVisiableSurvey::OnBnClickedButtonSelall)
	ON_BN_CLICKED(IDC_BUTTON_SELINV, &DLGHouseVisiableSurvey::OnBnClickedButtonSelinv)
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
	//mPage1.parent = (CObject*)this;
	//mPage2.parent = (CObject*)this;

	CRect rc;  
	mTab.GetClientRect(rc);  
	rc.top += 20;  
	rc.bottom -= 0;  
	rc.left += 0;  
	rc.right -= 0;  
	mPage1.MoveWindow(&rc);  
	mPage2.MoveWindow(&rc);  

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


void DLGHouseVisiableSurvey::OnBnClickedButtonSelall()
{
	// TODO: Add your control notification handler code here
	DLGTab1* pDlg = (DLGTab1*)mpDialog[mCurSelTab];
	pDlg->mRowMap.clear();
	int num = pDlg->mWndList.GetItemCount();
	for (int i = 0;i<num;++i)
	{
		pDlg->setLVCheck(i,true);
		CString s = pDlg->mWndList.GetItemText(i,0);
		pDlg->mRowMap[s] = true;
	}
}


void DLGHouseVisiableSurvey::OnBnClickedButtonSelinv()
{
	// TODO: Add your control notification handler code here
	DLGTab1* pDlg = (DLGTab1*)mpDialog[mCurSelTab];
	pDlg->mRowMap.clear();
	int num = pDlg->mWndList.GetItemCount();
	for (int i = 0;i<num;++i)
	{
		bool bmask = ListView_GetCheckState(pDlg->mWndList, i);
		pDlg->setLVCheck(i,!bmask);
		CString s = pDlg->mWndList.GetItemText(i,0);
		pDlg->mRowMap[s] = !bmask;
	}
}



BOOL DLGHouseVisiableSurvey::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc�����¼� 
			OnClose();
			return true;  
		case VK_RETURN: //Enter�����¼� 
			OnClose();
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}
