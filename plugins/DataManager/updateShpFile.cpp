#include "stdafx.h"
#include "updateShpFile.h"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "osgDB/ReadFile"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "DataManager.h"
extern CDataManager* m_pDM;
using namespace osgEarth::Features;
void CUpdateShpFile::FindSHPFile()
{
	if (!m_pDM->m_spViewer3D.valid())
	{
		return;
	}
	vecShp.clear();
	CStringArray* a = m_pDM->m_spViewer3D->GetLayersArray();

	for (int i = 0; i < a->GetCount(); i++)
	{
		if (a->GetAt(i).Right(4) == L".shp" || a->GetAt(i).Right(4) == L".SHP")
		{
			vecShp.push_back(a->GetAt(i));
		}
	}
}

void CUpdateShpFile::refetchShp(std::string sfield,int inout,int maxAge,int familyMem)
{
	USES_CONVERSION;
	FindSHPFile();
	if (vecShp.empty())
	{
		return;
	}
	for (int i = 0;i<vecShp.size();++i)
	{
		std::string path = W2A(vecShp[i]);
		if (path.length() == 0)
		{
			return;
		}
		osgEarth::Drivers::OGRFeatureOptions ogrOpt;
		ogrOpt.url() = path;
		ogrOpt.openWrite() = true;
		features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
		features->initialize();
		osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();
		while(cursor->hasMore())
		{
			osgEarth::Features::Feature* feature = cursor->nextFeature();
			osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
			FeatureID id = feature->getFID();

			if (feature->getString("完整房间号") == sfield)
			{
				if (inout != -1)
					feature->set(("刷卡次数"), inout);
				if (maxAge != -1)
					feature->set(("最大年龄"), maxAge);
				if (familyMem != -1)
					feature->set(("家庭成员数"), familyMem);
				osgEarth::Features::Feature* fcopy = new Feature(*feature);
				if(features->insertFeature(fcopy))
					features->deleteFeature(id);
				delete fcopy;
				return;
			}
		}
	}
}