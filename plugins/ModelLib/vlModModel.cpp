#include "stdafx.h"
#include "vlModModel.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

CvlModModel::CvlModModel(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"修改模型"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"在线视频管理"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"修改模型"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"修改模型"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"ModModel")); 
}

CvlModModel::~CvlModModel(void)
{
}

bool CvlModModel::Initialize()
{
	return true;
}

bool CvlModModel::UnInitialize()
{
	return true;
}

bool CvlModModel::SetBuddy(x3::IObject* val)
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
bool CvlModModel::OnClick()
{
	return false;
}

bool CvlModModel::Checked()
{
	return false;
}