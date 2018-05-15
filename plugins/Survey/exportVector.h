#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"
#include <vector>
#include "osg\NodeVisitor"

using namespace osgEarth::Util;
using namespace std;
USING_NAMESPACE_EARTHMATRIX

class CFindVertex:public osg::NodeVisitor
{
public:
	CFindVertex() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) 
	{
	}
	virtual void apply(osg::Geometry& geom)
	{
		osg::Vec3Array* verts = dynamic_cast<osg::Vec3Array*>(geom.getVertexArray());
		if (verts)
		{
			vector<osg::Vec3d> vec;
			for (size_t i = 0;i<verts->size();++i)
			{
				vec.push_back(verts->at(i));
			}
			mvecVertex.push_back(vec);
		}
		
		traverse(geom);
	}
	std::vector<vector<osg::Vec3d>> mvecVertex;
};

const char* const clsidExportVector = "00000000-4900-1000-0000-000000000005";

class CExportVector : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CExportVector, clsidExportVector)
		X3DEFINE_INTERFACE_ENTRY(IUICommon)
		X3DEFINE_INTERFACE_ENTRY(IUICommand)
		X3END_CLASS_DECLARE()

public:
	CExportVector(void);
	~CExportVector(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	x3::Object<IViewer3D> m_spViewer3D;

	// IUICommand
	virtual bool Checked();
	virtual bool OnClick();
};