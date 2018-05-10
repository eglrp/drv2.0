#pragma once
#include "osgDB/ImagePager"

class ImagePagerEx : public osgDB::ImagePager
{
public:
	ImagePagerEx(void);
	~ImagePagerEx(void);

public:
	virtual void requestImageFile(const std::string& fileName, osg::Object* attachmentPoint, int attachmentIndex, double timeToMergeBy, const osg::FrameStamp* framestamp, osg::ref_ptr<osg::Referenced>& imageRequest, const osg::Referenced* options);
	virtual void updateSceneGraph(const osg::FrameStamp &frameStamp);
	virtual void removeExpiredSubgraphs(const osg::FrameStamp &frameStamp);
};

