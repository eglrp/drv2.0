#include "stdafx.h"
#include "vlRefetchBuildingH.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "INodeEvents.h"
#include "x3py/observer/observerobject.h"
#include "osg/Group"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "osg/LineWidth"
#include "osg/osgUtil/PlaneIntersector"
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include "DialogLoadShp.h"
#include <osg/ComputeBoundsVisitor>

using namespace osgEarth::Features;
extern CString g_sShpPath;

CvlRefetchBuildingH::CvlRefetchBuildingH(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"顶高刷新"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"单体化"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"顶高刷新"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"顶高刷新"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"RefetchBuildingH"));
}

CvlRefetchBuildingH::~CvlRefetchBuildingH(void)
{
}

bool CvlRefetchBuildingH::Initialize()
{
	dStep = 1;
	bFindedStart = false;
	bFindedEnd = false;
	dStartHeight = 0;
	dEndHeight = 500;

	return true;
}

bool CvlRefetchBuildingH::UnInitialize()
{
	return true;
}

bool CvlRefetchBuildingH::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	m_spViewer3D = spViewer3D;

	return true;
}

// IUICommand
bool CvlRefetchBuildingH::OnClick()
{
	/*CString strPrompt(GETSTRINGT(L"Survey", _T("RefetchBuildingH"), _T("确定更新Shp文件中的楼顶高度信息么？")));
	if ( AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION ) == IDYES)
	{
	RefetchBuildingH();
	}
	else
	{

	}
	return true;*/
	CDialogLoadShp dlg ;
	if (dlg.DoModal() == IDOK)
	{
		CString strPrompt(GETSTRINGT(L"Survey", _T("RefetchBuildingH"), _T("确定更新Shp文件中的楼顶高度信息么？")));
		if ( AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION ) == IDYES)
		{
			m_sShp = g_sShpPath;
			RefetchBuildingH();
		}
	}
	return true;
}

CString CvlRefetchBuildingH::FindSHPFile()
{
	CStringArray* a = m_spViewer3D->GetLayersArray();

	CString sDefLayer = _T("");
	sDefLayer = m_spViewer3D->GetDefLayer();

	if (sDefLayer == _T(""))
	{
		for (int i = 0; i < a->GetCount(); i++)
		{
			if (a->GetAt(i).Right(4) == L".shp")
			{
				return a->GetAt(i);
			}
		}
		return L"";
	}
	else
	{
		for (int i = 0; i < a->GetCount(); i++)
		{
			if (a->GetAt(i).Find(sDefLayer) > -1)
			{
				return a->GetAt(i);
			}
		}
		return L"";
	}	
}

void CvlRefetchBuildingH::RefetchBuildingH()
{
	USES_CONVERSION;
	osgEarth::Drivers::OGRFeatureOptions ogrOpt;
	
	//m_sShp = FindSHPFile();

	std::string path = W2A(m_sShp);
	//path = "F:\\god\\building_buffer_2m.shp";
	if (path.length() == 0)
	{
		return;
	}

	if (int(path.find('/')) > -1)
	{
		CString sProjPath = m_spViewer3D->GetProjectFilePath();
		CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));
		std::string FileNameEx = path.substr(path.find_last_of( '/' )+1);

		path = T2A(sProjDir + _T("\\") + CString(FileNameEx.c_str()));
	}

	ogrOpt.url() = path;
	ogrOpt.openWrite() = true;

	features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
	features->initialize();
	
	boundNode = dynamic_cast<osg::Node*>(m_spViewer3D->getSceneData()->clone(osg::CopyOp::DEEP_COPY_ALL));
	osg::ComputeBoundsVisitor boundVisitor;
	boundNode->accept(boundVisitor);
	osg::BoundingBox boundingBox = boundVisitor.getBoundingBox();
	dStartHeight = boundingBox.zMin();
	dEndHeight = boundingBox.zMax();
	//dStartHeight = 300;
	//dEndHeight = 500;
	osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();
	osgEarth::Features::Feature* fcopy;
	std::fstream file;
	file.open("D:\\test.txt");
	file<<dStartHeight<<" "<<dEndHeight;
	file.close();
	while(cursor->hasMore())
	{
		osgEarth::Features::Feature* feature = cursor->nextFeature();
		osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);
		FeatureID id = feature->getFID();

		double h = 0;
		while(parts.hasMore())
		{
			osgEarth::Symbology::Geometry* part = parts.next();

			//关键
			m_geoPart = part;

			h = GetBuildingHeight();
			//file<<h<<std::endl;
		}
		feature->set(("TH"), h);

		fcopy =  dynamic_cast<osgEarth::Features::Feature*>(feature->clone(osg::CopyOp::DEEP_COPY_ALL));
		
		features->insertFeature(fcopy);
		features->deleteFeature(id);
		
	}
	AfxMessageBox(_T("刷新完成！"));
}

