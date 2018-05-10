#include "stdafx.h"
#include <fstream>
#include <Windows.h>
#include "vlAddModel.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/LineWidth>
#include "osg/PositionAttitudeTransform"
std::string fnModel;

double size;
//size_t path_index;
//std::vector<osg::Vec3d> vecKeyPath;

osg::Geode* drawPath(std::vector<osg::Vec3d>& vec,osg::Vec4 color,osg::Vec3d posOffset = osg::Vec3d(0,0,0))
{
	osg::Geode* geode = new osg::Geode;
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	for (int i = 0;i<vec.size();++i)
	{
		point->push_back(vec[i]-posOffset);
	}
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_STRIP,0,vec.size()));
	geode->addDrawable(lineGeom);

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setAttributeAndModes( new osg::LineWidth(1.5f) );
	ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	return geode;
}

void addPath(osg::Geode* geode,std::vector<osg::Vec3d>& vec,osg::Vec4 color,osg::Vec3d posOffset = osg::Vec3d(0,0,0))
{
	osg::Geometry* pathGeom = dynamic_cast<osg::Geometry*>(geode->getDrawable(geode->getNumDrawables()-1));
	osg::ref_ptr<osg::Vec3Array> vpt = dynamic_cast<osg::Vec3Array*>(pathGeom->getVertexArray());
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	point->push_back(vpt->back());

	for (int i = 0;i<vec.size();++i)
	{
		point->push_back(vec[i]);
	}

	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_STRIP,0,vec.size()+1));
	geode->addDrawable(lineGeom);
	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
}

MyMouseEventHandler::MyMouseEventHandler()
{

}

bool MyMouseEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::PUSH):
		{
			osg::Vec3d vecPos;
			osg::Vec3d pos = getPos(ea, aa, vecPos);
			if (fabs(pos.x()) < 0.001 && fabs(pos.y()) < 0.001 && fabs(pos.z()) < 0.001)
			{
				return false;
			}

			if (ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				return false;
			}
			bool flag = false;
			int n = spViewer3D->getRootNode()->asGroup()->getNumChildren();
			for (int i = 0;i<n;++i)
			{
				if (spViewer3D->getRootNode()->asGroup()->getChild(i)->getName() == "Ѳ��Ա")
				{
					osg::MatrixTransform* posi = dynamic_cast<osg::MatrixTransform*>(spViewer3D->getRootNode()->asGroup()->getChild(i));
					if (posi != NULL)
					{
						
						posi->setMatrix(osg::Matrix::scale(size,size,size)*osg::Matrix::translate(pos));
					}

					flag = true;
					break;
				}
			}
			if (!flag)
			{
				osg::Node* glider = osgDB::readNodeFile(fnModel);
				const osg::BoundingSphere& bs = glider->getBound();
				size = 2.0/(bs.radius()*2);
				size = 1;
				osg::MatrixTransform* positioned = new osg::MatrixTransform;
				positioned->setDataVariance(osg::Object::STATIC);
				positioned->setMatrix(osg::Matrix::scale(size,size,size)*osg::Matrix::translate(pos));
				positioned->addChild(glider);
				positioned->setName("Ѳ��Ա");
				positioned->setUpdateCallback(new CMyUpdateCallback);
				spViewer3D->AddNode(positioned);
				
				return false;
			}

			
			
			
			//positioned->setMatrix(osg::Matrix::translate(pos+osg::Vec3d(0,0,20)));
		}
	default:
		return false;
	}
}


osg::Vec3d MyMouseEventHandler::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
{
	pos = osg::Vec3d(0, 0, 0);
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (pViewer == NULL)
	{
		return osg::Vec3d(0, 0, 0);
	}

	// ��ȡ��ǰ��
	osgUtil::LineSegmentIntersector::Intersections intersection;
	double x = ea.getX();
	double y = ea.getY();
	pViewer->computeIntersections(ea.getX(), ea.getY(), intersection);


	osgUtil::LineSegmentIntersector::Intersections::iterator iter = intersection.begin();
	if (iter != intersection.end())
	{
		pos.x() = iter->getWorldIntersectPoint().x();
		pos.y() = iter->getWorldIntersectPoint().y();
		pos.z() = iter->getWorldIntersectPoint().z();
		return osg::Vec3d(iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z());
	}
	return osg::Vec3d(0, 0, 0);
}

