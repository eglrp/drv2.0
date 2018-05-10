// PathManaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PathManaDlg.h"
#include "afxdialogex.h"
#include "vlWalkMana.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/FileUtils"
#include "ComFun.h"
#include "osgDB/FileNameUtils"

// CPathManaDlg 对话框

IMPLEMENT_DYNAMIC(CPathManaDlg, CDialogEx)

CPathManaDlg::CPathManaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPathManaDlg::IDD, pParent)
{
	m_curPath = "";
}

CPathManaDlg::~CPathManaDlg()
{
}

void CPathManaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PATH, m_wndList);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnActive);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
}


BEGIN_MESSAGE_MAP(CPathManaDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CPathManaDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CPathManaDlg::OnBnClickedButtonDel)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_PATH, &CPathManaDlg::OnNMClickListPath)
	ON_BN_CLICKED(IDC_BUTTON_ACTIVE, &CPathManaDlg::OnBnClickedButtonActive)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CPathManaDlg::OnBnClickedButtonRefresh)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_IMPATH, &CPathManaDlg::OnBnClickedBtnImpath)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CPathManaDlg::OnEnChangeEditName)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PATH, &CPathManaDlg::OnNMClickListPath)
END_MESSAGE_MAP()


// CPathManaDlg 消息处理程序
BOOL CPathManaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_wndList.InsertColumn (0, _T("名称"), LVCFMT_LEFT, 160);
	m_wndList.InsertColumn (1, _T("时长"), LVCFMT_LEFT, 40);
	m_wndList.InsertColumn (2, _T("状态"), LVCFMT_LEFT, 40);

	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPathManaDlg::OnBnClickedButtonAdd()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
	CString sName;
	pEdit->GetWindowTextW(sName);

	int count = m_wndList.GetItemCount();

	for(int i = 0; i < count; i++)
	{
		CString str = m_wndList.GetItemText(i, 0);
		if (sName == str)
		{
			AfxMessageBox(_T("列表中名称已经存在，请换一个路径名！"));
			return;
		}
	}

	CvlWalkMana* p = (CvlWalkMana*)parent;
	if (p->AddPath(sName))
	{
		m_wndList.InsertItem(0, _T("1"));
		m_wndList.SetItemText (0, 0, sName);
	}
	else
		AfxMessageBox(_T("名称重复了！"));
}


void CPathManaDlg::OnBnClickedButtonDel()
{
	CString strPrompt(GETSTRINGT(L"WalkMana", _T("DelPrompt"), _T("确认删除么？")));

	if ( AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION ) == IDYES)
	{
	}
	else
	{
		return;
	}
	while(m_wndList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int nItem = m_wndList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		CString str = m_wndList.GetItemText(nItem, 0);
		if (str == m_curPath)
		{
			CvlWalkMana* p = (CvlWalkMana*)parent;
			p->RecordPath(L"");
		}
		CvlWalkMana* p = (CvlWalkMana*)parent;
		p->DelPath(str);
		m_wndList.DeleteItem(nItem);
	}
}

void CPathManaDlg::OnNMClickListPath(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = m_wndList.GetSelectionMark();
	if (nItem >= 0)
	{
		CString str = m_wndList.GetItemText(nItem, 0);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
		pEdit->SetWindowTextW(str);

		m_curPath = str;
	}
	*pResult = 0;
}

void CPathManaDlg::OnBnClickedButtonActive()
{
	// TODO: 在此添加控件通知处理程序代码
	CvlWalkMana* p = (CvlWalkMana*)parent;
	if (m_curPath == L"")
	{
		AfxMessageBox(L"没有选中路径！");
	}
	p->RecordPath(m_curPath);
}

