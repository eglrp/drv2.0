// DlgSearchAdv.cpp : 实现文件
//

#include "stdafx.h"
#include "EarthMatrixFunctions.h"
#include "IViewer3D.h"
#include "INodeEvents.h"
#include "x3py/observer/observerobject.h"
#include "osg/Group"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "osg/LineWidth"
#include "osg/osgUtil/PlaneIntersector"
#include "DlgSearchAdv.h"
#include "afxdialogex.h"
#include "DataManager.h"
#include "BackgroundDlg.h"
// CDlgSearchAdv 对话框

IMPLEMENT_DYNAMIC(CDlgSearchAdv, CDialogEx)

	CDlgSearchAdv::CDlgSearchAdv(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSearchAdv::IDD, pParent)
	, m_lowValue(_T(""))
	, m_upValue(_T(""))
	, m_numResults(_T("共 0 条"))
{
	dStep = 1;
	bFindedStart = false;
	bFindedEnd = false;
	dStartHeight = 0;
	dEndHeight = 200;
	m_pAttInfoWin = NULL;
	m_pDrawAreaHandle = nullptr;
}

CDlgSearchAdv::~CDlgSearchAdv()
{
}

void CDlgSearchAdv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FIELDS, m_cFields);
	DDX_Control(pDX, IDC_EDIT_KEY_STR, m_cKey);
	DDX_Control(pDX, IDC_LIST_SEARCHADV, m_cList);
	DDX_Control(pDX, IDC_CHECK_FILTER, m_combox_filter);
	DDX_Text(pDX, IDC_EDIT_FILTER_LOW, m_lowValue);
	DDX_Text(pDX, IDC_EDIT_FILTER_UP, m_upValue);
	DDX_Control(pDX, IDC_COMBO_FILTER, m_combo_filter);
	DDX_Text(pDX, IDC_STATIC_RESULTS, m_numResults);
	DDX_Control(pDX, IDC_STATIC_SEARCHADV_TITLE, m_dlgTitle);
	DDX_Control(pDX, IDC_BUTTON_SEARCHADVEXIT, m_EXIT);
	DDX_Control(pDX, IDC_COMBO_FIELDS2, m_cFields2);
	DDX_Control(pDX, IDC_EDIT_KEY_STR2, m_cKey2);
}


BEGIN_MESSAGE_MAP(CDlgSearchAdv, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DOSEARCH, &CDlgSearchAdv::OnBnClickedButtonDosearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SEARCHADV, &CDlgSearchAdv::OnNMClickList1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_ALL, &CDlgSearchAdv::OnBnClickedCheckShowAll)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SERCH_CIRCLE, &CDlgSearchAdv::OnBnClickedButtonSerchCircle)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SEARCHADVEXIT, &CDlgSearchAdv::OnBnClickedButtonSearchadvexit)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CDlgSearchAdv 消息处理程序


void CDlgSearchAdv::OnBnClickedButtonDosearch()
{
	if (m_pAttInfoWin)
	{
		m_pAttInfoWin->OnClose();
	}
	//获取两个搜索关键字
	m_cKey.GetWindowText(m_sKey);
	m_cKey2.GetWindowText(m_sKey2);
	//判断shp文件是否有效
	if (m_sShp == _T(""))
	{
		return;
	}

	m_cList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	if (m_combox_filter.GetCheck() == TRUE)
	{
		DoSearchWithFilter();
	}
	else
		DoSearch();
	
	char msg[99];
	sprintf_s(msg,"共 %d 条",vFeature.size());
	std::string str(msg);
	m_numResults = CString(str.c_str());
	UpdateData(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_SHOW_ALL))->SetCheck(FALSE); 
	CRect rc;
	GetDlgItem(IDC_STATIC_RESULTS)->GetWindowRect(&rc);      
	ScreenToClient(&rc);        
	InvalidateRect(rc);  
}

