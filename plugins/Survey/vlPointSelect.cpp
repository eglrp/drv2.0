#include "stdafx.h"
#include <fstream>
#include "vlPointSelect.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include "IViewer3D.h"
#include <osg/StateSet>
#include <osgEarthSymbology/Geometry>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include "osg/LineWidth"
#include "osg/osgUtil/PlaneIntersector"
#include "resource.h"
#include "BackgroundDlg.h"
#include "osgQueryVisitor.h"
#include "osgText/Text"
#include "osgText/Font"

#include <osgViewer/Renderer>

extern std::string s_manHole;
extern std::string s_smokeSensor;
extern std::string s_doorSensor;
extern std::string s_eleMeter;
CvlPointSelect* p;
int g_FontSize;//字体大小
int g_RowHeight;//行高

void dateTrans(std::string s1,std::string & s2)
{
	if(s1.size() != 14)
		return;
	std::string syear = s1.substr(0,4);
	std::string smonth = s1.substr(4,2);
	std::string sday = s1.substr(6,2);
	std::string shour = s1.substr(8,2);
	std::string smin = s1.substr(10,2);
	std::string ssec = s1.substr(12);
	s2 = syear + "-" + smonth + "-" + sday +" "+ shour+":"+smin+":"+ssec;
}

double PointToSegDist(double x, double y, double x1, double y1, double x2, double y2)  
{  
	if (y1 == y2)//平行x轴
	{
		double d1 = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
		double d2 = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
		double xMax = x1>x2?x1:x2;
		double xMin = x1>x2?x2:x1;
		if (x<=xMin || x>=xMax)
		{
			return d1<d2?d1:d2;
		}
		
		return sqrt((y - y1)*(y - y1));
	}
	if (x1 == x2)//平行y轴
	{
		double d1 = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
		double d2 = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));

		double yMax = y1>y2?y1:y2;
		double yMin = y1>y2?y2:y1;
		if (y<=yMin || y>=yMax)
		{
			return d1<d2?d1:d2;
		}
		return sqrt((x - x1)*(x - x1));
	}

	double k1 = (y2 - y1)/(x2 - x1);
	double b1 = y1 - k1*x1;

	double k2 = -1/k1;
	double b2 = y - k2*x;

	double xcross = (b1 - b2)/(k2 - k1);
	double ycross = k1*xcross + b1;
	if ((ycross<y1&&ycross>y2) ||
		(ycross<y2&&ycross>y1))
	{
		return sqrt((xcross - x)*(xcross - x) + (ycross - y)*(ycross - y));
	}
	double d1 = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
	double d2 = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
	return d1>d2?d2:d1;
	/*double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);  
	if (cross <= 0) return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));  

	double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);  
	if (cross >= d2) return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));  

	double r = cross / d2;  
	double px = x1 + (x2 - x1) * r;  
	double py = y1 + (y2 - y1) * r;  
	return sqrt((x - px) * (x - px) + (py - y1) * (py - y1));  */
}  

using namespace osgEarth::Features;

void PickModelHandler::Init()
{
    _selection = new osgManipulator::Selection;

    _draggerTranslate = new osgManipulator::TranslateAxisDragger;
    _draggerScale = new osgManipulator::ScaleAxisDragger;
    _draggerRotate = new osgManipulator::RotateSphereDragger;
    _dragger = _draggerTranslate.get();

    _draggerTranslate->setupDefaultGeometry();
    _draggerScale->setupDefaultGeometry();
    _draggerRotate->setupDefaultGeometry();

    _draggerTranslate->setNodeMask( 0 );
    _draggerScale->setNodeMask( 0 );
    _draggerRotate->setNodeMask( 0 );

    _manager = new osgManipulator::CommandManager;
    _manager->connect( *_dragger, *_selection );

	_buttonDown = false;
    _selectType = MODEL;
    _operationType = TRANSLATE;

	gTemp = nullptr;

	dStep = 1;
	bFindedStart = false;
	bFindedEnd = false;
	dStartHeight = 0;
	dEndHeight = 200;
	dxf_zOffset = 0;
}

bool PickModelHandler::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa )
{
    osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
    if( !view )
        return false;

    if( !_enablePick )
        return view->getCameraManipulator()->handle( ea, aa );

    switch( ea.getEventType() )
    {
    case osgGA::GUIEventAdapter::PUSH:
		_buttonDown = true;
        break;

    case osgGA::GUIEventAdapter::DRAG:
		_buttonDown = false;
		break;
    case osgGA::GUIEventAdapter::RELEASE:
		{
			if (_buttonDown)
			{
				eaX = ea.getX();
				eaY = ea.getY();
				SelectNode( ea, aa );
			}
			_buttonDown = false;
			return false;
		}
        //return DragNode( ea, aa );
        break;

    default:
        break;
    }
    return false;
}

bool PickModelHandler::SelectNode( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa )
{
    osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
    _pointer.reset();
    TestOperationType();
	//if (m_sDefLayer.Find(L".dxf") != -1 || m_sDefLayer.Find(L".DXF") != -1)//如果默认图层是dxf  ，则不需要碰撞点
	//{
	//	MakeBuilding3DBorder(osg::Vec3(eaX,eaY,0));
	//	return false;
	//}
	firstPt = osg::Vec3d(0,0,0);
    osgUtil::LineSegmentIntersector::Intersections hits;
    if( m_spViewer3D->getViewer()->computeIntersections( ea.getX(), ea.getY(), hits ) )
    {
        _pointer.setCamera( view->getCamera() );
        _pointer.setMousePosition( ea.getX(), ea.getY() );
        osgUtil::LineSegmentIntersector::Intersections::iterator hitr;
        for( hitr = hits.begin(); hitr != hits.end(); ++hitr )
        {
            osg::Vec3d v3d = hitr->getLocalIntersectPoint();
            _pointer.addIntersection( hitr->nodePath, hitr->getLocalIntersectPoint() );
			osg::Vec3d hitPt = hitr->getWorldIntersectPoint();
			if (fabs(hitPt.x()) < 0.0001)
			{
				continue;
			}
			else
			{
				firstPt = hitPt;
				MakeBuilding3DBorder(firstPt);
				break;
			}
        }
        osg::NodePath::iterator itr;
        for( itr = _pointer._hitList.front().first.begin(); itr != _pointer._hitList.front().first.end(); ++itr )
        {
            if( !_hasSelected )
            {
                switch( _selectType )
                {
                case GEOD:
                {
                    osg::ref_ptr<osg::Geode> gode = dynamic_cast<osg::Geode*>( *itr );
                    if( gode )
                    {
                        /*
                        float scale = gode->computeBound().radius() * 1.6;
                        _selection->setMatrix( osg::Matrix::identity() );
                        _dragger->setMatrix( osg::Matrix::scale( scale, scale, scale ) *
                        	osg::Matrix::translate( gode->computeBound().center() ) );
                        gode->getParent( 0 )->addChild( _selection.get() );
                        gode->getParent( 0 )->addChild( _dragger.get() );
                        _selection->addChild( gode.get() );
                        gode->getParent( 0 )->removeChild( gode.get() );
                        _dragger->setNodeMask( 1 );
                        _hasSelected = true;
                        */
                        float scale = gode->computeBound().radius() * 1;
                        osg::Vec3f f3 = gode->computeBound().center();
                        //AddCylinderNode(firstPt.x(), firstPt.y(), firstPt.z(), scale);
                        _hasSelected = true;
                        return true;
                    }
                }
                break;

                case MODEL:
                {
                    osg::ref_ptr<osg::Group> group = dynamic_cast<osg::Group*>( *itr );
                    if( group )
                    {
                        if( group->getName() == "MODEL" )
                        {
                            float scale = group->computeBound().radius() * 1.6;
                            _selection->setMatrix( osg::Matrix::identity() );
                            _dragger->setMatrix( osg::Matrix::scale( scale, scale, scale ) *
                                                 osg::Matrix::translate( group->computeBound().center() ) );
                            group->getParent( 0 )->addChild( _selection.get() );
                            group->getParent( 0 )->addChild( _dragger.get() );
                            _selection->addChild( group.get() );
                            group->getParent( 0 )->removeChild( group.get() );
                            _dragger->setNodeMask( 1 );
                            _hasSelected = true;
                            return true;
                        }
                    }
                }
                break;
                }
            }
            else
            {
                osgManipulator::Dragger* dragger = dynamic_cast<osgManipulator::Dragger*>( *itr );
                if( dragger )
                {
                    dragger->handle( _pointer, ea, aa );
                    _activeDragger = dragger;
                    return true;
                }
            }
        }
    }
    else
    {
		MakeBuilding3DBorder(firstPt);
        _draggerTranslate->setNodeMask( 0 );
        _draggerRotate->setNodeMask( 0 );
        _draggerScale->setNodeMask( 0 );
        _hasSelected = false;
        if( _selection->getNumChildren() != 0 )
        {
            osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>( _selection->getParent( 0 ) );
            if( mt )
            {
                mt->setMatrix( mt->getMatrix() * _selection->getMatrix() );
                mt->removeChild( _selection.get() );
                mt->removeChild( _dragger.get() );
                mt->addChild( _selection->getChild( 0 ) );
                _selection->removeChild( _selection->getChild( 0 ) );
            }
            else
            {
                osg::MatrixTransform* mt = new osg::MatrixTransform;
                mt->setMatrix( _selection->getMatrix() );
                _selection->getParent( 0 )->addChild( mt );
                _selection->getParent( 0 )->removeChild( _dragger.get() );
                _selection->getParent( 0 )->removeChild( _selection.get() );
                mt->addChild( _selection->getChild( 0 ) );
                _selection->removeChild( _selection->getChild( 0 ) );
            }
        }
        return view->getCameraManipulator()->handle( ea, aa );
    }
    return false;
}

