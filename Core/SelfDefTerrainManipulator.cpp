#include "stdafx.h"
#include "SelfDefTerrainManipulator.h"
#include "osgViewer/Viewer"
#include "Viewer3D.h"
bool CSelfDefTerrainManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	if (ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK)
	{
		osg::Vec3d pos;
		getPos(ea,aa,pos);
		if (pos != osg::Vec3d(0,0,0))
		{	
			osg::Vec3d eye;
			osg::Vec3d center;
			osg::Vec3d up;

			getTransformation(eye, center, up);		
			osg::Vec3d newEye = eye - center;
			newEye.normalize();
			newEye *= 100;
			newEye += pos;
			setTransformation(newEye, pos, up);
		}
		return false;
	}
	
	return StandardManipulator::handle(ea,aa);
}


void CSelfDefTerrainManipulator::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
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
	}
}

bool CSelfDefTerrainManipulator::handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	if( ea.getKey() == osgGA::GUIEventAdapter::KEY_Space )
	{
		return true;
	}

	return false;
}

bool CSelfDefTerrainManipulator::handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	osg::Vec3d eye,center,up;  
	getTransformation(eye,center,up); 
	if (H != -1 && eye.z()<H)
	{
		eye.z() = H;
		setTransformation(eye,center,up); 
	}

	osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

	// handle centering
	if( _flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT )
	{

		if( ((sm == osgGA::GUIEventAdapter::SCROLL_DOWN && _wheelZoomFactor > 0.)) ||
			((sm == osgGA::GUIEventAdapter::SCROLL_UP   && _wheelZoomFactor < 0.)) )
		{

			if( getAnimationTime() <= 0. )
			{
				// center by mouse intersection (no animation)
				setCenterByMousePointerIntersection( ea, us );
			}
			else
			{
				// start new animation only if there is no animation in progress
				if( !isAnimating() )
					startAnimationByMousePointerIntersection( ea, us );

			}

		}
	}

	switch( sm )
	{
		// mouse scroll up event
	case osgGA::GUIEventAdapter::SCROLL_UP:
		{
			// perform zoom
			zoomModel( _wheelZoomFactor, true );
			us.requestRedraw();
			us.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// mouse scroll down event
	case osgGA::GUIEventAdapter::SCROLL_DOWN:
		{
			// perform zoom
			zoomModel( -_wheelZoomFactor, true );
			us.requestRedraw();
			us.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// unhandled mouse scrolling motion
	default:
		return false;
	}
}

bool CSelfDefTerrainManipulator::performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy)
{
	// pan model.
	double scale = -0.3f * _distance * getThrowScale( eventTimeDelta );

	osg::Matrixd rotation_matrix;
	rotation_matrix.makeRotate(_rotation);

	// compute look vector.
	osg::Vec3d sideVector = getSideVector(rotation_matrix);

	// CoordinateFrame coordinateFrame = getCoordinateFrame(_center);
	// Vec3d localUp = getUpVector(coordinateFrame);
	osg::Vec3d localUp = _previousUp;

	osg::Vec3d forwardVector = localUp ^ sideVector;
	sideVector = forwardVector^localUp;

	forwardVector.normalize();
	sideVector.normalize();

	osg::Vec3d dv = forwardVector * (dy*scale) + sideVector * (dx*scale);

	_center += dv;

	// need to recompute the intersection point along the look vector.

	bool hitFound = false;

	if (_node.valid())
	{
		// now reorientate the coordinate frame to the frame coords.
		osg::CoordinateFrame coordinateFrame = getCoordinateFrame(_center);

		// need to reintersect with the terrain
		double distance = _node->getBound().radius() * 0.25f;


		coordinateFrame = getCoordinateFrame(_center);
		osg::Vec3d new_localUp = getUpVector(coordinateFrame);

		osg::Quat pan_rotation;
		pan_rotation.makeRotate(localUp, new_localUp);

		if (!pan_rotation.zeroRotation())
		{
			_rotation = _rotation * pan_rotation;
			_previousUp = new_localUp;
			//OSG_NOTICE<<"Rotating from "<<localUp<<" to "<<new_localUp<<"  angle = "<<acos(localUp*new_localUp/(localUp.length()*new_localUp.length()))<<std::endl;

			//clampOrientation();
		}
		else
		{
			OSG_INFO<<"New up orientation nearly inline - no need to rotate"<<std::endl;
		}
	}

	return true;
}

void CSelfDefTerrainManipulator::setHeightLimit(int h)
{
	if(h<0 && h != -1)
	{
		return;
	}
	H = h;
}

bool CSelfDefTerrainManipulator::subSetCenterByMousePointerIntersection(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	osg::View* view = us.asView();
	if (!view)
		return false;

	osg::Camera *camera = view->getCamera();
	if (!camera)
		return false;


	// prepare variables
	float x = (ea.getX() - ea.getXmin()) / (ea.getXmax() - ea.getXmin());
	float y = (ea.getY() - ea.getYmin()) / (ea.getYmax() - ea.getYmin());
	osgUtil::LineSegmentIntersector::CoordinateFrame cf;
	osg::Viewport *vp = camera->getViewport();
	if (vp) {
		cf = osgUtil::Intersector::WINDOW;
		x *= vp->width();
		y *= vp->height();

		//根据屏幕坐标设置碰撞的视点区域
		x = vp->width() / 2;
		y = vp->height() / 2;
	}
	else
		cf = osgUtil::Intersector::PROJECTION;

	// perform intersection computation
	osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(cf, x, y);
	osgUtil::IntersectionVisitor iv(picker.get());
	camera->accept(iv);

	// return on no intersections
	if (!picker->containsIntersections())
		return false;

	// get all intersections
	osgUtil::LineSegmentIntersector::Intersections& intersections = picker->getIntersections();

	// get current transformation
	osg::Vec3d eye, oldCenter, up;
	getTransformation(eye, oldCenter, up);

	// new center
	osg::Vec3d newCenter = (*intersections.begin()).getWorldIntersectPoint();

	// make vertical axis correction
	if (getVerticalAxisFixed())
	{

		osg::CoordinateFrame coordinateFrame = getCoordinateFrame(newCenter);
		osg::Vec3d localUp = getUpVector(coordinateFrame);

		fixVerticalAxis(newCenter - eye, up, up, localUp, true);

	}

	// set the new center
	setTransformation(eye, newCenter, up);


	// warp pointer
	// note: this works for me on standard camera on GraphicsWindowEmbedded and Qt,
	//       while it was necessary to implement requestWarpPointer like follows:
	//
	// void QOSGWidget::requestWarpPointer( float x, float y )
	// {
	//    osgViewer::Viewer::requestWarpPointer( x, y );
	//    QCursor::setPos( this->mapToGlobal( QPoint( int( x+.5f ), int( y+.5f ) ) ) );
	// }
	//
	// Additions of .5f are just for the purpose of rounding.
	centerMousePointer(ea, us);

	return true;
}