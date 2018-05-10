#include "stdafx.h"
#include "vlPOISearch.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

CvlPOISearch::CvlPOISearch(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"地名搜索"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"量测工具"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"地名搜索"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"地名搜索"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"POISearch")); 
}

CvlPOISearch::~CvlPOISearch(void)
{
}

bool CvlPOISearch::Initialize()
{
	return true;
}

bool CvlPOISearch::UnInitialize()
{
	return true;
}

bool CvlPOISearch::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	return true;
}

// IUICommand
bool CvlPOISearch::OnClick()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->FlyToViewport(117.118, 39.092, 2000);
	spViewer3D->FlyToViewport(114.43502037, 30.43188697 , 2000);
	return true;
}