void CDlgSearchAdv::DoSearch()
{
	USES_CONVERSION;
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(m_sShp);
	if (path.length() == 0)
	{
		return;
	}

	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' )+1);

		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}

	if (m_sKey == _T(""))//如果第一个搜索关键字为空 返回
	{
		return;
	}
	//获取两个搜索字段
	m_cFields.GetWindowText(m_sField);
	m_cFields2.GetWindowText(m_sField2);

	

	ogrOpt.url() = path;

	m_cList.DeleteAllItems();

	features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	features->initialize();

	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();
	vFeature.clear();
	while(cursor->hasMore())
	{
		//输出feature中的信息
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);

		while(parts.hasMore())
		{
			osgEarth::Symbology::Geometry* part = parts.next();
			bool bFinded = false;

			const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();

			if (/*!m_sField2.IsEmpty() &&*/ !m_sKey2.IsEmpty())//如果第二个条件有效
			{
				int n = 0;
				bool bFind1 = false,bFind2 = false;
				for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
				{
					CString sName = CString(i->first.c_str());
					CString sValue = CString(i->second.getString().c_str());
					if (sValue.Find(m_sKey) >= 0 && !sValue.IsEmpty() && !bFind1)
					{	
						if (m_sField == sName || m_sField.IsEmpty())
						{
							n++;
							bFind1 = true;
							if (n == 2)
							{
								bFinded = true;
								m_cList.InsertItem(0, _T("1"));
								vFeature.insert(vFeature.begin(), feature);
								v.insert(v.begin(), part);
								break;
							}
						}
					}
					else if (sValue.Find(m_sKey2) >= 0 && !sValue.IsEmpty() && !bFind2)
					{
						if (m_sField2 == sName || m_sField2.IsEmpty())
						{
							n++;
							bFind2 = true;
							if (n == 2)
							{
								bFinded = true;
								m_cList.InsertItem(0, _T("1"));
								vFeature.insert(vFeature.begin(), feature);
								v.insert(v.begin(), part);
								break;
							}
						}
					}
				}
			}
			else //否则只按第一个条件搜索
				for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
				{
					CString sName = CString(i->first.c_str());
					CString sValue = CString(i->second.getString().c_str());
					if (sValue.Find(m_sKey) >= 0 && sValue != _T(""))
					{
						if (!m_sField.IsEmpty())
						{
							if (m_sField == sName)
							{
								bFinded = true;
								m_cList.InsertItem(0, _T("1"));
								vFeature.insert(vFeature.begin(), feature);
								v.insert(v.begin(), part);
								break;
							}
						}
						else
						{
							bFinded = true;
							m_cList.InsertItem(0, _T("1"));
							vFeature.insert(vFeature.begin(), feature);
							v.insert(v.begin(), part);
							break;
						}

					}
				}

			if (bFinded)
			{
				int nIndex = 0;
				for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
				{

					std::string str = i->second.getString();
					int n = str.find(".");
					if ( n != -1)
					{
						str = str.substr(0,n+3<=str.length()?n+3:str.length());
					}
					CString sValue = CString(str.c_str());
					m_cList.SetItemText (0, nIndex++, sValue);
					/*CString sValue = CString(i->second.getString().c_str());
					m_cList.SetItemText (0, nIndex++, sValue);*/
				}
			}
		}
	}
}

void CDlgSearchAdv::DoSearchWithFilter()
{
	USES_CONVERSION;
	CString sFilter;
	int n = m_combo_filter.GetCurSel();
	if(n == -1)
	{
		return;
	}
	m_combo_filter.GetLBText(n,sFilter);
	UpdateData(TRUE);
	std::string s1 = W2A(m_lowValue);
	std::string s2 = W2A(m_upValue);
	double dlow = atof(s1.c_str());
	double dup = atof(s2.c_str());

	////////////////////////////////////////
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(m_sShp);
	if (path.length() == 0)
	{
		return;
	}

	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' )+1);

		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}

	

	m_cFields.GetWindowText(m_sField);

	ogrOpt.url() = path;

	m_cList.DeleteAllItems();

	features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	features->initialize();

	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();
	vFeature.clear();
	while(cursor->hasMore())
	{
		//输出feature中的信息
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);

		while(parts.hasMore())
		{
			osgEarth::Symbology::Geometry* part = parts.next();
			bool bFinded = false;

			const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
			for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
			{
				CString sName = CString(i->first.c_str());
				CString sValue = CString(i->second.getString().c_str());
				if (sName == sFilter)
				{
					std::string sv = W2A(sValue);
					double dv = atof(sv.c_str());
					if (dv>=dlow && dv <= dup)
					{
						bFinded = true;
						m_cList.InsertItem(0, _T("1"));
						vFeature.insert(vFeature.begin(), feature);
						v.insert(v.begin(), part);
						break;
					}
				}
			}

			if (bFinded)
			{
				int nIndex = 0;
				for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
				{

					std::string str = i->second.getString();
					int n = str.find(".");
					if ( n != -1)
					{
						str = str.substr(0,n+3<=str.length()?n+3:str.length());
					}
					CString sValue = CString(str.c_str());
					m_cList.SetItemText (0, nIndex++, sValue);
					/*CString sValue = CString(i->second.getString().c_str());
					m_cList.SetItemText (0, nIndex++, sValue);*/
				}
			}
		}
	}
}