CvlAddModel::CvlAddModel(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"��Ա��λ"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"������"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"��ҳ"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"��Ա��λ"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"��Ա��λ"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"AddModel"));
}

CvlAddModel::~CvlAddModel(void)
{
}

bool CvlAddModel::Initialize()
{
	bInActive = false;
    return true;
}

bool CvlAddModel::UnInitialize()
{
    return true;
}

bool CvlAddModel::SetBuddy(x3::IObject* val)
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
bool CvlAddModel::OnClick()
{
	if (bInActive)
	{
		Deactivate();
	}
	else
	{
		Activate();
	}
	return true;
	/*
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

    // Always create a matrix transform
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;

    GeoPoint geoPoint(spViewer3D->getMapNode()->getMapSRS(), 120.103828863, 30.179353521, 12,	ALTMODE_ABSOLUTE );

    osg::Matrixd matrix;
    geoPoint.createLocalToWorld( matrix );

    mt->setMatrix( matrix );

    osg::ref_ptr<osg::Node> node = new osg::Node;
    node = osgDB::readNodeFile("X:\\����Ŀ¼\\���ݵ���ϵͳ\\50��·����.OSGB");

	mt->addChild(node.get());
	mt->setName("50��·����");

	spViewer3D->AddNode(mt.get());
	*/
	return true;
}


bool CvlAddModel::Activate()
{
	USES_CONVERSION;

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	//////////////////////////////////////////////////////////////////////////
	CString strFilter(GETSTRINGT(A2CT(_getClassName()), _T("FilterModel"), _T("�ı��ļ�(*.txt)|*.txt||")));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal()!=IDOK )
	{
		return false;
	}
	string path = T2A(dlg.GetPathName());
	vector<osg::Vec3d> vecpt,vecKeyPath;
	std::string ObjectName;
	readPath(path.c_str(),vecpt,ObjectName);
	InsertPoint_1m(vecpt,vecKeyPath);

	int n = spViewer3D->getRootNode()->asGroup()->getNumChildren();
	for (int i = 0;i<n;++i)
	{
		if (spViewer3D->getRootNode()->asGroup()->getChild(i)->getName() == ObjectName)
		{
			AfxMessageBox(_T("�����Ѵ���!"));
			return false;
		}
	}
	string modelName = path.substr(0,path.find_last_of(".")) + ".OSGB";
	osg::Node* glider = osgDB::readNodeFile(modelName.c_str());
	const osg::BoundingSphere& bs = glider->getBound();
	size = 2.0/(bs.radius()*2);
	size = 1;
	osg::Group* group = new osg::Group;
	

	osg::MatrixTransform* positioned = new osg::MatrixTransform;
	positioned->setDataVariance(osg::Object::STATIC);
	positioned->setMatrix(osg::Matrix::scale(size,size,size)*osg::Matrix::translate(osg::Vec3d(1,1,1)));
	positioned->addChild(glider);
	positioned->setName(ObjectName.c_str());
	positioned->addChild(drawCicle(bs.center() - osg::Vec3d(0,0,1.9),1,osg::Vec4(1,1,0,1)));

	group->addChild(positioned);
	
	CMyUpdateCallback* mCK = new CMyUpdateCallback;
	osg::Geode* geode = mCK->drawLine(vecKeyPath[0],vecKeyPath[0],osg::Vec4(1,1,0,1));
	group->addChild(geode);
	mCK->vecKeyPath.assign(vecKeyPath.begin(),vecKeyPath.end());
	group->setUpdateCallback(mCK);
	group->setName(ObjectName.c_str());
	spViewer3D->AddNode(group);

	return true;
	//////////////////////////////////////////////////////////////////////////
	/*CString strFilter(GETSTRINGT(A2CT(_getClassName()), _T("FilterModel"), _T("ģ���ļ�(*.osg;*.osgb)|*.osg;*.osgb|�����ļ�(*.*)|*.*||")));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal()!=IDOK )
	{
		return false;
	}*/

	fnModel = T2A(dlg.GetPathName());
	if( fnModel.size()<=0 )
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	spViewer3D->Lock();

	m_spEventHandler = new MyMouseEventHandler();
	m_spEventHandler->spViewer3D = spViewer3D;
	m_spEventHandler->m_spAddModel = this;

	spViewer3D->getViewer()->addEventHandler( m_spEventHandler );

	spViewer3D->Unlock();

	bInActive = true;

	return true;
}

