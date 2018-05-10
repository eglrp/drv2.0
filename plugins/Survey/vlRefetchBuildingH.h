#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"
#include <osgEarthSymbology/Geometry>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>

USING_NAMESPACE_EARTHMATRIX

typedef struct
{
	int h;
	std::vector<osg::Vec3d> pts;
}STMeter;

const char* const clsidvlRefetchBuildingH = "00000000-0000-0000-0040-000000000080";
class CvlRefetchBuildingH : public IUICommon, public IUICommand
{
	X3BEGIN_CLASS_DECLARE(CvlRefetchBuildingH, clsidvlRefetchBuildingH)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
	X3END_CLASS_DECLARE()

public:
	CvlRefetchBuildingH(void);
	~CvlRefetchBuildingH(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	void RefetchBuildingH();

	// IUICommand
	virtual bool OnClick();

	CString FindSHPFile();

	double GetBuildingHeight();
	bool BuildingInThisHeight(int height, int & nCrossNum);

	osg::ref_ptr<osgEarth::Features::FeatureSource> features;
	osg::ref_ptr<osgEarth::Symbology::Geometry> m_geoPart;
	std::vector<osg::ref_ptr<osgEarth::Symbology::Geometry>> v;
	std::vector<osg::ref_ptr<osgEarth::Features::Feature>> vFeature;
	std::vector<STMeter> vPointsInPerM;

	int dStep;
	int dStartHeight;
	int dEndHeight;
	bool bFindedStart;
	bool bFindedEnd;
	int aPtsInHeight[1024];
	CString m_sShp;

	osg::Node* boundNode;
	x3::Object<IViewer3D> m_spViewer3D;
};