osg::ref_ptr<osg::Geode> CDlgSearchAdv::AddPolygonNode(osg::Vec3Array* arr,osg::Vec4 color)
{
	USES_CONVERSION;

	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);

	// same trick for shared normal.
	osg::ref_ptr<osg::Vec3Array> shared_normals = new osg::Vec3Array;
	shared_normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::Geometry* polyGeom = new osg::Geometry();

	int numCoords = arr->getNumElements();
	polyGeom->setVertexArray(arr);

	polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, numCoords));

	// use the shared color array.
	polyGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);

	// use the shared normal array.
	polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

	geode->addDrawable(polyGeom);

	CString s;
	s.Format(_T("线"));
	geode->setName(W2A(s.GetBuffer(0)));

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setMode(GL_BLEND, osg::StateAttribute::ON);
	ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	ss->setAttributeAndModes( new osg::LineWidth(3.5f) );

	return geode.get();
}

void CDlgSearchAdv::MakeBuildingStruct(std::string sTH,std::string sZ,osg::Vec4 color)
{
	if (gTemp != nullptr)
	{
		m_spViewer3D->RemoveNode(gTemp);
		gTemp = nullptr;
	}
	osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
	allPoints = m_geoPart->toVec3Array();

	double dH = 0;

	if (!sTH.empty())
	{
		dH = atof(sTH.data());
	}
	else
		dH = GetBuildingHeight();

	osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

	for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
	{
		osg::Vec3d v(*i);
		v.z() = v.z() + 1;
		v3->push_back(v);
	}

	gTemp = new osg::Group();
	double newZ = 0;
	if (!sZ.empty())
	{
		newZ = atof(sZ.data()) - 0.5;
	}
	for (int i = 0; i < v3->size(); i++)
	{
		osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
		osg::Vec3d ptVert, ptVertNext;

		ptVert = osg::Vec3d((*v3)[i]);
		ptVert.z() = newZ;
		if (i == v3->size() - 1)
		{
			ptVertNext = osg::Vec3d((*v3)[0]);
		}
		else
			ptVertNext = osg::Vec3d((*v3)[i + 1]);
		ptVertNext.z() = newZ;
		v3Vert->push_back(ptVert);

		v3Vert->push_back(ptVertNext);

		ptVertNext.z() = dH + dStep;
		v3Vert->push_back(ptVertNext);

		ptVert.z() = dH + dStep;
		v3Vert->push_back(ptVert);

		gTemp->addChild(AddPolygonNode(v3Vert.get(),color));
	}

	osg::ref_ptr< osg::Vec3Array > v3Top = new osg::Vec3Array;

	for (int i = 0; i < v3->size(); i++)
	{
		osg::Vec3d ptTop;

		ptTop = osg::Vec3d((*v3)[i]);
		ptTop.z() = dH + dStep;
		v3Top->push_back(ptTop);
	}
	gTemp->addChild(AddPolygonNode(v3Top.get(),color));

	gTemp->setName("线框");
	m_spViewer3D->AddNode(gTemp);

#ifdef _2D
	const osg::BoundingSphere& bs = gTemp->getBound();
	if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
		return;

	m_spViewer3D->CenterNode(gTemp, 250);
#else
	m_spViewer3D->FlyToViewport(v3f.x(), v3f.y(), 100);
#endif
}

