// InternetInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "InternetInfoDlg.h"
#include "afxdialogex.h"
#include "winsock.h" 
#include "mysql.h"
#include <osg/LineWidth>
#include "osg\MatrixTransform"
#include <osg/Billboard>
#include <osgText/Text3D>
#include "osgText/Font"  
#include "osgText/Text"
#include <osg/Material>
#include <fstream>
#include <list>
#include "DlgDB_Connect.h"
#include "BackgroundDlg.h"
using namespace std;

//CCriticalSection gCriticalSection;//临界区锁
vector<PERSON_LOCATION> g_vPersonLoc;
//data
extern DATACONTAINER vlxcclck,vlxcclrk,vdoor6,vpolices,vmanHole,vsmokeSensor,vdoorSensor,veleMeter;
extern int g_FontSize;//字体大小
extern int g_RowHeight;//行高
extern CDlgDB_Connect* g_DBconnector;

class PersonPath
{
public:
	CString name;
	vector< vector<CString> > vPath;
};

//功能函数
int findInPathRec(CString s,vector<PersonPath>& vs)
{
	USES_CONVERSION;
	if (vs.empty())
	{
		return -1;
	}
	for (size_t i = 0;i<vs.size();++i)
	{
		if (vs[i].name == s)
		{
			return i;
		}
	}
	return -1;
}

osg::Geode* drawPath(std::vector<osg::Vec3d>& vec,osg::Vec4 color,osg::Vec3d posOffset = osg::Vec3d(0,0,0))
{
	osg::Geode* geode = new osg::Geode;
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	for (int i = 0;i<vec.size();++i)
	{
		point->push_back(vec[i]-posOffset);
	}
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_STRIP,0,vec.size()));
	geode->addDrawable(lineGeom);

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setAttributeAndModes( new osg::LineWidth(1.5f) );
	ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	return geode;
}

void addPath(osg::Geode* geode,std::vector<osg::Vec3d>& vec,osg::Vec4 color,osg::Vec3d posOffset = osg::Vec3d(0,0,0))
{
	osg::Geometry* pathGeom = dynamic_cast<osg::Geometry*>(geode->getDrawable(geode->getNumDrawables()-1));
	osg::ref_ptr<osg::Vec3Array> vpt = dynamic_cast<osg::Vec3Array*>(pathGeom->getVertexArray());
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	point->push_back(vpt->back());

	for (int i = 0;i<vec.size();++i)
	{
		point->push_back(vec[i]);
	}
	
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_STRIP,0,vec.size()+1));
	geode->addDrawable(lineGeom);
}

osg::Billboard* createText(osg::Vec3d pos,osg::Vec4 textColor,osg::Vec4 outlineColor, float layoutCharacterSize, std::string title)
{
	osg::Billboard* geode = new osg::Billboard();
	geode->setMode(osg::Billboard::AXIAL_ROT);
	osgText::Text* text = new osgText::Text;
	osgText::Font* font = osgText::readFontFile("fonts/simhei.ttf");
	text->setFont(font);
	text->setColor(textColor);
	layoutCharacterSize *= 0.2f;
	text->setCharacterSize(layoutCharacterSize);
	text->setAlignment(osgText::TextBase::CENTER_BOTTOM);
	text->setBackdropType(osgText::Text::OUTLINE);
	text->setAxisAlignment(osgText::Text::SCREEN);
	text->setBackdropColor(outlineColor);
	text->setText(title.c_str());

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	text->setStateSet(stateset);

	geode->addDrawable(text);
	geode->setPosition(0,pos);
	return geode;
}
// CInternetInfoDlg 对话框

IMPLEMENT_DYNAMIC(CInternetInfoDlg, CDialogEx)

CInternetInfoDlg::CInternetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInternetInfoDlg::IDD, pParent)
{

}

CInternetInfoDlg::~CInternetInfoDlg()
{
}

void CInternetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Control(pDX, IDC_BUTTON_NETEXIT, m_EXIT);
}


