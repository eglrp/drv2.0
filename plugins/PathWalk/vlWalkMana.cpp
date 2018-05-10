#include "stdafx.h"
#include "vlWalkMana.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "PathManaDlg.h"
#include "osgDB/FileUtils"
#include "ComFun.h"

using namespace osg;

CvlWalkMana::CvlWalkMana(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"航迹规划"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"辖区巡航"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"航迹规划"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"航迹规划"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"WalkMana"));

    m_pWin = NULL;
    registerHandlers();
    bShow = false;
}

CvlWalkMana::~CvlWalkMana(void)
{
}

bool CvlWalkMana::Initialize()
{
	supportsEvent("PROJECTOPEN");
    return true;
}

bool CvlWalkMana::UnInitialize()
{
    return true;
}

bool CvlWalkMana::SetBuddy(x3::IObject* val)
{
    x3::Object<IViewer3D> spViewer3D(val);
    if( !spViewer3D.valid() )
        return false;

    if( m_spBuddy == spViewer3D )
        return true;

    m_spBuddy = spViewer3D;
    if( spViewer3D->getViewer() == nullptr )
        return false;

    Enable(false);

    return true;
}

// IUICommand
bool CvlWalkMana::OnClick()
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
}

bool CvlWalkMana::Checked()
{
    return bShow;
}

bool CvlWalkMana::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	if (m_pWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pWin = new CPathManaDlg();
		m_pWin->parent = (CObject*) this;
		m_pWin->Create(CPathManaDlg::IDD);
		m_pWin->m_sProjectPath = spViewer3D->GetProjectFilePath();
	}

	bShow = true;

	m_pWin->InitListData(spViewer3D->GetPathsArray());

	RECT r;
	m_pWin->GetWindowRect(&r);

	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	m_pWin->MoveWindow(&r);

	m_pWin->ShowWindow(SW_SHOW);

	return true;
}

bool CvlWalkMana::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	m_pWin->ShowWindow(SW_HIDE);

	bShow = false;
	return true;
}

bool CvlWalkMana::AddPath(CString sName)
{
    USES_CONVERSION;
    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    CString sPathFilePath = m_sProjectDir + sName + L".path";
    if (!osgDB::fileExists(W2A(sPathFilePath.GetBuffer(0))))
    {
        CStringArray* p = spViewer3D->GetPathsArray();
        p->Add(sPathFilePath);
        return true;
    }
    else
        return false;
}

bool CvlWalkMana::ImportPath(CString sPath)
{
	USES_CONVERSION;
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	std::string filename = osgDB::getStrippedName(W2A(sPath.GetBuffer(0)));

	CString sPathDest = m_sProjectDir + GetFileName(sPath);
	CString sPathPathDest = m_sProjectDir + GetFileName(sPath) + L".path";
	
	osgDB::copyFile( W2A(sPath.GetBuffer(0)), W2A(sPathDest.GetBuffer(0)) );

	if (osgDB::fileExists(W2A(sPathPathDest.GetBuffer(0))))
	{
		_unlink(W2A(sPathPathDest));
	}

	CStringArray* p = spViewer3D->GetPathsArray();
	p->Add(sPathDest);

	return true;
}

bool CvlWalkMana::DelPath(CString sName)
{
    USES_CONVERSION;
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	CString sPathFilePath = m_sProjectDir + sName + L".path";

	CStringArray* p = spViewer3D->GetPathsArray();
	for (int i=0; i < p->GetCount(); i++)
	{
		if (sPathFilePath == p->GetAt(i))
		{
			p->RemoveAt(i);
		}
	}

    if (osgDB::fileExists(W2A(sPathFilePath.GetBuffer(0))))
    {
        _unlink(W2A(sPathFilePath));
		_unlink(W2A(sPathFilePath + L".path"));
        return true;
    }
    else
        return false;
}

bool CvlWalkMana::RecordPath(CString sName)
{
    USES_CONVERSION;

    if (sName.GetLength() == 0)
    {
        DoRecord("");
        return false;
    }

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    CString sProjectPath = spViewer3D->GetProjectFilePath();
    CString sProjectDir = sProjectPath.Left(sProjectPath.ReverseFind('\\') + 1);
    CString sPathFilePath = sProjectDir + sName + L".path";
    /*if (osgDB::fileExists(W2A(sPathFilePath.GetBuffer(0))))
    {
    	CString strPrompt(GETSTRINGT(L"WalkMana", _T("PathExistPrompt"), _T("路径文件已经存在，确认覆盖么？")));

    	if ( AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION ) == IDYES)
    	{

    	}
    	else
    	{
    		return false;
    	}
    }
    */
    this->DoRecord(W2A(sPathFilePath.GetBuffer(0)));
    return true;
}

bool CvlWalkMana::DoRecord(std::string path)
{
    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    spViewer3D->SetRecordPath(path);
    return true;
}

