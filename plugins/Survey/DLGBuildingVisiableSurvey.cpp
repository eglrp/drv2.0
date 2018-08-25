// DLGBuildingVisiableSurvey.cpp : implementation file
//

#include "stdafx.h"
#include "DLGBuildingVisiableSurvey.h"
#include "afxdialogex.h"
#include "DLGCreatorData.h"
#include "HouseVisiableSurvey.h"
#include "xml/rapidxml.hpp"
#include "xml/rapidxml_utils.hpp"
#include "xml/rapidxml_print.hpp"
#include "xml/rapidxml_iterators.hpp"

extern osg::ref_ptr<buildingVisiableSurveyHandler> g_mBuildingVisiableSurveyHandler;
// CDLGBuildingVisiableSurvey dialog

IMPLEMENT_DYNAMIC(CDLGBuildingVisiableSurvey, CDialogEx)

CDLGBuildingVisiableSurvey::CDLGBuildingVisiableSurvey(CString sDefLayer,CWnd* pParent /*=NULL*/)
	:mDefLayer(sDefLayer), 
	mXmlFile(_T("")),
	mbNoXml(1),
	CDialogEx(CDLGBuildingVisiableSurvey::IDD, pParent)
{
	mVecObservePt.clear();
	p_mDLGDataCreator = NULL;
}

CDLGBuildingVisiableSurvey::~CDLGBuildingVisiableSurvey()
{
}

void CDLGBuildingVisiableSurvey::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_QUERY_TYPE, mComQueryType);
	DDX_Control(pDX, IDC_COMBO_QUERY_OBJECT, mComQueryObj);
	DDX_Control(pDX, IDC_LIST_QUERY_BUILDING_RES, mWndList);
}


BEGIN_MESSAGE_MAP(CDLGBuildingVisiableSurvey, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDLGBuildingVisiableSurvey::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BUTTON_LOADXML, &CDLGBuildingVisiableSurvey::OnBnClickedButtonLoadxml)
	ON_CBN_SELCHANGE(IDC_COMBO_QUERY_TYPE, &CDLGBuildingVisiableSurvey::OnCbnSelchangeComboQueryType)
	ON_BN_CLICKED(ID_BUTTON_DOSEARCH, &CDLGBuildingVisiableSurvey::OnBnClickedButtonDosearch)
END_MESSAGE_MAP()


// CDLGBuildingVisiableSurvey message handlers

//创建数据
void CDLGBuildingVisiableSurvey::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (p_mDLGDataCreator != NULL)
	{
		p_mDLGDataCreator->DestroyWindow();
		p_mDLGDataCreator = NULL;
	}
	
	p_mDLGDataCreator = new CDLGCreatorData(mDefLayer);
	p_mDLGDataCreator->parent = (CObject*)this;
	p_mDLGDataCreator->DoModal();
	
}


void CDLGBuildingVisiableSurvey::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CBuildingVisiableSurvey* p = (CBuildingVisiableSurvey*)parent;
	p->OnClick();
	CDialogEx::OnClose();
}


void CDLGBuildingVisiableSurvey::OnBnClickedButtonLoadxml()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	if (mbNoXml)
	{
		CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("XML文件(*.XML)|*.XML||"));
		if( dlg.DoModal() != IDOK )
		{
			return;
		}
		mXmlFile = dlg.GetPathName();
	}
	
	//清空
	mComQueryObj.ResetContent();
	mComQueryType.SetCurSel(0);
	m_vecAllObservePt.clear();
	m_vecAllBuilding.clear();
	mVecObservePt.clear();

	//获取xml数据
	rapidxml::file<> fdoc(W2A(mXmlFile));
	rapidxml::xml_document<> doc;
	doc.parse<0>(fdoc.data());
	//! 获取根节点
	rapidxml::xml_node<>* root = doc.first_node();
	//! 获取根节点第一个节点
	rapidxml::xml_node<>* node = root->first_node("ObservePos");
	while(node!=NULL)
	{
		rapidxml::xml_node<>* nameNode = node->first_node("NAME");
		if (node)
		{
			m_vecAllObservePt.push_back(CString(nameNode->value()));
			double x,y,z;
			nameNode = node->first_node("X");
			x = atof(nameNode->value());
			nameNode = node->first_node("Y");
			y = atof(nameNode->value());
			nameNode = node->first_node("Z");
			z = atof(nameNode->value());
			mVecObservePt.push_back(osg::Vec3d(x,y,z));
		}
		node = node->next_sibling();
	}
	node = root->next_sibling();
	if (node!=NULL)
	{
		rapidxml::xml_node<>* nameNode = node->first_node("BUILDING_NAME");
		while (nameNode!=NULL)
		{
			m_vecAllBuilding.push_back(CString(nameNode->value()));
			nameNode = nameNode->next_sibling();
		}
	}

	//填充及绘制
	OnCbnSelchangeComboQueryType();//填充combox控件
	mComQueryObj.SetCurSel(0);//选中第一项
	if (g_mBuildingVisiableSurveyHandler && !mVecObservePt.empty())
	{
		g_mBuildingVisiableSurveyHandler->reDrawObservePts(mVecObservePt);//绘制观察点
	}

	mbNoXml = true;//默认xml文件存在时初始化不弹出文件选择框
}

