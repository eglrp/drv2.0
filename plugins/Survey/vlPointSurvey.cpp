#include "stdafx.h"
#include "vlPointSurvey.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include <ogr_spatialref.h>

#include "resource.h"

#ifdef _2D
PointSuerveyToolEventHandler::PointSuerveyToolEventHandler(CPointSurveyInfoWin* win)
{
	m_pWin = win;
	readPrjInfo();
}
#else
PointSuerveyToolEventHandler::PointSuerveyToolEventHandler(CPointSurveyInfoWin* win, const osgEarth::SpatialReference* m_pEllipsoid2, osgEarth::Annotation::CircleNode* m_pCircleNode2)
{
	m_pWin = win;
	spf = m_pEllipsoid2;
	m_pEllipsoid = spf->getEllipsoid();
	m_pCircleNode = m_pCircleNode2;
}
#endif

bool PointSuerveyToolEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    switch(ea.getEventType())
    {
    case(osgGA::GUIEventAdapter::PUSH):
    {
        osg::Vec3d vecPos;
        osg::Vec3d pos = getPos(ea, aa, vecPos);

		double lon = 0,lat = 0;
		PointToWGS84(pos.x(),pos.y(),lon,lat);
        std::stringstream ss;
        ss.precision(10);
        ss << vecPos.x() << std::endl;
        std::string str;
        str = ss.str();
        ss.str("");

        ss << vecPos.y() << std::endl;
        std::string str2;
        str2 = ss.str();
        ss.str("");

        ss << vecPos.z() << std::endl;
        std::string str3;
        str3 = ss.str();
        ss.str("");

		ss << lon << std::endl;
		std::string strlon;
		strlon = ss.str();
		ss.str("");

		ss << lat << std::endl;
		std::string strlat;
		strlat = ss.str();
		ss.str("");

        m_pWin->SetText(str, str2, str3,strlon,strlat);

		
#ifdef _2D

#else
		m_pCircleNode->setPosition(	osgEarth::GeoPoint::GeoPoint(spf, vecPos, osgEarth::AltitudeMode::ALTMODE_RELATIVE));
#endif
        
    }
    default:
        return false;
    }
}


