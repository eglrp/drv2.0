#include "stdafx.h"
#include "vlLinetSurvey.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

CvlLineSurvey::CvlLineSurvey(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"线测量"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"量测工具"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"线测量"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"线测量"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"LineSurvey")); 

	isTestJu = false;
	jieDuan = 0;
	vectex = new osg::Vec3Array;
}

CvlLineSurvey::~CvlLineSurvey(void)
{
}

bool CvlLineSurvey::Initialize()
{
	return true;
}

bool CvlLineSurvey::UnInitialize()
{
	return true;
}

bool CvlLineSurvey::SetBuddy(x3::IObject* val)
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
bool CvlLineSurvey::OnClick()
{

	return true;
}