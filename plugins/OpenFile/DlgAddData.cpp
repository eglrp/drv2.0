// DlgAddData.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAddData.h"
#include "afxdialogex.h"
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "EarthMatrixFunctions.h"

#include <sstream>
#include <io.h>
#include <atlconv.h>
#include <atlstr.h>

USING_NAMESPACE_EARTHMATRIX

using namespace std;

std::string g_sDataPath;
vector<CString> vecPaths;
// CDlgAddData 对话框

IMPLEMENT_DYNAMIC(CDlgAddData, CDialogEx)

CDlgAddData::CDlgAddData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAddData::IDD, pParent)
{

}

CDlgAddData::~CDlgAddData()
{
}

void CDlgAddData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddData, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BRLOCAL, &CDlgAddData::OnBnClickedButtonBrlocal)
	ON_BN_CLICKED(IDOK, &CDlgAddData::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddData 消息处理程序


void CDlgAddData::OnBnClickedButtonBrlocal()
{
	USES_CONVERSION;
	CString filename,pathsList; 
	vecPaths.clear();
	CString strFilter(GETSTRINGT(_T("CDlgAddData"), _T("Filter"), _T("模型文件(*.ive;*.osg;*.osgb;*.osgt;*.dxf;*.shp;*.LAS)|*.ive;*.osg;*.osgb;*.osgt;*.cbs;*.dxf;*.shp;*.LAS|所有文件(*.*)|*.*||")));

	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT|OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal() != IDOK )
	{
		return;
	}
	else
	{
		POSITION fileNamesPosition = dlg.GetStartPosition(); 
		while(fileNamesPosition != NULL) 
		{ 
			filename = dlg.GetNextPathName(fileNamesPosition); 
			vecPaths.push_back(filename);
			pathsList = pathsList + filename + _T(";");
		}   
	}
	if (vecPaths.empty())
	{
		return;
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DATAURI);
	pEdit->SetWindowTextW(pathsList);
}


void CDlgAddData::OnBnClickedOk()
{
	USES_CONVERSION;

	CString sData;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DATAURI);
	pEdit->GetWindowText(sData);

	if (sData.GetLength() == 0)
	{
		AfxMessageBox(_T("实时视频地址和配置文件路径不能为空!"));
		return;
	}

	g_sDataPath = T2A(sData);
	CDialog::OnOK();
}


BOOL CDlgAddData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	//SetLayeredWindowAttributes(GetSysColor(COLOR_BTNFACE),100,1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