osg::Vec3d PointSuerveyToolEventHandler::getPos(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
{
    pos = osg::Vec3d(0, 0, 0);
    osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
    if (pViewer == NULL)
    {
        return osg::Vec3d(0, 0, 0);
    }
    // 获取当前点
    osgUtil::LineSegmentIntersector::Intersections intersection;
    double x = ea.getX();
    double y = ea.getY();
    pViewer->computeIntersections(ea.getX(), ea.getY(), intersection);


    osgUtil::LineSegmentIntersector::Intersections::iterator iter
    = intersection.begin();
    if (iter != intersection.end())
    {
#ifdef _2D
		pos.x() = iter->getWorldIntersectPoint().x();
		pos.y() = iter->getWorldIntersectPoint().y();
		pos.z() = iter->getWorldIntersectPoint().z();
		return osg::Vec3d(iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z());
#else
		m_pEllipsoid->convertXYZToLatLongHeight(
			iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z(),
			pos.y(), pos.x(), pos.z());
		pos.x() = osg::RadiansToDegrees(pos.x());
		pos.y() = osg::RadiansToDegrees(pos.y());
		return iter->getWorldIntersectPoint();
#endif

    }
    return osg::Vec3d(0, 0, 0);
}

void PointSuerveyToolEventHandler::readPrjInfo()
{
	/*TCHAR szFilePath[1024 + 1];
	GetModuleFileName(NULL, szFilePath, 1024);

	CString str_url = szFilePath;
	USES_CONVERSION;
	std::string curFilePath( W2A(str_url) );

	curFilePath = replaceFileName(curFilePath, "Data\\DRVdata\\total.prj");

	FILE* fpPrj = fopen(curFilePath.c_str(), "r");
	char str[1024];
	fgets(str, 1024, fpPrj);
	fclose(fpPrj);

	prjInfo = str;*/
	CvlPointSurvey* p = (CvlPointSurvey*)(m_pWin->parent);
	prjInfo = p->getPrj();
}

std::string PointSuerveyToolEventHandler::replaceFileName(std::string srcStr, std::string replaceStr)
{
	int pos = srcStr.rfind("\\") + 1;
	int length = srcStr.length() - 1;
	return srcStr.replace(pos, length, replaceStr);
}

void PointSuerveyToolEventHandler::PointToWGS84(double x, double y, double& lon, double& lat)
{
	lon = x;
	lat = y;
#ifdef _DEBUG
	//return;
#endif

	OGRCoordinateTransformation *poCT = NULL;

	if (poCT == nullptr)
	{
		OGRSpatialReference* oSourceSRS;
		oSourceSRS = (OGRSpatialReference*)OSRNewSpatialReference(prjInfo.c_str());

		if (oSourceSRS == NULL)
			return;

		OGRSpatialReference* oTargetSRS;
		oTargetSRS = (OGRSpatialReference*)OSRNewSpatialReference("GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]");
		if (oTargetSRS == NULL)
		{
			OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
			return;
		}

		poCT = OGRCreateCoordinateTransformation(oSourceSRS, oTargetSRS);

		if (poCT == nullptr)
		{
			OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
			OSRDestroySpatialReference((OGRSpatialReferenceH)oTargetSRS);
			return;
		}
	}

	poCT->Transform(1, &lon, &lat);

#if 0
	lon = x;
	lat = y;
	OGRSpatialReference* oSourceSRS;
	OGRSpatialReference oTargetSRS;
	OGRCoordinateTransformation *poCT;
	oSourceSRS = (OGRSpatialReference*)OSRNewSpatialReference(mSRS.c_str());
	if (oSourceSRS == NULL)
		return;

	oTargetSRS.importFromEPSG(4326);
	const char* err = CPLGetLastErrorMsg();
	poCT = OGRCreateCoordinateTransformation(oSourceSRS, &oTargetSRS);
	if (poCT == nullptr)
	{
		OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
		return;
	}

	poCT->Transform(1, &lon, &lat);
	OGRCoordinateTransformation::DestroyCT(poCT);
	OSRDestroySpatialReference((OGRSpatialReferenceH)oSourceSRS);
#endif


	if (poCT != nullptr)
		OGRCoordinateTransformation::DestroyCT((OGRCoordinateTransformation*)poCT);
	poCT = nullptr;
}


CvlPointSurvey::CvlPointSurvey(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"点测量"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"空间量测"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"点测量"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"点测量"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"PointSurvey"));
    bInSurvey = false;
	m_pPointSurveyInfoWin = NULL;
	m_spMeasureToolEventHandler = NULL;
}

CvlPointSurvey::~CvlPointSurvey(void)
{
}

bool CvlPointSurvey::Initialize()
{
    return true;
}

bool CvlPointSurvey::UnInitialize()
{
    return true;
}

bool CvlPointSurvey::SetBuddy(x3::IObject* val)
{
    x3::Object<IViewer3D> spViewer3D(val);
    if( !spViewer3D.valid() )
        return false;

    if( m_spBuddy == spViewer3D )
        return true;

    m_spBuddy = spViewer3D;
    if( spViewer3D->getViewer() == nullptr )
        return false;

    //m_pLayerGroup = new osg::Group;
    //spViewer3D->getMapNode()->addChild(m_pLayerGroup);

    //m_pLayerGroup->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    m_pCircleNode = NULL;

    m_circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()
    ->fill()->color() = osgEarth::Symbology::Color::Yellow;

    m_circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
    ->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;

    m_circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
    ->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

    m_circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
    ->verticalOffset() = 0.1;

    return true;
}


