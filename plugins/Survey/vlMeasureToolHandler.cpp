#include "StdAfx.h"
#include "vlMeasureToolHandler.h"
#include "LineSegmentIntersector"
#include "osg/Point"
#include "osg/LineWidth"
#include "osgUtil/PolytopeIntersector"
#include "EarthMatrixFunctions.h"
#include <sstream>

CvlMeasureToolHandler::CvlMeasureToolHandler( osg::Group* group ) :
    _mouseDown         (false),
    _group             (group),
    _gotFirstLocation  (false),
    _lastPointTemporary(false),
    _finished          (false),
    //_geoInterpolation  (GEOINTERP_GREAT_CIRCLE),
    _mouseButton       (osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON),
    _isPath            (true),
	_isPoint           (false),
    _intersectionMask  (0xffffffff),
    _originPos(0, 0, 0),
    _maxNum(0)
{
    _positionAttitudeTransform = nullptr;
    _geode = nullptr;
    _geometry = nullptr;
    _vertices = nullptr;
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

    _positionAttitudeTransform = new osg::PositionAttitudeTransform();
    _positionAttitudeTransform->setDataVariance(osg::Object::DYNAMIC);
    _positionAttitudeTransform->setPosition(_originPos);
    _positionAttitudeTransform->addChild(_geode.get());
    _group->addChild(_positionAttitudeTransform.get());
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


/*
void
	CvlMeasureToolHandler::setLineStyle( const Style& style )
{
	_feature->style() = style;
	_featureNode->init();
}
*/

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

    if( _vertices->size() > 2 && !_isPath && !_isPoint)
        _geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, (_finished && _lastPointTemporary) ? _vertices->size() - 1 : _vertices->size()));

    if( _vertices->size() > 1  && !_isPoint)
        _geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, (_finished && _lastPointTemporary) ? _vertices->size() - 1 : _vertices->size()));

    _geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, (_finished && _lastPointTemporary) ? _vertices->size() - 1 : _vertices->size()));
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
    }
    else if ( (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == _mouseButtonRight)
              || (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN && ea.getKey() == osgGA::GUIEventAdapter::KEY_Escape))
    {
        if (_gotFirstLocation)
        {
            _gotFirstLocation = false;
            _finished = true;
            //clear();

            rebuildGeometry();
            fireDistanceChanged();
            return false;
        }
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
            /*
            osg::Vec3d pt;
            if (getLocationAt(ea, viewer, pt))
            {
            	_lastPt = pt;
            }
            else
            {
            	_lastPt.set(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
            }

            if( _vertices->size()<=1 )
            {
            	_vertices->clear();
            	if( _lastPt.valid() )
            	{
            		_originPos = pt;
            		if( _positionAttitudeTransform.valid() )
            			_positionAttitudeTransform->setPosition(_originPos);
            		_vertices->push_back(pt - _originPos);
            	}

            	rebuildGeometry();
            }
            else if( _finished )
            {
            	if( _lastPointTemporary )
            		_vertices->back() = pt - _originPos;
            	else
            	{
            		_vertices->push_back(pt - _originPos);
            		_lastPointTemporary = true;
            	}
            	rebuildGeometry();
            }

            fireDistanceChanged();
            aa.requestRedraw();
            */
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

    /*
    if( _geometry!=nullptr )
    {
    	if( _geometry->getNumPrimitiveSets()>0 )
    		_geometry->removePrimitiveSet(0, _geometry->getNumPrimitiveSets());
    }
    */

    if( _vertices != nullptr )
        _vertices->clear();

    _gotFirstLocation = false;
    _lastPointTemporary = false;

    if(!_finished)
        fireDistanceChanged();

    if( m_spText.valid() )
        m_spText->setText(L"");
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
    double distance1 = 0;
    double distance2 = 0;
    double distance = 0;
	double height = 0;
	double disXY = 0;
    if( _vertices.valid() )
    {
        if( _isPath )
        {
            //////////////////////////////////////////////////////////////////////////
            //如果是计算路径长度
            unsigned int size = (_finished && _lastPointTemporary) ? (_vertices->size() - 1) : (_vertices->size());
            for(unsigned int i = 1; i < size; i++ )
            {
                osg::Vec3 vt = _vertices->at(i) - _vertices->at(i - 1);
                if( !_finished && _lastPointTemporary )
                {
                    if( i != size - 1 )
                        distance1 += vt.length();
                    else
                        distance2 = vt.length();
                }
                distance += vt.length();
            }
        }
        else
        {
            //////////////////////////////////////////////////////////////////////////
            //如果是计算面积

            int size = (_finished && _lastPointTemporary) ? (_vertices->size() - 1) : (_vertices->size());
            for(int i = 0; i < size - 2; i++ )
            {
                //已知顶点
                osg::Vec3 p1 = _vertices->at(0);
                osg::Vec3 p2 = _vertices->at(i + 1);
                osg::Vec3 p3 = _vertices->at(i + 2);
                //边向量
                osg::Vec3 vec1 = p2 - p1;;
                osg::Vec3 vec2 = p3 - p1;
                //边长
                float b1 = vec1.length();
                vec1.normalize();
                float b2 = vec2.length();
                vec2.normalize();
                //两边夹角
                float angle = acos(vec1 * vec2);
                //面积
                float area =  b1 * sin(angle) * b2 * 0.5f;

                distance += area;
            }
        }
    }

    if( m_spText.valid() )
    {
        std::wstringstream sstr;
        int prec = std::numeric_limits<double>::digits10;
        sstr.precision(prec);
        if( _lastPt.valid() )
        {
            sstr << L"X: " << RoundOffToString(_lastPt.x(), 4) << std::endl;
            sstr << L"Y: " << RoundOffToString(_lastPt.y(), 4) << std::endl;
            sstr << L"Z: " << RoundOffToString(_lastPt.z(), 4) << std::endl;
        }

        std::wstring app = x3::a2w("PowerMatrix");
        if( distance1 > std::numeric_limits<double>::epsilon() && fabs(distance1 - distance) > std::numeric_limits<double>::epsilon() )
            sstr << GETSTRINGW(app.c_str(), L"PreDistance:", L"前段距离:") << " " << RoundOffToString(distance1, 2) << std::endl;
        if( distance2 > std::numeric_limits<double>::epsilon() && fabs(distance2 - distance) > std::numeric_limits<double>::epsilon() )
            sstr << GETSTRINGW(app.c_str(), L"CurDistance:", L"当前距离:") << " " << RoundOffToString(distance2, 2) << std::endl;
        if( distance > 0 )
            sstr << GETSTRINGW(app.c_str(), L"TotalDistance:", L"总 距 离:") << " " << RoundOffToString(distance, 2) << std::endl;
        m_spText->setText( sstr.str().c_str() );
    }

    for (MeasureToolEventHandlerList::const_iterator i = _eventHandlers.begin(); i != _eventHandlers.end(); ++i)
    {
        i->get()->onDistanceChanged( this, distance, distance1, distance2);
    }
}

