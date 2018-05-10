#include "stdafx.h"
#include "vlOpenFileDem.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "x3py/manager/iworkpath.h"
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>

using namespace osg;
using namespace osgDB;

CvlOpenFileDem::CvlOpenFileDem(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"打开"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"文件"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"打开文件"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"打开文件"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"Open"));
}

CvlOpenFileDem::~CvlOpenFileDem(void)
{
}

bool CvlOpenFileDem::Initialize()
{
	supportsEvent("PROJECTOPEN");
	return true;
}

bool CvlOpenFileDem::UnInitialize()
{
	return true;
}

bool CvlOpenFileDem::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	Enable(false);

	registerHandlers();

	return true;
}

// IUICommand
bool CvlOpenFileDem::OnClick()
{
	USES_CONVERSION;
	IUICommand::Enable(false);
	Checked(true);
	
	CString strFilter(GETSTRINGT(A2CT(_getClassName()), _T("FilterDem"), _T("数字高程模型(*.tif)|*.tif")));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal()!=IDOK )
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}
	
	std::string filename = T2A(dlg.GetPathName());
	if( filename.size()<=0 )
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

	std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
	std::string FileNameEx = filename.substr(filename.find_last_of( '\\' )+1);
	std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));
	osgDB::Registry::instance()->setDataFilePathList( FilePath );

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

#ifdef _2D
	osg::HeightField* hf = readHeightFieldFile(filename.c_str());
	if (hf)
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(new osg::ShapeDrawable(hf));
		geode->setName(FileNameNoEx);
		spViewer3D->AddNode(geode.get(),nullptr);
		spViewer3D->GetLayersArray()->Add(dlg.GetPathName());
	}
	else
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

#else
	spViewer3D->OpenFile(filename, nullptr);
#endif // _2D
	/*

	std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
	std::string FileNameEx = filename.substr(filename.find_last_of( '\\' )+1);
	std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));
	osgDB::Registry::instance()->setDataFilePathList( FilePath );

	setlocale( LC_ALL, "chs" );

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	osg::ref_ptr<osg::Node> spNode = osgDB::readNodeFile(filename);
	setlocale( LC_ALL, "C" );

	spNode->setName(FileNameNoEx);

	if( !spNode.valid() )
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	const osg::BoundingSphere& bs = spNode->getBound();
	if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

	osg::Vec3d v3f = bs.center();
	osg::Vec3d v3dOut;

	//double lat,lon,h;

	const SpatialReference* epsg2382;
	std::string ProjFile = FilePath + "\\" + FileNameNoEx + ".prj";
	
	if (osgDB::fileExists(ProjFile))
	{
		std::string sContent = spViewer3D->GetFileContent(ProjFile);
		epsg2382 = SpatialReference::create(sContent);

	}
	else
		epsg2382 = SpatialReference::create("wgs84");
	const SpatialReference* wgs84 = spViewer3D->getMapNode()->getMapSRS();
	epsg2382->transform(v3f, wgs84, v3dOut);

	osg::Vec3d location = osg::Vec3d(v3dOut.x(), v3dOut.y(), v3dOut.z());    

	//////////////////////////////////////////////////////////////////////////
	osgEarth::Drivers::SimpleModelOptions simpleModelOptions;
	simpleModelOptions.url() = osgEarth::URI(filename);
	simpleModelOptions.location() = location;
	simpleModelOptions.node() = spNode.get();
	osgEarth::ModelLayerOptions layerOptions(FileNameNoEx, simpleModelOptions);    
	osg::ref_ptr<osgEarth::ModelLayer> mLayer = new osgEarth::ModelLayer(layerOptions);
	spViewer3D->getMapNode()->getMap()->addModelLayer(mLayer.get());
	//////////////////////////////////////////////////////////////////////////

	spViewer3D->FlyToViewport(v3dOut.x(), v3dOut.y(), v3dOut.z());
	*/

	Checked(false);
	IUICommand::Enable(true);
	return true;
}

bool CvlOpenFileDem::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
{
	if (eventKey == "PROJECTOPEN")
	{
		if (filename.length() > 0)
		{
			Enable(true);
		}
		else
			Enable(false);
	}
	return true;
}