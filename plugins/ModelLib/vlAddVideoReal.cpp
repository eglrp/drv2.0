#include "stdafx.h"
#include "vlAddVideoReal.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/WriteFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/LineWidth>

#include "osg/PositionAttitudeTransform"
#include "osg/CullFace"
#include "osg/PolygonMode"
#include "osgDB/FileUtils"

#include "osgUtil/SmoothingVisitor"

#include <sstream>
#include <io.h>
#include <atlconv.h>
#include <atlstr.h>

#include "DlgRealVideo.h"

extern std::string fnVideo;

/** create quad at specified position. */
osg::Drawable* MyMouseEventHandlerVideoReal::createVideoSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image)
{
	// set up the Geometry.
	osg::Geometry* geom = new osg::Geometry;

	osg::Vec3Array* coords = new osg::Vec3Array(4);
	(*coords)[0] = corner;
	(*coords)[1] = corner+width;
	(*coords)[2] = corner+width+height;
	(*coords)[3] = corner+height;


	geom->setVertexArray(coords);

	osg::Vec3Array* norms = new osg::Vec3Array(1);
	(*norms)[0] = width^height;
	(*norms)[0].normalize();

	geom->setNormalArray(norms, osg::Array::BIND_OVERALL);

	osg::Vec2Array* tcoords = new osg::Vec2Array(4);
	(*tcoords)[3].set(0.0f,0.0f);
	(*tcoords)[2].set(1.0f,0.0f);
	(*tcoords)[1].set(1.0f,1.0f);
	(*tcoords)[0].set(0.0f,1.0f);
	geom->setTexCoordArray(0,tcoords);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	//int i=0;
	//osg::Vec4Array* colors = new osg::Vec4Array;
	//colors = new osg::Vec4Array;
	//colors->push_back(osg::Vec4(0.8-0.1*i,0.1*i,0.2*i, 1.0));
	//geom->setColorArray(colors, osg::Array::BIND_OVERALL);

	if (image)
	{
		osg::StateSet* dstate = new osg::StateSet;
		osg::Texture2D* texture = new osg::Texture2D;

		texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
		texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
		texture->setResizeNonPowerOfTwoHint(false);

		texture->setImage(image);

		//set up the drawstate.
		dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
		dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		dstate->setTextureAttributeAndModes(0, texture,osg::StateAttribute::OFF);

		geom->setStateSet(dstate);
	}

	return geom;
}

osg::Node* MyMouseEventHandlerVideoReal::createVideoModel(osg::Vec3d v3d)
{
	// create the root node which will hold the model.
	osg::Group* root = new osg::Group();

	// add the drawable into a single goede to be shared...
	osg::Geode* geode = new osg::Geode();
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(fnVideo + ".ffmpeg");
	float y = 1.0;
	float s = image->s();
	float t = image->t();
	float x = y*(s/t);
	geode->addDrawable(
		createVideoSquare(
		osg::Vec3(v3d.x(), v3d.y(), v3d.z() + 2),
		osg::Vec3(6.0f*x, 0.0f, 0.0f),
		osg::Vec3(0.0f, 6.0f*y, 0.0f),
		image.get())
		);

	root->addChild(geode);
	root->setName(("实时视频"));
	return root;
}

MyMouseEventHandlerVideoReal::MyMouseEventHandlerVideoReal()
{

}

bool MyMouseEventHandlerVideoReal::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
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

			spViewer3D->Lock();

			osg::Node* node = createVideoModel(pos);
			spViewer3D->AddNode(node);
			m_spMovieHandler->addNode(node);
			m_spMovieHandler->play();

			//osg::Node* glider = osgDB::readNodeFile("c:\\osg\\data\\cow.osg");
			//const osg::BoundingSphere& bs = glider->getBound();
			//osg::MatrixTransform* positioned = new osg::MatrixTransform;
			//positioned->setDataVariance(osg::Object::STATIC);
			//positioned->setMatrix(osg::Matrix::translate(pos));

			//positioned->addChild(glider);

			//spViewer3D->AddNode(positioned);

			spViewer3D->Unlock();
		}
	default:
		return false;
	}
}


osg::Vec3d MyMouseEventHandlerVideoReal::getPos(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
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

CvlAddVideoReal::CvlAddVideoReal(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"添加视频"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"在线视频管理"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"添加视频"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"添加视频"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"AddVideoReal"));
}