bool PickModelHandler::DragNode( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa )
{
    osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
    if( _activeDragger )
    {
        _pointer._hitIter = _pointer._hitList.begin();
        _pointer.setCamera( view->getCamera() );
        _pointer.setMousePosition( ea.getX(), ea.getY() );
        _activeDragger->handle( _pointer, ea, aa );
        if( ea.getEventType() == osgGA::GUIEventAdapter::RELEASE )
        {
            _activeDragger = NULL;
            _pointer.reset();
        }
        return true;
    }
    else
        return view->getCameraManipulator()->handle( ea, aa );
}

void PickModelHandler::SetOperationType( OPERATION type )
{
    if( !_hasSelected )
        _operationType = type;
}

void PickModelHandler::TestOperationType()
{
    _manager->disconnect( *_dragger );
    switch( _operationType )
    {
    case TRANSLATE:
        _dragger = _draggerTranslate.get();
        break;
    case ROTATE:
        _dragger = _draggerRotate.get();
        break;
    case SCALE:
        _dragger = _draggerScale.get();
        break;
    }
    _manager->connect( *_dragger, *_selection );
}

bool PickModelHandler::SetBuddy(x3::IObject* val)
{
    m_spViewer3D = (IViewer3D*)val;

	m_sDefLayer = m_spViewer3D->GetDefLayer();

    return true;
}

void PickModelHandler::AddCylinderNode(double x, double y, double z, double r)
{
    USES_CONVERSION;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::ShapeDrawable> shape;

    osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
    hints->setDetailRatio(2.0f);

    shape = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(x, y, z), r, r), hints.get());
    shape->setColor(osg::Vec4(0.6f, 0.8f, 0.8f, 0.6f));
    geode->addDrawable(shape.get());

    osg::StateSet* ss = geode->getOrCreateStateSet();
    ss->setMode(GL_BLEND, osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    CString s;
    s.Format(_T("圆柱体%f"), r);
    geode->setName(W2A(s.GetBuffer(0)));

    m_spViewer3D->AddNode(geode.get());
}

osg::ref_ptr<osg::Geode> PickModelHandler::AddPolygonNode(osg::Vec3Array* arr,osg::Vec4 color)
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

CString PickModelHandler::FindSHPFile()
{
	USES_CONVERSION;
	CStringArray* a = m_spViewer3D->GetLayersArray();
	CString cs;
	for (int i = 0; i < a->GetCount(); i++)
	{
		cs = a->GetAt(i);
	}
	if (m_sDefLayer == _T(""))
	{
		for (int i = 0; i < a->GetCount(); i++)
		{
			if (a->GetAt(i).Right(4) == L".shp")
			{
				return a->GetAt(i);
			}
		}
		return L"";
	}
	else
	{
		if (string(W2A(m_sDefLayer)).find(".shp") == -1)
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

bool PickModelHandler::BuildingInThisHeight(double height, int & nCrossNum)
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

double PickModelHandler::GetBuildingHeight()
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

osg::ref_ptr<osg::Geode> PickModelHandler::drawPolyArea(std::vector<osg::Vec3d> vec,osg::Vec4 color)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array;
	for (int i = 0;i<vec.size();++i)
	{
		point->push_back(vec[i]);
	}
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_STRIP,0,point->size()));
	geode->addDrawable(lineGeom);

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setAttributeAndModes( new osg::LineWidth(3.5f) );
	ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	return geode;
}

osg::ref_ptr<osg::Geode> PickModelHandler::drawBoxArea(osg::BoundingBox box,osg::Vec4 color)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array;

	double xmax = box.xMax();
	double xmin = box.xMin();
	double ymax = box.yMax();
	double ymin = box.yMin();
	double zmax = box.zMax();
	double zmin = box.zMin();

	point->push_back(osg::Vec3d(xmin,ymin,zmin));
	point->push_back(osg::Vec3d(xmin,ymin,zmax));
	point->push_back(osg::Vec3d(xmax,ymin,zmax));
	point->push_back(osg::Vec3d(xmax,ymin,zmin));
	point->push_back(osg::Vec3d(xmax,ymax,zmin));
	point->push_back(osg::Vec3d(xmax,ymax,zmax));
	point->push_back(osg::Vec3d(xmin,ymax,zmax));
	point->push_back(osg::Vec3d(xmin,ymax,zmin));
	point->push_back(osg::Vec3d(xmin,ymin,zmin));
	point->push_back(osg::Vec3d(xmin,ymin,zmax));
	
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,0,4));
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,2,4));
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,4,4));
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,6,4));
	geode->addDrawable(lineGeom);

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setAttributeAndModes( new osg::LineWidth(3.5f) );
	ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	return geode;
}

