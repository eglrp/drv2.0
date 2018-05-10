// DLGSpecials.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataManager.h"
#include "DLGSpecials.h"
#include "afxdialogex.h"
#include "DlgDB_Connect.h"
#include "BackgroundDlg.h"

extern CDataManager* m_pDM;

// CDLGSpecials �Ի���

IMPLEMENT_DYNAMIC(CDLGSpecials, CDialogEx)

int CDLGSpecials::m_SortColumn = -1;
BOOL CDLGSpecials::m_bAsc = FALSE;

int CDLGSpecials::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	USES_CONVERSION;
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	std::string    strItem1 = std::string(W2A(pListCtrl->GetItemText(lParam1,m_SortColumn)));
	std::string    strItem2 = std::string(W2A(pListCtrl->GetItemText(lParam2,m_SortColumn)));

	double d1 = atof(strItem1.c_str());
	double d2 = atof(strItem2.c_str());
	if (d1 != 0.0 && d2 != 0.0)
	{
		if (m_bAsc)
		{
			return d1<= d2;
		}
		else
			return d1 > d2;
	}

	if (m_bAsc)
	{
		return strItem1 <= strItem2;
	}
	else
	{
		return strItem2 < strItem1;
	}
}

CDLGSpecials::CDLGSpecials(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDLGSpecials::IDD, pParent)
	, m_Info(_T("�� 0 ��"))
{

}

CDLGSpecials::~CDLGSpecials()
{
}

void CDLGSpecials::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SPECIALTYPE, m_combox_specialType);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Text(pDX, IDC_STATIC_INFO, m_Info);
	DDX_Control(pDX, IDC_STATIC_TSRQTITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_TSRQEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CDLGSpecials, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_1, &CDLGSpecials::OnBnClickedButtonSearch1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDLGSpecials::OnNMDblclkList1)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CDLGSpecials::OnLvnColumnclickList1)
	ON_BN_CLICKED(IDC_BUTTON_TSRQEXIT, &CDLGSpecials::OnBnClickedButtonTsrqexit)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CDLGSpecials ��Ϣ�������


