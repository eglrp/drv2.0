#include "StdAfx.h"
#include "NodeVisitor.h"

CNodeVisitor::~CNodeVisitor(void)
{
}

void CNodeVisitor::apply(osg::Node &node)
{
	CString sName = CString(node.getName().c_str());
	//if(sName.Find(sNameFind) > -1)
	if( sName == sNameFind )
	{
		//pHLH->clear();
		//pHLH->add(node.getOrCreateStateSet());
	}
	else
		traverse(node);
}
void CNodeVisitor::setGroup(osg::Group *gp)
{
	group = gp;
}

void CNodeVisitor::setName(CString name)
{
	sNameFind = name;
}