std::string PickModelHandler::queryOSG(osg::Vec3d v,osg::BoundingBox& box)
{
	USES_CONVERSION;
	std::string sRes;
	//搜索默认osg文件全路径
	CStringArray* a = m_spViewer3D->GetLayersArray();
	CString cs("");
	for (int i = 0; i < a->GetCount(); i++)
	{
		cs = a->GetAt(i);
		if (cs.Find(m_sDefLayer) != -1)
		{
			break;
		}
	}
	if (cs.IsEmpty())
	{
		return sRes;
	}
	osg::Node* node = osgDB::readNodeFile(W2A(cs));
	if (!node)
	{
		return sRes;
	}
	std::vector<std::string> vecS;
	std::vector<osg::BoundingBox> vecBox;
	std::vector<osg::Vec3d> vecCenter;
	COsgQueryVisitor qv;
	node->accept(qv);
	vecS = qv.vecName;
	vecBox = qv.vecBox;
	osg::ref_ptr<osg::Group> group = new osg::Group; 
	double dis = 99999;
	osg::Geode* geode;
	osg::Billboard* bill;
	for (size_t i = 0;i<vecS.size();++i)
	{
		double d = 0;
		if (qv.inBox(v,vecBox[i],d))
		{
			if (d<dis)
			{
				dis = d;
				sRes = vecS[i];
				box = vecBox[i];
			}
		}
	}
	return sRes;
}

bool PickModelHandler::queryDXF(osg::Vec3d v,_DXF::DL_DATA& dlData)
{
	USES_CONVERSION;
	int sx = GetSystemMetrics(SM_CXSCREEN);
	int sy = GetSystemMetrics(SM_CYSCREEN);
	//搜索默认dxf文件全路径
	CStringArray* a = m_spViewer3D->GetLayersArray();
	CString cs("");
	for (int i = 0; i < a->GetCount(); i++)
	{
		cs = a->GetAt(i);
		if (cs.Find(m_sDefLayer) != -1)
		{
			break;
		}
	}
	if (cs.IsEmpty())
	{
		return false;
	}
	double dis = 99999;
	std::vector<_DXF::DL_DATA> vecDxfData;
	readDxf(W2A(cs), vecDxfData);//读取dxf信息

	osg::ref_ptr<osg::Camera> camera = m_spViewer3D->getViewer()->getCamera();
	osgViewer::Renderer *render = dynamic_cast<osgViewer::Renderer *>(camera->getRenderer());
	osgUtil::SceneView *sceneView = render->getSceneView(0);
	//osg::Vec3 vScreen(eaX,eaY,0);
	//osg::Matrix matrix;
	COsgQueryVisitor qv;
	for (int i = 0;i<vecDxfData.size();++i)
	{
		std::vector<osg::Vec3d> vecScreen,vecTemp;
		osg::Vec3 ptWindow,center(0,0,0);
		//类型是polyline
		if (vecDxfData[i].type == "POLYLINE" /*&& firstPt.length() == 0*/)
		{
			for(int j = 0;j<vecDxfData[i].vecCoord.size();++j)
			{
				sceneView->projectObjectIntoWindow(osg::Vec3(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z == 0?vecDxfData[i].vecCoord[j].z+dxf_zOffset:vecDxfData[i].vecCoord[j].z)/**matrix*/, ptWindow);
				//vecTemp.push_back(osg::Vec3(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z == 0?vecDxfData[i].vecCoord[j].z+dxf_zOffset:vecDxfData[i].vecCoord[j].z));
				vecScreen.push_back(ptWindow);
				//center += ptWindow;
			}
			
			for (int k = 0;k < vecScreen.size()-1;++k)
			{
				if ((vecScreen[k].x()<0||vecScreen[k].x()>sx)||
					(vecScreen[k].y()<0||vecScreen[k].y()>sy)||
					(vecScreen[k+1].x()<0||vecScreen[k+1].x()>sx)||
					(vecScreen[k+1].y()<0||vecScreen[k+1].y()>sy))
				{
					continue;
				}
				double d = PointToSegDist(eaX,eaY,vecScreen[k].x(),vecScreen[k].y(),vecScreen[k+1].x(),vecScreen[k+1].y());
				if(d<dis)
				{
					dis = d;
					/*vecPolyon.clear();
					for (int j = 0;j<vecDxfData[i].vecCoord.size();++j)
					{
					vecPolyon.push_back(osg::Vec3d(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z));
					}*/
					dlData = vecDxfData[i];
					//vecPolyon.push_back(osg::Vec3d(vecDxfData[i].vecCoord[0].x,vecDxfData[i].vecCoord[0].y,vecDxfData[i].vecCoord[0].z));
					//sRes = vecDxfData[i].layer;
					
				}
			}
			//for(int j = 0;j<vecDxfData[i].vecCoord.size();++j)
			//{
			//	sceneView->projectObjectIntoWindow(osg::Vec3(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z == 0?vecDxfData[i].vecCoord[j].z+dxf_zOffset:vecDxfData[i].vecCoord[j].z)/**matrix*/, ptWindow);
			//	vecTemp.push_back(osg::Vec3(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z == 0?vecDxfData[i].vecCoord[j].z+dxf_zOffset:vecDxfData[i].vecCoord[j].z));
			//	vecScreen.push_back(ptWindow);
			//	center += ptWindow;
			//}
			//center/=vecDxfData[i].vecCoord.size();
			//center.z() = 0;
			//if (vecScreen.back() == vecScreen.front())
			//{
			//	vecScreen.pop_back();
			//}
			//
			//if (qv.inPolygon2D(vScreen,vecScreen) )
			//{
			//	if ((vScreen - center).length()<dis)
			//	{
			//		dis = (vScreen - center).length();
			//		vecPolyon = vecTemp;
			//		sRes = vecDxfData[i].layer;
			//	}

			//}

			//for (int k = 0;k<=vecDxfData[i].vecCoord.size()-1;++k)
			//{
			//	double d = PointToSegDist(v.x(),v.y(),vecDxfData[i].vecCoord[k].x,vecDxfData[i].vecCoord[k].y,vecDxfData[i].vecCoord[k+1].x,vecDxfData[i].vecCoord[k+1].y);
			//	if(d<dis)
			//	{
			//		dis = d;
			//		/*vecPolyon.clear();
			//		for (int j = 0;j<vecDxfData[i].vecCoord.size();++j)
			//		{
			//		vecPolyon.push_back(osg::Vec3d(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z));
			//		}*/
			//		dlData = vecDxfData[i];
			//		//vecPolyon.push_back(osg::Vec3d(vecDxfData[i].vecCoord[0].x,vecDxfData[i].vecCoord[0].y,vecDxfData[i].vecCoord[0].z));
			//		//sRes = vecDxfData[i].layer;
			//		
			//	}
			//}
		}
		else if (vecDxfData[i].type == "3DFACE")
		{
			std::vector<osg::Vec3d> vecT;
			for(int j = 0;j<vecDxfData[i].vecCoord.size();++j)
			{
				vecT.push_back(osg::Vec3(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z == 0?vecDxfData[i].vecCoord[j].z+10:vecDxfData[i].vecCoord[j].z));
			}
			if (qv.inPolygon2D(v,vecT) )
			{
				double zmax = -999,zmin = 999;
				for (int k = 0;k<vecDxfData[i].vecCoord.size();++k)
				{
					zmax = zmax < vecDxfData[i].vecCoord[k].z?vecDxfData[i].vecCoord[k].z:zmax;
					zmin = zmin > vecDxfData[i].vecCoord[k].z?vecDxfData[i].vecCoord[k].z:zmin;
				}
				if (v.z() >= zmin-0.01 && v.z() <= zmax+0.01)
				{
					/*vecPolyon = vecT;
					if (vecT.front() != vecT.back())
					{
						vecPolyon.push_back(vecPolyon.front());
					}*/
					dlData = vecDxfData[i];
					if (!(dlData.vecCoord.front() == dlData.vecCoord.back()))
					{
						dlData.vecCoord.push_back(dlData.vecCoord.front());
					}
					return true;//点在3dface上，直接返回结果
				}

			}
		}
		else if (vecDxfData[i].type == "LINE")
		{
			for(int j = 0;j<vecDxfData[i].vecCoord.size();++j)
			{
				sceneView->projectObjectIntoWindow(osg::Vec3(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z == 0?vecDxfData[i].vecCoord[j].z+dxf_zOffset:vecDxfData[i].vecCoord[j].z)/**matrix*/, ptWindow);
				//vecTemp.push_back(osg::Vec3(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z == 0?vecDxfData[i].vecCoord[j].z+dxf_zOffset:vecDxfData[i].vecCoord[j].z));
				vecScreen.push_back(ptWindow);
				//center += ptWindow;
			}
			if ((vecScreen[0].x()<0||vecScreen[0].x()>sx)||
				(vecScreen[0].y()<0||vecScreen[0].y()>sy)||
				(vecScreen[1].x()<0||vecScreen[1].x()>sx)||
				(vecScreen[1].y()<0||vecScreen[1].y()>sy))
			{
				;
			}
			else
			{
				double d = PointToSegDist(eaX,eaY,vecScreen[0].x(),vecScreen[0].y(),vecScreen[1].x(),vecScreen[1].y());
				if (d<dis)
				{
					dis = d;
					/*vecPolyon.clear();
					for (int j = 0;j<vecDxfData[i].vecCoord.size();++j)
					{
						vecPolyon.push_back(osg::Vec3d(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z));
					}*/
					dlData = vecDxfData[i];
				}
			}
			//double d = PointToSegDist(v.x(),v.y(),vecDxfData[i].vecCoord[0].x,vecDxfData[i].vecCoord[0].y,vecDxfData[i].vecCoord[1].x,vecDxfData[i].vecCoord[1].y);
			//if (d<dis)
			//{
			//	dis = d;
			//	/*vecPolyon.clear();
			//	for (int j = 0;j<vecDxfData[i].vecCoord.size();++j)
			//	{
			//		vecPolyon.push_back(osg::Vec3d(vecDxfData[i].vecCoord[j].x,vecDxfData[i].vecCoord[j].y,vecDxfData[i].vecCoord[j].z));
			//	}*/
			//	dlData = vecDxfData[i];
			//}
		}
	}
	if (dis >= 20)
	{
		return false;
	}
	return true;
}
osg::ref_ptr<osg::Billboard> PickModelHandler::createText(osg::Vec3d pos,osg::Vec4 textColor,osg::Vec4 outlineColor, float layoutCharacterSize, std::string title)
{
	osg::ref_ptr<osg::Billboard> geode = new osg::Billboard();
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
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	text->setStateSet(stateset);

	geode->addDrawable(text);
	geode->setPosition(0,pos);
	return geode;
}