BEGIN_MESSAGE_MAP(CInternetInfoDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CInternetInfoDlg::OnBnClickedButtonUpdate)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_NETEXIT, &CInternetInfoDlg::OnBnClickedButtonNetexit)
END_MESSAGE_MAP()


// CInternetInfoDlg 消息处理程序


BOOL CInternetInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_font.CreateFont(g_FontSize, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_BOLD, // nWeight 
		FALSE, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体"));
	SetBackgroundColor(RGB(253,254,255));
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetLayeredWindowAttributes(RGB(253,254,255),100,LWA_COLORKEY);
	m_wndList.SetTextColor(RGB(255,255,255));
	m_wndList.setColor(RGB(253,254,255));
	m_wndList.SetRowHeigt(g_RowHeight);
	m_wndList.SetTextBkColor(RGB(253,254,255));
	m_wndList.SetBkColor(RGB(253,254,255));
	m_wndList.InsertColumn (0, _T("名称"), LVCFMT_CENTER, 150);
	m_wndList.InsertColumn (1, _T("字段值"), LVCFMT_CENTER, 500);
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	m_wndList.SetFont(&m_font);

	SetTimer(1,10000,NULL);

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

void CInternetInfoDlg::showInfo(bool bshow)
{
	m_wndList.DeleteAllItems();
	if (bshow)
	{
		//删除表头
		CHeaderCtrl* pHeaderCtrl = m_wndList.GetHeaderCtrl();
		if(pHeaderCtrl)
		{
			int n = pHeaderCtrl->GetItemCount();
			for (int i = n-1;i>=0;--i)
			{
				m_wndList.DeleteColumn(i);
			}
		}
		std::vector<CString> vec;
		switch(_type)
		{
		case 1://车辆出口
			{
				m_wndList.InsertColumn (0, _T("车牌号码"), LVCFMT_CENTER, 70);
				m_wndList.InsertColumn (1, _T("车辆类型"), LVCFMT_CENTER, 70);
				m_wndList.InsertColumn (2, _T("出场时间"), LVCFMT_CENTER, 100);
				m_wndList.InsertColumn (3, _T("停留时间"), LVCFMT_CENTER, 100);
				std::list<std::string> slist;
				COMFUNC::getLastNRecord(vlxcclck.LData,slist,5);
				for (std::list<string>::iterator ite = slist.begin();ite != slist.end();++ite)
				{
					COMFUNC::split(*ite,"&&",vec);
					InsertRow(vec);
				}
				//插入表头
				vector<CString> cvec;
				cvec.push_back(_T("车牌号码"));
				cvec.push_back(_T("车辆类型"));
				cvec.push_back(_T("出场时间"));
				cvec.push_back(_T("停留时间"));
				InsertRow(cvec);
				
				AutoAdjustColumnWidth(&m_wndList);
				break;
			}
		case 2://车辆入口
			{
				m_wndList.InsertColumn (0, _T("车牌号码"), LVCFMT_CENTER, 70);
				m_wndList.InsertColumn (1, _T("车辆类型"), LVCFMT_CENTER, 70);
				m_wndList.InsertColumn (2, _T("入场时间"), LVCFMT_CENTER, 100);
				std::list<std::string> slist;
				COMFUNC::getLastNRecord(vlxcclrk.LData,slist,5);
				for (std::list<string>::iterator ite = slist.begin();ite != slist.end();++ite)
				{
					COMFUNC::split(*ite,"&&",vec);
					InsertRow(vec);
				}
				//插入表头
				vector<CString> cvec;
				cvec.push_back(_T("车牌号码"));
				cvec.push_back(_T("车辆类型"));
				cvec.push_back(_T("入场时间"));
				InsertRow(cvec);
				
				AutoAdjustColumnWidth(&m_wndList);
				break;
			}
		case 3://6号门禁
			{
				m_wndList.InsertColumn (0, _T("ID"), LVCFMT_CENTER, 50);
				m_wndList.InsertColumn (1, _T("姓名"), LVCFMT_CENTER, 70);
				m_wndList.InsertColumn (2, _T("身份证"), LVCFMT_CENTER, 100);
				m_wndList.InsertColumn (3, _T("单元"), LVCFMT_CENTER, 70);
				m_wndList.InsertColumn (4, _T("楼层"), LVCFMT_CENTER, 100);
				m_wndList.InsertColumn (5, _T("房间号"), LVCFMT_CENTER, 30);
				m_wndList.InsertColumn (6, _T("刷卡时间"), LVCFMT_CENTER, 100);
				m_wndList.InsertColumn (7, _T("in/out"), LVCFMT_CENTER, 10);
				std::list<std::string> slist;
				COMFUNC::getLastNRecord(vdoor6.LData,slist,5);
				for (std::list<string>::iterator ite = slist.begin();ite != slist.end();++ite)
				{
					COMFUNC::split(*ite,"&&",vec);
					InsertRow(vec);
				}
				vector<CString> cvec;
				cvec.push_back(_T("ID"));
				cvec.push_back(_T("姓名"));
				cvec.push_back(_T("身份证"));
				cvec.push_back(_T("单元"));
				cvec.push_back(_T("楼层"));
				cvec.push_back(_T("房间号"));
				cvec.push_back(_T("刷卡时间"));
				cvec.push_back(_T("in/out"));
				InsertRow(cvec);
				
				AutoAdjustColumnWidth(&m_wndList);
				break;
			}
		default:{break;}
		}
	}
}

void CInternetInfoDlg::updatePolicesPosition()
{
	USES_CONVERSION;
	CDataManager* p1 = dynamic_cast<CDataManager*>(parent);
	if (p1==NULL)
	{
		return;
	}
	osg::Group* root = p1->m_spViewer3D->getRootNode()->asGroup();
	if (root == NULL)
	{
		return;
	}
	vector<PersonPath> vVec;
	vector<CString> vec;
	for (list<string>::iterator ite = vpolices.LData.begin();ite != vpolices.LData.end();++ite)
	{
		COMFUNC::split(*ite,"&&",vec);
		int k = findInPathRec(vec[1],vVec);
		if (k == -1)
		{
			PersonPath pp;
			pp.name = vec[1];
			pp.vPath.push_back(vec);
			vVec.push_back(pp);
		}
		else
		{
			vVec[k].vPath.push_back(vec);
		}
	}
	////////////////////////////////////////////////
	if (vVec.size()>0)//如果有新数据
	{
		for (int i = 0;i<vVec.size();++i)
		{
			bool bFind = false;
			for (int j = 0;j<g_vPersonLoc.size();++j)
			{
				if ( string(W2A(vVec[i].name)) == g_vPersonLoc[j].name )
				{
					g_vPersonLoc[j].x = atof(string(W2A(vVec[i].vPath.back()[2])).data());
					g_vPersonLoc[j].y = atof(string(W2A(vVec[i].vPath.back()[3])).data());
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				PERSON_LOCATION pl;
				pl.name = string(W2A(vVec[i].name));
				pl.x = atof(string(W2A(vVec[i].vPath.back()[2])).data());
				pl.y = atof(string(W2A(vVec[i].vPath.back()[3])).data());
				pl.z = 0;
				g_vPersonLoc.push_back(pl);
			}
		}
	}
	///////////////////////////////////////////////////
	vpolices.LData.clear();
	for (int i = 0;i<vVec.size();++i)
	{
		std::string name = W2A(vVec[i].name);//警员名字
		//终点位置
		double x = atof(string(W2A(vVec[i].vPath.back()[2])).data());
		double y = atof(string(W2A(vVec[i].vPath.back()[3])).data());
		double z = 34.5;
		//获取路径
		std::vector<osg::Vec3d> vPt;
		for (int j = 0;j<vVec[i].vPath.size();++j)
		{
			vPt.push_back(osg::Vec3d(atof(string(W2A(vVec[i].vPath[j][2])).data()),
									atof(string(W2A(vVec[i].vPath[j][3])).data()),34.5));
		}
		int n = root->getNumChildren();
		bool flag = false;
		for (int i = 0;i<n;++i)
		{
			if (root->getChild(i)->getName() == name)
			{
				flag = true;
				osg::Group* group = dynamic_cast<osg::Group*>(root->getChild(i));
				if (group == nullptr)
				{
					continue;
				}
				osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(group->getChild(0));
				if (mt == nullptr)
				{
					continue;
				}

				mt->setMatrix(osg::Matrix::translate(osg::Vec3d(x,y,z)));
				osg::Node* node;
				osg::Geode* geode;
				for (int j = 0;j<group->getNumChildren();++j)
				{
					node = group->getChild(j);
					if (node->getName() == "path")
					{
						geode = dynamic_cast<osg::Geode*>(node);
						if (geode)
						{
							addPath(geode,vPt,osg::Vec4(1,1,0,1));
							break;
						}	
					}
				}
				break;
			}
		}
		if (!flag)
		{
			osg::Group* group = new osg::Group;
			osg::Node* glider = osgDB::readNodeFile("JC.OSGB");
			const osg::BoundingSphere& bs = glider->getBound();
			double size = 2.0/(bs.radius()*2);
			osg::MatrixTransform* positioned = new osg::MatrixTransform;
			positioned->setDataVariance(osg::Object::STATIC);
			positioned->setMatrix(osg::Matrix::scale(size,size,size)*osg::Matrix::translate(osg::Vec3d(x,y,z)));
			positioned->addChild(glider);
			positioned->setName("object");
			positioned->addChild(createText(osg::Vec3d(0,0,3),osg::Vec4(0,0,0.6,1),osg::Vec4(1,1,1,1),20,name));
			group->addChild(positioned);
			osg::Geode* geode = drawPath(vPt,osg::Vec4(1,1,0,1));
			geode->setName("path");
			group->addChild(geode);
			group->setName(name);
			p1->m_spViewer3D->Lock();
			p1->m_spViewer3D->AddNode(group);
			p1->m_spViewer3D->Unlock();
		}
	}
}

void CInternetInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	// TODO: 在此处添加消息处理程序代码
	showInfo(bShow);
	CDialogEx::OnShowWindow(bShow, nStatus);
}

void CInternetInfoDlg::InsertRow(std::vector<CString> vName)
{
	int m = m_wndList.GetItemCount();
	m = 0;
	m_wndList.InsertItem(m, _T("1"));
	for (int i = 0;i<vName.size();++i)
	{
		m_wndList.SetItemText (m, i, vName[i]);
	}
}

void CInternetInfoDlg::AutoAdjustDlg()
{
	CHeaderCtrl *pHeader = m_wndList.GetHeaderCtrl();  
	int nColumnCount = pHeader->GetItemCount();  
	int len = 0;
	for(int i = 0; i < nColumnCount; i++)  
	{  
		len +=  m_wndList.GetColumnWidth(i); //计算list的宽度
	}  
	RECT r1,r2,r3;
	((CButton*)GetDlgItem(IDC_BUTTON_NETEXIT))->GetWindowRect(&r3);
	int btnW = r3.right - r3.left;
	int btnH = r3.bottom - r3.top;
	GetWindowRect(&r1);
	r1.right = r1.left + len + 60;
	r1.bottom = r1.top + 25 + g_RowHeight * 6 + 35 ;
	
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->MoveWindow(&r1);
	}
	MoveWindow(&r1);
	r2.left = r1.left+2;
	r2.top = r1.top;
	r2.bottom = r1.bottom - 2;
	r2.right = r1.right - 2;
	ScreenToClient(&r2);
	((CStatic*)GetDlgItem(IDC_STATIC_edge1))->MoveWindow(&r2);
	m_wndList.GetWindowRect(&r2);
	r2.left = r1.left + 25;
	r2.right = r2.left + len;
	r2.top = r1.top + 25;
	r2.bottom = r1.bottom - 35;
	ScreenToClient(&r2);
	m_wndList.MoveWindow(&r2);
	GetWindowRect(&r1);
	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON_UPDATE);
	btn->GetWindowRect(&r2);
	r2.top = r1.bottom - 30;
	r2.bottom = r2.top + 25;
	r2.left = r1.right - 30 - 75;
	r2.right = r2.left + 75;
	ScreenToClient(&r2);
	btn->MoveWindow(&r2);

	r3.top = r1.top + 3;
	r3.bottom = r3.top + btnH;
	r3.right = r1.right - 3;
	r3.left = r3.right - btnW;
	ScreenToClient(&r3);
	((CButton*)GetDlgItem(IDC_BUTTON_NETEXIT))->MoveWindow(&r3);
	this->UpdateWindow();
}