bool CvlAddModel::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	spViewer3D->getViewer()->removeEventHandler(m_spEventHandler.get());
	m_spEventHandler = nullptr;


	bInActive = false;

	spViewer3D->Unlock();

	return true;
}

bool CvlAddModel::Checked()
{
	return bInActive;
}

void CvlAddModel::InsertPoint_1m(std::vector<osg::Vec3d>& vecIn,std::vector<osg::Vec3d>& vecOut)
{
	if (vecIn.size()<2)
	{
		vecOut.assign(vecIn.begin(),vecIn.end());
		return;
	}
	vecOut.clear();
	for (size_t i = 0;i<vecIn.size()-1;++i)
	{
		osg::Vec3d v = vecIn[i+1] - vecIn[i];

		double dis = v.length();
		if (dis <= 1)
		{
			continue;
		}
		int num = int(ceil(dis) - 1);//������

		double dx = v.x()/num;
		double dy = v.y()/num;
		double dz = v.z()/num;

		vecOut.push_back(vecIn[i]);
		for(int j = 1;j<=num;++j)
		{
			vecOut.push_back(osg::Vec3d(vecIn[i].x() + dx*j,vecIn[i].y() + dy*j,vecIn[i].z() + dz*j));
		}
	}
}

void CvlAddModel::readPath(const char* filename,std::vector<osg::Vec3d>& vecKeyPt,std::string& ObjectName)
{
	ifstream read_file;  
	read_file.open(filename, ios::in);

	vecKeyPt.clear();
	string line;  
	getline(read_file, ObjectName);//��ȡ����
	 
	while(getline(read_file, line))  
	{  
		if (line.empty())
		{
			continue;
		}
		int pos = line.find_first_of(' ');
		string sx = line.substr(0,pos);
		line = line.substr(pos + 1);
		pos = line.find_first_of(' ');
		string sy = line.substr(0,pos);
		string sz = line.substr(pos+1);

		vecKeyPt.push_back(osg::Vec3d(atof(sx.data()),atof(sy.data()),atof(sz.data())));
	}  
	read_file.close();
}

osg::Geode* CvlAddModel::drawCicle(osg::Vec3d center,float radiu,osg::Vec4 color)
{
	osg::Geode* geode = new osg::Geode;
	std::vector<osg::Vec3d> vec;

	osg::Vec3d p1 = center - osg::Vec3d(radiu,0,0);
	osg::Vec3d p2 = center + osg::Vec3d(radiu,0,0);
	vec.push_back(p1);
	osg::Vec3d p = p1 + osg::Vec3d(0.1,0,0);
	while (p.x() <= p2.x())
	{
		p.y() = center.y() + sqrt(radiu*radiu - (p.x()-center.x())*(p.x()-center.x()));
		vec.push_back(p);
		p.x() += 0.1;
	}

	p = p2 - osg::Vec3d(0.1,0,0);
	vec.push_back(p2);
	while (p.x()>=p1.x())
	{
		p.y() = center.y() - sqrt(radiu*radiu - (p.x()-center.x())*(p.x()-center.x()));
		vec.push_back(p);
		p.x() -= 0.1;
	}
	vec.push_back(p1);
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	for (int i = 0;i<vec.size();++i)
	{
		point->push_back(vec[i]);
	}
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,0,vec.size()));
	geode->addDrawable(lineGeom);

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setAttributeAndModes( new osg::LineWidth(2.5f) );
	ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	return geode;
}

osg::Geode* CMyUpdateCallback::drawLine(osg::Vec3d p1,osg::Vec3d p2,osg::Vec4 color)
{
	osg::Geode* geode = new osg::Geode;
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	point->push_back(p1);
	point->push_back(p2);
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINES,0,2));
	geode->addDrawable(lineGeom);
	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setAttributeAndModes( new osg::LineWidth(2.0f) );
	//ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	return geode;
}