void PickModelHandler::MakeBuilding3DBorder(osg::Vec3d vi)
{
    USES_CONVERSION;
    osgEarth::Drivers::OGRFeatureOptions ogrOpt;
    std::string path = W2A(FindSHPFile());
    if (path.length() == 0)
    {
		if (m_sDefLayer.Find(L".osg")!=-1 || m_sDefLayer.Find(L".OSG")!= -1)//如果默认图层是osg
		{
			osg::BoundingBox box;
			std::string name = queryOSG(vi,box);
			if (!name.empty())
			{
				if (gTemp != nullptr)
				{
					m_spViewer3D->RemoveNode(gTemp);
					gTemp = nullptr;
				}
				gTemp = new osg::Group();
				gTemp->addChild(createText(osg::Vec3d(box.center().x(),box.center().y(),box.zMin() + 1.1*(box.zMax() - box.zMin())),osg::Vec4(1,0,0,1),osg::Vec4(1,1,1,1),10,name));
				gTemp->addChild(drawBoxArea(box,osg::Vec4(1,1,0,1)));
				m_spViewer3D->AddNode(gTemp);
			}
		}
		else if (m_sDefLayer.Find(L".DXF")!=-1 || m_sDefLayer.Find(L".dxf")!= -1)//如果默认是dxf
		{
			std::vector<osg::Vec3d> vec;
			/*if (vi.length()<=0.00000001)
			{
				vi = osg::Vec3d(eaX,eaY,0);
			}*/
			DL_DATA dlData;
			bool bSuccess = queryDXF(vi,dlData);
			if (bSuccess)
			{
				if (gTemp != nullptr)
				{
					m_spViewer3D->RemoveNode(gTemp);
					gTemp = nullptr;
				}
				for (int i = 0;i<dlData.vecCoord.size();++i)
				{
					vec.push_back(osg::Vec3d(dlData.vecCoord[i].x,dlData.vecCoord[i].y,dlData.vecCoord[i].z));
				}
				/*osg::Vec3d center(0,0,0);
				for (int i = 0;i<dlData.size();++i)
				{
				center+=vec[i];
				}
				center/=vec.size();
				center.z() = center.z()==0? center.z()+ dxf_zOffset:center.z();*/
				gTemp = new osg::Group();
				//gTemp->addChild(createText(center+osg::Vec3(0,0,2),osg::Vec4(1,0,0,1),osg::Vec4(1,1,1,1),10,name));
				gTemp->addChild(drawPolyArea(vec,osg::Vec4(1,0,1,1)));
				gTemp->setName(dlData.type);
				m_spViewer3D->AddNode(gTemp);

				p->m_pAttInfoWin->Invalidate();
				p->m_pAttInfoWin->RemoveAllRows();
				p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
				p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
				p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
				int len = 0,maxW = 0;
				std::string sInfo;
				//sInfo = "X:"+toString(dlData.vecCoord[0].x) + " Y:"+ toString(dlData.vecCoord[0].y) + " Z:"+ toString(dlData.vecCoord[0].z);
				p->m_pAttInfoWin->InsertRow(_T("坐标Z"),CString(toString(dlData.vecCoord[0].y).c_str()));
				p->m_pAttInfoWin->InsertRow(_T("坐标Y"),CString(toString(dlData.vecCoord[0].y).c_str()));
				p->m_pAttInfoWin->InsertRow(_T("坐标X"),CString(toString(dlData.vecCoord[0].x).c_str()));
				len += 3;
				if (dlData.color.x == -1)
					p->m_pAttInfoWin->InsertRow(_T("颜色"),CString("BYBLOCK"));
				else if (dlData.color.x == -2)
					p->m_pAttInfoWin->InsertRow(_T("颜色"),CString("BYLAYER"));
				else
				{
					p->m_pAttInfoWin->InsertRow(_T("颜色B"),CString(toString(dlData.color.z).c_str()));
					p->m_pAttInfoWin->InsertRow(_T("颜色G"),CString(toString(dlData.color.y).c_str()));
					p->m_pAttInfoWin->InsertRow(_T("颜色R"),CString(toString(dlData.color.x).c_str()));
					len += 2;
				}
				len++;
				p->m_pAttInfoWin->InsertRow(_T("层"),CString(dlData.layer.c_str()));
				maxW = maxW>dlData.layer.size()?maxW:dlData.layer.size();
				len++;
				p->m_pAttInfoWin->changeDlgHeight((len+3)*g_RowHeight);
				p->m_pAttInfoWin->suitHeightAndWidth(len,maxW);//调整高度和列宽
				p->m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
				p->m_pAttInfoWin->UpdateWindow();
			}
		}
        return;
    }

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
    while(cursor->hasMore())
    {
        //输出feature中的信息
        feature = cursor->nextFeature();
        osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
		std::string sname = feature->getName();
		sname = feature->className();
		std::string sname1 = feature->getString("AcDbEntity");
		AttributeTable att =  feature->getAttrs();
		
        while(parts.hasMore())
        {
            osgEarth::Symbology::Geometry* part = parts.next();
            osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

            allPoints = part->toVec3Array();
			int kk = allPoints->size();
            Vec3dVector* vv = new Vec3dVector();
			osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

			for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
			{
				osg::Vec3d v(*i);
				vv->push_back(v);

				v.z() = v.z() + 1;
				v3->push_back(v);
			}

			//如果是兴趣点
			//采取遍历所有点的方式 保证选中
			if (allPoints->size() == 1)
			{
				osg::Vec3d v(*(allPoints->begin()));
				double k = distance2D(osg::Vec3d(vi.x(),vi.y(),0),osg::Vec3d(v.x(),v.y(),0));
				if (k<disMin)
				{
					disMin = k;
					mfeature = dynamic_cast<osgEarth::Features::Feature*>(feature->clone(osg::CopyOp::DEEP_COPY_ALL));
				}
			}
			//如果是面
			else
			{
				int type = 1;
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
							if (gTemp != nullptr)
							{
								m_spViewer3D->RemoveNode(gTemp);
								gTemp = nullptr;
							}
							osg::Vec4 color;
							std::string sTs = feature->getString("特殊人群");
							if (!sTs.empty())
							{
								if (sTs == "社区校正人员")
								{
									color = osg::Vec4(0,0,1,1);
									MakeFloor3DBorder(feature, dGC+dDG, dGC + dDG2,color);
									return;
								}
								if (sTs == "刑满释放人员")
								{
									color = osg::Vec4(0,0,0,1);
									MakeFloor3DBorder(feature, dGC+dDG, dGC + dDG2,color);
									return;
								}
								if (sTs == "吸毒人员")
								{
									color = osg::Vec4(1,0,0,1);
									MakeFloor3DBorder(feature, dGC+dDG, dGC + dDG2,color);
									return;
								}
							}

							MakeFloor3DBorder(feature, dGC+dDG, dGC + dDG2);
							return;
						}
						else
							continue;
					}


					if (gTemp != nullptr)
					{
						m_spViewer3D->RemoveNode(gTemp);
						gTemp = nullptr;
					}
					//获取房屋高度
					double dHeight = 0;
					m_geoPart = part;

					if (!sTH.empty())
					{
						dHeight = atof(sTH.data());
					}
					else
						dHeight = GetBuildingHeight();

					gTemp = new osg::Group();
					double newZ = 0;
					std::string sDG1 = feature->getString("z");
					if (!sDG1.empty())
					{
						newZ = atof(sDG1.data()) - 0.5;
					}
					for (int i=0;i<v3->size();i++)
					{
						osg::ref_ptr< osg::Vec3Array > v3Vert = new osg::Vec3Array;
						osg::Vec3d ptVert,ptVertNext;

						ptVert = osg::Vec3d((*v3)[i]);
						ptVert.z() = newZ;
						
						if (i == v3->size()-1)
						{
							ptVertNext = osg::Vec3d((*v3)[0]);
						}
						else
							ptVertNext = osg::Vec3d((*v3)[i+1]);
						ptVertNext.z() = newZ;

						v3Vert->push_back(ptVert);

						v3Vert->push_back(ptVertNext);

						ptVertNext.z() = dHeight + 0.5;
						v3Vert->push_back(ptVertNext);

						ptVert.z() = dHeight + 0.5;
						v3Vert->push_back(ptVert);

						gTemp->addChild(AddPolygonNode(v3Vert.get()));
					}

					osg::ref_ptr< osg::Vec3Array > v3Top = new osg::Vec3Array;

					//房屋顶盖，貌似也可以不画啊
					double h = dHeight;
					for (int i=0;i<v3->size();i++)
					{
						osg::Vec3d ptTop;

						ptTop = osg::Vec3d((*v3)[i]);
						ptTop.z() = h + 0.5;
						v3Top->push_back(ptTop);
					}
					gTemp->addChild(AddPolygonNode(v3Top.get()));
					//p->m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					p->m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pAttInfoWin->RemoveAllRows();
					
					
					const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
					int len = 0,maxW = 0;
					if (s_doorSensor.size()>0)//栋
					{
						std::vector<CString> vecs;
						p->m_pInternetInfoWin->analysisData(s_doorSensor,vecs);
						std::string s;
						dateTrans(W2A(vecs[4]),s);
						p->m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
						p->m_pAttInfoWin->InsertRow(_T("门磁状态"),vecs[2] );	
						maxW = s.size();
					}
					for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
					{
						//如果字段为全英文，跳过不显示
						if (p->allAlpha(i->first) || i->first == "高程" || i->first == "顶高" || i->first == "底高")
						{
							continue;
						}
						std::string str = i->second.getString();
						//处理：有小数点的字段保留小数点后两位
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
						p->m_pAttInfoWin->InsertRow(CString(i->first.c_str()),CString(str.c_str()));
						//记录显示的记录条数
						len++;
					}
					
					p->m_pAttInfoWin->changeDlgHeight((len+3)*g_RowHeight);
					//p->m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					p->m_pAttInfoWin->suitHeightAndWidth(len,maxW);//调整高度和列宽
					p->m_pAttInfoWin->UpdateWindow();
					p->m_pAttInfoWin->pChild->ShowWindow(SW_HIDE);
					p->m_pAttInfoWin->pChild1->ShowWindow(SW_HIDE);
					std::string sHeight = feature->getString("Elevation");
					double height = atof(sHeight.data());

					gTemp->setName("线框");
					m_spViewer3D->AddNode(gTemp);					
				}
			}   
        }
    }

	if (disMin<5)//如果最近的兴趣点在5米之内，则选中它
	{
		FILE* infile = fopen("temp.dat","r");
		int flag = 0;
		fscanf_s(infile,"%d",&flag);
		fclose(infile);
		std::string sTp = mfeature->getString("name");//获取热点名称
		//如果存在字段name
		if (!sTp.empty())
		{
			if (flag == 1)
			{
				if (sTp == "物业服务中心")
				{
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->_type = 3;
					p->m_pInternetInfoWin->showInfo(true);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_SHOW);
					p->m_pInternetInfoWin->AutoAdjustDlg();
					p->m_pInternetInfoWin->UpdateWindow();
				}
				else if (sTp == "车辆出口")
				{
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->_type = 1;
					p->m_pInternetInfoWin->showInfo(true);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_SHOW);
					p->m_pInternetInfoWin->AutoAdjustDlg();
					p->m_pInternetInfoWin->UpdateWindow();
				}
				else if (sTp == "车辆入口" )
				{
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->_type = 2;
					p->m_pInternetInfoWin->showInfo(true);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_SHOW);
					p->m_pInternetInfoWin->AutoAdjustDlg();
					p->m_pInternetInfoWin->UpdateWindow();
				}
				else if (sTp == "门禁视频")
				{
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_SHOW);
					std::string sW = mfeature->getString("width");
					std::string sH = mfeature->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());
					p->m_pCVMediaPlayerInfoWin->changePlayerSize(dW,dH);
					p->m_pCVMediaPlayerInfoWin->setupPolygonRegion();
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pCVMediaPlayerInfoWin->open("Data\\DRVdata\\door.mp4");
					p->m_pCVMediaPlayerInfoWin->Invalidate();
					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
				}
				else if (sTp == "西门门禁视频")
				{
					std::string source = mfeature->getString("source");
					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pVLCPlayerInfoWin->ShowWindow(SW_SHOW);
					p->m_pVLCPlayerInfoWin->setRtspAddress(source);
					p->m_pVLCPlayerInfoWin->connectRTSP();
				}
				else if (sTp == "井盖")
				{
					std::vector<CString> vec;
					if (s_manHole.empty())
					{
						for (int i = 0;i<=10;++i)
						{
							vec.push_back(_T(" "));
						}
					}
					else
						p->m_pInternetInfoWin->analysisData(s_manHole,vec);

					p->m_pAttInfoWin->RemoveAllRows();
					std::string s;
					dateTrans(W2A(vec[9]),s);
					p->m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
					p->m_pAttInfoWin->InsertRow(_T("水位状态"),vec[8]);
					p->m_pAttInfoWin->InsertRow(_T("终端电压"),vec[7]);
					p->m_pAttInfoWin->InsertRow(_T("报警频率"),vec[6]);
					p->m_pAttInfoWin->InsertRow(_T("上报频率"),vec[5]);
					p->m_pAttInfoWin->InsertRow(_T("当前角度"),vec[4]);
					p->m_pAttInfoWin->InsertRow(_T("报警角度"),vec[3]);
					p->m_pAttInfoWin->InsertRow(_T("功能码"),vec[2]);
					p->m_pAttInfoWin->InsertRow(_T("安装位置"),vec[1]);
					p->m_pAttInfoWin->InsertRow(_T("编号"),vec[0]);

					int w = s.size() + 3;
					w = w<5?5:w;
					p->m_pAttInfoWin->changeDlgHeight((10+1)*g_RowHeight);
					//p->m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					p->m_pAttInfoWin->suitHeightAndWidth(10,w);//调整高度和列宽
					p->m_pAttInfoWin->UpdateWindow();
					p->m_pAttInfoWin->selfDefShowWindow(SW_SHOW);

					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else if (sTp == "烟感器")
				{
					std::vector<CString> vec;
					if (s_smokeSensor.empty())
					{
						for (int i = 0;i<6;++i)
						{
							vec.push_back(_T(" "));
						}
					}
					else
						p->m_pInternetInfoWin->analysisData(s_smokeSensor,vec);
					
					p->m_pAttInfoWin->RemoveAllRows();
					std::string s;
					dateTrans(W2A(vec[5]),s);
					p->m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
					p->m_pAttInfoWin->InsertRow(_T("测试键"),vec[4]);
					p->m_pAttInfoWin->InsertRow(_T("电池状态"),vec[3]);
					p->m_pAttInfoWin->InsertRow(_T("烟雾状态"),vec[2]);
					p->m_pAttInfoWin->InsertRow(_T("安装位置"),vec[1]);
					p->m_pAttInfoWin->InsertRow(_T("编号"),vec[0]);

					int w = s.size() + 3;
					w = w<5?5:w;
					p->m_pAttInfoWin->changeDlgHeight((6+1)*g_RowHeight);
					//p->m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					p->m_pAttInfoWin->suitHeightAndWidth(6,w);//调整高度和列宽
					p->m_pAttInfoWin->UpdateWindow();
					p->m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else if (sTp == "门磁")
				{
					std::vector<CString> vec;
					if (s_doorSensor.empty())
					{
						for (int i = 0;i<5;++i)
						{
							vec.push_back(_T(" "));
						}
					}
					else
						p->m_pInternetInfoWin->analysisData(s_doorSensor,vec);

					p->m_pAttInfoWin->RemoveAllRows();
					std::string s;
					dateTrans(W2A(vec[4]),s);
					p->m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
					p->m_pAttInfoWin->InsertRow(_T("RF类型"),vec[3]);
					p->m_pAttInfoWin->InsertRow(_T("门禁状态"),vec[2]);
					p->m_pAttInfoWin->InsertRow(_T("安装位置"),vec[1]);
					p->m_pAttInfoWin->InsertRow(_T("编号"),vec[0]);

					int w = s.size() + 3;
					w = w<5?5:w;
					p->m_pAttInfoWin->changeDlgHeight((5+1)*g_RowHeight);
					//p->m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					p->m_pAttInfoWin->suitHeightAndWidth(5,w);//调整高度和列宽
					p->m_pAttInfoWin->UpdateWindow();
					p->m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else if (sTp == "电表")
				{
					std::vector<CString> vec;
					if (s_eleMeter.empty())
					{
						for (int i = 0;i<7;++i)
						{
							vec.push_back(_T(" "));
						}
					}
					else
						p->m_pInternetInfoWin->analysisData(s_eleMeter,vec);

					p->m_pAttInfoWin->RemoveAllRows();
					std::string s;
					dateTrans(W2A(vec[6]),s);
					p->m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
					p->m_pAttInfoWin->InsertRow(_T("电流"),vec[5]);
					p->m_pAttInfoWin->InsertRow(_T("电压"),vec[4]);
					p->m_pAttInfoWin->InsertRow(_T("电能"),vec[3]);
					p->m_pAttInfoWin->InsertRow(_T("电表类型"),vec[2]);
					p->m_pAttInfoWin->InsertRow(_T("安装位置"),vec[1]);
					p->m_pAttInfoWin->InsertRow(_T("电表号"),vec[0]);

					int w = s.size() + 3;
					w = w<5?5:w;
					p->m_pAttInfoWin->changeDlgHeight((7+1)*g_RowHeight);
					//p->m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
					p->m_pAttInfoWin->suitHeightAndWidth(7,w);//调整高度和列宽
					p->m_pAttInfoWin->UpdateWindow();
					p->m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else//其他
				{
					p->m_pShowPictureInfoWin->setupPolygonRegion();
					p->m_pShowPictureInfoWin->setPath("Data\\DRVdata\\1024商家信息.PNG");
					p->m_pShowPictureInfoWin->setColorType(3);
					p->m_pShowPictureInfoWin->ShowWindow(SW_SHOW);
					p->m_pShowPictureInfoWin->Invalidate();

					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
				}
			}
			else
			{
				if (sTp == "物业服务中心")//显示刷卡记录html页面
				{
					p->m_pHtmlInfoWin->setHTML("Data\\DRVdata\\swiperecord\\index.html");
					std::string sW = mfeature->getString("width");
					std::string sH = mfeature->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());
					p->m_pHtmlInfoWin->changeHTMLsize(dW,dH);
					p->m_pHtmlInfoWin->setupPolygonRegion();
					p->m_pHtmlInfoWin->ShowWindow(SW_SHOW);
					p->m_pHtmlInfoWin->Invalidate();

					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else if (sTp == "门禁视频")
				{	
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_SHOW);
					std::string sW = mfeature->getString("width");
					std::string sH = mfeature->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());
					p->m_pCVMediaPlayerInfoWin->changePlayerSize(dW,dH);
					p->m_pCVMediaPlayerInfoWin->setupPolygonRegion();
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pCVMediaPlayerInfoWin->open("Data\\DRVdata\\door.mp4");
					p->m_pCVMediaPlayerInfoWin->Invalidate();
					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else if (sTp == "车辆入口")
				{
					std::string sW = mfeature->getString("width");
					std::string sH = mfeature->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());

					p->m_pShowPictureInfoWin->changePictureSize(dW,dH);
					p->m_pShowPictureInfoWin->setupPolygonRegion();
					p->m_pShowPictureInfoWin->setPath("Data\\DRVdata\\车辆卡口.png");
					p->m_pShowPictureInfoWin->setColorType(5);
					p->m_pShowPictureInfoWin->ShowWindow(SW_SHOW);
					p->m_pShowPictureInfoWin->Invalidate();

					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else if (sTp == "车辆出口")
				{
					std::string sW = mfeature->getString("width");
					std::string sH = mfeature->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());

					p->m_pShowPictureInfoWin->changePictureSize(dW,dH);
					p->m_pShowPictureInfoWin->setupPolygonRegion();
					p->m_pShowPictureInfoWin->setPath("Data\\DRVdata\\车辆卡口.png");
					p->m_pShowPictureInfoWin->setColorType(5);
					p->m_pShowPictureInfoWin->ShowWindow(SW_SHOW);
					p->m_pShowPictureInfoWin->Invalidate();

					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
				}
				else if (sTp == "门禁视频")
				{
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_SHOW);
					std::string sW = mfeature->getString("width");
					std::string sH = mfeature->getString("height");
					int dW = atof(sW.data());
					int dH = atof(sH.data());
					p->m_pCVMediaPlayerInfoWin->changePlayerSize(dW,dH);
					p->m_pCVMediaPlayerInfoWin->setupPolygonRegion();
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
					p->m_pCVMediaPlayerInfoWin->open("Data\\DRVdata\\door.mp4");
					p->m_pCVMediaPlayerInfoWin->Invalidate();
					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
				}
				else//其他
				{
					p->m_pShowPictureInfoWin->setupPolygonRegion();
					p->m_pShowPictureInfoWin->setPath("Data\\DRVdata\\1024商家信息.PNG");
					p->m_pShowPictureInfoWin->setColorType(3);
					p->m_pShowPictureInfoWin->ShowWindow(SW_SHOW);
					p->m_pShowPictureInfoWin->Invalidate();

					p->m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
					p->m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
					p->m_pCVMediaPlayerInfoWin->KillTimer(1);
				}
			}		
		}
	}
}