double CDlgSearchAdv::GetBuildingHeight()
{
	double dHeight = 0.0;
	double ds,de;
	ds = dStartHeight;
	de = dEndHeight;

	//从0米开始，每0.5米计算一次范围内的横切点的Z值，一直计算到100米
	//得到200个点，取200个点中的最大值
	double aHeight[1024] = {0};
	int n = 0;
	for (double h = ds; h < de; h = h + dStep)
	{
		int nCrossNum = 0;
		if (BuildingInThisHeight(h, nCrossNum))
		{
			aHeight[n] = h;
		}
		aPtsInHeight[n] = nCrossNum;

		if (!bFindedStart)
		{
			if (nCrossNum > 1000 && dStartHeight < 10)
			{
				dStartHeight = h;
				bFindedStart = true;
			}
		}

		if (!bFindedEnd && bFindedStart)
		{
			if (nCrossNum < 161)
			{
				dEndHeight = h+2;
				bFindedEnd = true;
			}
		}

		n++;
	}

	for (int i = 0; i < 1024; i++)
	{
		if (aHeight[i] > dHeight)
		{
			dHeight = aHeight[i];
		}
	}

	return dHeight;
}

bool CDlgSearchAdv::BuildingInThisHeight(double height, int & nCrossNum)
{
	//建筑物的范围坐标
	osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
	allPoints = m_geoPart->toVec3Array();

	Vec3dVector* vv = new Vec3dVector();
	for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
	{
		osg::Vec3d v(*i);
		vv->push_back(v);
	}

	osgEarth::Symbology::Ring r(vv);

	osg::Plane plane(osg::Vec4(0, 0, 1, -1 * height));
	osg::ref_ptr<osgUtil::PlaneIntersector> pi = new osgUtil::PlaneIntersector(plane);
	osgUtil::IntersectionVisitor iv(pi);
	m_spViewer3D->getRootNode()->accept(iv);
	if(pi->containsIntersections())
	{
		for(osgUtil::PlaneIntersector::Intersections::iterator it = pi->getIntersections().begin();
			it != pi->getIntersections().end(); it++)
		{
			//交点坐标
			std::vector<osg::Vec3d> intersectPoints = it->polyline;

			//osg::Geode* geode=new osg::Geode;
			//osg::Geometry* geom=new osg::Geometry;
			//osg::Vec3Array* v3a=new osg::Vec3Array;
			for (int i = 0; i < intersectPoints.size(); i++)
			{
				nCrossNum++;

				//v3a->push_back(intersectPoints.at(i));
				osg::Vec3d v3d = intersectPoints.at(i);
				osg::Vec3d v3dNew = it->matrix->preMult(v3d);
				if (r.contains2D(v3dNew.x(), v3dNew.y()))
				{
					return true;
				}
			}
			//geom->setVertexArray(v3a);
			//geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,intersectPoints.size()));
			//geode->addDrawable(geom);

			//osg::ref_ptr<osg::MatrixTransform> spMatrixTransfrom = new osg::MatrixTransform(*(it->matrix.get()));
			//spMatrixTransfrom->addChild(geode);
			//spMatrixTransfrom->setName(("相交面"));
			//m_spViewer3D->AddNode(spMatrixTransfrom);
		}
	}

	return false;
}

void CDlgSearchAdv::ShowAttWin()
{
	AFX_MANAGE_STATE_EX;

	CBackgroundDlg* dlg = nullptr;
	if (m_pAttInfoWin == NULL)
	{
		m_pAttInfoWin = new CDlgAttInfo();
		m_pAttInfoWin->parent = (CObject*) this;
		if (!m_pAttInfoWin->Create(IDD_DIALOG_ENTINFO))
		{
			AfxMessageBox(_T("创建窗口失败"));
		}
		dlg =  new CBackgroundDlg();
		dlg->Create(IDD_DIALOG_BACKGROUND);
		m_pAttInfoWin->parentBk = (CObject*)dlg;
	}

	RECT r,r1;
	m_pAttInfoWin->GetWindowRect(&r);

	/*int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;*/
	int nWidth = (m_spViewer3D->rectView3D.right - m_spViewer3D->rectView3D.left)/5.0;
	int nHeight = r.bottom - r.top;
	r.left = m_spViewer3D->rectView3D.left + 20;
	r.top = m_spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	m_pAttInfoWin->MoveWindow(&r);
	((CBackgroundDlg*)m_pAttInfoWin->parentBk)->MoveWindow(&r);
	
	m_pAttInfoWin->selfDefShowWindow(SW_SHOW);

	r1.top = r.top + m_pAttInfoWin->btnH + 4;
	r1.bottom = r.bottom - 10;
	r1.left = r.left + 3;
	r1.right = r.right - 3;
	m_pAttInfoWin->ScreenToClient(&r1);
	m_pAttInfoWin->m_wndList.MoveWindow(&r1);
	r1.top = r.top +3;
	r1.bottom = r1.top + m_pAttInfoWin->btnH;
	r1.right = r.right - 3;
	r1.left = r1.right - m_pAttInfoWin->btnW;
	m_pAttInfoWin->ScreenToClient(&r1);
	m_pAttInfoWin->m_quitBtn.MoveWindow(&r1);
	//m_pAttInfoWin->setupPolygonRegion();
}

