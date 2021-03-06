#include "stdafx.h"
#include "HouseVisiableSurvey.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include <osgUtil/IntersectVisitor>
#include <osg/LineSegment>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include "osg/LineWidth"
#include <osgEarthSymbology/Geometry>
#include "ThreadSafeUpdateCallback.h"

osg::ref_ptr<houseVisiableSurveyHandler> g_mHouseVisiableSurveyHandler = nullptr;

houseVisiableSurveyHandler::houseVisiableSurveyHandler(DLGHouseVisiableSurvey* pWin)
{
	gTemp = nullptr;
	_mouseDown = false;
	_isBusy = false;
	mValIgnore = 0.1;
	m_pDLGHouseVisiableSurveyWin = pWin;
}

bool houseVisiableSurveyHandler::SetBuddy(x3::IObject* val)
{
	m_spViewer3D = (IViewer3D*)val;

	m_sDefLayer = m_spViewer3D->GetDefLayer();

	return true;
}

void houseVisiableSurveyHandler::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
{
	pos = osg::Vec3d(0, 0, 0);
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (pViewer == NULL)
	{
		return ;
	}
	// 获取当前点
	osgUtil::LineSegmentIntersector::Intersections intersection;
	double x = ea.getX();
	double y = ea.getY();
	pViewer->computeIntersections(ea.getX(), ea.getY(), intersection);

	osgUtil::LineSegmentIntersector::Intersections::iterator iter
		= intersection.begin();
	if (iter != intersection.end())
	{

		pos.x() = iter->getWorldIntersectPoint().x();
		pos.y() = iter->getWorldIntersectPoint().y();
		pos.z() = iter->getWorldIntersectPoint().z();
		return;
	}
	return ;
}

CString houseVisiableSurveyHandler::FindSHPFile()
{
	USES_CONVERSION;
	CStringArray* a = m_spViewer3D->GetLayersArray();
	if (m_sDefLayer == _T(""))
	{
		for (int i = 0; i < a->GetCount(); i++)
		{
			if (a->GetAt(i).Right(4) == L".shp" || a->GetAt(i).Right(4) == L".SHP")
			{
				return a->GetAt(i);
			}
		}
		return L"";
	}
	else
	{
		if (string(W2A(m_sDefLayer)).find(".shp") == -1 &&
			string(W2A(m_sDefLayer)).find(".SHP") == -1)
		{
			return L"";
		}
		for (int i = 0; i < a->GetCount(); i++)
		{
			if (a->GetAt(i).Find(m_sDefLayer) > -1)
			{
				return a->GetAt(i);
			}
		}
		return L"";
	}
}

