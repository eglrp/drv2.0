#include "stdafx.h"
#include "vlPlayVideo.h"
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

CvlPlayVideo::CvlPlayVideo(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"播放视频"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"在线视频管理"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"播放视频"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"播放视频"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"PlayVideo"));
}

CvlPlayVideo::~CvlPlayVideo(void)
{
}

bool CvlPlayVideo::Initialize()
{
	bInActive = false;
    return true;
}

bool CvlPlayVideo::UnInitialize()
{
    return true;
}

bool CvlPlayVideo::SetBuddy(x3::IObject* val)
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
bool CvlPlayVideo::OnClick()
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


bool CvlPlayVideo::Activate()
{
	USES_CONVERSION;

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	if (m_spMovieHandler == nullptr)
	{
		m_spMovieHandler = new MovieEventHandler();
		m_spMovieHandler->setMouseTracking(false);
		m_spMovieHandler->addNode(spViewer3D->getRootNode());
		m_spMovieHandler->play();
		spViewer3D->getViewer()->addEventHandler( m_spMovieHandler );
	}

	spViewer3D->Unlock();

	bInActive = true;

	return true;
}

bool CvlPlayVideo::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	spViewer3D->getViewer()->removeEventHandler(m_spMovieHandler.get());
	m_spMovieHandler = nullptr;


	bInActive = false;

	spViewer3D->Unlock();

	return true;
}

bool CvlPlayVideo::Checked()
{
	return bInActive;
}