//切换查询类型时 更新查询对象combox
void CDLGBuildingVisiableSurvey::OnCbnSelchangeComboQueryType()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	mComQueryObj.ResetContent();
	
	CString s;
	mComQueryType.GetLBText(mComQueryType.GetCurSel(),s);
	if (s.CompareNoCase(_T("观察点")) == 0)
	{
		for (int i = 0;i<m_vecAllObservePt.size();++i)
		{
			mComQueryObj.AddString(m_vecAllObservePt[i]);
		}
		mComQueryObj.SetCurSel(0);
	}
	else if(s.CompareNoCase(_T("建筑")) == 0)
	{
		for (int i = 0;i<m_vecAllBuilding.size();++i)
		{
			mComQueryObj.AddString(m_vecAllBuilding[i]);
		}
		mComQueryObj.SetCurSel(0);
	}
}

//开始查询
void CDLGBuildingVisiableSurvey::OnBnClickedButtonDosearch()
{
	// TODO: Add your control notification handler code here
	CString sType,sObj;
	mComQueryType.GetLBText(mComQueryType.GetCurSel(),sType);
	mComQueryObj.GetLBText(mComQueryObj.GetCurSel(),sObj);
	if (sType.IsEmpty() ||sObj.IsEmpty())
	{
		return;
	}
	if (sType.CompareNoCase(_T("观察点")) == 0)
	{
		doSearch(sObj,mXmlFile,0);
	}
	else if (sType.CompareNoCase(_T("建筑")) == 0)
	{
		doSearch(sObj,mXmlFile,1);
	}
	//绘制查询结果
	if (g_mBuildingVisiableSurveyHandler)
	{
		g_mBuildingVisiableSurveyHandler->reDrawLines(mPtToMultiPt.pt,mPtToMultiPt.vecMultiPt,mVecObservePt);
		
	}
}

void CDLGBuildingVisiableSurvey::doSearch(CString sObj,CString sXml,int queryType)
{
	USES_CONVERSION;
	rapidxml::file<> fdoc(W2A(sXml));
	rapidxml::xml_document<> doc;
	doc.parse<0>(fdoc.data());
	mWndList.DeleteAllItems();
	mPtToMultiPt.vecMultiPt.clear();
	//! 获取根节点
	rapidxml::xml_node<>* root = doc.first_node();
	//! 获取根节点第一个节点
	rapidxml::xml_node<>* node = root->first_node("ObservePos");
	if (queryType == 0)//按观察点查询
	{
		while(node!=NULL)
		{
			rapidxml::xml_node<>* nameNode = node->first_node("NAME");
			if (node)
			{
				if (sObj.CompareNoCase(CString(nameNode->value())) == 0)
				{
					//获取观察点坐标
					double x_observe,y_observe,z_observe;
					x_observe = atof((node->first_node("X"))->value());
					y_observe = atof((node->first_node("Y"))->value());
					z_observe = atof((node->first_node("Z"))->value());
					mPtToMultiPt.pt = osg::Vec3d(x_observe,y_observe,z_observe);

					rapidxml::xml_node<>* node1 = node->first_node("building");
					while(node1!=NULL)
					{
						rapidxml::xml_node<>* node2 = node1->first_node("BUILDING_NAME");
						if (node2!=NULL)
						{
							//获取可视建筑坐标
							double x_build,y_build,z_build;
							x_build = atof((node1->first_node("BUILDING_X"))->value());
							y_build = atof((node1->first_node("BUILDING_Y"))->value());
							z_build = atof((node1->first_node("BUILDING_Z"))->value());
							mPtToMultiPt.vecMultiPt.push_back(osg::Vec3d(x_build,y_build,z_build));
							int n = mWndList.GetItemCount();
							mWndList.InsertItem(0, CString(node2->value()));
						}
						node1 = node1->next_sibling();
					}
					break;
				}
			}
			node = node->next_sibling();
		}
	}
	else//按建筑查询
	{
		while(node!=NULL)
		{
			rapidxml::xml_node<>* nameNode = node->first_node("NAME");
			if (node)
			{
				rapidxml::xml_node<>* node1 = node->first_node("building");
				while(node1!=NULL)
				{
					rapidxml::xml_node<>* node2 = node1->first_node("BUILDING_NAME");
					if (node2!=NULL)
					{
						if (sObj.CompareNoCase(	CString(node2->value())) == 0)
						{
							//获取可视建筑坐标
							double x_build,y_build,z_build;
							x_build = atof((node1->first_node("BUILDING_X"))->value());
							y_build = atof((node1->first_node("BUILDING_Y"))->value());
							z_build = atof((node1->first_node("BUILDING_Z"))->value());
							mPtToMultiPt.pt = osg::Vec3d(x_build,y_build,z_build);
							//获取观察点坐标
							double x_observe,y_observe,z_observe;
							x_observe = atof((node->first_node("X"))->value());
							y_observe = atof((node->first_node("Y"))->value());
							z_observe = atof((node->first_node("Z"))->value());
							mPtToMultiPt.vecMultiPt.push_back(osg::Vec3d(x_observe,y_observe,z_observe));
							int n = mWndList.GetItemCount();
							mWndList.InsertItem(0, CString(nameNode->value()));
							break;
						}
					}
					node1 = node1->next_sibling();
				}
			}
			node = node->next_sibling();
		}
	}
}

#include  <io.h>
#include  <stdlib.h>
BOOL CDLGBuildingVisiableSurvey::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	USES_CONVERSION;
	// TODO:  Add extra initialization here
	//初始化列表控件
	mWndList.InsertColumn (0, _T("字段名"), LVCFMT_LEFT, 200);
	//根据默认图层检测对应xml数据文件
	std::string xmlFile = W2A(mDefLayer);
	xmlFile = xmlFile.substr(0,xmlFile.size() - 3) + "xml";
	if( (_access( xmlFile.c_str(), 0 )) != -1 )
	{
		mXmlFile = CString(xmlFile.c_str());
		mbNoXml = false;
		OnBnClickedButtonLoadxml();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