void CInternetInfoDlg::AutoAdjustColumnWidth(CListCtrl *pListCtrl)  
{  
	pListCtrl->SetRedraw(FALSE);  
	CHeaderCtrl *pHeader = pListCtrl->GetHeaderCtrl();  
	int nColumnCount = pHeader->GetItemCount();  

	for(int i = 0; i < nColumnCount; i++)  
	{  
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE);  
		int nColumnWidth = pListCtrl->GetColumnWidth(i);  
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);  
		int nHeaderWidth = pListCtrl->GetColumnWidth(i);  

		pListCtrl->SetColumnWidth(i, max(nColumnWidth, nHeaderWidth) );  
	}  
	pListCtrl->SetRedraw(TRUE);  
}  

BOOL CInternetInfoDlg::PreTranslateMessage(MSG* pMsg)
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


void CInternetInfoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->ShowWindow(SW_HIDE);
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->ShowWindow(SW_HIDE);
	}
	CDataManager* p1 = dynamic_cast<CDataManager*>(parent);
	CDlgSearchAdv* p2 = dynamic_cast<CDlgSearchAdv*>(parent);
	if (p1 != nullptr)
	{
		if (p1->gTemp != nullptr)
		{
			(p1->m_spViewer3D)->RemoveNode(p1->gTemp);
			p1->gTemp = nullptr;
		}
	}
	else if (p2 != nullptr)
	{
		if (p2->gTemp != nullptr)
		{
			(p2->m_spViewer3D)->RemoveNode(p2->gTemp);
			p2->gTemp = nullptr;
		}
	}
	//CDialogEx::OnClose();
}