BOOL CDLGSpecials::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_combox_specialType.SetCurSel(0);
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("����"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (1, _T("�Ա�"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (2, _T("¥��"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (3, _T("����"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (4, _T("���֤"), LVCFMT_LEFT, 180);
	m_wndList.InsertColumn (5, _T("�绰"), LVCFMT_LEFT, 110);
	m_wndList.InsertColumn (6, _T("��ע"), LVCFMT_LEFT, 130);
	m_font1.CreatePointFont(110, _T("����"));
	GetDlgItem(IDC_STATIC_INFO)->SetFont(&m_font1);
	m_wndList.SetFont(&m_font1);
	m_wndList.SetRowHeigt(25);
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.SetTextColor(RGB(255,255,255));
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
		_T("����"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("������Ⱥ"));

	m_brBk.CreateSolidBrush(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);

	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(   
		NULL,   
		_T("Data\\DRVdata\\quit.bmp"),                           // ͼƬȫ·��  
		IMAGE_BITMAP,                          // ͼƬ��ʽ  
		0,0,   
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);  // ע��LR_LOADFROMFILE  
	m_EXIT.SetBitmap(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDLGSpecials::OnBnClickedButtonSearch1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	CString sType;
	m_combox_specialType.GetLBText(m_combox_specialType.GetCurSel(),sType);
	std::string typeStr(W2A(sType));

	CString sDefLayer = m_pDM->FindDefSHPFile(m_pDM->m_HouseDefLayer);
	if (sDefLayer.IsEmpty())
	{
		AfxMessageBox(_T("������Ĭ��ʸ��ͼ�㣡"));
		return;
	}

	m_wndList.DeleteAllItems();
	int num = 0;
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(sDefLayer);
	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_pDM->m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' ) + 1);
		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}
	ogrOpt.url() = path;
	m_pDM->features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	m_pDM->features->initialize();
	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = m_pDM->features->createFeatureCursor();
	while(cursor->hasMore())
	{
		//���feature�е���Ϣ
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		std::string s = feature->getString("������Ⱥ");

		if ((s == typeStr && typeStr != "����") || (!s.empty() && typeStr == "����"))
		{
			m_wndList.InsertItem(0, _T("1"));
			
			m_wndList.SetItemText (0, 0, CString((feature->getString("����")).c_str()));
			m_wndList.SetItemText (0, 1, CString((feature->getString("�Ա�")).c_str()));
			m_wndList.SetItemText (0, 2, CString((feature->getString("��Ԫ")).c_str()));
			m_wndList.SetItemText (0, 3, CString((feature->getString("�����")).c_str()));
			//�������֤
			std::string sID = feature->getString("���֤��");
			if (sID.size() != 18)
			{
				sID = " ";
			}
			else
			{
				for (int j = 3;j<=14;++j)
				{
					sID[j] = '*';
				}
			}
			m_wndList.SetItemText (0, 4, CString(sID.c_str()));
			std::string s1 = feature->getString("�ֻ�����");
			if (s1.size() == 11)
			{
				for (int i = 4;i<=7;++i)
				{
					s1[i] = '*';
				}
			}
			m_wndList.SetItemText (0, 5, CString(s1.c_str()));
			m_wndList.SetItemText (0, 6, CString(s.c_str()));
			num++;
		}
	}
	m_Info.Format(_T("�� %d ��"),num);
	//UpdateData(FALSE);
	CStatic* cs = (CStatic*)GetDlgItem(IDC_STATIC_INFO);
	cs->SetWindowTextW(m_Info);
	CRect rc;
	GetDlgItem(IDC_STATIC_INFO)->GetWindowRect(&rc);      
	ScreenToClient(&rc);        
	InvalidateRect(rc);
	if (num == 0)
	{
		AfxMessageBox(_T("δ�ҵ�����,����Ĭ��ͼ�㣡"));
		return;
	}
}


void CDLGSpecials::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	int nItem = m_wndList.GetSelectionMark();
	if (nItem < 0)
	{
		return;
	}
	CString sIDCard;
	
	sIDCard = m_wndList.GetItemText(nItem, 4);
	CString mDefLayer = m_pDM->FindSHPFile();
	if (mDefLayer == _T(""))
	{
		AfxMessageBox(_T("������Ĭ��ʸ��ͼ�㣡"));
		return;
	}

	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(mDefLayer);
	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_pDM->m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' ) + 1);
		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}
	ogrOpt.url() = path;
	m_pDM->features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	m_pDM->features->initialize();
	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = m_pDM->features->createFeatureCursor();
	while(cursor->hasMore())
	{
		//���feature�е���Ϣ
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		if (feature->getString("���֤��") == std::string(W2A(sIDCard)))
		{
			std::string sGC = feature->getString("�߳�");
			std::string sDG = feature->getString("�׸�");
			std::string sDG2 = feature->getString("����");
			if (!sGC.empty() && !sDG.empty() && !sDG2.empty())
			{
				double dGC = atof(sGC.data());
				double dDG = atof(sDG.data());
				double dDG2 = atof(sDG2.data());

				if (m_pDM->gTemp != nullptr){
					m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
					m_pDM->gTemp = nullptr;
				}
				m_pDM->MakeFloor3DBorder(feature, dGC+dDG, dGC + dDG2,osg::Vec4(1,1,0,1),false);
				return;
			}
		}
	}
	*pResult = 0;
}


BOOL CDLGSpecials::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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


void CDLGSpecials::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pDM->gTemp != nullptr )
	{
		m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
		m_pDM->gTemp = nullptr;
	}
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
}


HBRUSH CDLGSpecials::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return m_brBk;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CDLGSpecials::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_SortColumn = pNMListView->iSubItem;
	m_bAsc=!m_bAsc;//�����ǽ���

	m_wndList.SortItems(MyCompareProc, (LPARAM) &m_wndList);

	int count = m_wndList.GetItemCount();   //����
	for (int i=0; i<count;  i++)  
	{  
		m_wndList.SetItemData(i,i);  
	}
	*pResult = 0;
}


void CDLGSpecials::OnBnClickedButtonTsrqexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}


void CDLGSpecials::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDCtl==IDC_BUTTON_SEARCH_1)         //checking for the button 
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