void PickModelHandler::MakeFloor3DBorder(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Vec4 color)
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
			p->m_pAttInfoWin->Invalidate();
			p->m_pAttInfoWin->RemoveAllRows();
			p->m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
			p->m_pHtmlInfoWin->ShowWindow(SW_HIDE);
			p->m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
			p->m_pShowPictureInfoWin->ShowWindow(SW_HIDE);

			int len = 0,maxW = 0;
			bool bexpand = false;
			const osgEarth::Features::AttributeTable& attrs = feature->getAttrs();
			p->m_pAttInfoWin->setLocation(firstPt.x(),firstPt.y());
			p->m_pAttInfoWin->InsertRow(_T("生成二维码"),_T(""));
			if (p->m_pAttInfoWin->pQren != NULL)
			{
				p->m_pAttInfoWin->pQren->DestroyWindow();
			}
			if (s_eleMeter.size()>0)//栋
			{
				std::vector<CString> vecs;
				p->m_pInternetInfoWin->analysisData(s_eleMeter,vecs);
				std::string s;
				dateTrans(W2A(vecs[6]),s);
				p->m_pAttInfoWin->InsertRow(_T("更新时间"),CString(s.c_str()));
				p->m_pAttInfoWin->InsertRow(_T("电能"),vecs[3] + _T("Kwh"));
				maxW = s.size();
			}
			for( osgEarth::Features::AttributeTable::const_iterator i = attrs.begin(); i != attrs.end(); ++i)
			{
				if (p->allAlpha(i->first) || i->first == "高程" || i->first == "顶高" || i->first == "底高")
				{
					continue;
				}
				std::string str = i->second.getString();
				if (CString(i->first.c_str()) == _T("家庭成员")) 
				{
					p->m_pAttInfoWin->analysisString( str,p->m_pAttInfoWin->vName );
					bexpand = true; 
				}
				if (CString(i->first.c_str()) == _T("成员身份证"))
				{
					p->m_pAttInfoWin->analysisString( str,p->m_pAttInfoWin->vID );
					continue;
				}
				int n = str.find(".");
				if ( n != -1)
				{
					str = str.substr(0,n+3<=str.length()?n+3:str.length());
				}
				
				if (bexpand)
				{
					str += "(详细)";
					CString cs = CString(str.c_str());
					p->m_pAttInfoWin->InsertRow(CString(i->first.c_str()),cs);
					bexpand = false;
				}
				else
					p->m_pAttInfoWin->InsertRow(CString(i->first.c_str()),CString(str.c_str()));
				len++;

				if (str.size()>maxW)
				{
					maxW = str.size();
				}
			}
			
			p->m_pAttInfoWin->changeDlgHeight((len+3)*g_RowHeight);
			//p->m_pAttInfoWin->MovetoPoint(m_spViewer3D->rectView3D);
			p->m_pAttInfoWin->suitHeightAndWidth(len,maxW);//调整高度和列宽
			p->m_pAttInfoWin->UpdateWindow();
			p->m_pAttInfoWin->pChild->ShowWindow(SW_HIDE);
			p->m_pAttInfoWin->pChild1->ShowWindow(SW_HIDE);
			std::string sHeight = feature->getString("Elevation");
			double height = atof(sHeight.data());

			gTemp->setName("线框");
			//gTemp->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);//关闭深度测试
			m_spViewer3D->AddNode(gTemp);
	}
}