void houseVisiableSurveyHandler::getVisiableBuilding(osg::Vec3d vi)
{
	USES_CONVERSION;
	int numVisiableBuilding = 0;
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(FindSHPFile());
	if(path.empty())
		return;
	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' )+1);

		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}

	ogrOpt.url() = path;
	ogrOpt.openWrite() = true;

	osg::ref_ptr<osgEarth::Features::FeatureSource> features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	features->initialize();

	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();

	double disMin = 99999;
	osgEarth::Features::Feature* mfeature;
	osgEarth::Features::Feature* feature;
	std::vector<std::vector<osg::Vec3d> > vecLineInsect;
	m_pDLGHouseVisiableSurveyWin->UpdateData(TRUE);
	if (m_pDLGHouseVisiableSurveyWin->mCheckInsrtVal.GetCheck())//判断是否要插值
	{
		_insert = true;
		std::string s = W2A(m_pDLGHouseVisiableSurveyWin->mStrInsertVal);
		mValInsert = atof(s.c_str());
	}
	else
		_insert = false;
	if (m_pDLGHouseVisiableSurveyWin->mCheckIgnore.GetCheck())//判断是否起点忽略
	{
		std::string s = W2A(m_pDLGHouseVisiableSurveyWin->mStrIgnore);
		if( !(m_pDLGHouseVisiableSurveyWin->mStrIgnore).IsEmpty())
			mValIgnore = atof(s.c_str());
		else
			mValIgnore = 0.1;
	}
	else
		mValIgnore = 0.1;
	while(cursor->hasMore())
	{
		//输出feature中的信息
		feature = cursor->nextFeature();
		osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
		std::string sname = feature->getName();
		sname = feature->className();
		std::string sname1 = feature->getString("AcDbEntity");
		//AttributeTable att =  feature->getAttrs();
		std::string sGC = feature->getString("高程");
		std::string sDG = feature->getString("底高");
		std::string sDG2 = feature->getString("顶高");
		std::string sFloor = feature->getString("完整房间号");
		if (sDG2.empty()||sDG.empty()||sGC.empty())
		{
			continue;
		}
		double gc = atof(sGC.c_str());
		double dg = atof(sDG.c_str());
		double dg2 = atof(sDG2.c_str());
		bool bVisiable = false;

		std::vector<osg::Vec3d> vecIntersect;
		
		while(parts.hasMore())
		{
			osgEarth::Symbology::Geometry* part = parts.next();
			osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

			allPoints = part->toVec3Array();
			int kk = allPoints->size();
			Vec3dVector* vv = new Vec3dVector();
			osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;


			if(allPoints->size()<2)
				continue;

			if (_insert)
			{
				InsertPoints(allPoints,mValInsert);
			}
			
			for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
			{
				osg::Vec3d v(*i);
				v.z() = (dg + dg2)/2+ gc;
				bool b = testIntersect(m_spViewer3D->getRootNode()->asGroup(),vi,v,vecIntersect);
				if (vecIntersect.size()<1)
				{
					bVisiable = true;
					std::vector<osg::Vec3d> vec;
					vec.push_back(v);
					vec.push_back(vi);
					vecLineInsect.push_back(vec);
					break;
				}
			}
		}
		HOUSEDATA mHouseData;
		mHouseData.bVisiable = bVisiable;
		if (bVisiable)
		{		
			mHouseData.vecCoord.assign(vecLineInsect.back().begin(),vecLineInsect.back().end());
			numVisiableBuilding++;
		}
		else
		{
			mHouseData.vecCoord.assign(vecIntersect.begin(),vecIntersect.end());;		
		}
		mVecData[CString(sFloor.c_str())] = mHouseData;
	}
	if (gTemp != nullptr)
	{
		m_spViewer3D->RemoveNode(gTemp);
		gTemp = nullptr;
	}
	gTemp = new osg::Group;
	gTemp->addUpdateCallback(new CThreadSafeUpdateCallback);
	m_pDLGHouseVisiableSurveyWin->mPage1.mWndList.DeleteAllItems();
	m_pDLGHouseVisiableSurveyWin->mPage2.mWndList.DeleteAllItems();
	m_pDLGHouseVisiableSurveyWin->mPage1.mRowMap.clear();
	m_pDLGHouseVisiableSurveyWin->mPage2.mRowMap.clear();
	int num1 = 0,num2 = 0;
	for (map<CString,HOUSEDATA ,less<CString> >::iterator iter = mVecData.begin();iter != mVecData.end();++iter) 
	{
		if ((*iter).second.bVisiable)
		{
			drawLine(gTemp,(*iter).second.vecCoord,W2A((*iter).first));
			m_pDLGHouseVisiableSurveyWin->mPage1.InsertRow((*iter).first,L"");
			num1++;
		}
		else
		{
			//drawLine(gTemp,(*iter).second,W2A((*iter).first.name),true);
			m_pDLGHouseVisiableSurveyWin->mPage2.InsertRow((*iter).first,L"");
			num2++;
		}
	}
	m_pDLGHouseVisiableSurveyWin->updateStaticControl(num1,num1+num2);
	m_pDLGHouseVisiableSurveyWin->mTab.SetCurSel(0);
	m_pDLGHouseVisiableSurveyWin->mCurSelTab = 0;
	m_pDLGHouseVisiableSurveyWin->mPage1.ShowWindow(SW_SHOW);
	m_pDLGHouseVisiableSurveyWin->mPage2.ShowWindow(SW_HIDE);
	if (numVisiableBuilding>0)
	{
		gTemp->setName("通视线");
		m_spViewer3D->AddNode(gTemp);
	}
}

