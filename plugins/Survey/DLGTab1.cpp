// DLGTab1.cpp : 实现文件
//

#include "stdafx.h"
#include "DLGTab1.h"
#include "afxdialogex.h"
#include "DLGHouseVisiableSurvey.h"
#include "HouseVisiableSurvey.h"

extern osg::ref_ptr<houseVisiableSurveyHandler> g_mHouseVisiableSurveyHandler;
// DLGTab1 对话框

IMPLEMENT_DYNAMIC(DLGTab1, CDialogEx)

DLGTab1::DLGTab1(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLGTab1::IDD, pParent)
{
	mRowMap.clear();
}

DLGTab1::~DLGTab1()
{
}

void DLGTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mWndList);
}


BEGIN_MESSAGE_MAP(DLGTab1, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &DLGTab1::OnLvnItemchangedList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &DLGTab1::OnNMClickList1)
END_MESSAGE_MAP()


// DLGTab1 消息处理程序


BOOL DLGTab1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	RECT rc;  
	GetClientRect(&rc);  
	//ScreenToClient(&rc);
	rc.bottom -= 45;
	mWndList.MoveWindow(&rc);  
	
	mWndList.InsertColumn (0, _T("字段名"), LVCFMT_LEFT, 200);
	mWndList.InsertColumn (1, _T("字段值"), LVCFMT_LEFT, 5);

	mWndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/|LVS_NOCOLUMNHEADER|LVS_EX_CHECKBOXES);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DLGTab1::InsertRow(CString sName, CString sValue)
{
	USES_CONVERSION;
	int n = mWndList.GetItemCount();
	mWndList.InsertItem(n, _T("1"));
	mWndList.SetItemText (n, 0, sName);
	mWndList.SetItemText (n, 1, sValue);
	mRowMap[sName] = true;
	setLVCheck(n,TRUE);
}

void DLGTab1::setLVCheck(WPARAM ItemIndex, BOOL bCheck)
{
	ListView_SetItemState(mWndList.m_hWnd, ItemIndex,
		UINT((int(bCheck) + 1) << 12), LVIS_STATEIMAGEMASK);
}


BOOL DLGTab1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)  
	{
		return true;  
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void DLGTab1::changeOneGeometry(CString name,bool bAdd)
{
	if (g_mHouseVisiableSurveyHandler)
	{
		g_mHouseVisiableSurveyHandler->changeOneGeometry(name,bAdd);
	}
}


void DLGTab1::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void DLGTab1::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	DWORD dwPos = GetMessagePos();
	CPoint pt(dwPos);
	mWndList.ScreenToClient(&pt);

	UINT nFlag;
	int nItem = mWndList.HitTest(pt, &nFlag);
	if (nFlag == LVHT_ONITEMSTATEICON)
	{
		BOOL bCheckState = mWndList.GetCheck(nItem);
		if (!bCheckState)
		{
			changeOneGeometry(mWndList.GetItemText(nItem,0),true);
		}
		else
			changeOneGeometry(mWndList.GetItemText(nItem,0),false);
	}
	*pResult = 0;
}