void CDlgSearchAdv::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = m_cList.GetSelectionMark();
	if (nItem >= 0)
	{
		int type = 1;
		((CButton*)GetDlgItem(IDC_CHECK_SHOW_ALL))->SetCheck(FALSE); 
		CString str = m_cList.GetItemText(nItem, 0);
		DWORD_PTR  dd = m_cList.GetItemData(nItem);
		//part = (osgEarth::Symbology::Geometry*)(m_cList.GetItemData(nItem));
		m_geoPart = v.at(nItem);

		std::string sGC = vFeature.at(nItem)->getString("高程");
		std::string sDG = vFeature.at(nItem)->getString("底高");
		std::string sDG2 = vFeature.at(nItem)->getString("顶高");
		std::string sTH = vFeature.at(nItem)->getString("TH");
		std::string sZ = vFeature.at(nItem)->getString("z");

		if (!sGC.empty() && !sDG.empty() && !sDG2.empty())
		{
			double dGC = atof(sGC.data());
			double dDG = atof(sDG.data());
			double dDG2 = atof(sDG2.data());
			
			if (gTemp != nullptr)
			{
				m_spViewer3D->RemoveNode(gTemp);
				gTemp = nullptr;
			}
			osg::Vec4 color;
			std::string sTs = vFeature.at(nItem)->getString("特殊人群");

			if (!sTs.empty())
			{
				if (sTs == "社区校正人员")
				{
					color = osg::Vec4(0,0,1,1);
					MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2,color);
				}
				else if (sTs == "刑满释放人员")
				{
					color = osg::Vec4(0,0,0,1);
					MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2,color);
				}
				else if (sTs == "吸毒人员")
				{
					color = osg::Vec4(1,0,0,1);
					MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2,color);
				}
				else
					MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2);
			}
			else
				MakeFloor3DBorder(vFeature.at(nItem), dGC+dDG, dGC + dDG2);
			type = 2;
		}
		else if (!sTH.empty())
			MakeBuildingStruct(sTH,sZ);

		ShowAttWin();
		
		m_pAttInfoWin->RemoveAllRows();
		
		const osgEarth::Features::AttributeTable& attrs = vFeature.at(nItem)->getAttrs();
		int len = 0,maxW = 0;
		for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
		{
			//如果字段为全英文，跳过不显示
			if (COMFUNC::allAlpha(i->first) || i->first == "高程" || i->first == "顶高" || i->first == "底高" )
			{
				continue;
			}
			std::string str = i->second.getString();
			if (CString(i->first.c_str()) == _T("家庭成员")) 
			{
				COMFUNC::split(str,std::string(","),m_pAttInfoWin->vName );
				//m_pAttInfoWin->analysisString( str,m_pAttInfoWin->vName );

			}
			if (CString(i->first.c_str()) == _T("成员身份证"))
			{
				COMFUNC::split(str,std::string(","),m_pAttInfoWin->vID );
				//m_pAttInfoWin->analysisString( str,m_pAttInfoWin->vID );
				continue;
			}
			int n = str.find(".");
			if ( n != -1 && isdigit(str[str.size() - 1]))
			{
				str = str.substr(0,n+3<=str.length()?n+3:str.length());
			}
			//记录最长的字段值长度
			if (str.size()>maxW)
			{
				maxW = str.size();
			}
			m_pAttInfoWin->InsertRow(CString(i->first.c_str()),CString(str.c_str()));
			len++;
		}
		m_pAttInfoWin->changeDlgHeight(len*30+30);
		//m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
		m_pAttInfoWin->suitHeightAndWidth(len,maxW);//调整高度和列宽
		m_pAttInfoWin->UpdateWindow();
	}
	*pResult = 0;
}