void CvlMeasureToolHandler::CreateViewportInfoWin()
{
    m_spViewportInfo = new osg::Camera;
    m_spViewportInfo->setViewMatrix(osg::Matrix::identity());
    m_spViewportInfo->setRenderOrder(osg::Camera::POST_RENDER);
    m_spViewportInfo->setClearMask(GL_DEPTH_BUFFER_BIT);
    m_spViewportInfo->setAllowEventFocus(false);
    m_spViewportInfo->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    m_spViewportInfo->setProjectionMatrixAsOrtho2D(0, 400, 0, 100);
    m_spViewportInfo->setViewport(10, 10, 400, 100);

    osg::ref_ptr<osg::Geode> gnode = new osg::Geode;
    gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    gnode->addDrawable(text);
    text->setDataVariance(osg::Object::DYNAMIC);
    osg::ref_ptr<osgText::Font> font  = osgText::readRefFontFile("fonts/simhei.ttf");
    text->setFont(font);
    //text->setFont("fonts/simhei.ttf");
    float fSize = 18.0f;
    text->setCharacterSize(fSize);
    text->setFontResolution(fSize, fSize);
    text->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    text->setAlignment(osgText::Text::LEFT_BOTTOM);
    text->setText(L"");
// 	text->setBackdropType(osgText::Text::OUTLINE);
// 	text->setBackdropImplementation(osgText::Text::NO_DEPTH_BUFFER);
// 	text->setBackdropColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.5f));
// 	text->setBackdropOffset(0.1);
// 	text->setLineSpacing(0.4);

    text->setPosition(osg::Vec3(10, 10, 0.0));

    m_spText = text.get();

    m_spViewportInfo->addChild(gnode);
}