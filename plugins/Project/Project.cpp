// Project.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Project.h"
#include "afxdialogex.h"
#include "EarthMatrixExports.h"
#include "osgDB/FileUtils"
#include "EarthMatrixFunctions.h"
#include "IViewer3D.h"

// CProject �Ի���

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


// CProject ��Ϣ�������


BOOL CProject::OnInitDialog()
{
	AFX_MANAGE_STATE_EX;
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//ѡ����Ŀ�ļ�����·��
void CProject::OnBnClickedOK()
{
	USES_CONVERSION;

	CString strNewProjName(GETSTRINGT(L"Project", _T("NewProjectName"), _T("��Ŀ1")));

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

	CString strSelectFile(GETSTRINGT(L"Project", _T("SelectFile"), _T("����ѡ����Ŀ�ļ�����·����")));

	std::string filename(T2A(m_strFilePath));
	if (osgDB::fileExists(filename))
	{
		CString strPrompt(GETSTRINGT(L"Project", _T("ExistPrompt"), _T("��Ŀ�ļ��Ѿ����ڣ�ȷ�ϸ���ô��")));
		if ( AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION ) == IDYES)
		{
		}
		else
		{
			CString strSaveCancel(GETSTRINGT(L"Project", _T("SaveCancel"), _T("����ȡ����")));
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