void PickModelHandler::Clear()
{
	if (gTemp != nullptr)
	{
		m_spViewer3D->RemoveNode(gTemp);
		gTemp = nullptr;
	}
	if (p->m_pAttInfoWin != NULL)
	{
		CBackgroundDlg* dlg = (CBackgroundDlg*)(p->m_pAttInfoWin->parentBk);
		if (dlg)
		{
			dlg->DestroyWindow();
		}
		p->m_pAttInfoWin->DestroyWindow();
	}
	if (p->m_pCVMediaPlayerInfoWin != NULL)
	{
		p->m_pCVMediaPlayerInfoWin->DestroyWindow();
	}
	if (p->m_pShowPictureInfoWin != NULL)
	{
		p->m_pShowPictureInfoWin->DestroyWindow();
	}
	if (p->m_pHtmlInfoWin != NULL)
	{
		p->m_pHtmlInfoWin->DestroyWindow();
	}
	if (p->m_pInternetInfoWin != NULL)
	{
		CBackgroundDlg* dlg = (CBackgroundDlg*)(p->m_pInternetInfoWin->parentBk);
		if (dlg)
		{
			dlg->ShowWindow(SW_HIDE);
		}
		p->m_pInternetInfoWin->ShowWindow(SW_HIDE);
	}
	if (p->m_pVLCPlayerInfoWin != NULL)
	{
		p->m_pVLCPlayerInfoWin->DestroyWindow();
	}

	p->m_pAttInfoWin = NULL;
	p->m_pCVMediaPlayerInfoWin = NULL;
	p->m_pShowPictureInfoWin = NULL;
	p->m_pHtmlInfoWin = NULL;
	p->m_pVLCPlayerInfoWin = NULL;
}


