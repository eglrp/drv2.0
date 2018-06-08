#include "stdafx.h"
#include "vlSetProject.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "SelfDefTerrainManipulator.h"
#include "osgGA/KeySwitchMatrixManipulator"
#include <fstream>

CvlSetProject::CvlSetProject(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"本地设置"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"配置管理"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"本地设置"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"本地设置"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"SetProject")); 

	Enable(false);

	m_pWin = NULL;
	bShow = false;

	registerHandlers();
}

CvlSetProject::~CvlSetProject(void)
{
}

bool CvlSetProject::Initialize()
{
	supportsEvent("PROJECTOPEN");
	return true;
}

bool CvlSetProject::UnInitialize()
{
	return true;
}

bool CvlSetProject::SetBuddy(x3::IObject* val)
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
bool CvlSetProject::OnClick()
{
	if (bShow)
	{
		Deactivate();
	}
	else
	{
		Activate();
	}

	IUICommand::Checked(bShow);
	return true;
}

bool CvlSetProject::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
{
	if (eventKey == "PROJECTOPEN")
	{
		if (filename.length() > 0)
		{
			Enable(true);
		}
	}
	return true;
}

bool CvlSetProject::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	if (m_pWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pWin = new CProjectSetDlg;
		m_pWin->m_nOSG_MAX_PAGEDLOD = spViewer3D->GetOSG_MAX_PAGEDLOD();
		m_pWin->m_nOSG_NUM_DATABASE_THREADS = spViewer3D->GetOSG_NUM_DATABASE_THREADS();
		m_pWin->m_nScreenNum = spViewer3D->GetScreenNum();
		m_pWin->mPrj = spViewer3D->GetPrj();
		m_pWin->mPrjStr = CString(m_pWin->mPrj.c_str());
		m_pWin->parent = (CObject*) this; 
		m_pWin->Create(CProjectSetDlg::IDD);
	}

	bShow = true;

	m_pWin->ShowWindow(SW_SHOW);
	/*
	RECT r;
	m_pWin->GetWindowRect(&r);

	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	m_pWin->MoveWindow(&r);
	*/
	return true;
}

bool CvlSetProject::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	m_pWin->ShowWindow(SW_HIDE);

	bShow = false;
	return true;
}

void CvlSetProject::SetOSG_MAX_PAGEDLOD(int nMax)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	spViewer3D->SetOSG_MAX_PAGEDLOD(nMax);
}

void CvlSetProject::SetOSG_NUM_DATABASE_THREADS(int nThreads)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	spViewer3D->SetOSG_NUM_DATABASE_THREADS(nThreads);
}

void CvlSetProject::SetScreenNum(int nNum)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	spViewer3D->SetScreenNum(nNum);
}

void CvlSetProject::SetCameraLimitHeight(int h)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if (h <0 &&h != -1)
	{
		return;
	}
	osgGA::CameraManipulator* curMani = spViewer3D->getViewer()->getCameraManipulator();
	osgGA::KeySwitchMatrixManipulator* keyMani = dynamic_cast<osgGA::KeySwitchMatrixManipulator*>(curMani);
	if (keyMani == NULL)
	{
		return;
	}
	CSelfDefTerrainManipulator* newMani = dynamic_cast<CSelfDefTerrainManipulator*>(keyMani->getCurrentMatrixManipulator());
	if (newMani == NULL)
	{
		return;
	}
	newMani->setHeightLimit(h);
}

CString CvlSetProject::GetHouseLayer()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	return spViewer3D->GetDefLayer();
}

CString CvlSetProject::GetBuildLayer()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	return spViewer3D->GetDefLayer();
}

void CvlSetProject::SetHouseLayer(CString s)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->SetHouseDefLayer(s);
}

void CvlSetProject::SetBuildLayer(CString s)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->SetBuildDefLayer(s);
}

void CvlSetProject::setPrj(std::string s)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	spViewer3D->SetPrj(s);
}