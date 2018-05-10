#include "StdAfx.h"
#include "DrawAreaHandler.h"
#include "LineSegmentIntersector"
#include "osg/Point"
#include "osg/LineWidth"
#include "osgUtil/PolytopeIntersector"
#include "EarthMatrixFunctions.h"
#include <sstream>
#include "DataManager.h"
#include <fstream>

extern CDataManager* m_pDM;

CvlMeasureToolHandler::CvlMeasureToolHandler( osg::Group* group ) :
    _mouseDown         (false),
    _group             (group),
    _gotFirstLocation  (false),
    _lastPointTemporary(false),
    _finished          (false),
    //_geoInterpolation  (GEOINTERP_GREAT_CIRCLE),
    _mouseButton       (osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON),
    _isPath            (true),
    _intersectionMask  (0xffffffff),
    _originPos(0, 0, 0),
    _maxNum(0)
{
    _positionAttitudeTransform = nullptr;
    _geode = nullptr;
    _geometry = nullptr;
    _vertices = nullptr;
	curGroup = nullptr;
    _mouseButtonRight = osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON;
    rebuild();
}

CvlMeasureToolHandler::~CvlMeasureToolHandler()
{
    if( _group.valid() && _positionAttitudeTransform.valid() )
    {
        _group->removeChild(_positionAttitudeTransform.get());
        _geometry = nullptr;
        _vertices = nullptr;
        _geode = nullptr;
        _positionAttitudeTransform = nullptr;
    }

    _group = nullptr;
}


void CvlMeasureToolHandler::rebuild()
{
    if( _group.valid() && _positionAttitudeTransform.valid() )
    {
        _group->removeChild(_positionAttitudeTransform.get());
        _geometry = nullptr;
        _vertices = nullptr;
        _geode = nullptr;
        _positionAttitudeTransform = nullptr;
    }

    if( !_group.valid() )
        return;

    _geometry = new osg::Geometry();
    _geometry->setDataVariance(osg::Object::DYNAMIC);
    _geometry->setUseDisplayList( false );
    _geometry->setUseVertexBufferObjects( true );
    osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
    shared_colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    _geometry->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);

    // same trick for shared normal.
    osg::ref_ptr<osg::Vec3Array> shared_normals = new osg::Vec3Array;
    shared_normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
    _geometry->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);

    _vertices = new osg::Vec3Array();
    _geometry->setVertexArray(_vertices.get());

    _geode = new osg::Geode();
    _geode->setDataVariance(osg::Object::DYNAMIC);
    _geode->addDrawable(_geometry.get());
    _geode->getOrCreateStateSet()->setAttributeAndModes( new osg::Point(4.0f) );
    _geode->getOrCreateStateSet()->setAttributeAndModes( new osg::LineWidth(2.0f) );
    _geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	_geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	
	labelGeode = new osg::Geode();
	labelGeode->setDataVariance(osg::Object::DYNAMIC);
	labelGeode->addDrawable(_geometry.get());
	labelGeode->getOrCreateStateSet()->setAttributeAndModes( new osg::Point(2.0f) );
	labelGeode->getOrCreateStateSet()->setAttributeAndModes( new osg::LineWidth(2.0f) );
	labelGeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	labelGeode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );

	textGeode = new osg::Billboard();
	textGeode->setDataVariance(osg::Object::DYNAMIC);
	textGeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	textGeode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );

    _positionAttitudeTransform = new osg::PositionAttitudeTransform();
    _positionAttitudeTransform->setDataVariance(osg::Object::DYNAMIC);
    _positionAttitudeTransform->setPosition(_originPos);
    _positionAttitudeTransform->addChild(_geode.get());
	_positionAttitudeTransform->addChild(labelGeode.get());
	
    _group->addChild(_positionAttitudeTransform.get());
	_group->addChild(textGeode.get());
}

bool CvlMeasureToolHandler::getIsPath() const
{
    return _isPath;
}

void CvlMeasureToolHandler::setIsPath( bool path )
{
    if (_isPath != path)
    {
        _isPath = path;
        _finished = true;
        clear();
        _gotFirstLocation = false;
    }
}

std::wstring RoundOffToString(double f, int n)
{
    std::wstringstream ss;
    int prec = std::numeric_limits<double>::digits10;
    ss.precision(prec);
    ss << f;

    std::wstring s = ss.str();
    int nDot = s.find(L".");
    return s.substr(0, nDot + 1 + n);
}

