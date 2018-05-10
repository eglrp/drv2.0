// SniperDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "SniperDlg.h"
#include "afxdialogex.h"
#include "BackgroundDlg.h"
#include "ExplosiveDisposalDlg.h"
// CSniperDlg 对话框

IMPLEMENT_DYNAMIC(CSniperDlg, CDialogEx)

CSniperDlg::CSniperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSniperDlg::IDD, pParent)
{
	p_QrencodeDlg = NULL;
}

CSniperDlg::~CSniperDlg()
{
}

void CSniperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SNIPERINFO, m_sniperInfo);
	DDX_Control(pDX, IDC_STATIC_snipertitle, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_sniperexit, m_EXIT);
}


BEGIN_MESSAGE_MAP(CSniperDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1_EXTRACTQRCODE, &CSniperDlg::OnBnClickedButton1Extractqrcode)
	ON_BN_CLICKED(IDC_BUTTON_sniperexit, &CSniperDlg::OnBnClickedButtonsniperexit)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CSniperDlg 消息处理程序
void CSniperDlg::InitialHandle()
{
	sniperHandle = new LineAnalysisEventHandler(m_spViewer3D->getRootNode()->asGroup());
	sniperHandle->editInfo = &m_sniperInfo;
	m_spViewer3D->getViewer()->addEventHandler(sniperHandle);
}

void CSniperDlg::UnInitialHandle()
{
	sniperHandle->_group->removeChild(sniperHandle->lineGroup);
	sniperHandle->_group->removeChild(sniperHandle->saveGroup);
	m_spViewer3D->getViewer()->removeEventHandler(sniperHandle.get());
	sniperHandle = nullptr;
}

BOOL CSniperDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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


void CSniperDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UnInitialHandle();
	CBackgroundDlg* dlg = (CBackgroundDlg*)parent;
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
	CExplosiveDisposalDlg* dlg1 = (CExplosiveDisposalDlg*)parent1;
	if (dlg1)
	{
		dlg1->pDLG_sniper = nullptr;
	}
}


void CSniperDlg::OnBnClickedButton1Extractqrcode()
{
	// TODO: 在此添加控件通知处理程序代码
	if (p_QrencodeDlg != NULL)
	{
		p_QrencodeDlg->DestroyWindow();	
	}
	USES_CONVERSION;
	CString cmsg = _T(" ");
	m_sniperInfo.GetWindowTextW(cmsg);
	std::string msg(W2A(cmsg));
	DeleteFile(_T("Data\\DRVdata\\listInfo.png"));

	std::string slist = std::string("-o Data\\DRVdata\\listInfo.png ") +  msg;
	BeginWaitCursor();
	ShellExecute(this->m_hWnd, _T("open"), _T("qrencode.exe"), CString(slist.c_str()), _T(""), SW_HIDE);
	EndWaitCursor();
	while( !PathFileExists(_T("Data\\DRVdata\\listInfo.png")) );
	
	p_QrencodeDlg = new CDLGQrencode();
	p_QrencodeDlg->Create(IDD_DIALOG_QRENCODEINFO);
	p_QrencodeDlg->ShowWindow(SW_SHOW);
}


void CSniperDlg::OnBnClickedButtonsniperexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


BOOL CSniperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		TRUE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("黑体"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("狙击线分析"));
	m_brBk.CreateSolidBrush(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);
	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // 图片全路径  
		IMAGE_BITMAP,                          // 图片格式  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // 注意LR_LOADFROMFILE  
	m_EXIT.SetBitmap(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CSniperDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID()==IDC_EDIT_SNIPERINFO)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(RGB(253,254,255));
		m_editBrush=::CreateSolidBrush(RGB(253,254,255));  
		return m_editBrush; 
	}
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return m_brBk;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSniperDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDCtl==IDC_BUTTON1_EXTRACTQRCODE)         //checking for the button 
	{
		CDC dc;
		RECT rect;
		dc.Attach(lpDrawItemStruct ->hDC);   // Get the Button DC to CDC

		rect = lpDrawItemStruct->rcItem;     //Store the Button rect to our local rect.

		dc.Draw3dRect(&rect,RGB(0,0,0),RGB(0,0,0)); 

		dc.FillSolidRect(&rect,RGB(100,100,100));//Here you can define the required color to appear on the Button.

		UINT state=lpDrawItemStruct->itemState; //This defines the state of the Push button either pressed or not. 

		if((state & ODS_SELECTED))
		{
			dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

		}
		else
		{
			dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
		}

		dc.SetBkColor(RGB(100,100,100));   //Setting the Text Background color
		dc.SetTextColor(RGB(255,255,255));     //Setting the Text Color


		TCHAR buffer[MAX_PATH];           //To store the Caption of the button.
		ZeroMemory(buffer,MAX_PATH );     //Intializing the buffer to zero
		::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH); //Get the Caption of Button Window 

		dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);//Redraw the Caption of Button Window 

		dc.Detach(); // Detach the Button DC
	}                
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