void CDlgSearchAdv::MakeFloor3DBorder(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Vec4 color)
{
	osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);

	while(parts.hasMore())
	{
		osgEarth::Symbology::Geometry* part = parts.next();
		osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

		allPoints = part->toVec3Array();

		osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

		for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
		{
			osg::Vec3d v(*i);
			v.z() = v.z() + 1;
			v3->push_back(v);
		}

		gTemp = new osg::Group();
		for (int i=0;i<v3->size();i++)
		{
			osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
			osg::Vec3d ptVert,ptVertNext;

			ptVert = osg::Vec3d((*v3)[i]);
			if (i == v3->size()-1)
			{
				ptVertNext = osg::Vec3d((*v3)[0]);
			}
			else
				ptVertNext = osg::Vec3d((*v3)[i+1]);

			ptVert.z() = dFloor;
			ptVertNext.z() = dFloor;
			v3Vert->push_back(ptVert);
			v3Vert->push_back(ptVertNext);

			ptVertNext.z() = dCeil;
			v3Vert->push_back(ptVertNext);

			ptVert.z() = dCeil;
			v3Vert->push_back(ptVert);

			gTemp->addChild(AddPolygonNode(v3Vert.get(),color));
		}

		gTemp->setName("线框");
		m_spViewer3D->AddNode(gTemp);
	}

#ifdef _2D
	const osg::BoundingSphere& bs = gTemp->getBound();
	if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
		return;

	m_spViewer3D->CenterNode(gTemp, 150);
#else
	m_spViewer3D->FlyToViewport(v3f.x(), v3f.y(), 100);
#endif
}

BOOL CDlgSearchAdv::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton*)GetDlgItem(IDC_CHECK_SHOW_ALL))->SetCheck(FALSE); 
	((CButton*)GetDlgItem(IDC_CHECK_NEIBOR))->SetCheck(FALSE); 
	// TODO:  在此添加额外的初始化
	m_combo_filter.SetCurSel(0);
	m_cList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cList.setColor(RGB(253,254,255));
	m_cList.SetTextBkColor(RGB(253,254,255));
	m_cList.SetBkColor(RGB(253,254,255));
	m_cList.SetTextColor(RGB(255,255,255));
	m_font1.CreatePointFont(110, _T("宋体"));
	GetDlgItem(IDC_STATIC_RESULTS)->SetFont(&m_font1);
	m_cList.SetFont(&m_font1);
	m_cList.SetRowHeigt(25);
	m_font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("黑体"));                 // lpszFacename
	m_dlgTitle.SetFont(&m_font);
	m_dlgTitle.SetWindowTextW(_T("高级查询"));

	m_brBk.CreateSolidBrush(RGB(253,254,255));
	SetBackgroundColor(RGB(253,254,255));
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


