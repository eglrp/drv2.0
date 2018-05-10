#include "stdafx.h"
#include "sniperToolHandle.h"
#include "osgUtil/Optimizer"
#include "osg/LineWidth"


extern CDataManager* m_pDM;

HOUSEINFO gHouseInfo;

GetPosHandler::GetPosHandler(osg::Group* group)
{
	_group = group;
	bLButtonDown = false;
}

void GetPosHandler::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
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
		if (findHouseInOSG(iter->getWorldIntersectPoint().x(),iter->getWorldIntersectPoint().y(),iter->getWorldIntersectPoint().z()))
		{
			mx = iter->getWorldIntersectPoint().x();
			my = iter->getWorldIntersectPoint().y();
			mz = iter->getWorldIntersectPoint().z();
		}
		return;
	}
	return ;
}

bool GetPosHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if ( ea.getHandled() )
		return false;
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer)
		return false;

	if(ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		bLButtonDown = true;
		return false;	
	}
	if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)
	{
		if (bLButtonDown)
		{
			getPos(ea, aa);
			bLButtonDown = false;
		}
	}
	return false;
}

bool GetPosHandler::findHouseInOSG(double mx,double my,double mz)
{
	osg::Vec3d vi(mx,my,mz);
	USES_CONVERSION;
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	std::string path = W2A(m_pDM->FindDefSHPFile(m_pDM->m_HouseDefLayer));
	if (path.length() == 0)
	{
		return false;
	}

	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_pDM->m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' )+1);

		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}

	ogrOpt.url() = path;
	ogrOpt.openWrite() = true;

	osg::ref_ptr<osgEarth::Features::FeatureSource> features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	features->initialize();
	
	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();

	osgEarth::Features::Feature* mfeature;
	osgEarth::Features::Feature* feature;
	while(cursor->hasMore())
	{
		//输出feature中的信息
		feature = cursor->nextFeature();
		osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
		
		while(parts.hasMore())
		{
			osgEarth::Symbology::Geometry* part = parts.next();
			if ( part->getType() != Geometry::TYPE_POLYGON )
			{
				return false;
			}
			
			osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

			allPoints = part->toVec3Array();

			Vec3dVector* vv = new Vec3dVector();
			osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

			for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
			{
				osg::Vec3d v(*i);
				vv->push_back(v);

				v.z() = v.z() + 1;
				v3->push_back(v);
			}

			osgEarth::Symbology::Ring r(vv);
			if (r.contains2D(vi.x(), vi.y()))
			{
				std::string sGC = feature->getString("高程");
				std::string sDG = feature->getString("底高");
				std::string sDG2 = feature->getString("顶高");
				std::string sTH = feature->getString("TH");

				if (!sGC.empty() && !sDG.empty() && !sDG2.empty())
				{
					double dGC = atof(sGC.data());
					double dDG = atof(sDG.data());
					double dDG2 = atof(sDG2.data());

					if (vi.z()>dGC+dDG && vi.z()<dGC+dDG2)
					{
						houseName = feature->getString("完整房间号");
						if (houseName.empty())
						{
							return false;
						}
						gHouseInfo.ownerName = feature->getString("姓名");
						gHouseInfo.houseID = houseName;
						gHouseInfo.idCard = feature->getString("身份证号");
						gHouseInfo.phone = feature->getString("手机号码");
						gHouseInfo.sex = feature->getString("性别");
						gHouseInfo.sDG = feature->getString("底高");
						gHouseInfo.sDG2 = feature->getString("顶高");
						if (m_pDM->gTemp != nullptr)
						{
							m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
							m_pDM->gTemp = nullptr;
						}
						MakeFloor3DBorder(feature, dGC+dDG, dGC + dDG2,osg::Vec4(1,0,0,1));
						return true;
					}
				}
			}
		}
	}
	return false;
}

void GetPosHandler::MakeFloor3DBorder(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Vec4 color)
{
	USES_CONVERSION;
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

		m_pDM->gTemp = new osg::Group();
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

			m_pDM->gTemp->addChild(m_pDM->AddPolygonNode(v3Vert.get(),color));
		}
		m_pDM->gTemp->setName("线框");
		//gTemp->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);//关闭深度测试
		m_pDM->m_spViewer3D->AddNode(m_pDM->gTemp);
	}
}

LineAnalysisEventHandler::LineAnalysisEventHandler(osg::Group* group)
{
	_group = group;
	lineGroup = new osg::Group();
	saveGroup = new osg::Group();
	_group->addChild(saveGroup);
	_group->addChild(lineGroup);
}

void LineAnalysisEventHandler::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
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