void CvlMeasureToolHandler::rebuildGeometry()
{
    if( !_geometry.valid() || !_vertices.valid() || _vertices->size() <= 0 )
    {
        if( _positionAttitudeTransform.valid() )
            _positionAttitudeTransform->removeChild(_geode.get());
        return;
    }

    _vertices->dirty();
    if( _geometry->getNumPrimitiveSets() > 0 )
        _geometry->removePrimitiveSet(0, _geometry->getNumPrimitiveSets());

    if( _vertices->size() > 2 && !_isPath )
        _geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, (_finished && _lastPointTemporary) ? _vertices->size() - 1 : _vertices->size()));

    if( _vertices->size() == 2 )
        _geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, (_finished && _lastPointTemporary) ? _vertices->size() - 1 : _vertices->size()));

	if( _vertices->size() > 2 )
		_geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, (_finished && _lastPointTemporary) ? _vertices->size() - 1 : _vertices->size()));
    //_geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, (_finished && _lastPointTemporary) ? _vertices->size() - 1 : _vertices->size()));
    _geode->removeDrawable(_geometry.get());
    _geode->addDrawable(_geometry.get());

    _positionAttitudeTransform->removeChild(_geode.get());
    _positionAttitudeTransform->addChild(_geode.get());
}

bool CvlMeasureToolHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    if ( ea.getHandled() )
        return false;

    osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    if (!viewer)
        return false;

    if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == _mouseButton)
    {
        _mouseDown = true;
        _mouseDownX = ea.getX();
        _mouseDownY = ea.getY();
    }
    else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == _mouseButton)
    {
		if (_vertices->size()>10)
		{
			return false;
		}
        float eps = 1.0f;

        if(_mouseDown)
            _mouseDown = false;
        else
            return false;

        if (osg::equivalent(ea.getX(), _mouseDownX) && osg::equivalent(ea.getY(), _mouseDownY))
        {
            osg::Vec3d pt;
            if (getLocationAt(ea, viewer, pt))
            {
                if (!_gotFirstLocation)
                {
                    _finished = false;
                    _lastPointTemporary = false;
                    clear();
                    _gotFirstLocation = true;
                    _originPos = pt;
                    _positionAttitudeTransform->setPosition(_originPos);
                    _vertices->push_back(pt - _originPos);
                }
                else
                {
                    if (_lastPointTemporary)
                    {
                        _vertices->back() = pt - _originPos;
                        _lastPointTemporary = false;
                    }
                    else
                    {
                        _vertices->push_back(pt - _originPos);
                    }
                    rebuildGeometry();

                    //_gotFirstLocation = false;

                    if( _maxNum > 0 && _vertices->size() == _maxNum )
                        _finished = true;

                    if (_finished /*|| !_isPath*/)
                    {
                        _gotFirstLocation = false;
                    }

                    fireDistanceChanged();
                    aa.requestRedraw();
                }
            }
        }
		if (_vertices->size()>1)
		{
			center = _vertices->front();
			radiu = (_vertices->back() - _vertices->front()).length();
			osg::Vec3d pos = _originPos;
			clear();
			createCircle(center,radiu,_vertices);
			rebuildGeometry();
			aa.requestRedraw();
			USES_CONVERSION;
			std::string sDefLayer = W2A(m_pDM->m_sDefLayer);
			if (sDefLayer.find(".shp") == -1 && sDefLayer.find(".SHP") == -1 && 
				sDefLayer.find(".dxf") == -1 && sDefLayer.find(".DXF") == -1)
			{
				int n = _group->getNumChildren();
				for (int i = 0;i<n;++i)
				{
					if (_group->getChild(i)->asGroup()->getName() == sDefLayer)
					{
						curGroup = _group->getChild(i)->asGroup();
						HighLightObjectInCircle(curGroup,center,radiu,pos);
						break;
					}
				}
			}
		}
    }
    else if ( (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == _mouseButtonRight)
              || (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN && ea.getKey() == osgGA::GUIEventAdapter::KEY_Escape))
    {
		
		clear();
		clearText();
		return false;
    }
    else if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE)
    {
        if (_gotFirstLocation)
        {
            osg::Vec3d pt;
            if (getLocationAt(ea, viewer, pt))
            {
                _lastPt = pt;
                if (!_lastPointTemporary)
                {
                    _vertices->push_back(pt - _originPos);
                    _lastPointTemporary = true;
                }
                else
                {
                    _vertices->back() = pt - _originPos;
                }
                
				osg::Vec3d p1 = _vertices->front() + _originPos;
				osg::Vec3d p2 = _vertices->back() + _originPos;
				p1.z() = p2.z();
				double len = (p1-p2).length();
				char msg[99];
				sprintf_s(msg,"半径 = %.2f米\0",len);
				textGeode->removeDrawables(0,textGeode->getNumDrawables());
				osgText::Text* text = new osgText::Text;
				osgText::Font* font = osgText::readFontFile("fonts/simhei.ttf");
				text->setFont(font);
				text->setColor(osg::Vec4(1,0,0,1));
				text->setCharacterSize(20* 0.3f);
				text->setAlignment(osgText::TextBase::CENTER_BOTTOM);
				text->setBackdropType(osgText::Text::NONE);
				text->setAxisAlignment(osgText::Text::SCREEN);
				text->setText(string(msg).c_str());
				textGeode->addDrawable(text);
				textGeode->setPosition(0,pt);

				rebuildGeometry();
				fireDistanceChanged();
                aa.requestRedraw();
            }
            else
            {
                _lastPt.set(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
            }
        }
        else
        {
           
        }
    }
    else if( ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN &&	(ea.getKey() == osgGA::GUIEventAdapter::KEY_BackSpace
             || ea.getKey() == osgGA::GUIEventAdapter::KEY_Delete) )
    {
        if (_gotFirstLocation)
        {
            if( _vertices->size() > 1 )
            {
                if( _vertices->size() == 2 )
                {
                    if( _lastPointTemporary )
                    {
                        _vertices->clear();
                        _gotFirstLocation = false;
                    }
                    else
                        _vertices->erase(_vertices->begin());
                }
                else
                {
                    if( _lastPointTemporary )
                        _vertices->erase(_vertices->begin() + _vertices->size() - 2);
                    else
                        _vertices->erase(_vertices->begin() + _vertices->size() - 1);
                }

                rebuildGeometry();
                fireDistanceChanged();
                aa.requestRedraw();
            }
            else if( _vertices->size() == 1 )
            {
                _vertices->clear();
                _gotFirstLocation = false;
                rebuildGeometry();
                fireDistanceChanged();
                aa.requestRedraw();
            }
        }
    }
    return false;
}