std::vector<std::string> CvlAddVideoReal::split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size=str.size();

	for( int i=0; i<size; i++ )
	{
		pos = str.find(pattern, i);
		if(pos<size)
		{
			std::string s = str.substr(i, pos-i);
			if( s.size()>0 )
				result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}

	return result;
}

std::string CvlAddVideoReal::ToOSGNode(std::string imageFilePath, std::string inFilePath)
{
	float maxDistance = 1000;
	float minPixel = 20.0f;

    osg::ref_ptr<osg::Image> spImage = osgDB::readRefImageFile(imageFilePath);
    if( !spImage.valid() )
    {
        printf("Open Image File %s Failed.\n", imageFilePath.c_str());
        return "";
    }

    if (osgDB::findDataFile(inFilePath).empty() )
    {
        printf("Open Config File %s Failed.\n", inFilePath.c_str());
        return "";
    }

	std::string outFilePath = "";
	if( outFilePath.size()<=0 )	
		outFilePath = osgDB::getFilePath(inFilePath) + "\\" + osgDB::getStrippedName(inFilePath) + ".osgb";

    std::ifstream inFile(inFilePath);
    if( !inFile.is_open() )
    {
        printf("Open Config File %s Failed.\n", inFilePath.c_str());
        return "";
    }

    std::string line;
    int nPolyCount = 0;
    if( std::getline(inFile, line) )
        nPolyCount = atoi(line.c_str());
    if( nPolyCount <= 0 )
    {
        inFile.close();
        return "";
    }

    osg::ref_ptr<osg::Geode> spGeode = new osg::Geode;
    spGeode->setName(osgDB::getStrippedName(inFilePath));
    osg::ref_ptr<osg::Geometry> spGeometry = new osg::Geometry;
    spGeode->addDrawable(spGeometry.get());

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec2Array> uvs = new osg::Vec2Array;
    osg::Vec3d center(0.0, 0.0, 0.0);
    bool first = true;
    for( int i = 0; i < nPolyCount; i++ )
    {
        int nPtCount = 0;
        if( std::getline(inFile, line) )
            nPtCount = atoi(line.c_str());
        if( nPtCount <= 0 )
            continue;

        // 设置多边形索引
        osg::ref_ptr<osg::DrawElementsUInt> spDrawElementsUInt = new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
        for( int j = 0; j < nPtCount; j++ )
        {
            if( std::getline(inFile, line) )
            {
                std::vector<std::string> vals = split(line, " ");
                if( vals.size() < 5 )
					continue;

                if( first )
                {
                    center.set(atof(vals.at(0).c_str()), atof(vals.at(1).c_str()), atof(vals.at(2).c_str()));
                    first = false;
                }

                osg::Vec3 pos(atof(vals.at(0).c_str()) - center.x(), atof(vals.at(1).c_str()) - center.y(), atof(vals.at(2).c_str()) - center.z());
                osg::Vec2 uv(atof(vals.at(3).c_str()), atof(vals.at(4).c_str()));

                //避免添加重复点
                unsigned int m = 0U;
                for(; m < vertices->size(); m++)
                {
                    if( vertices->at(m) == pos && uvs->at(m) == uv )
                    {
                        spDrawElementsUInt->push_back(m);
                        break;
                    }
                }

                if( m < vertices->size() )
                    continue;

                spDrawElementsUInt->push_back(vertices->size());
                vertices->push_back(pos);
                uvs->push_back(uv);
            }
        }

        if( spDrawElementsUInt->size() >= 3 )
            spGeometry->addPrimitiveSet(spDrawElementsUInt.get());
    }
    inFile.close();

    if( spGeometry->getNumPrimitiveSets() > 0 )
    {
        spGeometry->setVertexArray(vertices.get());
        spGeometry->setTexCoordArray(0U, uvs.get(), osg::Array::BIND_PER_VERTEX);
        osgUtil::SmoothingVisitor::smooth(*(spGeometry.get()));//自动生成法线
        osg::ref_ptr<osg::StateSet> spStateset = spGeode->getOrCreateStateSet();
        if( spStateset.valid() )
        {
            spStateset->setMode( GL_BLEND, osg::StateAttribute::ON );
            spStateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
            spStateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
// 			spStateset->setAttribute( new osg::CullFace(osg::CullFace::BACK) );
// 			spStateset->setMode(GL_CULL_FACE,osg::StateAttribute::ON);

            osg::ref_ptr<osg::Texture2D> spTexture = new osg::Texture2D;
            spTexture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
            spTexture->setImage(spImage.get());
            spStateset->setTextureAttributeAndModes(0, spTexture.get(), osg::StateAttribute::ON);

            // 获取纹理文件与输出的osg文件的相对路径，避免使用绝对路径
            std::string relImagePath;
            if( osgDB::getPathRoot(outFilePath) != osgDB::getPathRoot(imageFilePath) )
            {
                relImagePath = imageFilePath;
            }
            else
            {
                relImagePath = osgDB::getPathRelative(osgDB::getFilePath(outFilePath), osgDB::getFilePath(imageFilePath));
                if( relImagePath.size() > 0 )
                    relImagePath += "/" + osgDB::getSimpleFileName(imageFilePath);
                else
                    relImagePath = osgDB::getSimpleFileName(imageFilePath);
            }
            spImage->setFileName(relImagePath);
        }

        osg::ref_ptr<osg::Node> spNode = spGeode;
        if( maxDistance < FLT_MAX || minPixel > FLT_EPSILON )
        {
            osg::ref_ptr<osg::LOD> spLOD = new osg::LOD();
            spLOD->setName(spGeode->getName());
            /*
            spLOD->setCenter(spGeode->getBoundingBox().center());
            spLOD->setRadius(spGeode->getBoundingBox().radius());
            spLOD->setCenterMode(osg::LOD::USER_DEFINED_CENTER);
            */
            if( maxDistance < FLT_MAX )
            {
                spLOD->setRangeMode(osg::LOD::DISTANCE_FROM_EYE_POINT);
                spLOD->addChild(spGeode.get(), 0.0f, maxDistance);
            }
            else
            {
                spLOD->setRangeMode(osg::LOD::PIXEL_SIZE_ON_SCREEN);
                spLOD->addChild(spGeode.get(), minPixel, FLT_MAX);
            }

            spNode = spLOD;
        }

        osg::ref_ptr<osg::PositionAttitudeTransform> spPositionAttitudeTransform = new osg::PositionAttitudeTransform;
        spPositionAttitudeTransform->setName(spGeode->getName());
        spPositionAttitudeTransform->setPosition(center);
        spPositionAttitudeTransform->addChild(spNode.get());

		osg::ref_ptr<osgDB::Options> spOptions = new osgDB::Options;
		spOptions->setPluginStringData("WriteImageHint", "UseExternal");
		osgDB::writeNodeFile(*(spPositionAttitudeTransform.get()), outFilePath, spOptions.get()); 
    }

	return outFilePath;
}

