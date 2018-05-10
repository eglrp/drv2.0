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

	bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);
public:
	void setHeightLimit(int h);

private:
	int H;
};