osg::Geode* CMyUpdateCallback::drawPath(std::vector<osg::Vec3d>& vec,osg::Vec4 color)
{
	osg::Geode* geode = new osg::Geode;
	osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
	shared_colors->push_back(color);
	osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
	for (int i = 0;i<vec.size();++i)
	{
		point->push_back(vec[i]);
	}
	osg::Geometry* lineGeom = new osg::Geometry();
	lineGeom->setVertexArray(point.get());
	lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
	lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,0,vec.size()));
	geode->addDrawable(lineGeom);

	osg::StateSet* ss = geode->getOrCreateStateSet();
	ss->setAttributeAndModes( new osg::LineWidth(1.5f) );
	//ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	return geode;
}

CMyUpdateCallback::CMyUpdateCallback(void)
{
	lastPosition = osg::Vec3d(1,1,1);
	Callback_n = 0;
	path_index = 0;
}
CMyUpdateCallback::~CMyUpdateCallback(void)
{

}

void CMyUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if (Callback_n == 30)
	{
		Callback_n = 0;
	}
	else
	{
		Callback_n++;
		return;
	}
	osg::Group* spGroup = node->asGroup();
	if( spGroup!=NULL && spGroup->getNumChildren() == 2 )
	{
		osg::MatrixTransform* posi = dynamic_cast<osg::MatrixTransform*>(spGroup->getChild(0));
		if (posi!=NULL)
		{
			osg::Geode* geode = dynamic_cast<osg::Geode*>(spGroup->getChild(1));
			std::vector<osg::Vec3d> vec;
			if (path_index == 0)
			{
				vec.push_back(vecKeyPath[0]);
				vec.push_back(vecKeyPath[0]);

				int n = geode->getNumDrawables();
				for (int i = n-1;i>0;--i)
				{
					geode->removeDrawables(i);
				}
			}
			else
			{
				vec.push_back(vecKeyPath[path_index -1]);
				vec.push_back(vecKeyPath[path_index]);
				addPath(geode,vec,osg::Vec4(1,1,0,1));
			}
			
			osg::Vec3d posNow = posi->getMatrix().getTrans();
			osg::Vec3d posNew = vecKeyPath[path_index];
			path_index++;
			if (path_index >= vecKeyPath.size())
			{
				path_index = 0;
			}
			osg::Vec3d v = posNew - posNow;
			osg::Vec3d vz = osg::Vec3d(0,-1,0);
			double mo = v.length();
			if (mo == 0)
			{
				return;
			}
			double ji = v.x() * vz.x() + v.y() * vz.y() + v.z() * vz.z();
			double angle = acos(ji / (mo * 1));
			angle = angle * 180 / osg::PI;
			if (v.x()>=0)
			{
				posi->setMatrix(osg::Matrix::scale(size,size,size)*osg::Matrix::rotate(osg::inDegrees(angle),0,0,1)*osg::Matrix::translate(posNew));
			}
			else
				posi->setMatrix(osg::Matrix::scale(size,size,size)*osg::Matrix::rotate(osg::inDegrees(-angle),0,0,1)*osg::Matrix::translate(posNew));
			
		}	
	}
};


//void CMyUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
//{
//	osg::Group* spGroup = node->asGroup();
//	if( spGroup!=NULL )
//	{
//		osg::PositionAttitudeTransform* posi = dynamic_cast<osg::PositionAttitudeTransform*>(spGroup);
//		if (posi!=NULL)
//		{
//			osg::Vec3d posNow = posi->getPosition();
//			osg::Vec3d v = posNow - lastPosition;
//			osg::Vec3d vz = osg::Vec3d(0,-1,0);
//			double mo = v.length();
//			double ji = v.x() * vz.x() + v.y() * vz.y() + v.z() * vz.z();
//			double angle = acos(ji / (mo * 1));
//			angle = angle * 180 / osg::PI;
//			
//			osg::Vec3d scale = posi->getScale();
//			posi->setScale(scale*0.2);
//			
//			osg::Quat qa = posi->getAttitude();
//			if (v.x() >= 0)
//			{
//				
//				qa.makeRotate(angle,0,0,1);
//				posi->setAttitude(qa);
//				//posi->asMatrixTransform()->setMatrix(osg::Matrix::rotate(osg::inDegrees(angle),0,0,1));
//				
//			}
//			else
//			{
//				qa.makeRotate(-angle,0,0,1);
//				posi->setAttitude(qa);
//			}
//			lastPosition = posNow;
//		}
//	}
//
//};