bool CvlMeasureToolHandler::getLocationAt(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer, osg::Vec3d &outPt)
{
    osg::ref_ptr<osgUtil::LineSegmentIntersectorEx> picker;
    // use window coordinates
    // remap the mouse x,y into viewport coordinates.
    osg::Viewport* viewport = viewer->getCamera()->getViewport();
    float mx = viewport->x() + (int)((float)viewport->width() * (ea.getXnormalized() * 0.5f + 0.5f));
    float my = viewport->y() + (int)((float)viewport->height() * (ea.getYnormalized() * 0.5f + 0.5f));
    picker = new osgUtil::LineSegmentIntersectorEx( osgUtil::Intersector::WINDOW, mx, my );
    picker->setIntersectionLimit(osgUtil::Intersector::LIMIT_NEAREST);
    picker->setBuffer(3.0f);
    osgUtil::IntersectionVisitor iv(picker.get());
    _geode->setNodeMask(0x0);
    viewer->getCamera()->accept(iv);
    _geode->setNodeMask(0x1);
    if (picker->containsIntersections())
    {
        osgUtil::LineSegmentIntersectorEx::Intersection first = picker->getFirstIntersection();
        outPt = first.getWorldIntersectPoint();
        return true;
    }

    return false;
}

void CvlMeasureToolHandler::clear()
{
    _originPos.set(0, 0, 0);

    if( _positionAttitudeTransform != nullptr )
    {
        if( _positionAttitudeTransform->getNumChildren() > 0 )
            _positionAttitudeTransform->removeChildren(0, _positionAttitudeTransform->getNumChildren());
    }

    if( _geode != nullptr )
    {
        if( _geode->getNumDrawables() > 0 )
            _geode->removeDrawables(0, _geode->getNumDrawables());
    }


    if( _vertices != nullptr )
        _vertices->clear();

	if (curGroup)
	{
		CancelObjectHighLight(curGroup);
	}
	
    _gotFirstLocation = false;
    _lastPointTemporary = false; 
}

void CvlMeasureToolHandler::setMouseButton(int mouseButton)
{
    _mouseButton = mouseButton;
}

int CvlMeasureToolHandler::getMouseButton() const
{
    return _mouseButton;
}


void CvlMeasureToolHandler::addEventHandler(MeasureToolEventHandler* handler )
{
    _eventHandlers.push_back( handler );
}

void CvlMeasureToolHandler::fireDistanceChanged()
{
    
}