CvlAddVideoReal::~CvlAddVideoReal(void)
{
}

bool CvlAddVideoReal::Initialize()
{
	bInActive = false;
	m_spMovieHandler = nullptr;
    return true;
}

bool CvlAddVideoReal::UnInitialize()
{
    return true;
}

bool CvlAddVideoReal::SetBuddy(x3::IObject* val)
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
bool CvlAddVideoReal::OnClick()
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
    node = osgDB::readNodeFile("X:\\工作目录\\杭州电力系统\\50线路数据.OSGB");

	mt->addChild(node.get());
	mt->setName("50线路数据");

	spViewer3D->AddNode(mt.get());
	*/
	return true;
}

extern std::string g_sRealVideoPath;
extern std::string g_sConfigPath;

void CvlAddVideoReal::activaLocalVideoHander(std::string slocalVideo,std::string slocalVideoConfig)
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if (osgDB::fileExists(slocalVideoConfig))
	{
		spViewer3D->Lock();
		std::string nodefile = ToOSGNode(slocalVideo + ".ffmpeg", slocalVideoConfig);

		osg::ref_ptr<osg::Node> pnode = osgDB::readNodeFile(nodefile);
		pnode->setName(osgDB::getStrippedName(slocalVideo));
		spViewer3D->AddNode(pnode);

		if (m_spMovieHandler == nullptr)
		{
			m_spMovieHandler = new MovieEventHandler();
			m_spMovieHandler->setMouseTracking(false);
			//m_spMovieHandler->set(spViewer3D->getSceneData());
			m_spMovieHandler->addNode(pnode);
			m_spMovieHandler->play();
			spViewer3D->getViewer()->addEventHandler( m_spMovieHandler );
		}
		else
		{
			m_spMovieHandler->addNode(pnode);
			m_spMovieHandler->play();
		}

		spViewer3D->Unlock();
		return ;
	}

	if( slocalVideo.size() <= 0 )
	{
		return ;
	}
	//////////////////////////////////////////////////////////////////////////

	spViewer3D->Lock();

	m_spEventHandler1 = new MyMouseEventHandlerVideo();
	m_spEventHandler1->spViewer3D = spViewer3D;
