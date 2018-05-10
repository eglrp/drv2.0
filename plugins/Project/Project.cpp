// Project.cpp : 实现文件
//

#include "stdafx.h"
#include "Project.h"
#include "afxdialogex.h"
#include "EarthMatrixExports.h"
#include "osgDB/FileUtils"
#include "EarthMatrixFunctions.h"
#include "IViewer3D.h"

// CProject 对话框

IMPLEMENT_DYNAMIC(CProject, CDialogEx)

CProject::CProject(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProject::IDD, pParent)
{

}

CProject::~CProject()
{
}

void CProject::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProject, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CProject::OnBnClickedOK)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CProject::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CProject 消息处理程序


BOOL CProject::OnInitDialog()
{
	AFX_MANAGE_STATE_EX;
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//选择项目文件保存路径
void CProject::OnBnClickedOK()
{
	USES_CONVERSION;

	CString strNewProjName(GETSTRINGT(L"Project", _T("NewProjectName"), _T("项目1")));

    CFileDialog dlg(TRUE,_T(".drvp"),strNewProjName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"DRV Project File(*.drvp)|*.drvp||",NULL);  
      
    if(dlg.DoModal() == IDOK)  
    {  
        m_strFilePath = dlg.GetPathName();
    }  
    else  
	{

    }

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATH);
	pEdit->SetWindowTextW(m_strFilePath);
}



void CProject::OnBnClickedButtonSave()
{
	USES_CONVERSION;

	CString strSelectFile(GETSTRINGT(L"Project", _T("SelectFile"), _T("请先选择项目文件保存路径！")));

	std::string filename(T2A(m_strFilePath));
	if (osgDB::fileExists(filename))
	{
		CString strPrompt(GETSTRINGT(L"Project", _T("ExistPrompt"), _T("项目文件已经存在，确认覆盖么？")));
		if ( AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION ) == IDYES)
		{
		}
		else
		{
			CString strSaveCancel(GETSTRINGT(L"Project", _T("SaveCancel"), _T("保存取消！")));
			AfxMessageBox(strSaveCancel);
			m_strFilePath = _T("");
		}
	}
	else
	{
		if (m_strFilePath.GetLength() == 0)
		{
			AfxMessageBox(strSelectFile);
			return;
		}
	}

	this->OnOK();
}
