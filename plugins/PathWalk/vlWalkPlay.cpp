#include "stdafx.h"
#include "vlWalkPlay.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "osgDB/FileUtils"
#include "osg/AnimationPath"

CvlWalkPlay::CvlWalkPlay(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"巡航"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"辖区巡航"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"开始巡航"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"开始巡航，按P键暂停，左/右小括号键减速/加速"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"Walk")); 

	bInPlay = false;

	registerHandlers();
}

CvlWalkPlay::~CvlWalkPlay(void)
{
}

bool CvlWalkPlay::Initialize()
{
	return true;
}

bool CvlWalkPlay::UnInitialize()
{
	return true;
}

bool CvlWalkPlay::SetBuddy(x3::IObject* val)
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
	return true;
}

// IUICommand
bool CvlWalkPlay::OnClick()
{
	if (bInPlay)
	{
		Deactivate();
		x3::Object<IViewer3D> spViewer3D(m_spBuddy);
		spViewer3D->SetViewport(m_sViewport);
	}
	else
	{
		x3::Object<IViewer3D> spViewer3D(m_spBuddy);
		m_sViewport = spViewer3D->GetViewportPos();
		Activate();
	}
	return true;
}

bool CvlWalkPlay::Checked()
{
	return bInPlay;
}

bool CvlWalkPlay::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	std::string pathc = spViewer3D->GetRecordPath() + ".path";

	if (osgDB::fileExists(pathc))
	{
		//spViewer3D->Lock();

		osg::ref_ptr<osg::AnimationPath> spPath = new osg::AnimationPath();
		std::ifstream in(pathc);
		spPath->read(in);
		
		m_spKeyswitchManipulator = new CSelfKeySwitchMatrixManipulator();
		_spAnimationPathManipulator = new CSelfAnimationPathManipulator(spPath.get());
		_oldManipulator = spViewer3D->getViewer()->getCameraManipulator();

		m_spKeyswitchManipulator->addMatrixManipulator(0,"0",_spAnimationPathManipulator);
		m_spKeyswitchManipulator->addMatrixManipulator(1,"1",_oldManipulator);

		m_spKeyswitchManipulator->selectMatrixManipulator(0);
		//2016年10月14日：这里不需要再添加Handler了，设置完操作器就行了
		//spViewer3D->getViewer()->addEventHandler(_spAnimationPathManipulator.get());

		if (m_spKeyswitchManipulator != NULL)
		{
			//_oldManipulator = spViewer3D->getViewer()->getCameraManipulator();
			spViewer3D->getViewer()->setCameraManipulator(m_spKeyswitchManipulator.get());
		}

		//spViewer3D->Unlock();
	}

	bInPlay = true;
	return true;
}

bool CvlWalkPlay::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();
	//spViewer3D->getViewer()->removeEventHandler(_spAnimationPathManipulator.get());
	spViewer3D->getViewer()->setCameraManipulator(_oldManipulator.get());
	spViewer3D->Unlock();

	bInPlay = false;
	return true;
}

bool CvlWalkPlay::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
{
	if (eventKey == "PATHACTIVE")
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