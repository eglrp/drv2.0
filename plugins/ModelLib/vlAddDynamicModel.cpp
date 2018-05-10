#include "stdafx.h"
#include "vlAddDynamicModel.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/LineWidth>

std::string fnDyModel;

CvlAddDynamicModel::CvlAddDynamicModel(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"动态地物"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"在线视频管理"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"动态地物"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"动态地物"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"AddDynamicModel"));
}

CvlAddDynamicModel::~CvlAddDynamicModel(void)
{
}

bool CvlAddDynamicModel::Initialize()
{
	bInActive = false;
    return true;
}

bool CvlAddDynamicModel::UnInitialize()
{
    return true;
}

bool CvlAddDynamicModel::SetBuddy(x3::IObject* val)
{
    x3::Object<IViewer3D> spViewer3D(val);
    if( !spViewer3D.valid() )
        return false;

    if( m_spBuddy == spViewer3D )
        return true;

    m_spBuddy = spViewer3D;
    if( spViewer3D->getViewer() == nullptr )
        return false;

    return true;
}

// IUICommand
bool CvlAddDynamicModel::OnClick()
{
	if (bInActive)
	{
		Deactivate();
	}
	else
	{
		Activate();
	}
	return true;
}


bool CvlAddDynamicModel::Activate()
{
	USES_CONVERSION;

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	//////////////////////////////////////////////////////////////////////////
	CString strFilter(GETSTRINGT(A2CT(_getClassName()), _T("FilterModel"), _T("模型文件(*.osg)|*.osg")));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal()!=IDOK )
	{
		return false;
	}

	fnDyModel = T2A(dlg.GetPathName());
	if( fnDyModel.size()<=0 )
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	spViewer3D->Lock();

	m_spMeasureEventHandler = new CvlDynamicPathHandler(spViewer3D);
	m_spMeasureEventHandler->setIntersectionMask( 0x1 );

	spViewer3D->getViewer()->addEventHandler(m_spMeasureEventHandler.get());

	spViewer3D->Unlock();

	bInActive = true;

	return true;
}

bool CvlAddDynamicModel::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	spViewer3D->getViewer()->removeEventHandler(m_spMeasureEventHandler.get());
	m_spMeasureEventHandler = nullptr;


	bInActive = false;

	spViewer3D->Unlock();

	return true;
}

bool CvlAddDynamicModel::Checked()
{
	return bInActive;
}