bool CvlWalkMana::StopRecord()
{
    //x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    //spViewer3D->StopRecord(path);
    return true;
}

void CvlWalkMana::OnPathActive(std::string filename)
{

}

std::string CvlWalkMana::GetFileContent(std::string path)
{
    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    return spViewer3D->GetFileContent(path);
}



void CvlWalkMana::BuildPath(std::string path, CStringArray* a)
{
    USES_CONVERSION;

    std::string pathc;
    pathc = path + ".path";
    Vec3d ptF;
    Vec3d ptN;

    Vec4d roF, roN;
	Vec3d roF3,roN3;

    double speed = 10.0;//两米每秒，一秒25帧
    CStringArray aOut;

    double t = 0;

    osgDB::ofstream _fout(pathc.c_str());
    _fout.precision(16);
    if (!_fout)
    {
        return;
    }

	double dMeanHeight = 0;
	double dHeightTotal = 0;
	double nPtsNum = 0;
	for (int i = 0; i < a->GetCount(); i++)
    {
        CStringArray aPt;
		AnalyzeData(aPt, a->GetAt(i), L" ");
		if (aPt.GetCount() < 5)
		{
			continue;
		}

		double h = _tstof(aPt.GetAt(2));
		dHeightTotal += h;
		nPtsNum++;
    }
	dMeanHeight = dHeightTotal/a->GetCount();

    for (int i = 0; i < a->GetCount() - 2; i++)
    {
        CStringArray aPt;
        AnalyzeData(aPt, a->GetAt(i), L" ");
        ptF = Vec3d(_tstof(aPt.GetAt(0)), _tstof(aPt.GetAt(1)), _tstof(aPt.GetAt(2)));
		roF = Vec4d(_tstof(aPt.GetAt(3)), _tstof(aPt.GetAt(4)), _tstof(aPt.GetAt(5)), _tstof(aPt.GetAt(6)));
        roF3 = Vec3d(_tstof(aPt.GetAt(3)), _tstof(aPt.GetAt(4)), _tstof(aPt.GetAt(5)));

        AnalyzeData(aPt, a->GetAt(i + 1), L" ");
        ptN = Vec3d(_tstof(aPt.GetAt(0)), _tstof(aPt.GetAt(1)), _tstof(aPt.GetAt(2)));
		roN = Vec4d(_tstof(aPt.GetAt(3)), _tstof(aPt.GetAt(4)), _tstof(aPt.GetAt(5)), _tstof(aPt.GetAt(6)));
        roN3 = Vec3d(_tstof(aPt.GetAt(3)), _tstof(aPt.GetAt(4)), _tstof(aPt.GetAt(5)));

        double len = (ptN - ptF).length();
		double lenR = (roN3 - roF3).length();
		double lenW = roN.w() - roF.w();

		/*if ( roN3.x() > 0 && (roN3.x() - roF3.x() > 0.5) )
		{
		roF3.x() += 0.5;
		}

		if ( roN3.y() < 0 && (roN3.y() - roF3.y() > 0.5) )
		{
		roF3.y() -= 0.5;
		}

		if ( roN3.z() < 0 && fabs(roN3.z() - roF3.z()) > 1)
		{
		roF3.z() -= 1;
		}*/

        double delta = speed / 25.0; //每一帧的行走距离

        int insertNum = len / delta;

        double x, y, z, rx, ry, rz, w;
        CString sNew;

        for (int j = 0; j < insertNum; j++)
        {
            x = ptF.x() + (ptN.x() - ptF.x()) / insertNum * (j + 1);
            y = ptF.y() + (ptN.y() - ptF.y()) / insertNum * (j + 1);
            z = ptF.z() + (ptN.z() - ptF.z()) / insertNum * (j + 1);
			//z = dMeanHeight;

			rx = roF3.x() + (roN3.x() - roF3.x())/insertNum * (j + 1);
			ry = roF3.y() + (roN3.y() - roF3.y())/insertNum * (j + 1);
			rz = roF3.z() + (roN3.z() - roF3.z())/insertNum * (j + 1);

			w = roF.w() + (roN.w() - roF.w())/insertNum * (j + 1);

            t += 1.0 / 25.0;
            _fout << t << " " << x << " " << y << " " << z << " " << rx << " " << ry << " " << rz << " " << w << std::endl;
            //aOut.Add(sNew);
        }
    }

    _fout.close();

    /*
    osgDB::ofstream _fout(pathc.c_str());
    _fout.precision(16);
    if (!_fout)
    {
    	return;
    }

    CString sAll;
    for (int i=0; i<aOut.GetCount();i++)
    {
    	sAll += aOut.GetAt(i) + L"\n";
    }

    _fout << W2A(sAll) <<std::endl;

    _fout.close();
    */
}

bool CvlWalkMana::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
{
	if (eventKey == "PROJECTOPEN")
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