double CvlRefetchBuildingH::GetBuildingHeight()
{
	double dHeight = 0.0;
	int ds,de;
	ds = dStartHeight;
	de = dEndHeight;

	//从0米开始，每0.5米计算一次范围内的横切点的Z值，一直计算到100米
	//得到200个点，取200个点中的最大值
	double aHeight[1024] = {0};
	int n = 0;

	for (int h = ds; h < de; h = h + dStep)
	{
		int nCrossNum = 0;
		//CString s;
		//s.Format(_T("%d"), h);
		//AfxMessageBox(s);
		if (BuildingInThisHeight(h, nCrossNum))
		{
			aHeight[n] = h;
			/*CString s;
			s.Format(_T("%d"), h);
			AfxMessageBox(s);
			return h;*/
		}
		aPtsInHeight[n] = nCrossNum;
		
		if (!bFindedStart)
		{
			if (nCrossNum > 1000 && dStartHeight < 10)
			{
				dStartHeight = h;
				bFindedStart = true;
			}
		}

		if (!bFindedEnd && bFindedStart)
		{
			if (nCrossNum < 161)
			{
				dEndHeight = h+2;
				bFindedEnd = true;
			}
		}

		n++;
	}

	for (int i = 0; i < 1024; i++)
	{
		if (aHeight[i] > dHeight)
		{
			dHeight = aHeight[i];
		}
	}

	return dHeight;
}

bool CvlRefetchBuildingH::BuildingInThisHeight(int height, int & nCrossNum)
{
	//建筑物的范围坐标
	osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;
	allPoints = m_geoPart->toVec3Array();

	Vec3dVector* vv = new Vec3dVector();
	for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
	{
		osg::Vec3d v(*i);
		vv->push_back(v);
	}

	osgEarth::Symbology::Ring r(vv);

	for (int i = 0; i < vPointsInPerM.size(); i++)
	{
		if (vPointsInPerM.at(i).h == height)
		{
			//交点坐标
			std::vector<osg::Vec3d> intersectPoints = vPointsInPerM.at(i).pts;
			for (int i = 0; i < intersectPoints.size(); i++)
			{
				osg::Vec3d v3d = intersectPoints.at(i);

				if (r.contains2D(v3d.x(), v3d.y()))
				{
					return true;
				}
			}

			return false;
		}
	}

	bool bFind = false;

	osg::Plane plane(osg::Vec4(0, 0, 1, -1 * height));
	osg::ref_ptr<osgUtil::PlaneIntersector> pi = new osgUtil::PlaneIntersector(plane);
	osgUtil::IntersectionVisitor iv(pi);
	m_spViewer3D->getRootNode()->accept(iv);
	if(pi->containsIntersections())
	{
		STMeter m;
		m.h = height;

		for(osgUtil::PlaneIntersector::Intersections::iterator it = pi->getIntersections().begin();
			it != pi->getIntersections().end(); it++)
		{
			//交点坐标
			std::vector<osg::Vec3d> intersectPoints = it->polyline;

			
			for (int i = 0; i < intersectPoints.size(); i++)
			{
				nCrossNum++;

				osg::Vec3d v3d = intersectPoints.at(i);
				osg::Vec3d v3dNew = it->matrix->preMult(v3d);
				m.pts.insert(m.pts.end(),v3dNew);
				
				if (r.contains2D(v3dNew.x(), v3dNew.y()))
				{
					bFind = true;
				}
			}
		}
		vPointsInPerM.insert(vPointsInPerM.end(), m);
	}

	return bFind;
}