bool CvlPointSurvey::Checked()
{
    return bInSurvey;
}

// IUICommand
bool CvlPointSurvey::OnClick()
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

bool CvlPointSurvey::Activate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	if (m_pPointSurveyInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pPointSurveyInfoWin = new CPointSurveyInfoWin();
		m_pPointSurveyInfoWin->parent = (CObject*) this; 
		m_pPointSurveyInfoWin->Create(IDD_DIALOG_POINTSURVEY);
	}

	//spViewer3D->Lock();
	m_spMeasureToolEventHandler = new CvlMeasureToolHandler(spViewer3D->getSceneData()->asGroup());
	m_spMeasureToolEventHandler->setIntersectionMask( 0x1 );
	m_spMeasureToolEventHandler->setIsPoint(true);

	spViewer3D->getViewer()->addEventHandler( m_spMeasureToolEventHandler );

	m_pLayerGroup = new osg::Group;
#ifdef _2D
	spViewer3D->getRootNode()->asGroup()->addChild(m_pLayerGroup);
#else
	spViewer3D->getMapNode()->addChild(m_pLayerGroup);
#endif

	m_pLayerGroup->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

#ifdef _2D

#else
	if(m_pCircleNode == nullptr)
	{
		osg::Vec3d center =   osg::Vec3d(0, 0, 0);

		m_pCircleNode = new osgEarth::Annotation::CircleNode(
			spViewer3D->getMapNode(),
			osgEarth::GeoPoint::GeoPoint(spViewer3D->getMapNode()->getMap()->getSRS(),
			center, osgEarth::AltitudeMode::ALTMODE_RELATIVE),
			2,
			m_circleStyle
			);
		m_pLayerGroup->addChild(m_pCircleNode);
	}
#endif


#ifdef _2D
	m_spPointEventHandler = new PointSuerveyToolEventHandler(m_pPointSurveyInfoWin);
#else
	m_spPointEventHandler = new PointSuerveyToolEventHandler(m_pPointSurveyInfoWin, spViewer3D->getMapNode()->getMap()->getProfile()->getSRS(), m_pCircleNode);
#endif

	spViewer3D->getViewer()->addEventHandler( m_spPointEventHandler );

	//spViewer3D->Unlock();

	m_pPointSurveyInfoWin->ShowWindow(SW_SHOW);

	RECT r;
	m_pPointSurveyInfoWin->GetWindowRect(&r);

	int nWidth = r.right - r.left;
	int nHeight = r.bottom - r.top;

	r.left = spViewer3D->rectView3D.left + 20;
	r.top = spViewer3D->rectView3D.top + 20;
	r.right = r.left + nWidth;
	r.bottom = r.top + nHeight;

	m_pPointSurveyInfoWin->MoveWindow(&r);

	bInSurvey = true;

	return true;
}
bool CvlPointSurvey::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	//spViewer3D->Lock();
	//spViewer3D->Lock();
	spViewer3D->getViewer()->removeEventHandler(m_spPointEventHandler.get());
	spViewer3D->getViewer()->removeEventHandler(m_spMeasureToolEventHandler.get());
	//spViewer3D->Unlock();
	m_spMeasureToolEventHandler = nullptr;
	m_spPointEventHandler = nullptr;

	m_pPointSurveyInfoWin->ShowWindow(SW_HIDE);

	if (m_pLayerGroup != nullptr)
	{
#ifdef _2D
		//spViewer3D->Lock();
		spViewer3D->getRootNode()->asGroup()->removeChild(m_pLayerGroup);
		//spViewer3D->Unlock();
#else
		m_pLayerGroup->removeChild(m_pCircleNode);
		spViewer3D->getMapNode()->removeChild(m_pLayerGroup);
		m_pCircleNode = nullptr;
#endif

		m_pLayerGroup = nullptr;
	}

	bInSurvey = false;

	//spViewer3D->Unlock();

	return true;
}

