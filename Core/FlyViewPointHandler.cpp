#include "StdAfx.h"
#include <osgViewer/Viewer>
#include "FlyViewPointHandler.h"
#include <osgEarthUtil/EarthManipulator>
using namespace osgEarth::Util;

using namespace osgEarth;

extern ST_VIEWPOINT gViewPoint;
extern osg::ref_ptr<EarthManipulator> gEarthManipulator;




void flyToViewpoint(EarthManipulator* manip, const Viewpoint& vp)
{
	Viewpoint currentVP = manip->getViewpoint();
	double distance = currentVP.focalPoint()->distanceTo(vp.focalPoint().get());
	double duration = osg::clampBetween(distance / VP_METERS_PER_SECOND, VP_MIN_DURATION, VP_MAX_DURATION);
	manip->setViewpoint( vp, duration );
}

FlyViewPointHandler::FlyViewPointHandler(void)
{
}


FlyViewPointHandler::~FlyViewPointHandler(void)
{
}

void FlyViewPointHandler::getUsage(osg::ApplicationUsage &usage) const
{

}

bool FlyViewPointHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	if (!gViewPoint.bDone)
	{
		flyToViewpoint(gEarthManipulator.get(), Viewpoint("XXX",
			gViewPoint.x, gViewPoint.y, 50,   // longitude, latitude, altitude
			0, -60, 1999) // heading, pitch, range 
			); 
		gViewPoint.bDone = true;
	}
	return false;
}