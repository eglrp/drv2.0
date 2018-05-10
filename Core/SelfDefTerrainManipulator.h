#pragma once
#include "osgGA/TerrainManipulator"
#include "osg/Node"

class CSelfDefTerrainManipulator : public osgGA::TerrainManipulator
{
public:
	CSelfDefTerrainManipulator(int h = -1)
	{
		H = h;
	}
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual bool handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual bool subSetCenterByMousePointerIntersection(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	void getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos);

	bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);
public:
	void setHeightLimit(int h);

private:
	int H;
};

