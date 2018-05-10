#pragma once
#include "osg/AnimationPath"
#include "osgGA/AnimationPathManipulator"

class CSelfAnimationPathManipulator : public osgGA::AnimationPathManipulator
{
public:
	CSelfAnimationPathManipulator( osg::AnimationPath* animationPath=0 ):AnimationPathManipulator(animationPath)
	{

	}
	bool isPause(){return _isPaused;}
};
