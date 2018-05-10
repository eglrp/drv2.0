#pragma once
#include "EarthMatrixExports.h"
#include "x3py/module/plugininc.h"
#include "x3py/observer/fireeventex.h"
#include "x3py/observer/fireobjeventex.h"
#include "x3py/observer/observerobject.h"
#include "x3py/objptr.h"
#include "osg/Node"

BEGIN_NAMESPACE_EARTHMATRIX

#ifndef NODE_EVENT_NAMESPACE
#define NODE_EVENT_NAMESPACE "EarthMatrixNodeEvent"
#endif

X3DEFINE_OBJEVENT_4(EventNodeAdd, x3::IObject*, osg::Node*, osg::Node*, std::string, NODE_EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_3(EventNodeRemove, x3::IObject*, osg::Node*, osg::Node*, NODE_EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_2(EventNodeChanged, x3::IObject*, osg::Node*, NODE_EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_2(EventNodeSelected, x3::IObject*, osg::Node*, NODE_EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_1(EventProjectOpen, std::string, NODE_EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_1(EventPathActive, std::string, NODE_EVENT_NAMESPACE);
X3DEFINE_OBJEVENT_1(EventSetDefLayer, std::string, NODE_EVENT_NAMESPACE);
END_NAMESPACE_EARTHMATRIX