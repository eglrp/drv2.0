// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// EarthMatrixRibbon.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "EarthMatrixRibbon.h"
#include "MainFrm.h"

#include "EarthMatrixRibbonDoc.h"
#include "EarthMatrixRibbonView.h"
#include "PluginManager.h"
#include "SplashWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef _LOCK
#pragma comment(lib,"CheckModule.lib")
extern "C" int CheckModule(int iModuleID,int CheckDongleLicense);
#endif
// CEarthMatrixRibbonApp

BEGIN_MESSAGE_MAP(CEarthMatrixRibbonApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEarthMatrixRibbonApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEarthMatrixRibbonApp ����

CEarthMatrixRibbonApp::CEarthMatrixRibbonApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EarthMatrixRibbon.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CEarthMatrixRibbonApp ����

CEarthMatrixRibbonApp theApp;

char* CString2char(CString& str)
{
	int len = str.GetLength();
	char* chRtn = (char*)malloc((len * 2 + 1) * sizeof(char)); //CString�ĳ����к�����һ������
	memset(chRtn, 0, 2 * len + 1);
	USES_CONVERSION;
#if _MSC_VER>=1400
	strcpy((LPSTR)chRtn, OLE2A(str.LockBuffer()));
#else
	_tcscpy(chRtn, str);
#endif
	return chRtn;
}
// CEarthMatrixRibbonApp ��ʼ��

BOOL CEarthMatrixRibbonApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if( !CPluginManager::Initialize() )
		return FALSE;

	CWinAppEx::InitInstance();
#ifdef _LOCK
	int a = CheckModule(78,0);
	a=1;
	if (a)
	{
		
	}
	else
	{
		AfxMessageBox(_T("û�м�⵽����������������˳���"));
		ExitInstance();
		return FALSE;

		/*
		GetExePath();

		GetHWInfoFromDll();

		CString sRegCode =  MD5(CString2char(m_sHWinfo)).Left(32);
		CString sRegCodeTxt;
		CFile file;
		if (file.Open(_T("reg.txt"), CFile::modeReadWrite))
		{
			long length = (long)file.GetLength();
			char *pbuf = new char[length];
			file.Read(pbuf, length);
			sRegCodeTxt = CString(pbuf).Left(length);

			file.Close();
		}

		if (sRegCodeTxt != sRegCode)
		{
			AfxMessageBox(L"ע�����ļ�����ȷ�������˳���");
			ExitInstance();
			return FALSE;
		}

		//���д����ж�
		CFile fileNum;
		if (fileNum.Open(_T("C:\\Windows\\num.txt"), CFile::modeReadWrite))
		{
		}
		else
		{
			fileNum.Open(_T("C:\\Windows\\num.txt"), CFile::modeCreate|CFile::modeReadWrite);
			fileNum.Write("400", 3);
			fileNum.SeekToBegin();
		}
		char *pbufNum = new char[3];
		fileNum.Read(pbufNum, 3);
		int num = _atoi64(pbufNum);
		if (num > 100)
		{
			num--;
			fileNum.SeekToBegin();
			fileNum.Write(NULL, 0);

			CString s;
			s.Format(_T("%d"), num);
			fileNum.Write(CString2char(s),3);
		}
		else
		{
			AfxMessageBox(_T("���ô����Ѿ����꣬�����˳���"));
			ExitInstance();
			return FALSE;
		}
		*/
	}
#endif	

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	//InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	//��ʾ�����Ի���
	CSplashWnd *pSplashWindow = new CSplashWnd;//��������
	pSplashWindow->create();
	pSplashWindow->CenterWindow();
	pSplashWindow->ShowWindow(SW_SHOW);  //��ʾ����
	pSplashWindow->UpdateWindow();
	Sleep(2000);  //��ʾ�����������ʱ��
	
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEarthMatrixRibbonDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CEarthMatrixRibbonView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	//֣˼�ɣ�2016��7��29�ղ�Ҫ�������޸ģ���Ȼ������Ҳ�ɿհ׵���
	//��PreCreateWindow���޸ľͿ�����
	//m_pMainWnd->SetWindowText(_T(""));
	pSplashWindow->DestroyWindow(); //������������
	delete pSplashWindow; //ɾ��
	return TRUE;
}

int CEarthMatrixRibbonApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CEarthMatrixRibbonApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CEarthMatrixRibbonApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEarthMatrixRibbonApp �Զ������/���淽��

void CEarthMatrixRibbonApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CEarthMatrixRibbonApp::LoadCustomState()
{
}

void CEarthMatrixRibbonApp::SaveCustomState()
{
}

CString CEarthMatrixRibbonApp::MD5(char* szSource)
{
	char* pTarget;
	typedef char* (*LPFUN)(char*);

#ifndef _WIN64
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll32.dll")); //����б�Ҫ��д��·��
#else
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll64.dll")); //����б�Ҫ��д��·��
#endif

	LPFUN MD5 = (LPFUN)GetProcAddress(hLib, "MD5");

	if (hLib == NULL || MD5 == NULL)
	{
		AfxMessageBox(_T("GetHWInfoDll.dll���س���"));
	}
	else
	{
		pTarget = (*MD5)(szSource);
		CString sTarget = CString(pTarget);

#ifdef _WIN64
		CFile file;
		if (file.Open(_T("temp2.tmp"), CFile::modeReadWrite))
		{
			long length = (long)file.GetLength();
			char *pbuf = new char[length];
			file.Read(pbuf, length);
			sTarget = CString(pbuf).Left(length);
		}
		file.Close();
		DeleteFile(_T("temp2.tmp"));
#endif

		FreeLibrary(hLib);
		return sTarget;
	}
	return _T("");
}

// CEarthMatrixRibbonApp ��Ϣ�������
void CEarthMatrixRibbonApp::GetHWInfoFromDll()
{
	//��ȡӲ����
	char* pHWInfo;
	typedef char* (*LPFUN)(void);

#ifndef _WIN64
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll32.dll")); //����б�Ҫ��д��·��
#else
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll64.dll")); //����б�Ҫ��д��·��
#endif

	LPFUN GetHWInfo = (LPFUN)GetProcAddress(hLib, "GetHWInfo");

	if (hLib == NULL)
	{
		AfxMessageBox(_T("GetHWInfoDll.dll���س���"));
	}
	else
	{
		pHWInfo = (*GetHWInfo)();
		m_sHWinfo = CString(pHWInfo);
		//#ifdef _WIN64
		CFile file;
		if (file.Open(_T("temp.tmp"), CFile::modeReadWrite))
		{
			m_sHWinfo = _T("");
			long length = (long)file.GetLength();
			char *pbuf = new char[length]();
			file.Read(pbuf, length);
			m_sHWinfo = CString(pbuf).Left(length);
		}
		file.Close();
		DeleteFile(_T("temp.tmp"));
		//#endif
		m_sHWinfo = m_sHWinfo.Left(8);
		FreeLibrary(hLib);
	}
	//��ȡӲ���� END
}


BOOL CEarthMatrixRibbonApp::GetExePath()
{
	TCHAR modulePath[512];

	DWORD pathLength = GetModuleFileName(NULL, modulePath, 512);

	if (pathLength)
	{
		CString sPath;
		sPath = modulePath;
		int rs = sPath.ReverseFind('\\'); //�������һ��'\\'��λ��
		m_sInstallPath = sPath.Left(rs);            //ȥ���ļ���
		return TRUE;
	}
	return FALSE;
}