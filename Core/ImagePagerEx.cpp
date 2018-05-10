#include "ImagePagerEx.h"
#include "osgDB/Registry"


ImagePagerEx::ImagePagerEx(void)
{
// 	_imageThreads.push_back(new ImageThread(this, ImageThread::HANDLE_ALL_REQUESTS, "Image Thread 4"));
// 	_imageThreads.push_back(new ImageThread(this, ImageThread::HANDLE_ALL_REQUESTS, "Image Thread 5"));
// 	_imageThreads.push_back(new ImageThread(this, ImageThread::HANDLE_ALL_REQUESTS, "Image Thread 6"));
}


ImagePagerEx::~ImagePagerEx(void)
{
}

void ImagePagerEx::requestImageFile(const std::string& fileName, osg::Object* attachmentPoint, int attachmentIndex, double timeToMergeBy, const osg::FrameStamp* framestamp, osg::ref_ptr<osg::Referenced>& imageRequest, const osg::Referenced* options)
{
	osgDB::Options* readOptions = dynamic_cast<osgDB::Options*>(const_cast<osg::Referenced*>(options));
	if (!readOptions)
	{
		readOptions = osgDB::Registry::instance()->getOptions();
	}

	bool alreadyAssigned = dynamic_cast<osgDB::ImagePager::ImageRequest*>(imageRequest.get()) && (imageRequest->referenceCount()>1);
	if (alreadyAssigned)
	{				
		osg::ref_ptr<osgDB::ImagePager::ImageRequest> request = dynamic_cast<osgDB::ImagePager::ImageRequest*>(imageRequest.get());
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readQueue->_requestMutex);
		request->_timeToMergeBy = framestamp ? framestamp->getReferenceTime() : timeToMergeBy;
		return;
	}

	osgDB::ImagePager::requestImageFile(fileName, attachmentPoint, attachmentIndex, timeToMergeBy, framestamp, imageRequest, options);
}

void ImagePagerEx::updateSceneGraph(const osg::FrameStamp &frameStamp)
{
	removeExpiredSubgraphs(frameStamp);

	osgDB::ImagePager::updateSceneGraph(frameStamp);
}

void ImagePagerEx::removeExpiredSubgraphs(const osg::FrameStamp &frameStamp)
{	
	if (frameStamp.getFrameNumber()==0)
	{
		// No need to remove anything on first frame.
		return;
	}
	
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readQueue->_requestMutex);

	if (!_readQueue->_requestList.empty())
	{
		for( RequestQueue::RequestList::reverse_iterator it = _readQueue->_requestList.rbegin(); it != _readQueue->_requestList.rend();)
		{
			ImageRequest* imageRequest = it->get();
			if( imageRequest->_timeToMergeBy<frameStamp.getReferenceTime() )		
				it = RequestQueue::RequestList::reverse_iterator(_readQueue->_requestList.erase((++it).base()));		
			else
				++it;
		}

		_readQueue->updateBlock();
	}
	
}