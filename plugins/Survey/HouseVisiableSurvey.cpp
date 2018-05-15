#include "stdafx.h"
#include "HouseVisiableSurvey.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

CHouseVisiableSurvey::CHouseVisiableSurvey(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"房间可视分析"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"查询搜索"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"房间可视分析"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"房间可视分析"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"LineSurvey")); 
	bInSurvey = false;
	p_mDLGHouseVisiableSurveyWin = nullptr;
}

CHouseVisiableSurvey::~CHouseVisiableSurvey(void)
{
}

bool CHouseVisiableSurvey::Initialize()
{
	return true;
}

bool CHouseVisiableSurvey::UnInitialize()
{
	return true;
}

bool CHouseVisiableSurvey::SetBuddy(x3::IObject* val)
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
bool CHouseVisiableSurvey::OnClick()
{
	if (bInSurvey)
	{
		Deactivate();
	}
	else
	{
		Activate();

	}
	return true;
}

bool CHouseVisiableSurvey::Checked()
{
	return bInSurvey;
}

bool CHouseVisiableSurvey::Activate()
{
	if (p_mDLGHouseVisiableSurveyWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		p_mDLGHouseVisiableSurveyWin = new DLGHouseVisiableSurvey();
		p_mDLGHouseVisiableSurveyWin->parent = (CObject*) this;
		p_mDLGHouseVisiableSurveyWin->Create(IDD_DIALOG_HOUSEVISIABLESURVER);
	}
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	p_mDLGHouseVisiableSurveyWin->ShowWindow(SW_SHOW);

	RECT r;
	p_mDLGHouseVisiableSurveyWin->GetWindowRect(&r);

	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	p_mDLGHouseVisiableSurveyWin->MoveWindow(&r);
	bInSurvey = true;
	return true;
}

bool CHouseVisiableSurvey::Deactivate()
{
	if (p_mDLGHouseVisiableSurveyWin)
	{
		p_mDLGHouseVisiableSurveyWin->ShowWindow(SW_HIDE);
	}
	bInSurvey = false;
	return true;
}
