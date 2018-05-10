#pragma once
#include <osgViewer/Viewer>
class FlyViewPointHandler :
	public osgGA::GUIEventHandler
{
public:
	FlyViewPointHandler(void);
	~FlyViewPointHandler(void);

	virtual void getUsage(osg::ApplicationUsage &usage) const;
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);


};

