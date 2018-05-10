#include "stdafx.h"
#include "vlWalkRec.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include <osgViewer/ViewerEventHandlers>

CvlWalkRec::CvlWalkRec(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"录制航线"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"辖区巡航"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"录制航线"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"录制航线"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"Walk")); 

	registerHandlers();

	m_pWin = NULL;
	bShow = false;
}

CvlWalkRec::~CvlWalkRec(void)
{
}

bool CvlWalkRec::Initialize()
{
	supportsEvent("PATHACTIVE");
	return true;
}

bool CvlWalkRec::UnInitialize()
{
	return true;
}

bool CvlWalkRec::SetBuddy(x3::IObject* val)
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
bool CvlWalkRec::OnClick()
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

bool CvlWalkRec::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	if (m_pWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pWin = new CWalkRecDlg;
		m_pWin->parent = (CObject*) this; 
		m_pWin->Create(CWalkRecDlg::IDD);
	}

	bShow = true;

	m_pWin->ShowWindow(SW_SHOW);
	aPts.RemoveAll();
	m_pWin->m_editNum.SetWindowTextW(L"0");

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

bool CvlWalkRec::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	m_pWin->ShowWindow(SW_HIDE);

	bShow = false;
	return true;
}

void CvlWalkRec::AddKeyPt()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	const osg::Matrixd& m = spViewer3D->getViewer()->getCamera()->getInverseViewMatrix();
	osg::AnimationPath::ControlPoint* spCP = new osg::AnimationPath::ControlPoint(m.getTrans(), m.getRotate());

	CString sKeyPt = L"";
	osg::Vec3d position = spCP->getPosition();
	osg::Quat rotation = spCP->getRotation();
	sKeyPt.Format(L"%.8f %.8f %.8f %.8f %.8f %.8f %.8f", position.x(),position.y(),position.z(),rotation.x(),rotation.y(),rotation.z(),rotation.w());

	aPts.Add(sKeyPt);

	CString sNum = L"";
	sNum.Format(L"%d", aPts.GetCount());
	m_pWin->m_editNum.SetWindowTextW(sNum);
	m_pWin->UpdateData(TRUE);
}

void CvlWalkRec::SaveKeyPts()
{
	USES_CONVERSION;
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	osgDB::ofstream _fout(spViewer3D->GetRecordPath().c_str());
	_fout.precision(16);
	if (!_fout)
	{
		return;
	}

	for (int i=0; i<aPts.GetCount();i++)
	{
		_fout << W2A(aPts.GetAt(i)) <<std::endl;
	}
}

void CvlWalkRec::ClearPts()
{
	aPts.RemoveAll();
	CString sNum = L"";
	sNum.Format(L"%d", aPts.GetCount());
	m_pWin->m_editNum.SetWindowTextW(sNum);
	m_pWin->UpdateData(TRUE);
}

bool CvlWalkRec::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
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