bool LineAnalysisEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if ( ea.getHandled() )
		return false;

	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer)
		return false;

	if(ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		osg::Vec3d vecPos;
		getPos(ea, aa, vecPos);
		if (vecPos != osg::Vec3d(0,0,0))
		{
			if (vecCoord.size() < 2)
				vecCoord.push_back(vecPos);
		}
	}
	else if(ea.getEventType() == osgGA::GUIEventAdapter::MOVE)
	{
		osg::Vec3d vecPos;
		getPos(ea, aa, vecPos);
		if (vecCoord.size() == 1 && vecPos.length()>100)
		{
			vector<osg::Vec3d> vec;
			getIntersectPt(_group,vecCoord[0],vecPos,vec);
			drawSniperLine(lineGroup,vec);
		}
	}
	else if ((ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON))
	{
		vecCoord.clear();
		if (lineGroup->getNumChildren()>0)
		{
			for (int i = 0;i<lineGroup->getNumChildren();++i)
			{
				osg::Node* node = dynamic_cast<osg::Node*>((lineGroup->getChild(i))->clone(osg::CopyOp::DEEP_COPY_ALL));
				saveGroup->addChild(node);
			}
			lineGroup->removeChildren(0,lineGroup->getNumChildren());
			
		}
	}
	else if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN && ea.getKey() == osgGA::GUIEventAdapter::KEY_Escape)
	{
		vecCoord.clear();
		if (lineGroup->getNumChildren()>0)
		{
			lineGroup->removeChildren(0,lineGroup->getNumChildren());
		}
		if (saveGroup->getNumChildren()>0)
		{
			saveGroup->removeChildren(0,saveGroup->getNumChildren());
		}
	}
	return false;
}

void LineAnalysisEventHandler::drawSniperLine(osg::Group* lineGroup,vector<osg::Vec3d>& vecInsect)
{
	if (vecInsect.size()<2)
	{
		return;
	}
	if (lineGroup->getNumChildren()>0)
	{
		lineGroup->removeChildren(0,lineGroup->getNumChildren());
	}
	
	osg::ref_ptr<osg::Geode> _geode = new osg::Geode();
	_geode->setDataVariance(osg::Object::DYNAMIC);
	_geode->getOrCreateStateSet()->setAttributeAndModes( new osg::LineWidth(2.0f) );
	_geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	_geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

	osg::ref_ptr<osg::Geometry> _geometry1 = new osg::Geometry();
	_geometry1->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	shared_colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	_geometry1->setColorArray(shared_colors.get(), osg::Array::BIND_PER_PRIMITIVE_SET);

	osg::ref_ptr<osg::Vec3Array> vert1 = new osg::Vec3Array();
	
	/*vert1->push_back(vecInsect[0]);
	vert1->push_back(vecInsect[1]);
	_geometry1->setVertexArray(vert1.get());
	_geometry1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
	_geode->addDrawable(_geometry1);
	if (vecInsect.size() == 2)
	{
	lineGroup->addChild(_geode);
	return ;
	}*/

	for (int i = 0;i<vecInsect.size() ;i++)
	{
		vert1->push_back(vecInsect[i]);
	}
	_geometry1->setVertexArray(vert1.get());
	_geometry1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
	_geometry1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 1, vert1->size() - 1));
	
	_geode->addDrawable(_geometry1);
	lineGroup->addChild(_geode);

	//更新窗口信息
	std::string sresult("目标可见");
	if (vecInsect.size() > 2)
	{
		sresult = "目标不可见";
	}
	osg::Vec3d disVer(vecInsect[0].x() - vecInsect.back().x(),vecInsect[0].y() - vecInsect.back().y(),0);
	double hori = abs(vecInsect[0].z() - vecInsect.back().z());
	char strMsg[1024];
	sprintf(strMsg,"狙击点坐标：\r\n%.2f,%.2f,%.2f\r\n\r\n目标点坐标：\r\n%.2f,%.2f,%.2f\r\n\r\n实际距离：\r\n%.2f米\r\n\r\n水平距离：\r\n%.2f米\r\n\r\n垂直距离：\r\n%.2f米\r\n\r\n%s",
		vecInsect[0].x(),vecInsect[0].y(),vecInsect[0].z(),
		vecInsect.back().x(),vecInsect.back().y(),vecInsect.back().z(),
		(vecInsect[0] - vecInsect.back()).length(),
		disVer.length(),
		hori,
		sresult.c_str()
		);
	editInfo->SetWindowTextW(CString(strMsg));
}

void LineAnalysisEventHandler::getIntersectPt(osg::Group* group,osg::Vec3d p1,osg::Vec3d p2,vector<osg::Vec3d>& vecInsect)
{
	vecInsect.clear();
	vecInsect.push_back(p1);
	//碰撞检测
	osg::ref_ptr<osgUtil::LineSegmentIntersector> ls = new osgUtil::LineSegmentIntersector(p1, p2);
	// 创建一个IV
	osgUtil::IntersectionVisitor iv(ls);
	// 把线段添加到IV中
	group->accept(iv);
	if (ls.valid() && ls->containsIntersections())
	{
		for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = ls->getIntersections().begin();
			hitr != ls->getIntersections().end();++hitr)
		{
			osg::Vec3d p = hitr->getWorldIntersectPoint();
			if ((p-p1).length()>=0.5 && (p-p2).length()>=0.5)
			{
				vecInsect.push_back(p);
			}	
		}	
	}
	vecInsect.push_back(p2);
}
