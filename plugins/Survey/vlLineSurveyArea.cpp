#include "stdafx.h"
#include "vlLineSurveyArea.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

MyMeasureToolEventHandlerArea::MyMeasureToolEventHandlerArea(CAreaSurveyDlg* win)
{
	m_pWin = win;
}

void MyMeasureToolEventHandlerArea::onDistanceChanged(CvlMeasureToolHandler* sender, double distance, double distance1, double distance2)
{
	m_pWin->SetArea(distance, distance1);
}


CvlLineSurveyArea::CvlLineSurveyArea(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"面积测量"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"空间量测"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"面积测量-三维面"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"面积测量"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"LineSurveyArea")); 

	bInSurvey = false;
	m_pWin = NULL;
}

CvlLineSurveyArea::~CvlLineSurveyArea(void)
{
}

bool CvlLineSurveyArea::Initialize()
{
	return true;
}

bool CvlLineSurveyArea::UnInitialize()
{
	return true;
}

bool CvlLineSurveyArea::SetBuddy(x3::IObject* val)
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

bool CvlLineSurveyArea::Checked()
{
	return bInSurvey;
}

// IUICommand
bool CvlLineSurveyArea::OnClick()
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

bool CvlLineSurveyArea::Activate()
{
	if (m_pWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pWin = new CAreaSurveyDlg();
		m_pWin->parent = (CObject*) this;
		m_pWin->Create(IDD_DIALOG_AREA);
	}

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->Lock();

#ifdef _2D
	m_spMeasureEventHandler = new CvlMeasureToolHandler(spViewer3D->getRootNode()->asGroup());

#else
	m_spMeasureEventHandler = new CvlMeasureToolHandler(spViewer3D->getRootNode()->asGroup());    
#endif

	m_spMeasureEventHandler->setIntersectionMask( 0x1 );
	m_spMeasureEventHandler->setIsPath(false);

	spViewer3D->getViewer()->addEventHandler( m_spMeasureEventHandler );

	m_spMyMeasureEventHandler = new MyMeasureToolEventHandlerArea(m_pWin);
	m_spMeasureEventHandler->addEventHandler( m_spMyMeasureEventHandler.get() );

#ifdef _2D

#else
	/*Style style = m_spMeasureEventHandler->getLineStyle();
	style.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
	style.getOrCreate<LineSymbol>()->stroke()->width() = 4.0f;
	m_spMeasureEventHandler->setLineStyle(style);*/
#endif

	//spViewer3D->Unlock();

	m_pWin->ShowWindow(SW_SHOW);

	RECT r;
	m_pWin->GetWindowRect(&r);

	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	m_pWin->MoveWindow(&r);

	bInSurvey = true;

	return true;
}
bool CvlLineSurveyArea::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->Lock();

	spViewer3D->getViewer()->removeEventHandler(m_spMeasureEventHandler.get());
	m_spMeasureEventHandler = nullptr;

	m_pWin->ShowWindow(SW_HIDE);

	//spViewer3D->Unlock();
	bInSurvey = false;
	return true;
}

void CvlLineSurveyArea::SetPathMode(bool bPath)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	m_spMeasureEventHandler->setIsPath(bPath);

	spViewer3D->Unlock();
}