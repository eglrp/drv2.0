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
std::vector<HOUSEDATA > gVecData;

osg::ref_ptr<houseVisiableSurveyHandler> g_mHouseVisiableSurveyHandler = nullptr;

houseVisiableSurveyHandler::houseVisiableSurveyHandler(DLGHouseVisiableSurvey* pWin)
{
	gTemp = nullptr;
	_mouseDown = false;
	
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
	std::vector<std::string> vecFloorIn;
	std::vector<std::string> vecFloorOut;
	gVecData.clear();
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
		std::string sFloor = feature->getString("Layer");
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

			for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
			{
				osg::Vec3d v(*i);
				v.z() = (dg + dg2)/2+ gc;
				bool b = testIntersect(m_spViewer3D->getRootNode()->asGroup(),v,vi,vecIntersect);
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
		
		mHouseData.bVisiable = bVisiable;
		mHouseData.name = CString(sFloor.c_str());
		if (bVisiable)
		{		
			mHouseData.vecCoord.assign(vecLineInsect.back().begin(),vecLineInsect.back().end());
			
			//vecFloorIn.push_back(sFloor);
			numVisiableBuilding++;
		}
		else
		{
			mHouseData.vecCoord.assign(vecIntersect.begin(),vecIntersect.end());;
			
		}
		mVecData.push_back(mHouseData);
	}
	//std::sort(vecFloorIn.begin(),vecFloorIn.end());
	//std::sort(vecFloorOut.begin(),vecFloorOut.end());
	if (gTemp != nullptr)
	{
		m_spViewer3D->RemoveNode(gTemp);
		gTemp = nullptr;
	}
	gTemp = new osg::Group;
	m_pDLGHouseVisiableSurveyWin->mPage1.mWndList.DeleteAllItems();
	m_pDLGHouseVisiableSurveyWin->mPage2.mWndList.DeleteAllItems();
	for (vector<HOUSEDATA >::iterator iter = mVecData.begin();iter != mVecData.end();++iter) 
	{
		if ((*iter).bVisiable)
		{
			drawLine(gTemp,(*iter).vecCoord,W2A((*iter).name));
			m_pDLGHouseVisiableSurveyWin->mPage1.InsertRow((*iter).name,L"");
		}
		else
		{
			//drawLine(gTemp,(*iter).second,W2A((*iter).first.name),true);
			m_pDLGHouseVisiableSurveyWin->mPage2.InsertRow((*iter).name,L"");
		}
	}
	
	if (numVisiableBuilding>0)
	{
		gTemp->setName("通视线");
		m_spViewer3D->AddNode(gTemp);
	}
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
			if ((p-p1).length()>=0.5 && (p-p2).length()>=0.5)
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
	shared_colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
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
	lineGroup->addChild(_geode);
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
			if (vecPos.length() >0.1)
			{
				getVisiableBuilding(vecPos);
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
		for (vector<HOUSEDATA >::iterator iter = mVecData.begin();iter != mVecData.end();++iter) 
		{
			if (0 == name.Compare((iter->name)))
			{
				std::vector<osg::Vec3d> vec = (*iter).vecCoord;
				std::string sname = W2A(name);
				drawLine(gTemp,vec,sname,!((*iter).bVisiable));
				break;
			}
		}	
	}
	else//删除
	{
		/*for (int i = 0;i<gTemp->getNumChildren();++i)
		{
			if (gTemp->getChild(i)->getName() == W2A(name))
			{
				gTemp->removeChild(i);
				break;
			}
		}*/
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
