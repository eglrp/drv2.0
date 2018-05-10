#include "stdafx.h"
#include "vlLineSurveyTri.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

MyMeasureToolEventHandler::MyMeasureToolEventHandler(CvZLineSurveyInfoWin* win)
{
	m_pWin = win;
}

void MyMeasureToolEventHandler::onDistanceChanged(CvlMeasureToolHandler* sender, double distance, double distance1, double distance2)
{
	m_pWin->SetDist(distance, distance1, distance2);
}


CvlLineSurveyTri::CvlLineSurveyTri(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"线测量"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"空间量测"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"线测量-考虑斜坡"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"线测量"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"LineSurveyTri")); 

	bInSurvey = false;
	m_pLineSurveyInfoWin = NULL;
}

CvlLineSurveyTri::~CvlLineSurveyTri(void)
{
}

bool CvlLineSurveyTri::Initialize()
{
	return true;
}

bool CvlLineSurveyTri::UnInitialize()
{
	return true;
}

bool CvlLineSurveyTri::SetBuddy(x3::IObject* val)
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

bool CvlLineSurveyTri::Checked()
{
	return bInSurvey;
}

// IUICommand
bool CvlLineSurveyTri::OnClick()
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

bool CvlLineSurveyTri::Activate()
{
	if (m_pLineSurveyInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pLineSurveyInfoWin = new CvZLineSurveyInfoWin();
		m_pLineSurveyInfoWin->parent = (CObject*) this;
		m_pLineSurveyInfoWin->Create(IDD_DIALOG_LINESURVEY);
	}

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->Lock();

#ifdef _2D
	m_spMeasureEventHandler = new CvlMeasureToolHandler(spViewer3D->getRootNode()->asGroup());
#else
	m_spMeasureEventHandler = new CvlMeasureToolHandler(spViewer3D->getRootNode()->asGroup());    
#endif

	m_spMeasureEventHandler->setIntersectionMask( 0x1 );
	//m_spMeasureEventHandler->setIsPath(true);

	spViewer3D->getViewer()->addEventHandler( m_spMeasureEventHandler );

	m_spMyMeasureEventHandler = new MyMeasureToolEventHandler(m_pLineSurveyInfoWin);
	m_spMeasureEventHandler->addEventHandler( m_spMyMeasureEventHandler.get() );

#ifdef _2D

#else
	/*Style style = m_spMeasureEventHandler->getLineStyle();
	style.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
	style.getOrCreate<LineSymbol>()->stroke()->width() = 4.0f;
	m_spMeasureEventHandler->setLineStyle(style);*/
#endif

	//spViewer3D->Unlock();

	m_pLineSurveyInfoWin->ShowWindow(SW_SHOW);

	RECT r;
	m_pLineSurveyInfoWin->GetWindowRect(&r);

	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	m_pLineSurveyInfoWin->MoveWindow(&r);

	bInSurvey = true;

	return true;
}
bool CvlLineSurveyTri::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->Lock();

	spViewer3D->getViewer()->removeEventHandler(m_spMeasureEventHandler.get());
	m_spMeasureEventHandler = nullptr;

	m_pLineSurveyInfoWin->ShowWindow(SW_HIDE);

	//spViewer3D->Unlock();
	bInSurvey = false;
	return true;
}

void CvlLineSurveyTri::SetPathMode(bool bPath)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	m_spMeasureEventHandler->setIsPath(bPath);

	spViewer3D->Unlock();
}