void CDlgSearchAdv::OnBnClickedCheckShowAll()
{
	// TODO: 在此添加控件通知处理程序代码	
	CButton* btn = ((CButton*)GetDlgItem(IDC_CHECK_SHOW_ALL));
	if (btn->GetCheck() == BST_UNCHECKED)
	{
		if (gTemp != nullptr)
		{
			m_spViewer3D->RemoveNode(gTemp);
			gTemp = nullptr;
		}
	}
	else
	{
		if (vFeature.size()>0)
		{
			osg::ref_ptr<osg::Group> group = new osg::Group;
			for (int nItem = 0;nItem<vFeature.size();++nItem)
			{
				CString str = m_cList.GetItemText(nItem, 0);
				CString strValue = m_cList.GetItemText(nItem, 1);
				DWORD_PTR  dd = m_cList.GetItemData(nItem);
				//part = (osgEarth::Symbology::Geometry*)(m_cList.GetItemData(nItem));
				m_geoPart = v.at(nItem);

				std::string sGC = vFeature.at(nItem)->getString("高程");
				std::string sDG = vFeature.at(nItem)->getString("底高");
				std::string sDG2 = vFeature.at(nItem)->getString("顶高");
				std::string sTH = vFeature.at(nItem)->getString("TH");
				std::string sZ = vFeature.at(nItem)->getString("z");

				if (!sGC.empty() && !sDG.empty() && !sDG2.empty())
				{
					double dGC = atof(sGC.data());
					double dDG = atof(sDG.data());
					double dDG2 = atof(sDG2.data());

					osg::Vec4 color;
					std::string sTs = vFeature.at(nItem)->getString("特殊人群");
					if (!sTs.empty())
					{
						if (sTs == "社区校正人员")
						{
							color = osg::Vec4(0,0,1,1);
							MakeFloor3DBorderAll(vFeature.at(nItem), dGC+dDG, dGC + dDG2,group,color);
							continue;
						}
						if (sTs == "刑满释放人员")
						{
							color = osg::Vec4(0,0,0,1);
							MakeFloor3DBorderAll(vFeature.at(nItem), dGC+dDG, dGC + dDG2,group,color);
							continue;
						}
						if (sTs == "吸毒人员")
						{
							color = osg::Vec4(1,0,0,1);
							MakeFloor3DBorderAll(vFeature.at(nItem), dGC+dDG, dGC + dDG2,group,color);
							continue;
						}
					}

					MakeFloor3DBorderAll(vFeature.at(nItem), dGC + dDG, dGC + dDG2,group,osg::Vec4(1,1,0,1));
				}
				else 
					MakeBuildingStructAll(sTH,sZ,group);
				
			}
			if(gTemp!=nullptr)
			{
				m_spViewer3D->RemoveNode(gTemp);
				gTemp = nullptr;
			}
			if (group!=nullptr)
			{
				gTemp = new osg::Group();
				gTemp->addChild(group);
				gTemp->setName("线框");
				m_spViewer3D->AddNode(gTemp);
			}	
		}
	}
}

void CDlgSearchAdv::MakeFloor3DBorderAll(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Group* group,osg::Vec4 color)
{
	osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
	while(parts.hasMore())
	{
		osgEarth::Symbology::Geometry* part = parts.next();
		osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

		allPoints = part->toVec3Array();

		osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

		for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
		{
			osg::Vec3d v(*i);
			v.z() = v.z() + 1;
			v3->push_back(v);
		}


		for (int i = 0; i < v3->size(); i++)
		{
			osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
			osg::Vec3d ptVert, ptVertNext;

			ptVert = osg::Vec3d((*v3)[i]);
			if (i == v3->size() - 1)
			{
				ptVertNext = osg::Vec3d((*v3)[0]);
			}
			else
				ptVertNext = osg::Vec3d((*v3)[i + 1]);

			ptVert.z() = dFloor;
			ptVertNext.z() = dFloor;
			v3Vert->push_back(ptVert);
			v3Vert->push_back(ptVertNext);

			ptVertNext.z() = dCeil;
			v3Vert->push_back(ptVertNext);

			ptVert.z() = dCeil;
			v3Vert->push_back(ptVert);

			group->addChild(AddPolygonNode(v3Vert.get(),color));
		}
	}
}

void CDlgSearchAdv::MakeBuildingStructAll(std::string sTH,std::string sZ,osg::Group* group,osg::Vec4 color)
{
	osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
	allPoints = m_geoPart->toVec3Array();

	double dH = 0;

	if (!sTH.empty())
	{
		dH = atof(sTH.data());
	}
	else
		dH = GetBuildingHeight();

	osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

	for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
	{
		osg::Vec3d v(*i);
		v.z() = v.z() + 1;
		v3->push_back(v);
	}
	double newZ = 0;
	if (!sZ.empty())
	{
		newZ = atof(sZ.data()) - 0.5;
	}
	for (int i = 0; i < v3->size(); i++)
	{
		osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
		osg::Vec3d ptVert, ptVertNext;

		ptVert = osg::Vec3d((*v3)[i]);
		ptVert.z() = newZ;
		if (i == v3->size() - 1)
		{
			ptVertNext = osg::Vec3d((*v3)[0]);
		}
		else
			ptVertNext = osg::Vec3d((*v3)[i + 1]);
		ptVertNext.z() = newZ;
		v3Vert->push_back(ptVert);

		v3Vert->push_back(ptVertNext);

		ptVertNext.z() = dH + dStep;
		v3Vert->push_back(ptVertNext);

		ptVert.z() = dH + dStep;
		v3Vert->push_back(ptVert);

		group->addChild(AddPolygonNode(v3Vert.get(),color));
	}
}

