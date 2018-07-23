#include "stdafx.h"
#include "vlLinetSurvey.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

void LineSurveyEventHandler::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
{
	pos = osg::Vec3d(0, 0, 0);
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (pViewer == NULL)
	{
		return ;
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

		pos.x() = iter->getWorldIntersectPoint().x();
		pos.y() = iter->getWorldIntersectPoint().y();
		pos.z() = iter->getWorldIntersectPoint().z();
		return;
	}
	return ;
}

bool LineSurveyEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if ( ea.getHandled() )
		return false;

	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (!viewer)
		return false;

	if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		_mouseDown = true;
		_mouseDownX = ea.getX();
		_mouseDownY = ea.getY();
	}
	else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE && ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
	{
		if(_mouseDown)
			_mouseDown = false;
		else
			return false;
		if (osg::equivalent(ea.getX(), _mouseDownX) && osg::equivalent(ea.getY(), _mouseDownY))
		{
			osg::Vec3d vecPos;
			getPos(ea, aa, vecPos);
			if (vecPos != osg::Vec3d(0,0,0))
			{
				if (vecCoord.size() >= 2)
					vecCoord.clear();
				vecCoord.push_back(vecPos);	
			}
		}
	}
	else if(ea.getEventType() == osgGA::GUIEventAdapter::MOVE)
	{
		osg::Vec3d vecPos;
		getPos(ea, aa, vecPos);
		vector<osg::Vec3d> vec;
		if (vecCoord.size() == 1 && vecPos.length() > 0)
		{
			vec.push_back(vecCoord[0]);
			vec.push_back(vecPos);
			drawLine(lineGroup,vec);
			double d1 = (vec[0] - vec[1]).length();
			double h = abs(vec[0].z() - vec[1].z());
			double d2 = (osg::Vec3d(vec[0].x(),vec[0].y(),0) - osg::Vec3d(vec[1].x(),vec[1].y(),0)).length();
			setDis(d1,d2,h);
		}
	}
	return false;
}

void LineSurveyEventHandler::drawLine(osg::Group* lineGroup,vector<osg::Vec3d>& vecInsect)
{
	if (vecInsect.size()<2)
	{
		return;
	}
	if (lineGroup->getNumChildren()>0)
	{
		lineGroup->removeChildren(0,lineGroup->getNumChildren());
	}
	
	osg::ref_ptr<osg::Geode> _geode = new osg::Geode();
	_geode->setDataVariance(osg::Object::DYNAMIC);
	_geode->getOrCreateStateSet()->setAttributeAndModes( new osg::LineWidth(2.0f) );
	_geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	_geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );

	osg::ref_ptr<osg::Geometry> _geometry1 = new osg::Geometry();
	_geometry1->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	_geometry1->setColorArray(shared_colors.get(), osg::Array::BIND_PER_PRIMITIVE_SET);

	osg::ref_ptr<osg::Vec3Array> vert1 = new osg::Vec3Array();
	
	for (int i = 0;i<vecInsect.size() ;i++)
	{
		vert1->push_back(vecInsect[i]);
	}
	_geometry1->setVertexArray(vert1.get());
	_geometry1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, vert1->size()));
	
	_geode->addDrawable(_geometry1);
	lineGroup->removeChildren(0,lineGroup->getNumChildren());
	lineGroup->addChild(_geode);
}

LineSurveyEventHandler::LineSurveyEventHandler(osg::Group* group,CvZLineSurveyInfoWin* pWin)
{
	_group = group;
	lineGroup = new osg::Group();
	_group->addChild(lineGroup);
	m_pLineSurveyInfoWin = pWin;
	_mouseDown = false;
	
}

void LineSurveyEventHandler::setDis(double d1,double d2,double h)
{
	char msg[999];
	sprintf_s(msg,"直线距离：%f (米)\r\n水平距离：%f (米)\r\n高差：%f (米)",d1,d2,h);
	CEdit* pEdit = (CEdit*)m_pLineSurveyInfoWin->GetDlgItem(IDC_EDIT_LINESURVEY_DIST);
	pEdit->SetWindowTextW(CString(msg));
}

CvlLineSurvey::CvlLineSurvey(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"单线测量"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"空间量测"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"单线测量"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"单线测量"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"LineSurvey")); 
	bInSurvey = false;
	isTestJu = false;
	jieDuan = 0;
	m_pLineSurveyInfoWin = NULL;
	vectex = new osg::Vec3Array;
}

CvlLineSurvey::~CvlLineSurvey(void)
{
}

bool CvlLineSurvey::Initialize()
{
	return true;
}

bool CvlLineSurvey::UnInitialize()
{
	return true;
}

bool CvlLineSurvey::SetBuddy(x3::IObject* val)
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
bool CvlLineSurvey::OnClick()
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

bool CvlLineSurvey::Checked()
{
	return bInSurvey;
}

bool CvlLineSurvey::Activate()
{
	if (m_pLineSurveyInfoWin == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pLineSurveyInfoWin = new CvZLineSurveyInfoWin();
		m_pLineSurveyInfoWin->parent = (CObject*) this;
		m_pLineSurveyInfoWin->Create(IDD_DIALOG_LINESURVEY);
	}
	else
	{
		CEdit* pEdit = (CEdit*)m_pLineSurveyInfoWin->GetDlgItem(IDC_EDIT_LINESURVEY_DIST);
		pEdit->SetWindowTextW(L"");
	}
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	//spViewer3D->Lock();

#ifdef _2D
	m_spLineSurveyEventHandler = new LineSurveyEventHandler(spViewer3D->getRootNode()->asGroup(),m_pLineSurveyInfoWin);
#else
	m_spLineSurveyEventHandler = new LineSurveyEventHandler(spViewer3D->getRootNode()->asGroup(),m_pLineSurveyInfoWin);    
#endif
	spViewer3D->getViewer()->addEventHandler(m_spLineSurveyEventHandler.get());
	

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
bool CvlLineSurvey::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if (m_spLineSurveyEventHandler)
	{
		m_spLineSurveyEventHandler->_group->removeChild(m_spLineSurveyEventHandler->lineGroup);
	}
	spViewer3D->getViewer()->removeEventHandler(m_spLineSurveyEventHandler.get());
	m_spLineSurveyEventHandler = nullptr;
	bInSurvey = false;
	m_pLineSurveyInfoWin->ShowWindow(SW_HIDE);
	return true;
}

