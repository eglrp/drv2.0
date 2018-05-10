// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// EarthMatrixRibbon.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEarthMatrixRibbonApp 构造

CEarthMatrixRibbonApp::CEarthMatrixRibbonApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EarthMatrixRibbon.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CEarthMatrixRibbonApp 对象

CEarthMatrixRibbonApp theApp;

char* CString2char(CString& str)
{
	int len = str.GetLength();
	char* chRtn = (char*)malloc((len * 2 + 1) * sizeof(char)); //CString的长度中汉字算一个长度
	memset(chRtn, 0, 2 * len + 1);
	USES_CONVERSION;
#if _MSC_VER>=1400
	strcpy((LPSTR)chRtn, OLE2A(str.LockBuffer()));
#else
	_tcscpy(chRtn, str);
#endif
	return chRtn;
}
// CEarthMatrixRibbonApp 初始化

BOOL CEarthMatrixRibbonApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
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
		AfxMessageBox(_T("没有检测到软件加密锁，程序将退出！"));
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
			AfxMessageBox(L"注册码文件不正确，程序将退出！");
			ExitInstance();
			return FALSE;
		}

		//进行次数判断
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
			AfxMessageBox(_T("试用次数已经用完，程序将退出！"));
			ExitInstance();
			return FALSE;
		}
		*/
	}
#endif	

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	//InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	//显示启动对话框
	CSplashWnd *pSplashWindow = new CSplashWnd;//创建对象
	pSplashWindow->create();
	pSplashWindow->CenterWindow();
	pSplashWindow->ShowWindow(SW_SHOW);  //显示窗口
	pSplashWindow->UpdateWindow();
	Sleep(2000);  //表示启动画面持续时间
	
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEarthMatrixRibbonDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CEarthMatrixRibbonView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生

	//郑思成：2016年7月29日不要在这里修改，不然任务栏也成空白的了
	//在PreCreateWindow中修改就可以了
	//m_pMainWnd->SetWindowText(_T(""));
	pSplashWindow->DestroyWindow(); //销毁启动画面
	delete pSplashWindow; //删除
	return TRUE;
}

int CEarthMatrixRibbonApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CEarthMatrixRibbonApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CEarthMatrixRibbonApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEarthMatrixRibbonApp 自定义加载/保存方法

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
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll32.dll")); //如果有必要请写明路径
#else
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll64.dll")); //如果有必要请写明路径
#endif

	LPFUN MD5 = (LPFUN)GetProcAddress(hLib, "MD5");

	if (hLib == NULL || MD5 == NULL)
	{
		AfxMessageBox(_T("GetHWInfoDll.dll加载出错。"));
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

// CEarthMatrixRibbonApp 消息处理程序
void CEarthMatrixRibbonApp::GetHWInfoFromDll()
{
	//获取硬件码
	char* pHWInfo;
	typedef char* (*LPFUN)(void);

#ifndef _WIN64
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll32.dll")); //如果有必要请写明路径
#else
	HMODULE hLib = LoadLibrary(m_sInstallPath + _T("\\GetHWInfoDll64.dll")); //如果有必要请写明路径
#endif

	LPFUN GetHWInfo = (LPFUN)GetProcAddress(hLib, "GetHWInfo");

	if (hLib == NULL)
	{
		AfxMessageBox(_T("GetHWInfoDll.dll加载出错。"));
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
	//获取硬件码 END
}


BOOL CEarthMatrixRibbonApp::GetExePath()
{
	TCHAR modulePath[512];

	DWORD pathLength = GetModuleFileName(NULL, modulePath, 512);

	if (pathLength)
	{
		CString sPath;
		sPath = modulePath;
		int rs = sPath.ReverseFind('\\'); //查找最后一个'\\'的位置
		m_sInstallPath = sPath.Left(rs);            //去掉文件名
		return TRUE;
	}
	return FALSE;
}