void houseVisiableSurveyHandler::InsertPoints(osg::ref_ptr<osg::Vec3Array> allPoints,double rio)
{
	std::vector<osg::Vec3> vecPoints;
	for (int i = 0;i<allPoints->size()-1;++i)
	{
		osg::Vec3 p_start = allPoints->at(i);
		osg::Vec3 p_end = allPoints->at(i+1);
		double len = (p_start-p_end).length();

		double step = rio/len;
		double dx = step * (p_end.x() - p_start.x());
		double dy = step * (p_end.y() - p_start.y());
		double dz = step * (p_end.z() - p_start.z());

		osg::Vec3d pt = p_start;

		while ((pt-p_end).length()>rio)
		{
			pt = p_start + osg::Vec3(dx,dy,dz);

			vecPoints.push_back(pt);
			p_start = pt;
		}
		vecPoints.push_back(p_end);
	}
	osg::Vec3 p_start = allPoints->back();
	osg::Vec3 p_end = allPoints->at(0);
	double len = (p_start-p_end).length();

	double step = rio/len;
	double dx = step * (p_end.x() - p_start.x());
	double dy = step * (p_end.y() - p_start.y());
	double dz = step * (p_end.z() - p_start.z());

	osg::Vec3d pt = p_start;

	while ((pt-p_end).length()>rio)
	{
		pt = p_start + osg::Vec3(dx,dy,dz);

		vecPoints.push_back(pt);
		p_start = pt;
	}
	vecPoints.push_back(p_end);
	allPoints->assign(vecPoints.begin(), vecPoints.end());
}

bool houseVisiableSurveyHandler::testIntersect(osg::Group* group,osg::Vec3d p1,osg::Vec3d p2,std::vector<osg::Vec3d>& vecIntersect)
{
	bool bIntersect = false;
	vecIntersect.clear();
	//碰撞检测
	osg::ref_ptr<osgUtil::LineSegmentIntersector> ls = new osgUtil::LineSegmentIntersector(p1, p2);
	// 创建一个IV
	osgUtil::IntersectionVisitor iv(ls);
	// 把线段添加到IV中
	group->accept(iv);
	if (ls.valid() && ls->containsIntersections())
	{
		bIntersect = true;
		for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = ls->getIntersections().begin();
			hitr != ls->getIntersections().end();++hitr)
		{
			osg::Vec3d p = hitr->getWorldIntersectPoint();
			if ((p-p1).length()>=mValIgnore && (p-p2).length()>=0.1)
			{
				vecIntersect.push_back(p);
			}	
		}	
	}
	return bIntersect;
}


void houseVisiableSurveyHandler::drawLine(osg::Group* lineGroup,std::vector<osg::Vec3d>& vecInsect,string name,bool bDepthTest)
{
	if (vecInsect.size()<2)
	{
		return;
	}
	osg::Vec4 color;
	if (bDepthTest)
	{
		color = osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
		color = osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f);
	osg::ref_ptr<osg::Geode> _geode = new osg::Geode();
	_geode->setDataVariance(osg::Object::DYNAMIC);
	_geode->getOrCreateStateSet()->setAttributeAndModes( new osg::LineWidth(2.0f) );
	if (bDepthTest)
	{
		_geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
	}
	else
		_geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	_geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	osg::ref_ptr<osg::Geometry> _geometry1 = new osg::Geometry();
	_geometry1->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	_geometry1->setColorArray(shared_colors.get(), osg::Array::BIND_PER_PRIMITIVE_SET);

	osg::ref_ptr<osg::Vec3Array> vert1 = new osg::Vec3Array();
	
	for (int i = 0;i<vecInsect.size() ;i++)
	{
		vert1->push_back(vecInsect[i]);
	}
	_geometry1->setVertexArray(vert1.get());
	_geometry1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vert1->size()));
	_geode->addDrawable(_geometry1);
	_geode->setName(name);
	osg::ref_ptr<CThreadSafeUpdateCallback> spCallback = dynamic_cast<CThreadSafeUpdateCallback*>(lineGroup->getUpdateCallback());
	if( spCallback != NULL )
		spCallback->AddChild(lineGroup, _geode);
	//lineGroup->addChild(_geode);
}

bool houseVisiableSurveyHandler::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa )
{
	if ( ea.getHandled() )
		return false;

	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer)
		return false;

	if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		_mouseDown = true;
		_mouseDownX = ea.getX();
		_mouseDownY = ea.getY();
	}
	else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		if(_mouseDown)
			_mouseDown = false;
		else
			return false;
		if (osg::equivalent(ea.getX(), _mouseDownX) && osg::equivalent(ea.getY(), _mouseDownY))
		{
			osg::Vec3d vecPos;
			getPos(ea, aa, vecPos);
			if (vecPos.length() >0.1 && !_isBusy)
			{	
				_isBusy = true;
				
				getVisiableBuilding(vecPos);
				
				_isBusy = false;
				return true;
			}
		}
	}
	return false;
}


