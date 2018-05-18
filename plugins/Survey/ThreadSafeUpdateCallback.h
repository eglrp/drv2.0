#pragma once
#include "EarthMatrixExports.h"
#include "osg/NodeCallback"
#include "osg/Group"
#include <map>

BEGIN_NAMESPACE_EARTHMATRIX

class CThreadSafeUpdateCallback : public osg::NodeCallback
{
public:
	CThreadSafeUpdateCallback(void);
	~CThreadSafeUpdateCallback(void);

public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

public:
	bool AddChild(osg::Node* parent, osg::Node* node);
	bool RemoveChild(osg::Node* parent, osg::Node* node);

protected:
	typedef std::map<long, osg::ref_ptr<osg::Group>> MAP_NODE;
	MAP_NODE m_mapAddChilds;
	MAP_NODE m_mapRemoveChilds;
	OpenThreads::Mutex _addMutex;
	OpenThreads::Mutex _removeMutex;
};

END_NAMESPACE_EARTHMATRIX