void CvlMeasureToolHandler::createCircle(osg::Vec3d center,double radiu,osg::ref_ptr<osg::Vec3Array> _vertices)
{
	for (double d =center.x() - radiu;d < center.x() + radiu;d+=0.1)
	{
		_vertices->push_back(osg::Vec3d(d,sqrt(radiu*radiu - (d-center.x())*(d-center.x())) + center.y(),center.z()));	
	}
	for (double d = center.x() + radiu;d>center.x() - radiu;d-=0.1)
	{
		_vertices->push_back(osg::Vec3d(d,-sqrt(radiu*radiu - (d-center.x())*(d-center.x())) + center.y(),center.z()));
	}
}

void CvlMeasureToolHandler::HighLightObjectInCircle(osg::Group* group,osg::Vec3d& ct0,double& radiu,osg::Vec3d offset)
{
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	vc->push_back(osg::Vec4(0.1,1.0,0.0,1.0));
	int num = 0;
	osg::Vec3d ct = ct0 + offset;
	int numDrawables = labelGeode->getNumDrawables();
	for (int i = numDrawables - 1;i>=0;--i)
	{
		labelGeode->removeDrawables(i);
	}

	int numObj = 0;
	int n = group->getNumChildren();
	for (int i = 0;i<n;i++ )
	{
		osg::ref_ptr<osg::Node> child = group->getChild(i);
		osg::Vec3d c = child->getBound().center();
		double r = child->getBound().radius();
		c.z() = ct.z();
		if ((c - ct).length() <= radiu)
		{
			DrawLabelBox(labelGeode,child->getBound().center() - offset,r,0.5);
			numObj++;
		}
	}
	osgText::Text* text = dynamic_cast<osgText::Text*>(textGeode->getDrawable(0));
	char msg[100];
	sprintf_s(msg,"半径 = %.2f米\n已选择%d个对象",radiu,numObj);
	std::string sMsg(msg);
	text->setText(sMsg);
	_positionAttitudeTransform->removeChild(labelGeode.get());
	_positionAttitudeTransform->addChild(labelGeode.get());
}

void CvlMeasureToolHandler::CancelObjectHighLight(osg::Group* group)
{
	_positionAttitudeTransform->removeChild(labelGeode);
}

void CvlMeasureToolHandler::DrawLabelBox(osg::ref_ptr<osg::Geode> geode,osg::Vec3d o,double r,double w )
{
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(osg::Vec4(0,1,1,1));
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	point->push_back(osg::Vec3d(o.x() - w,o.y() - w,o.z() - r/2));
	point->push_back(osg::Vec3d(o.x() + w,o.y() - w,o.z() - r/2));
	point->push_back(osg::Vec3d(o.x() + w,o.y() - w,o.z() + r/2));
	point->push_back(osg::Vec3d(o.x() - w,o.y() - w,o.z() + r/2));

	point->push_back(osg::Vec3d(o.x() - w,o.y() - w,o.z() + r/2));
	point->push_back(osg::Vec3d(o.x() - w,o.y() - w,o.z() - r/2));
	point->push_back(osg::Vec3d(o.x() - w,o.y() + w,o.z() - r/2));
	point->push_back(osg::Vec3d(o.x() - w,o.y() + w,o.z() + r/2));

	point->push_back(osg::Vec3d(o.x() - w,o.y() + w,o.z() - r/2));
	point->push_back(osg::Vec3d(o.x() - w,o.y() + w,o.z() + r/2));
	point->push_back(osg::Vec3d(o.x() + w,o.y() + w,o.z() + r/2));
	point->push_back(osg::Vec3d(o.x() + w,o.y() + w,o.z() - r/2));

	point->push_back(osg::Vec3d(o.x() + w,o.y() + w,o.z() + r/2));
	point->push_back(osg::Vec3d(o.x() + w,o.y() + w,o.z() - r/2));
	point->push_back(osg::Vec3d(o.x() + w,o.y() - w,o.z() - r/2));
	point->push_back(osg::Vec3d(o.x() + w,o.y() - w,o.z() + r/2));

	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,0,4));
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,4,4));
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,8,4));
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,12,4));

	geode->addDrawable(lineGeom);
}

void CvlMeasureToolHandler::removeText()
{
	if (textGeode.valid())
	{
		_group->removeChild(textGeode);
	}
}

void CvlMeasureToolHandler::clearText()
{
	if (textGeode.valid())
	{
		textGeode->removeDrawables(0,textGeode->getNumDrawables());
	}
}