void houseVisiableSurveyHandler::changeOneGeometry(CString name,bool bAdd)
{
	USES_CONVERSION;
	if (bAdd)//新增
	{
		int kk = mVecData.size();
		for (map<CString,HOUSEDATA,less<CString> >::iterator iter = mVecData.begin();iter != mVecData.end();++iter) 
		{
			if (0 == name.Compare((iter->first)))
			{
				std::vector<osg::Vec3d> vec = (*iter).second.vecCoord;
				std::string sname = W2A(name);
				drawLine(gTemp,vec,sname,!((*iter).second.bVisiable));
				break;
			}
		}	
	}
	else//删除
	{
		for (int i = 0;i<gTemp->getNumChildren();++i)
		{
			if (gTemp->getChild(i)->getName() == W2A(name))
			{
				gTemp->removeChild(i);
				break;
			}
		}
	}
}

void houseVisiableSurveyHandler::clearGeodeGroup(osg::Group* group)
{
	if (!group)
	{
		return;
	}
	while(group->getNumChildren()>0)
	{
		group->removeChildren(0,1);
	}
}

CHouseVisiableSurvey::CHouseVisiableSurvey(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"房间可视分析"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"查询搜索"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"房间可视分析"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"房间可视分析"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"LineSurvey")); 
	bInSurvey = false;
	p_mDLGHouseVisiableSurveyWin = nullptr;
	//p_mHouseVisiableSurveyHandler = nullptr;
}

CHouseVisiableSurvey::~CHouseVisiableSurvey(void)
{
}

bool CHouseVisiableSurvey::Initialize()
{
	return true;
}

bool CHouseVisiableSurvey::UnInitialize()
{
	return true;
}

bool CHouseVisiableSurvey::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	m_val = val;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	return true;
}

// IUICommand
bool CHouseVisiableSurvey::OnClick()
{
	if (bInSurvey)
	{
		Deactivate();
	}
	else
	{
		Activate();

	}
	return true;
}

bool CHouseVisiableSurvey::Checked()
{
	return bInSurvey;
}

bool CHouseVisiableSurvey::Activate()
{
	if (p_mDLGHouseVisiableSurveyWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		p_mDLGHouseVisiableSurveyWin = new DLGHouseVisiableSurvey();
		p_mDLGHouseVisiableSurveyWin->parent = (CObject*) this;
		p_mDLGHouseVisiableSurveyWin->Create(IDD_DIALOG_HOUSEVISIABLESURVER);
	}
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	
	p_mDLGHouseVisiableSurveyWin->mStaticNum = _T("");
	p_mDLGHouseVisiableSurveyWin->UpdateData(FALSE);
	p_mDLGHouseVisiableSurveyWin->ShowWindow(SW_SHOW);
	

	if (g_mHouseVisiableSurveyHandler == nullptr)
	{
		g_mHouseVisiableSurveyHandler = new houseVisiableSurveyHandler(p_mDLGHouseVisiableSurveyWin);
		g_mHouseVisiableSurveyHandler->SetBuddy(m_val);
		spViewer3D->getViewer()->addEventHandler( g_mHouseVisiableSurveyHandler );
	}


	RECT r;
	p_mDLGHouseVisiableSurveyWin->GetWindowRect(&r);

	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	p_mDLGHouseVisiableSurveyWin->MoveWindow(&r);
	bInSurvey = true;
	return true;
}

bool CHouseVisiableSurvey::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if (p_mDLGHouseVisiableSurveyWin)
	{
		p_mDLGHouseVisiableSurveyWin->mPage1.mWndList.DeleteAllItems();
		p_mDLGHouseVisiableSurveyWin->mPage2.mWndList.DeleteAllItems();
		p_mDLGHouseVisiableSurveyWin->ShowWindow(SW_HIDE);
	}

	spViewer3D->getViewer()->removeEventHandler( g_mHouseVisiableSurveyHandler );
	spViewer3D->RemoveNode(g_mHouseVisiableSurveyHandler->gTemp);
	g_mHouseVisiableSurveyHandler = nullptr;
	bInSurvey = false;
	return true;
}
