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

//��������
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
		CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("XML�ļ�(*.XML)|*.XML||"));
		if( dlg.DoModal() != IDOK )
		{
			return;
		}
		mXmlFile = dlg.GetPathName();
	}
	
	//���
	mComQueryObj.ResetContent();
	mComQueryType.SetCurSel(0);
	m_vecAllObservePt.clear();
	m_vecAllBuilding.clear();
	mVecObservePt.clear();

	//��ȡxml����
	rapidxml::file<> fdoc(W2A(mXmlFile));
	rapidxml::xml_document<> doc;
	doc.parse<0>(fdoc.data());
	//! ��ȡ���ڵ�
	rapidxml::xml_node<>* root = doc.first_node();
	//! ��ȡ���ڵ��һ���ڵ�
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

	//��估����
	OnCbnSelchangeComboQueryType();//���combox�ؼ�
	mComQueryObj.SetCurSel(0);//ѡ�е�һ��
	if (g_mBuildingVisiableSurveyHandler && !mVecObservePt.empty())
	{
		g_mBuildingVisiableSurveyHandler->reDrawObservePts(mVecObservePt);//���ƹ۲��
	}

	mbNoXml = true;//Ĭ��xml�ļ�����ʱ��ʼ���������ļ�ѡ���
}

//�л���ѯ����ʱ ���²�ѯ����combox
void CDLGBuildingVisiableSurvey::OnCbnSelchangeComboQueryType()
{
	// TODO: Add your control notification handler code here
	USES_CONVERSION;
	mComQueryObj.ResetContent();
	
	CString s;
	mComQueryType.GetLBText(mComQueryType.GetCurSel(),s);
	if (s.CompareNoCase(_T("�۲��")) == 0)
	{
		for (int i = 0;i<m_vecAllObservePt.size();++i)
		{
			mComQueryObj.AddString(m_vecAllObservePt[i]);
		}
		mComQueryObj.SetCurSel(0);
	}
	else if(s.CompareNoCase(_T("����")) == 0)
	{
		for (int i = 0;i<m_vecAllBuilding.size();++i)
		{
			mComQueryObj.AddString(m_vecAllBuilding[i]);
		}
		mComQueryObj.SetCurSel(0);
	}
}

//��ʼ��ѯ
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
	if (sType.CompareNoCase(_T("�۲��")) == 0)
	{
		doSearch(sObj,mXmlFile,0);
	}
	else if (sType.CompareNoCase(_T("����")) == 0)
	{
		doSearch(sObj,mXmlFile,1);
	}
	//���Ʋ�ѯ���
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
	//! ��ȡ���ڵ�
	rapidxml::xml_node<>* root = doc.first_node();
	//! ��ȡ���ڵ��һ���ڵ�
	rapidxml::xml_node<>* node = root->first_node("ObservePos");
	if (queryType == 0)//���۲���ѯ
	{
		while(node!=NULL)
		{
			rapidxml::xml_node<>* nameNode = node->first_node("NAME");
			if (node)
			{
				if (sObj.CompareNoCase(CString(nameNode->value())) == 0)
				{
					//��ȡ�۲������
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
							//��ȡ���ӽ�������
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
	else//��������ѯ
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
							//��ȡ���ӽ�������
							double x_build,y_build,z_build;
							x_build = atof((node1->first_node("BUILDING_X"))->value());
							y_build = atof((node1->first_node("BUILDING_Y"))->value());
							z_build = atof((node1->first_node("BUILDING_Z"))->value());
							mPtToMultiPt.pt = osg::Vec3d(x_build,y_build,z_build);
							//��ȡ�۲������
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
	//��ʼ���б�ؼ�
	mWndList.InsertColumn (0, _T("�ֶ���"), LVCFMT_LEFT, 200);
	//����Ĭ��ͼ�����Ӧxml�����ļ�
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
