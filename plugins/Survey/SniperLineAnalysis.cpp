#include "stdafx.h"
#include "SniperLineAnalysis.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "osg/LineWidth"

LineAnalysisEventHandler::LineAnalysisEventHandler(osg::Group* group)
{
	_group = group;
	lineGroup = new osg::Group();
	saveGroup = new osg::Group();
	saveGroup->setName("vector");//标记为矢量
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

CSniperLineAnalysis::CSniperLineAnalysis(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"狙击线分析"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"空间量测"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"狙击线分析"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"狙击线分析"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"PointSurvey"));

	bActive = false;
	m_spLineAnalysisEventHandler = NULL;
}

CSniperLineAnalysis::~CSniperLineAnalysis(void)
{
}

bool CSniperLineAnalysis::Initialize()
{
	return true;
}

bool CSniperLineAnalysis::UnInitialize()
{
	return true;
}

bool CSniperLineAnalysis::Checked()
{
	return bActive;
}

bool CSniperLineAnalysis::SetBuddy(x3::IObject* val)
{
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy == spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer() == nullptr )
		return false;

	return true;
}

// IUICommand
bool CSniperLineAnalysis::OnClick()
{
	if (bActive)
	{
		Deactivate();
		
	}
	else
	{
		Activate();
		
	}
	return true;
}

bool CSniperLineAnalysis::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	m_spLineAnalysisEventHandler = new LineAnalysisEventHandler(spViewer3D->getRootNode()->asGroup());
	spViewer3D->getViewer()->addEventHandler( m_spLineAnalysisEventHandler );
	bActive = true;
	return true;
}
bool CSniperLineAnalysis::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	m_spLineAnalysisEventHandler->_group->removeChild(m_spLineAnalysisEventHandler->lineGroup);
	m_spLineAnalysisEventHandler->_group->removeChild(m_spLineAnalysisEventHandler->saveGroup);
	spViewer3D->getViewer()->removeEventHandler(m_spLineAnalysisEventHandler.get());
	m_spLineAnalysisEventHandler = nullptr;
	bActive = false;
	return true;
}