//bool CDlgSearchAdv::allAlpha(std::string str)
//{
//	if (str.empty())
//	{
//		return true;
//	}
//	bool result = false;
//	for (size_t i = 0;i<str.size();++i)
//	{
//		if (!isalpha(str[i]) && str[i] != '_' && str[i] != ' ')
//		{
//			return false;
//		}
//	}
//	return true;
//}

void CDlgSearchAdv::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (gTemp != nullptr )
	{
		if (m_pAttInfoWin != NULL)
		{
			if (m_pAttInfoWin->IsWindowVisible())
			{
				/*CBackgroundDlg* dlg = (CBackgroundDlg*)(parent1);
				if (dlg)
				{
				dlg->DestroyWindow();
				}
				DestroyWindow();
				return;*/
				m_pAttInfoWin->OnClose();
			}
		}
		m_spViewer3D->RemoveNode(gTemp);
		gTemp = nullptr;
	}
	
	if (m_pDrawAreaHandle != nullptr)
	{
		m_pDrawAreaHandle->clear();
		m_pDrawAreaHandle->removeText();
		m_spViewer3D->getViewer()->removeEventHandler( m_pDrawAreaHandle );
	}
	CBackgroundDlg* dlg = (CBackgroundDlg*)(parent1);
	if (dlg)
	{
		dlg->DestroyWindow();
	}
	DestroyWindow();
	//CDialogEx::OnClose();
}

void CDlgSearchAdv::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (gTemp != nullptr )
	{
		if (m_pAttInfoWin != NULL)
		{
			if (m_pAttInfoWin->IsWindowVisible())
			{
				DestroyWindow();
				return;
			}
		}
		m_spViewer3D->RemoveNode(gTemp);
		gTemp = nullptr;
	}
	DestroyWindow();
	//CDialogEx::OnCancel();
}


BOOL CDlgSearchAdv::PreTranslateMessage(MSG* pMsg)
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
			OnBnClickedButtonDosearch();
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSearchAdv::OnBnClickedButtonSerchCircle()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CButton* btn = (CButton*)GetDlgItem(IDC_BUTTON_SERCH_CIRCLE);
	CString title;
	btn->GetWindowTextW(title);
	if ( std::string(W2A(title)) == std::string("圆形区域搜索"))
	{
		if ( m_spViewer3D.valid())
		{
			m_pDrawAreaHandle = new CvlMeasureToolHandler(m_spViewer3D->getSceneData()->asGroup());
			m_spViewer3D->getViewer()->addEventHandler( m_pDrawAreaHandle );
			btn->SetWindowText(_T("关闭搜索"));
		}
	}
	else
	{
		btn->SetWindowText(_T("圆形区域搜索"));
		m_pDrawAreaHandle->clear();
		m_spViewer3D->getViewer()->removeEventHandler( m_pDrawAreaHandle );
	}
}



HBRUSH CDlgSearchAdv::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	//HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_BTN)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_ZD || pWnd->GetDlgCtrlID() == IDC_STATIC_NR ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_ZD2 || pWnd->GetDlgCtrlID() == IDC_STATIC_NR2 ||
		pWnd->GetDlgCtrlID() == IDC_STATIC__ || pWnd->GetDlgCtrlID() == IDC_STATIC_RESULTS ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_SEARCHADV_TITLE || pWnd->GetDlgCtrlID() == IDC_STATIC_QJ)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250,250,250));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}


void CDlgSearchAdv::OnBnClickedButtonSearchadvexit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


void CDlgSearchAdv::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDCtl==IDC_BUTTON_DOSEARCH || nIDCtl==IDC_BUTTON_SERCH_CIRCLE || nIDCtl== IDC_CHECK_SHOW_ALL)         //checking for the button 
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
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
