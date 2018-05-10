#include "stdafx.h"
#include "SelfDefTerrainManipulator.h"

bool CSelfDefTerrainManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	osg::Vec3d eye,center,up;  
	getTransformation(eye,center,up); 
	if (H != -1 && eye.z()<H)
	{
		eye.z() = H;
		setTransformation(eye,center,up); 
	}
	return StandardManipulator::handle(ea,us);

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