//////////////////////////////////////////////////////////////////////////
CvlPointSelect::CvlPointSelect(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"单体查询"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"查询搜索"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"单体查询"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"单体查询"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"AttShow"));
    bInSurvey = false;

	p = this;
	m_pAttInfoWin = NULL;
	m_pShowPictureInfoWin = NULL;
	m_pHtmlInfoWin = NULL;
	m_pCVMediaPlayerInfoWin = NULL;
	m_pInternetInfoWin = NULL;
	m_pVLCPlayerInfoWin = NULL;

	int ScreenH = GetSystemMetrics(SM_CYSCREEN); 
	if (ScreenH < 1024)
	{
		g_FontSize = 12;
		g_RowHeight = 24;
	}
	else
	{
		g_FontSize = 16;
		g_RowHeight = 30;
	}
}

CvlPointSelect::~CvlPointSelect(void)
{
}

bool CvlPointSelect::Initialize()
{
    return true;
}

bool CvlPointSelect::UnInitialize()
{
    return true;
}

bool CvlPointSelect::SetBuddy(x3::IObject* val)
{
    x3::Object<IViewer3D> spViewer3D(val);
    if( !spViewer3D.valid() )
        return false;

    if( m_spBuddy == spViewer3D )
        return true;

    m_spBuddy = spViewer3D;
    if( spViewer3D->getViewer() == nullptr )
        return false;

    m_val = val;
    return true;
}


