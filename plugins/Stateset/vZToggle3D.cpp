#include "stdafx.h"
#include "vZToggle3D.h"
#include "EarthMatrixFunctions.h"
#include "IViewer3D.h"

osg::DisplaySettings::StereoMode stereoMode[] =
{
    osg::DisplaySettings::QUAD_BUFFER,
    osg::DisplaySettings::ANAGLYPHIC,
    osg::DisplaySettings::HORIZONTAL_SPLIT,
    osg::DisplaySettings::VERTICAL_SPLIT,
    osg::DisplaySettings::LEFT_EYE,
    osg::DisplaySettings::RIGHT_EYE,
    osg::DisplaySettings::HORIZONTAL_INTERLACE,
    osg::DisplaySettings::VERTICAL_INTERLACE
};

osg::DisplaySettings::DisplayType displayType[] =
{
    osg::DisplaySettings::MONITOR,
    osg::DisplaySettings::POWERWALL,
    osg::DisplaySettings::REALITY_CENTER,
    osg::DisplaySettings::HEAD_MOUNTED_DISPLAY
};

CvZToggle3DHandler::CvZToggle3DHandler(void)
{

}

CvZToggle3DHandler::CvZToggle3DHandler(osg::DisplaySettings* pds)
{
    if (!pds)
    {
        pds = new osg::DisplaySettings;
    }
    _pds = pds;

	_stereo = true;
	_stereoMode = 0;
	_displayType = 0;
	_eyeSeperation = 0.05;
    _pds->setStereo(true);
    _pds->setEyeSeparation(_eyeSeperation);
}

CvZToggle3DHandler::~CvZToggle3DHandler(void)
{
}

bool CvZToggle3DHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
    if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
    {
        switch(ea.getKey())
        {
        case osgGA::GUIEventAdapter::KEY_F1:
            _stereo = !_stereo;
            _pds->setStereo(_stereo);
            break;
        case osgGA::GUIEventAdapter::KEY_Left:
            if (_stereoMode == 0)
                _stereoMode = 6;
            else
                _stereoMode--;
            _pds->setStereoMode(stereoMode[_stereoMode]);
            us.requestContinuousUpdate();
            break;
        case osgGA::GUIEventAdapter::KEY_Right:
            if (_stereoMode == 6)
                _stereoMode = 0;
            else
                _stereoMode++;
            _pds->setStereoMode(stereoMode[_stereoMode]);
            break;
        case osgGA::GUIEventAdapter::KEY_Up:
            if (_displayType == 3)
                _displayType = 0;
            else
                _displayType++;
            _pds->setDisplayType(displayType[_displayType]);
            break;
        case osgGA::GUIEventAdapter::KEY_Down:
            if (_displayType == 0)
                _displayType = 3;
            else
                _displayType--;
            _pds->setDisplayType(displayType[_displayType]);
            break;
        case osgGA::GUIEventAdapter::KEY_KP_Add:
            _eyeSeperation += 0.01;
            _pds->setEyeSeparation(_eyeSeperation);
            break;
        case osgGA::GUIEventAdapter::KEY_KP_Subtract:
            _eyeSeperation -= 0.01;
            _pds->setEyeSeparation(_eyeSeperation);
            break;
        default:
            break;
        }
        return true;
    }
    return false;
}

CvZToggle3D::CvZToggle3D(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"3D显示"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"查看"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"视图"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"进入或退出3D模式"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"F1进入3D模式，左右方向键切换3D模式，上下方向键切换显示类型，加减号键调整眼距"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"Stateset3D"));

	m_spHandler = nullptr;
	bIn3D = false;
}


CvZToggle3D::~CvZToggle3D(void)
{
	m_spHandler = nullptr;
}

bool CvZToggle3D::Initialize()
{
	return true;
}

bool CvZToggle3D::UnInitialize()
{
	return true;
}

bool CvZToggle3D::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;

	registerHandlers();

	IUICommand::Enable(true);
	return true;
}

// IUICommand
bool CvZToggle3D::OnClick()
{
	if (bIn3D)
	{
		Deactivate();
	}
	else
	{
		Activate();
	}
	return true;
}

bool CvZToggle3D::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if( !spViewer3D.valid())
		return false;

	if( spViewer3D->getViewer()==nullptr )
		return false;

	spViewer3D->Lock();

	if( !m_spHandler.valid() )
	{
		osg::ref_ptr<osg::DisplaySettings> dps = spViewer3D->getViewer()->getDisplaySettings();
		if (!dps)
		{
			dps = new osg::DisplaySettings();
		}
		spViewer3D->getViewer()->setDisplaySettings(dps.get());
		m_spHandler = new CvZToggle3DHandler(spViewer3D->getViewer()->getDisplaySettings());
	}

	spViewer3D->getViewer()->addEventHandler(m_spHandler.get());

	spViewer3D->Unlock();

	bIn3D = true;

	return true;
}

bool CvZToggle3D::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	spViewer3D->getViewer()->removeEventHandler( m_spHandler.get() );

	osg::ref_ptr<osg::DisplaySettings> dps = spViewer3D->getViewer()->getDisplaySettings();
	dps->setStereo(false);

	m_spHandler = nullptr;

	spViewer3D->Unlock();

	bIn3D = false;

	return true;
}

bool CvZToggle3D::Checked()
{
	return bIn3D;
}

bool CvZToggle3D::OnAnything3(x3::IObject* sender, const std::string& eventKey, long wParam, long lParam)
{
	return true;
}