void CInternetInfoDlg::OnBnClickedButtonUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON_UPDATE);
	btn->EnableWindow(FALSE);
	if (g_DBconnector->getData())
	{
		AfxMessageBox(_T("更新失败！"));
		btn->EnableWindow(TRUE);
		return;
	}
	showInfo(true);
	btn->EnableWindow(TRUE);
}

void CInternetInfoDlg::selfDefShowWindow(UINT i)
{
	CBackgroundDlg* dlg = (CBackgroundDlg*)parentBk;
	if (dlg)
	{
		dlg->ShowWindow(i);
	}
	ShowWindow(i);
}

void CInternetInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	/*CRect rcGlassArea; 
	GetClientRect ( &rcGlassArea ); 
	dc.FillSolidRect ( &rcGlassArea, RGB(14,108,195));*/	
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CInternetInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDataManager* p1 = dynamic_cast<CDataManager*>(parent);
	if (p1 == nullptr)
	{
		return;
	}
	
	if (p1->m_wndTreeCtrl.GetRootItem() == NULL)
	{
		return;
	}
	if (IsWindowVisible())
	{
		showInfo(true);
		AutoAdjustDlg();
		UpdateWindow();
	}
	updatePolicesPosition();
	CDialogEx::OnTimer(nIDEvent);
}


void CInternetInfoDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDCtl==IDC_BUTTON_UPDATE)         //checking for the button 
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


void CInternetInfoDlg::OnBnClickedButtonNetexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}