bool CvlPointSelect::Checked()
{
    return bInSurvey;
}

// IUICommand
bool CvlPointSelect::OnClick()
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

bool CvlPointSelect::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//对各种显示窗口初始化
	if (m_pAttInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pAttInfoWin = new CAttInfoDlg();
		m_pAttInfoWin->_rect = spViewer3D->rectView3D;
		m_pAttInfoWin->parent = (CObject*) this; 
		m_pAttInfoWin->prjInfo = spViewer3D->GetPrj();
		m_pAttInfoWin->Create(IDD_DIALOG_POINTSELECT);
		CBackgroundDlg* dlg = new CBackgroundDlg();
		dlg->Create(IDD_DIALOG_BACKGROUND1);
		m_pAttInfoWin->parentBk = (CObject*)dlg;
	}
	if (m_pCVMediaPlayerInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pCVMediaPlayerInfoWin = new CCVMediaPlayerDlg();
		//m_pCVMediaPlayerInfoWin->parent = (CObject*) this; 
		m_pCVMediaPlayerInfoWin->Create(IDD_DIALOG_CVPLAYER);
	}
	if (m_pShowPictureInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pShowPictureInfoWin = new CShowPictureDlg();
		m_pShowPictureInfoWin->Create(IDD_DIALOG_PICTURE);
	}
	if (m_pHtmlInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pHtmlInfoWin = new CHtmlDlg(); 
		m_pHtmlInfoWin->Create(IDD_DIALOG_HTML);
	}
	if (m_pInternetInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pInternetInfoWin = new CDLGInternetInfo();
		m_pInternetInfoWin->parent = (CObject*)this;
		m_pInternetInfoWin->Create(IDD_DIALOG_NETINFO);
		CBackgroundDlg* dlg = new CBackgroundDlg();
		dlg->Create(IDD_DIALOG_BACKGROUND1);
		m_pInternetInfoWin->parentBk = (CObject*)dlg;
	}
	if (m_pVLCPlayerInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pVLCPlayerInfoWin = new CVLCPlayerDlg();
		m_pVLCPlayerInfoWin->Create(IDD_DIALOG_VLCPLAYER);
	}
	//添加鼠标点击事件
	pickhandler = new PickModelHandler();
	pickhandler->EnableSelect(true);
	pickhandler->SetSelectType(PickModelHandler::SELECT::GEOD);
	pickhandler->SetBuddy(m_val);
	spViewer3D->getViewer()->addEventHandler( pickhandler.get() );
	bInSurvey = true;

	//////////显示属性查询窗口
	m_pAttInfoWin->selfDefShowWindow(SW_SHOW);
	m_pAttInfoWin->selfDefShowWindow(SW_HIDE);
	RECT r,r1;
	m_pAttInfoWin->GetWindowRect(&r);
	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;
	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;
	
	((CBackgroundDlg*)m_pAttInfoWin->parentBk)->MoveWindow(&r);
	m_pAttInfoWin->MoveWindow(&r);
	m_pAttInfoWin->RemoveAllRows();

	r1.top = r.top + m_pAttInfoWin->btnH + 5;
	r1.bottom = r.bottom - 10;
	r1.left = r.left + 3;
	r1.right = r.right - 3;
	m_pAttInfoWin->ScreenToClient(&r1);
	m_pAttInfoWin->m_wndList.MoveWindow(&r1);
	r1.top = r.top + 4;
	r1.bottom = r1.top + m_pAttInfoWin->btnH;
	r1.right = r.right - 3;
	r1.left = r1.right - m_pAttInfoWin->btnW;
	m_pAttInfoWin->ScreenToClient(&r1);
	m_pAttInfoWin->m_EXIT.MoveWindow(&r1);
	
	//////////显示视频窗口
	m_pCVMediaPlayerInfoWin->ShowWindow(SW_HIDE);
	m_pCVMediaPlayerInfoWin->GetWindowRect(&r);
	nWidth = r.right - r.left;
	nHeight = r.bottom - r.top;
	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;
	m_pCVMediaPlayerInfoWin->MoveWindow(&r);

	/////////显示图片的窗口
	m_pShowPictureInfoWin->ShowWindow(SW_HIDE);
	m_pShowPictureInfoWin->GetWindowRect(&r);
	nWidth = r.right - r.left;
	nHeight = r.bottom - r.top;
	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;
	m_pShowPictureInfoWin->MoveWindow(&r);
	
	
	/////////显示HTML的窗口
	m_pHtmlInfoWin->ShowWindow(SW_HIDE);
	m_pHtmlInfoWin->GetWindowRect(&r);
	nWidth = r.right - r.left;
	nHeight = r.bottom - r.top;
	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight; 
	m_pHtmlInfoWin->MoveWindow(&r);

	/////////显示网络窗口
	m_pInternetInfoWin->selfDefShowWindow(SW_SHOW);
	m_pInternetInfoWin->selfDefShowWindow(SW_HIDE);
	m_pInternetInfoWin->GetWindowRect(&r);
	nWidth = r.right - r.left;
	nHeight = r.bottom - r.top;
	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight; 

	((CBackgroundDlg*)m_pInternetInfoWin->parentBk)->MoveWindow(&r);
	m_pInternetInfoWin->MoveWindow(&r);
	/////////显示vlc窗口
	m_pVLCPlayerInfoWin->ShowWindow(SW_HIDE);
	m_pVLCPlayerInfoWin->GetWindowRect(&r);
	nWidth = r.right - r.left;
	nHeight = r.bottom - r.top;
	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight; 
	m_pVLCPlayerInfoWin->MoveWindow(&r);
	//m_pVLCPlayerInfoWin->connectRTSP();
	return true;
}

bool CvlPointSelect::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	pickhandler->Clear();
	spViewer3D->getViewer()->removeEventHandler( pickhandler.get() );
	bInSurvey = false;
	return true;
}

bool CvlPointSelect::allAlpha(std::string str)
{
	if (str.empty())
	{
		return true;
	}
	bool result = false;
	for (size_t i = 0;i<str.size();++i)
	{
		if (!isalpha(str[i]) && str[i] != '_' && str[i] != ' ')
		{
			return false;
		}
	}
	return true;
}