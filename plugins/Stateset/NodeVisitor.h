#pragma once
#include <osgViewer/View>

class CNodeVisitor :
	public osg::NodeVisitor
{
public:
	virtual void CNodeVisitor::apply(osg::Node &node);

	void CNodeVisitor::setGroup(osg::Group *gp);

	void CNodeVisitor::setName(CString name);

public:
	CNodeVisitor():osg::NodeVisitor(TRAVERSE_ACTIVE_CHILDREN){}
	~CNodeVisitor(void);

private:
	osg::ref_ptr<osg::Group> group;
	CString sNameFind;
};