void CPathManaDlg::InitListData(CStringArray* aPaths)
{
	USES_CONVERSION;
	m_wndList.DeleteAllItems();
	CvlWalkMana* p = (CvlWalkMana*)this->parent;

	for (int i=0; i < aPaths->GetCount(); i++)
	{
		CString sL = aPaths->GetAt(i);
		CString sFileName = sL.Right(sL.GetLength() - sL.ReverseFind('\\') - 1);
		sFileName = sFileName.Left(sFileName.ReverseFind('.'));
		m_wndList.InsertItem(0, _T("1"));
		m_wndList.SetItemText (0, 0, sFileName);

		CString fpath = m_sProjectPath.Left(m_sProjectPath.ReverseFind('\\') + 1) + sFileName + L".path";
		CString fpathpath = m_sProjectPath.Left(m_sProjectPath.ReverseFind('\\') + 1) + sFileName + L".path.path";
		std::string path = W2A(fpath.GetBuffer(0));
		std::string pathpath = W2A(fpathpath.GetBuffer(0));

		if (osgDB::fileExists(path))
		{
			CString sC = getFileContent(fpath);//p->GetFileContent(path);
			CStringArray a;
			AnalyzeData(a, sC, L"\r\n");
			CString sPtNum = L"";
			sPtNum.Format(L"%d", a.GetCount() - 1);
			m_wndList.SetItemText (0, 1, sPtNum);
			m_wndList.SetItemText (0, 2, L"正常");
		}

		if (osgDB::fileExists(path) && !osgDB::fileExists(pathpath) )
		{
			CString sC = getFileContent(fpath);//p->GetFileContent(path);
			CStringArray a;
			AnalyzeData(a, sC, L"\r\n");
			p->BuildPath(path, &a);
		}
	}
}

void CPathManaDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CvlWalkMana* p = (CvlWalkMana*)this->parent;
	p->OnClick();
	CDialogEx::OnClose();
}


void CPathManaDlg::OnBnClickedButtonRefresh()
{
	USES_CONVERSION;

	CvlWalkMana* p = (CvlWalkMana*)this->parent;

	int count = m_wndList.GetItemCount();

	for(int i = 0; i < count; i++)
	{
		CString str = m_wndList.GetItemText(i, 0);
		CString fpath = m_sProjectPath.Left(m_sProjectPath.ReverseFind('\\') + 1) + str + L".path";
		std::string path = W2A(fpath.GetBuffer(0));

		if (osgDB::fileExists(path))
		{
			CString sC = getFileContent(fpath);//p->GetFileContent(path);
			CStringArray a;
			AnalyzeData(a, sC, L"\r\n");
			CString sPtNum = L"";
			sPtNum.Format(L"%d", a.GetCount() - 1);
			m_wndList.SetItemText (i, 1, sPtNum);
			m_wndList.SetItemText (i, 2, L"正常");

			p->BuildPath(path, &a);
		}
	}
}

void CPathManaDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CPathManaDlg::OnBnClickedBtnImpath()
{
	USES_CONVERSION;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"DRV Path File(*.path)|*.path||", NULL);

	if(dlg.DoModal() == IDOK)
	{
		CString sPath = dlg.GetPathName();
		if ( !osgDB::fileExists( W2A(sPath.GetBuffer(0)) ) )
		{
			CString strFileNotExist(GETSTRINGT(L"Path", _T("OpenPathFileNotExist"), _T("漫游路径文件不存在，请重新选择！")));
			AfxMessageBox(strFileNotExist);
		}
		else
		{
			std::string filename = osgDB::getStrippedName(W2A(sPath.GetBuffer(0)));
			CString sName = CString(filename.c_str());
			int count = m_wndList.GetItemCount();

			for(int i = 0; i < count; i++)
			{
				CString str = m_wndList.GetItemText(i, 0);
				if (sName == str)
				{
					AfxMessageBox(_T("路径名已经存在，请换一个路径名！"));
					return;
				}
			}

			CvlWalkMana* p = (CvlWalkMana*)parent;

			if (p->ImportPath(sPath))
			{
				m_wndList.InsertItem(0, _T("1"));
				m_wndList.SetItemText (0, 0, sName);
			}
			OnBnClickedButtonRefresh();
		}
	}
	else
	{

	}
}


void CPathManaDlg::OnEnChangeEditName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}