//	m_spEventHandler1->m_spAddModel = this;

	spViewer3D->getViewer()->addEventHandler( m_spEventHandler1 );

	spViewer3D->Unlock();

	bInActive = true;

	if (m_spMovieHandler == nullptr)
	{
		m_spMovieHandler = new MovieEventHandler();
		m_spMovieHandler->setMouseTracking(false);
		m_spMovieHandler->set(spViewer3D->getSceneData());
		m_spMovieHandler->play();
		spViewer3D->getViewer()->addEventHandler( m_spMovieHandler );
	}

	m_spEventHandler1->m_spMovieHandler = m_spMovieHandler.get();

	return ;
}

bool CvlAddVideoReal::Activate()
{
	USES_CONVERSION;

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	//////////////////////////////////////////////////////////////////////////
	AFX_MANAGE_STATE_EX;
	CDlgRealVideo* dlg = new CDlgRealVideo();
	//dlg->Create(CDlgRealVideo::IDD);
	if( dlg->DoModal()!=IDOK )
	{
		return false;
	}
	if (dlg->isLocal)
	{
		activaLocalVideoHander(dlg->slocalVideo,dlg->slocalVideoConfig);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	//fnVideo = "rtsp://admin:admin123456@192.168.0.246:554/h264/1/main/av_stream";
	//fnVideo = "rtsp://admin:admin123456@27.17.16.6:554/h264/1/main/av_stream";
	fnVideo = g_sRealVideoPath;

	g_sRealVideoPath = "";

	if( fnVideo.size()<=0 )
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////

	std::string configFile = g_sConfigPath;// = T2A(dlg.GetPathName());
	g_sConfigPath = "";

	//////////////////////////////////////////////////////////////////////////
	spViewer3D->Lock();

	std::string nodefile = ToOSGNode(fnVideo + ".ffmpeg", configFile);

	osg::ref_ptr<osg::Node> pnode = osgDB::readNodeFile(nodefile);
	pnode->setName(osgDB::getStrippedName(configFile));
	spViewer3D->AddNode(pnode);
	spViewer3D->GetLayersArray()->Add( CString(nodefile.c_str()) );
	spViewer3D->GetLayerCheck()->Add(_T("1"));

	if (m_spMovieHandler == nullptr)
	{
		m_spMovieHandler = new MovieEventHandler();
		m_spMovieHandler->setMouseTracking(false);
		m_spMovieHandler->addNode(pnode);
		m_spMovieHandler->play();
		spViewer3D->getViewer()->addEventHandler( m_spMovieHandler );
	}
	else
	{
		m_spMovieHandler->addNode(pnode);
		m_spMovieHandler->play();
	}

	spViewer3D->Unlock();

	/*

	bInActive = true;

	if (m_spMovieHandler == nullptr)
	{
		m_spMovieHandler = new MovieEventHandler();
		m_spMovieHandler->setMouseTracking(false);
		m_spMovieHandler->set(spViewer3D->getSceneData());
		m_spMovieHandler->play();
		spViewer3D->getViewer()->addEventHandler( m_spMovieHandler );
	}

	m_spEventHandler->m_spMovieHandler = m_spMovieHandler.get();
	*/
	return true;
}

bool CvlAddVideoReal::Deactivate()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->Lock();

	spViewer3D->getViewer()->removeEventHandler(m_spEventHandler.get());
	spViewer3D->getViewer()->removeEventHandler(m_spEventHandler1.get());
	m_spEventHandler = nullptr;
	m_spEventHandler1 = nullptr;

	bInActive = false;

	spViewer3D->Unlock();

	return true;
}

bool CvlAddVideoReal::Checked()
{
	return bInActive;
}