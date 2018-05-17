// DLGHouseVisiableSurvey.cpp : implementation file
//

#include "stdafx.h"
#include "DLGHouseVisiableSurvey.h"
#include "afxdialogex.h"
#include "HouseVisiableSurvey.h"

extern osg::ref_ptr<houseVisiableSurveyHandler> g_mHouseVisiableSurveyHandler;
// DLGHouseVisiableSurvey dialog

IMPLEMENT_DYNAMIC(DLGHouseVisiableSurvey, CDialogEx)

DLGHouseVisiableSurvey::DLGHouseVisiableSurvey(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLGHouseVisiableSurvey::IDD, pParent)
	, mStaticNum(_T(""))
	, mStrIgnore(_T(""))
	, mStrInsertVal(_T(""))
{

}

DLGHouseVisiableSurvey::~DLGHouseVisiableSurvey()
{
}

void DLGHouseVisiableSurvey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, mTab);
	DDX_Text(pDX, IDC_STATIC_NUM, mStaticNum);
	DDX_Control(pDX, IDC_CHECK2, mCheckIgnore);
	DDX_Control(pDX, IDC_CHECK3, mCheckInsrtVal);
	DDX_Text(pDX, IDC_EDIT1, mStrIgnore);
	DDX_Text(pDX, IDC_EDIT2, mStrInsertVal);
	DDX_Control(pDX, IDC_EDIT1, mEditIgnore);
	DDX_Control(pDX, IDC_EDIT2, mEditInsertVal);
}


BEGIN_MESSAGE_MAP(DLGHouseVisiableSurvey, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &DLGHouseVisiableSurvey::OnTcnSelchangeTab1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SELALL, &DLGHouseVisiableSurvey::OnBnClickedButtonSelall)
	ON_BN_CLICKED(IDC_BUTTON_SELINV, &DLGHouseVisiableSurvey::OnBnClickedButtonSelinv)
	ON_BN_CLICKED(IDC_CHECK2, &DLGHouseVisiableSurvey::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &DLGHouseVisiableSurvey::OnBnClickedCheck3)
END_MESSAGE_MAP()


// DLGHouseVisiableSurvey message handlers


BOOL DLGHouseVisiableSurvey::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mTab.InsertItem(0, _T("可视"));  
	mTab.InsertItem(1, _T("不可视"));
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
	mPage1.changeListPos();
	mPage2.changeListPos();
	//把对话框对象指针保存起来  
	mpDialog[0] = &mPage1;  
	mpDialog[1] = &mPage2;  
	//显示初始页面  
	mpDialog[0]->ShowWindow(SW_SHOW);  
	mpDialog[1]->ShowWindow(SW_HIDE);  
	//保存当前选择  
	mCurSelTab = 0;

	mEditIgnore.EnableWindow(FALSE);
	mEditInsertVal.EnableWindow(FALSE);
	mCheckIgnore.SetCheck(FALSE);
	mCheckInsrtVal.SetCheck(FALSE);
	mStrIgnore = _T("3");
	mStrInsertVal = _T("1");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DLGHouseVisiableSurvey::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int lastTab = mCurSelTab;
	mCurSelTab = mTab.GetCurSel();
	if (lastTab == mCurSelTab)
	{
		return;
	}
	mpDialog[lastTab]->ShowWindow(SW_HIDE);   
	//把新的页面显示出来  
	mpDialog[mCurSelTab]->ShowWindow(SW_SHOW);
	DLGTab1* pDlg = (DLGTab1*)mpDialog[mCurSelTab];
	int num = pDlg->mWndList.GetItemCount();
	g_mHouseVisiableSurveyHandler->clearGeodeGroup(g_mHouseVisiableSurveyHandler->gTemp);
	for (int i = 0;i<num;++i)
	{
		bool bmask = ListView_GetCheckState(pDlg->mWndList, i);
		if (!bmask)
		{	
			continue;
		}
		CString s = pDlg->mWndList.GetItemText(i,0);
		g_mHouseVisiableSurveyHandler->changeOneGeometry(s,true);
	}
	updateStaticControl(num,  mPage1.mWndList.GetItemCount() + mPage2.mWndList.GetItemCount());
	
	*pResult = 0;
}

void DLGHouseVisiableSurvey::updateStaticControl(int num1,int num2)
{
	mStaticNum.Format(_T("%d/%d"),num1,  num2);
	UpdateData(FALSE);
}

void DLGHouseVisiableSurvey::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CHouseVisiableSurvey* p = (CHouseVisiableSurvey*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}


void DLGHouseVisiableSurvey::OnBnClickedButtonSelall()
{
	// TODO: Add your control notification handler code here
	if (!g_mHouseVisiableSurveyHandler)
	{
		return;
	}
	DLGTab1* pDlg = (DLGTab1*)mpDialog[mCurSelTab];
	pDlg->mRowMap.clear();
	int num = pDlg->mWndList.GetItemCount();
	for (int i = 0;i<num;++i)
	{
		bool bmask = ListView_GetCheckState(pDlg->mWndList, i);
		if(bmask)
		{
			continue;
		}
		pDlg->setLVCheck(i,true);
		CString s = pDlg->mWndList.GetItemText(i,0);
		pDlg->mRowMap[s] = true;
		g_mHouseVisiableSurveyHandler->changeOneGeometry(s,true);
	}

}


void DLGHouseVisiableSurvey::OnBnClickedButtonSelinv()
{
	// TODO: Add your control notification handler code here
	if (!g_mHouseVisiableSurveyHandler)
	{
		return;
	}
	DLGTab1* pDlg = (DLGTab1*)mpDialog[mCurSelTab];
	pDlg->mRowMap.clear();
	int num = pDlg->mWndList.GetItemCount();
	g_mHouseVisiableSurveyHandler->clearGeodeGroup(g_mHouseVisiableSurveyHandler->gTemp);
	for (int i = 0;i<num;++i)
	{
		bool bmask = ListView_GetCheckState(pDlg->mWndList, i);
		pDlg->setLVCheck(i,!bmask);
		if (bmask)
		{	
			continue;
		}
		CString s = pDlg->mWndList.GetItemText(i,0);
		pDlg->mRowMap[s] = !bmask;
		g_mHouseVisiableSurveyHandler->changeOneGeometry(s,true);
	}
}



BOOL DLGHouseVisiableSurvey::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc按键事件 
			OnClose();
			return true;  
		case VK_RETURN: //Enter按键事件 
			OnClose();
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void DLGHouseVisiableSurvey::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	if (mCheckIgnore.GetCheck())
	{
		mEditIgnore.EnableWindow(TRUE);
	}
	else
		mEditIgnore.EnableWindow(FALSE);
}


void DLGHouseVisiableSurvey::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	if (mCheckInsrtVal.GetCheck())
	{
		mEditInsertVal.EnableWindow(TRUE);
	}
	else
		mEditInsertVal.EnableWindow(FALSE);
}
