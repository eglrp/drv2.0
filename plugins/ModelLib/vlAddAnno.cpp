#include "stdafx.h"
#include "vlAddAnno.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

CvlAddAnno::CvlAddAnno(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"注记管理"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"注记"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"注记管理"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"注记管理"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"AddAnno"));

	bShow = false;
	m_pWin = NULL;
}

CvlAddAnno::~CvlAddAnno(void)
{
}

bool CvlAddAnno::Initialize()
{
    return true;
}

bool CvlAddAnno::UnInitialize()
{
    return true;
}

bool CvlAddAnno::SetBuddy(x3::IObject* val)
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
bool CvlAddAnno::OnClick()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	CString sProjectPath = spViewer3D->GetProjectFilePath();
	m_sProjectDir = sProjectPath.Left(sProjectPath.ReverseFind('\\') + 1);

	if (bShow)
	{
		Deactivate();
	}
	else
	{
		Activate();
	}
	return true;
	return true;
}

bool CvlAddAnno::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	if (m_pWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pWin = new CAddAnnoDlg();
		m_pWin->parent = (CObject*) this;
		m_pWin->Create(CAddAnnoDlg::IDD);
		m_pWin->m_sProjectPath = spViewer3D->GetProjectFilePath();
	}

	bShow = true;

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

	return true;
}

bool CvlAddAnno::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	m_pWin->ShowWindow(SW_HIDE);

	bShow = false;
	return true;
}
