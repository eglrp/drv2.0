#include "stdafx.h"
#include "Stateset.h"
#include "EarthMatrixFunctions.h"
#include "x3py/module/normalobject.h"
#include "IViewer3D.h"
#include "osgViewer/Viewer"
#include "vZCompass.h"
#include "x3py/module/plugininc.h"
#include "x3py/manager/iworkpath.h"
#include "x3py/utilfunc/convstr.h"
#include "osgUtil/LineSegmentIntersector"
#include <afx.h>
#include "osgDB/ConvertUTF"

#include <osg/ShapeDrawable>
#include <osg/Depth>
#include <osg/TextureCubeMap>
#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>

#include <osgViewer/ViewerEventHandlers>
#include "ComFun.h"
#include <sstream>

#include "NodeVisitor.h"
using namespace std;

USING_NAMESPACE_EARTHMATRIX

//////////////////////////////////////////////////////////////////////////
// class to handle events with a pick
class PickHandler : public osgGA::GUIEventHandler {
public:

	PickHandler(){
	}

	x3::Object<IViewer3D> spViewer3D;

	~PickHandler() {}

	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor* nv);

	virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);

protected:

};

bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor* nv)
{
	switch(ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::DOUBLECLICK):
		{
			int nn = ea.getKey();
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				return false;
				/*osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				if (view) pick(view,ea);*/
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				osgUtil::LineSegmentIntersector::Intersections intersections;
				bool b2 = view->computeIntersections(ea, nv->getNodePath(), intersections);

				if (view) pick(view,ea);
				/*
				osgUtil::LineSegmentIntersector::Intersections intersections;

				bool b1 = view->computeIntersections(ea, intersections);
				bool b2 = view->computeIntersections(ea, nv->getNodePath(), intersections);
				bool foundIntersection = view==0 ? false :
					(nv==0 ? view->computeIntersections(ea, intersections) :
					view->computeIntersections(ea, nv->getNodePath(), intersections));

				if (b1 || b2)
				{
					// use the nearest intersection
					const osgUtil::LineSegmentIntersector::Intersection& intersection = *(intersections.begin());
					osg::Drawable* drawable = intersection.drawable.get();
					osg::Geometry* geometry = drawable ? drawable->asGeometry() : 0;
					osg::Vec3Array* vertices = geometry ? dynamic_cast<osg::Vec3Array*>(geometry->getVertexArray()) : 0;
					int nVer = vertices->size();
					if (vertices && vertices->size() == 4)
					{
						Vec3d v3d1 = vertices->at(0) * (*intersection.matrix);
						Vec3d v3d2 = vertices->at(1) * (*intersection.matrix);
						Vec3d v3d3 = vertices->at(2) * (*intersection.matrix);
						Vec3d v3d4 = vertices->at(3) * (*intersection.matrix);

						Vec3d eye,center,up;
						eye.x() = (v3d1.x() + v3d2.x() + v3d3.x() + v3d4.x())/4;
						eye.y() = (v3d1.y() + v3d2.y() + v3d3.y() + v3d4.y())/4;
						eye.z() = (v3d1.z() + v3d2.z() + v3d3.z() + v3d4.z())/4 + 100;

						center.x() = eye.x();
						center.y() = eye.y();
						center.z() = eye.z() - 100;

						//-0.100017,-0.994986,0.000000
						up.x() = -0.1;
						up.y() = -0.99;
						up.z() = 0;
						CString s;
						s.Format(L"%f,%f,%f,%f,%f,%f,%f,%f,%f", eye.x(),eye.y(),eye.z(),center.x(),center.y(),center.z(),up.x(),up.y(),up.z());
						spViewer3D->SetViewport(s);
						
						//////////////////////////////////////////////////////////////////////////
						// get the vertex indices.
						const osgUtil::LineSegmentIntersector::Intersection::IndexList& indices = intersection.indexList;
						const osgUtil::LineSegmentIntersector::Intersection::RatioList& ratios = intersection.ratioList;

						if (indices.size()==3 && ratios.size()==3)
						{
							unsigned int i1 = indices[0];
							unsigned int i2 = indices[1];
							unsigned int i3 = indices[2];

							float r1 = ratios[0];
							float r2 = ratios[1];
							float r3 = ratios[2];

							osg::Array* texcoords = (geometry->getNumTexCoordArrays()>0) ? geometry->getTexCoordArray(0) : 0;
							osg::Vec2Array* texcoords_Vec2Array = dynamic_cast<osg::Vec2Array*>(texcoords);
							if (texcoords_Vec2Array)
							{
								// we have tex coord array so now we can compute the final tex coord at the point of intersection.
								osg::Vec2 tc1 = (*texcoords_Vec2Array)[i1];
								osg::Vec2 tc2 = (*texcoords_Vec2Array)[i2];
								osg::Vec2 tc3 = (*texcoords_Vec2Array)[i3];
								osg::Vec2 tc = tc1*r1 + tc2*r2 + tc3*r3;

								//osg::notify(osg::NOTICE)<<"We hit tex coords "<<tc<<std::endl;

							}
						}
						else
						{
							//osg::notify(osg::NOTICE)<<"Intersection has insufficient indices to work with";
						}

					}

					if( drawable!=NULL )
					{
						osg::StateSet* stateset  = drawable->getStateSet();
						if( stateset )
						{
							osg::StateAttribute* attr = stateset->getTextureAttribute(0,osg::StateAttribute::TEXTURE);
							if (attr)
							{
								osg::ImageStream* pImageStream = NULL;
								osg::Texture2D* texture2D = dynamic_cast<osg::Texture2D*>(attr);
								if (texture2D)
									pImageStream = dynamic_cast<osg::ImageStream*>(texture2D->getImage());
								else
								{
									osg::TextureRectangle* textureRec = dynamic_cast<osg::TextureRectangle*>(attr);
									if (textureRec)
										pImageStream = dynamic_cast<osg::ImageStream*>(textureRec->getImage());
								}

								if( pImageStream )
								{
									osg::ImageStream::StreamStatus playToggle = pImageStream->getStatus();
									if (playToggle != osg::ImageStream::PLAYING)
										pImageStream->play();
									else
										pImageStream->pause();
								}
							}
						}
					}
				}
				*/
			}
			return false;
		}
	default:
		return false;
	}
}

void PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
{
	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	if (view->computeIntersections(ea,intersections))
	{
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			std::ostringstream os;
			if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
			{
				// the geodes are identified by name.
				//os<<"Object \""<<hitr->nodePath.back()->getName()<<"\""<<std::endl;
				
				CString strName = CString( hitr->nodePath.back()->getName().c_str());

				CNodeVisitor bv;
				bv.setName(strName);
				spViewer3D->getRootNode()->accept(bv);

				//pHLH->add(hitr->nodePath.back()->getOrCreateStateSet());
			}
			else if (hitr->drawable.valid())
			{
				//pHLH->add(hitr->drawable->getOrCreateStateSet());
				//os<<"Object \""<<hitr->drawable->className()<<"\""<<std::endl;
				//gPickObj.strName = CString( hitr->drawable->className() );
				//gPickObj.bDone = FALSE;
			}

			//break;
			/*
			os<<"        local coords vertex("<< hitr->getLocalIntersectPoint()<<")"<<"  normal("<<hitr->getLocalIntersectNormal()<<")"<<std::endl;
			os<<"        world coords vertex("<< hitr->getWorldIntersectPoint()<<")"<<"  normal("<<hitr->getWorldIntersectNormal()<<")"<<std::endl;
			const osgUtil::LineSegmentIntersector::Intersection::IndexList& vil = hitr->indexList;
			for(unsigned int i=0;i<vil.size();++i)
			{
				os<<"        vertex indices ["<<i<<"] = "<<vil[i]<<std::endl;
			}
			*/
			//gdlist += os.str();
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// CStatesetCommand
CStatesetCommand::CStatesetCommand(void)
{
    std::wstring app = L"Plugin_Stateset";
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"��ʾģʽ"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"��ͼ"));
}


CStatesetCommand::~CStatesetCommand(void)
{
    UnInitialize();
}

bool CStatesetCommand::Initialize()
{
    x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
    std::wstring path = spAppWorkPath->getWorkPath();
    sWorkDir = x3::w2a(path);
    return true;
}

bool CStatesetCommand::UnInitialize()
{
    return true;
}

bool CStatesetCommand::SetBuddy(x3::IObject* val)
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

bool CStatesetCommand::Enable()
{
    if( !m_spBuddy.valid() )
        return false;

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if( !spViewer3D.valid() )
        return false;

    return spViewer3D->getStateSetManipulator() != nullptr;
}

osgGA::StateSetManipulator* CStatesetCommand::getStateSetManipulator()
{
    if( !m_spBuddy.valid() )
        return nullptr;

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if( !spViewer3D.valid() )
        return nullptr;

    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = spViewer3D->getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return nullptr;

    return spStatesetManipulator.release();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetBackface
CStatesetBackface::CStatesetBackface()
{
    std::wstring app = L"Plugin_Stateset";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"������Ч"));
    Name(GETSTRINGW(app.c_str(), L"Backface_Name", L"����ü�"));
    Tooltip(GETSTRINGW(app.c_str(), L"Backface_Tooltip", L"����ü�"));
    Description(GETSTRINGW(app.c_str(), L"Backface_Description", L"����ü�"));
    BitmapName(GETSTRINGW(app.c_str(), L"Backface_BitmapName", L"StatesetBackface"));
}

CStatesetBackface::~CStatesetBackface()
{

}

bool CStatesetBackface::Checked()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    return spStatesetManipulator->getBackfaceEnabled();
}

bool CStatesetBackface::OnClick()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    spStatesetManipulator->setBackfaceEnabled(!(spStatesetManipulator->getBackfaceEnabled()));
    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetLighting
CStatesetLighting::CStatesetLighting()
{
    std::wstring app = L"Plugin_Stateset";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"������Ч"));
    Name(GETSTRINGW(app.c_str(), L"Lighting_Name", L"�ƹ�"));
    Tooltip(GETSTRINGW(app.c_str(), L"Lighting_Tooltip", L"�ƹ�"));
    Description(GETSTRINGW(app.c_str(), L"Lighting_Description", L"�ƹ�"));
    BitmapName(GETSTRINGW(app.c_str(), L"Lighting_BitmapName", L"StatesetLighting"));
}

CStatesetLighting::~CStatesetLighting()
{

}

bool CStatesetLighting::Checked()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    return spStatesetManipulator->getLightingEnabled();
}

bool CStatesetLighting::OnClick()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    spStatesetManipulator->setLightingEnabled(!(spStatesetManipulator->getLightingEnabled()));
    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetTexture
CStatesetTexture::CStatesetTexture()
{
    std::wstring app = L"Plugin_Stateset";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"������Ч"));
    Name(GETSTRINGW(app.c_str(), L"Texture_Name", L"��ͼ"));
    Tooltip(GETSTRINGW(app.c_str(), L"Texture_Tooltip", L"��ͼ"));
    Description(GETSTRINGW(app.c_str(), L"Texture_Description", L"��ͼ"));
    BitmapName(GETSTRINGW(app.c_str(), L"Texture_BitmapName", L"StatesetTexture"));
}

CStatesetTexture::~CStatesetTexture()
{

}

bool CStatesetTexture::Checked()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    return spStatesetManipulator->getTextureEnabled();
}

bool CStatesetTexture::OnClick()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    spStatesetManipulator->setTextureEnabled(!(spStatesetManipulator->getTextureEnabled()));
    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetPolygonModePoint
CStatesetPolygonModePoint::CStatesetPolygonModePoint()
{
    std::wstring app = L"Plugin_Stateset";
    Name(GETSTRINGW(app.c_str(), L"PolygonModePoint_Name", L"��"));
    Tooltip(GETSTRINGW(app.c_str(), L"PolygonModePoint_Tooltip", L"��ģʽ"));
    Description(GETSTRINGW(app.c_str(), L"PolygonModePoint_Description", L"��ģʽ"));
    BitmapName(GETSTRINGW(app.c_str(), L"PolygonModePoint_BitmapName", L"StatesetPolygonModePoint"));
}

CStatesetPolygonModePoint::~CStatesetPolygonModePoint()
{

}

bool CStatesetPolygonModePoint::Checked()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    return spStatesetManipulator->getPolygonMode() == osg::PolygonMode::POINT;
}

bool CStatesetPolygonModePoint::OnClick()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    spStatesetManipulator->setPolygonMode(Checked() ? osg::PolygonMode::FILL : osg::PolygonMode::POINT);
    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetPolygonModeLine
CStatesetPolygonModeLine::CStatesetPolygonModeLine()
{
    std::wstring app = L"Plugin_Stateset";
    Name(GETSTRINGW(app.c_str(), L"PolygonModeLine_Name", L"�߿�"));
    Tooltip(GETSTRINGW(app.c_str(), L"PolygonModeLine_Tooltip", L"�߿�ģʽ"));
    Description(GETSTRINGW(app.c_str(), L"PolygonModeLine_Description", L"�߿�ģʽ"));
    BitmapName(GETSTRINGW(app.c_str(), L"PolygonModeLine_BitmapName", L"StatesetPolygonModeLine"));
}

CStatesetPolygonModeLine::~CStatesetPolygonModeLine()
{

}

bool CStatesetPolygonModeLine::Checked()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    return spStatesetManipulator->getPolygonMode() == osg::PolygonMode::LINE;
}

bool CStatesetPolygonModeLine::OnClick()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    spStatesetManipulator->setPolygonMode(Checked() ? osg::PolygonMode::FILL : osg::PolygonMode::LINE);
    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetPolygonModeFill
CStatesetPolygonModeFill::CStatesetPolygonModeFill()
{
    std::wstring app = L"Plugin_Stateset";
    Name(GETSTRINGW(app.c_str(), L"PolygonModeFill_Name", L"ʵ��"));
    Tooltip(GETSTRINGW(app.c_str(), L"PolygonModeFill_Tooltip", L"ʵ��ģʽ"));
    Description(GETSTRINGW(app.c_str(), L"PolygonModeFill_Description", L"ʵ��ģʽ"));
    BitmapName(GETSTRINGW(app.c_str(), L"PolygonModeFill_BitmapName", L"StatesetPolygonModeFill"));
}

CStatesetPolygonModeFill::~CStatesetPolygonModeFill()
{

}

bool CStatesetPolygonModeFill::Checked()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    return spStatesetManipulator->getPolygonMode() == osg::PolygonMode::FILL;
}

bool CStatesetPolygonModeFill::OnClick()
{
    osg::ref_ptr<osgGA::StateSetManipulator> spStatesetManipulator = getStateSetManipulator();
    if( !spStatesetManipulator.valid() )
        return false;

    spStatesetManipulator->setPolygonMode(osg::PolygonMode::FILL);
    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetNorthPointer ָ����
CStatesetNorthPointer::CStatesetNorthPointer()
{
    std::wstring app = L"View";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"�������"));
	Name(GETSTRINGW(app.c_str(), L"NorthPointer_Name", L"ָ����"));
	Tooltip(GETSTRINGW(app.c_str(), L"NorthPointer_Tooltip", L"ָ����"));
    Description(GETSTRINGW(app.c_str(), L"NorthPointer_Description", L"ָ����"));
    BitmapName(GETSTRINGW(app.c_str(), L"NorthPointer_BitmapName", L"StatesetNorthPointer"));

    IUICommand::Checked(true);
    m_spCompass = nullptr;
}

CStatesetNorthPointer::~CStatesetNorthPointer()
{

}

bool CStatesetNorthPointer::SetBuddy(x3::IObject* val)
{
    CStatesetCommand::SetBuddy(val);

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if (m_spCompass == nullptr)
    {
        //ָ����
        m_spCompass = new CvZCompass;

        m_spCompass->setMainCamera( spViewer3D->getViewer()->getCamera() );
        m_spCompass->setName("ָ����");

        spViewer3D->Lock();

        spViewer3D->getRootNode()->asGroup()->addChild(m_spCompass.get());

        int nWidth = spViewer3D->getViewer()->getCamera()->getViewport()->width();
        int nHeight = spViewer3D->getViewer()->getCamera()->getViewport()->height();

        m_spCompass->ChangePosition(nWidth, nHeight);

        //֣˼�ɣ�����������һ��ref�������˳���ʱ������
        //ԭ�����this������ָ������߸�OSG�Ĳ���ͬһ��������
        //OSG���Ƴ����handler��ʱ���ּ������0�˾ͻ�ɾ��֮
        //�ȵ����ȥɾ��this��ʱ�򣬾ͻᱨ�ڴ���ʳ�ͻ�Ĵ���
        osg::ref_ptr<osgGA::GUIEventHandler> spGUIEventHandler = dynamic_cast<osgGA::GUIEventHandler*>(this);
        spGUIEventHandler->ref();
        spViewer3D->getViewer()->addEventHandler(spGUIEventHandler.get());

        spViewer3D->Unlock();
    }
    return true;
}

bool CStatesetNorthPointer::Checked()
{
    return m_spCompass == nullptr ? false : (m_spCompass->getNodeMask() != 0);
}

bool CStatesetNorthPointer::OnClick()
{
	if (m_spCompass != nullptr)
	{
		if (m_spCompass->getNodeMask() == 0)
			m_spCompass->setNodeMask(1);
		else
			m_spCompass->setNodeMask(0);
	}

	return true;
}

bool CStatesetNorthPointer::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	//TRACE("Handle\n");
    osg::ref_ptr<osgViewer::View> spView = dynamic_cast<osgViewer::View*>(&aa);
    if( spView == NULL || spView->getCamera() == NULL )
        return false;

    switch(ea.getEventType())
    {
    case osgGA::GUIEventAdapter::RESIZE:
    {
        setWindowSize(spView->getCamera(), ea.getWindowWidth(), ea.getWindowHeight());
    }
    break;

    default:
        break;
    }

    return false;
}

void CStatesetNorthPointer::setWindowSize(osg::Camera* pCamera, int width, int height)
{
    if (m_spCompass.valid())
    {
        m_spCompass->ChangePosition(width, height);
    }
}

bool CStatesetNorthPointer::UnInitialize()
{
    x3::Object<IViewer3D> spViewer3D(m_spBuddy);

    osg::ref_ptr<osgGA::GUIEventHandler> spGUIEventHandler = dynamic_cast<osgGA::GUIEventHandler*>(this);
    spViewer3D->getViewer()->removeEventHandler(spGUIEventHandler.get());

    m_spCompass = nullptr;

    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetViewportInfo �ӵ���Ϣ
CStatesetViewportInfo::CStatesetViewportInfo()
{
    std::wstring app = L"View";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"�������"));
    Name(GETSTRINGW(app.c_str(), L"ViewportInfo_Name", L"�ӵ���Ϣ"));
    Tooltip(GETSTRINGW(app.c_str(), L"ViewportInfo_Tooltip", L"�ӵ���Ϣ"));
    Description(GETSTRINGW(app.c_str(), L"ViewportInfo_Description", L"�ӵ���Ϣ"));
    BitmapName(GETSTRINGW(app.c_str(), L"ViewportInfo_BitmapName", L"StatesetViewportInfo"));

    IUICommand::Checked(true);
    m_spViewportInfo = nullptr;
}

CStatesetViewportInfo::~CStatesetViewportInfo()
{

}

void CStatesetViewportInfo::CreateViewportInfoWin()
{
    m_spViewportInfo = new osg::Camera;
    m_spViewportInfo->setViewMatrix(osg::Matrix::identity());
    m_spViewportInfo->setRenderOrder(osg::Camera::POST_RENDER);
    m_spViewportInfo->setClearMask(GL_DEPTH_BUFFER_BIT); 
    m_spViewportInfo->setAllowEventFocus(false);
    m_spViewportInfo->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    m_spViewportInfo->setProjectionMatrixAsOrtho2D(0, 200, 0, 210);
    m_spViewportInfo->setViewport(10, 10, 200, 210);

    osg::ref_ptr<osg::Geode> gnode = new osg::Geode;
    gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    gnode->addDrawable(text);
	text->setDataVariance(osg::Object::DYNAMIC);
    text->setFont(sWorkDir + "Skins\\Default\\msyh.ttf");
	//text->setFont("cambria.ttc");
    text->setCharacterSize(14);
    text->setText(L"");
    text->setBackdropType(osgText::Text::OUTLINE);
    text->setBackdropImplementation(osgText::Text::NO_DEPTH_BUFFER);
    text->setBackdropColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.5f));
    text->setBackdropOffset(0.1);
    text->setLineSpacing(0.4);

    text->setPosition(osg::Vec3(10, 190, 0.0));

	m_spText = text.get();

    m_spViewportInfo->addChild(gnode);
}

bool CStatesetViewportInfo::SetBuddy(x3::IObject* val)
{
    CStatesetCommand::SetBuddy(val);

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	TRACE("Begin SetBuddy\n");
    if (m_spViewportInfo == nullptr)
    {
		spViewer3D->Lock();

        CreateViewportInfoWin();

        spViewer3D->getRootNode()->asGroup()->addChild(m_spViewportInfo.get());

        int nWidth = spViewer3D->getViewer()->getCamera()->getViewport()->width();
        int nHeight = spViewer3D->getViewer()->getCamera()->getViewport()->height();

        //֣˼�ɣ�����������һ��ref�������˳���ʱ������
        //ԭ�����this������ָ������߸�OSG�Ĳ���ͬһ��������
        //OSG���Ƴ����handler��ʱ���ּ������0�˾ͻ�ɾ��֮
        //�ȵ����ȥɾ��this��ʱ�򣬾ͻᱨ�ڴ���ʳ�ͻ�Ĵ���
        osg::ref_ptr<osgGA::GUIEventHandler> spGUIEventHandler = dynamic_cast<osgGA::GUIEventHandler*>(this);
        spGUIEventHandler->ref();
        spViewer3D->getViewer()->addEventHandler(spGUIEventHandler.get()); 
		
		osg::ref_ptr<PickHandler> pk;
		pk = new PickHandler();
		pk->ref();
		pk->spViewer3D = spViewer3D;
		spViewer3D->getViewer()->addEventHandler(pk.get());

		spViewer3D->Unlock();
    }
	TRACE("End SetBuddy\n");
    return true;
}

bool CStatesetViewportInfo::Checked()
{
    return m_spViewportInfo == nullptr ? false : (m_spViewportInfo->getNodeMask() != 0);
}

bool CStatesetViewportInfo::OnClick()
{
    if (m_spViewportInfo != nullptr)
    {
        if (m_spViewportInfo->getNodeMask() == 0)
            m_spViewportInfo->setNodeMask(1);
        else
            m_spViewportInfo->setNodeMask(0);
    }

    return true;
}

bool CStatesetViewportInfo::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	//TRACE("handle\n");
    osg::ref_ptr<osgViewer::View> spView = dynamic_cast<osgViewer::View*>(&aa);
    if( spView == NULL || spView->getCamera() == NULL )
        return false;

    switch(ea.getEventType())
    {
	case osgGA::GUIEventAdapter::RESIZE:
		{
			
		}
		break;
	case osgGA::GUIEventAdapter::FRAME:
		{
			x3::Object<IViewer3D> spViewer3D(m_spBuddy);

			//�ӵ������ʵʱ��γ��Ϣ;
			osgUtil::LineSegmentIntersector::Intersections results;
			osg::Vec3d worldPos, llhPos;
			float eaX = ea.getX();
			float eaY = ea.getY();
			
			std::wstringstream ssMouse, ssViewpoint;

			if (spViewer3D->getViewer()->computeIntersections(eaX, eaY, results))
			{
				osgUtil::LineSegmentIntersector::Intersections::iterator iter;
				iter = results.begin();
				worldPos.x() = iter->getWorldIntersectPoint().x();
				worldPos.y() = iter->getWorldIntersectPoint().y();
				worldPos.z() = iter->getWorldIntersectPoint().z();
#ifdef _2D
				ssMouse << L"[ ���λ�� ] : \nX : " << RoundOffToString(worldPos.x(),6);
				ssMouse << L"\nY : " << RoundOffToString(worldPos.y(),6);
				ssMouse << L"\nZ : " << RoundOffToString(worldPos.z(),3);
#else
				spViewer3D->getMapNode()->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(worldPos.x(),worldPos.y(),worldPos.z(), llhPos.y(), llhPos.x(), llhPos.z());
				llhPos.x() = osg::RadiansToDegrees(llhPos.x());
				llhPos.y() = osg::RadiansToDegrees(llhPos.y());

				ssMouse << L"[���λ��]��\n���ȣ�" << RoundOffToString(llhPos.x(),6);
				ssMouse << L"\nγ�ȣ�" << RoundOffToString(llhPos.y(),6);
				ssMouse << L"\n�̣߳�" << RoundOffToString(llhPos.z(),3);

				osg::ref_ptr<EarthManipulator> em = spViewer3D->getEarthManipulator();
				if (em)
				{
					osgEarth::Viewpoint vp = em->getViewpoint();

					ssViewpoint << L"\n[�ӵ�λ��]��";
					ssViewpoint << L"\n���ȣ�" << RoundOffToString(vp.focalPoint()->x(), 6);
					ssViewpoint << L"\nγ�ȣ�" << RoundOffToString(vp.focalPoint()->y(), 6);
					ssViewpoint << L"\n�߶ȣ�" << RoundOffToString(vp.getRange(), 3);
					ssViewpoint << L"\n�����ǣ�" << RoundOffToString(vp.getPitch(), 2);
					ssViewpoint << L"\n��λ�ǣ�" << RoundOffToString(vp.getHeading(), 2);
				}
#endif				
			}

#ifdef _2D
			const osg::Matrixd& m = spViewer3D->getViewer()->getCamera()->getInverseViewMatrix();
			osg::AnimationPath::ControlPoint* spCP = new osg::AnimationPath::ControlPoint(m.getTrans(), m.getRotate());

			osg::Vec3d position = spCP->getPosition();
			osg::Quat rotation = spCP->getRotation();

			ssMouse << L"\n\n[ ��ͷ�ӵ� ] : \nX : " << RoundOffToString(rotation.x(),6);
			ssMouse << L"\nY : " << RoundOffToString(rotation.y(),6);
			ssMouse << L"\nZ : " << RoundOffToString(rotation.z(),6);
			ssMouse << L"\nW : " << RoundOffToString(rotation.w(),6);
#else

#endif

			//m_sViewportInfo = (ssMouse.str() + ssViewpoint.str()).c_str();

			spViewer3D->Lock();
			//m_spText->setText( osgDB::convertStringFromCurrentCodePageToUTF8(m_sViewportInfo), osgText::String::ENCODING_UTF8);
			m_spText->setText((ssMouse.str() + ssViewpoint.str()).c_str());
			spViewer3D->Unlock();
		}
		break;
	default:
		break;
    }
    return false;
}

bool CStatesetViewportInfo::UnInitialize()
{
    x3::Object<IViewer3D> spViewer3D(m_spBuddy);

    osg::ref_ptr<osgGA::GUIEventHandler> spGUIEventHandler = dynamic_cast<osgGA::GUIEventHandler*>(this);
    spViewer3D->getViewer()->removeEventHandler(spGUIEventHandler.get());

	m_spViewportInfo = nullptr;
	m_spText = nullptr;

    return true;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetThumb ����ͼ
CStatesetThumb::CStatesetThumb()
{
    std::wstring app = L"View";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"�������"));
    Name(GETSTRINGW(app.c_str(), L"Thumb_Name", L"����ͼ"));
    Tooltip(GETSTRINGW(app.c_str(), L"Thumb_Tooltip", L"����ͼ"));
    Description(GETSTRINGW(app.c_str(), L"Thumb_Description", L"����ͼ"));
    BitmapName(GETSTRINGW(app.c_str(), L"Thumb_BitmapName", L"StatesetThumb"));

    IUICommand::Checked(true);
    m_spThumb = nullptr;
}

CStatesetThumb::~CStatesetThumb()
{

}

bool CStatesetThumb::SetBuddy(x3::IObject* val)
{
    CStatesetCommand::SetBuddy(val);

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);

    if (m_spThumb == nullptr)
    {
        //����ͼ
		m_spThumb = new CvZThumb;
        m_spThumb->setMainCamera( spViewer3D->getViewer()->getCamera() );

        spViewer3D->Lock();

        spViewer3D->getRootNode()->asGroup()->addChild(m_spThumb.get());

        int nWidth = spViewer3D->getViewer()->getCamera()->getViewport()->width();
        int nHeight = spViewer3D->getViewer()->getCamera()->getViewport()->height();

        m_spThumb->ChangePosition(nWidth, nHeight);

        //֣˼�ɣ�����������һ��ref�������˳���ʱ������
        //ԭ�����this������ָ������߸�OSG�Ĳ���ͬһ��������
        //OSG���Ƴ����handler��ʱ���ּ������0�˾ͻ�ɾ��֮
        //�ȵ����ȥɾ��this��ʱ�򣬾ͻᱨ�ڴ���ʳ�ͻ�Ĵ���
        osg::ref_ptr<osgGA::GUIEventHandler> spGUIEventHandler = dynamic_cast<osgGA::GUIEventHandler*>(this);
        spGUIEventHandler->ref();
        spViewer3D->getViewer()->addEventHandler(spGUIEventHandler.get());

		m_spThumb->setNodeMask(0);

        spViewer3D->Unlock();
    }

    return true;
}

bool CStatesetThumb::Checked()
{
    return m_spThumb == nullptr ? false : (m_spThumb->getNodeMask() != 0);
}

bool CStatesetThumb::OnClick()
{
    if (m_spThumb != nullptr)
    {
        if (m_spThumb->getNodeMask() == 0)
            m_spThumb->setNodeMask(1);
        else
            m_spThumb->setNodeMask(0);
    }

    return true;
}

bool CStatesetThumb::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{

    osg::ref_ptr<osgViewer::View> spView = dynamic_cast<osgViewer::View*>(&aa);
    if( spView == NULL || spView->getCamera() == NULL )
        return false;

    switch(ea.getEventType())
    {
    case osgGA::GUIEventAdapter::RESIZE:
    {
        setWindowSize(spView->getCamera(), ea.getWindowWidth(), ea.getWindowHeight());
    }
    break;
	case osgGA::GUIEventAdapter::FRAME:
		{
			x3::Object<IViewer3D> spViewer3D(m_spBuddy);

			osg::ref_ptr<EarthManipulator> em = spViewer3D->getEarthManipulator();
			if (em)
			{
				osgEarth::Viewpoint vp = em->getViewpoint();
#ifdef _2D

#else
				m_spThumb->Lon = vp.focalPoint()->x();
				m_spThumb->Lat = vp.focalPoint()->y();
#endif

				m_spThumb->Height = vp.getRange();
			}
		}
		break;
    default:
        break;
    }
    return false;
}

void CStatesetThumb::setWindowSize(osg::Camera* pCamera, int width, int height)
{
    if (m_spThumb.valid())
    {
        m_spThumb->ChangePosition(width, height);
    }
}

bool CStatesetThumb::UnInitialize()
{
    x3::Object<IViewer3D> spViewer3D(m_spBuddy);

    osg::ref_ptr<osgGA::GUIEventHandler> spGUIEventHandler = dynamic_cast<osgGA::GUIEventHandler*>(this);
    spViewer3D->getViewer()->removeEventHandler(spGUIEventHandler.get());

    m_spThumb = nullptr;

    return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CStatesetSkyBox ��պ�

//��ȡ����ͼ
osg::ref_ptr<osg::TextureCubeMap> readCubeMap(std::string sWorkDir)
{
	osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap;

	osg::ref_ptr<osg::Image> imagePosX = osgDB::readImageFile(sWorkDir + "Data/SkyBox/5.jpg");
	osg::ref_ptr<osg::Image> imageNegX = osgDB::readImageFile(sWorkDir + "Data/SkyBox/4.jpg");
	osg::ref_ptr<osg::Image> imagePosY = osgDB::readImageFile(sWorkDir + "Data/SkyBox/1.jpg");
	osg::ref_ptr<osg::Image> imageNegY = osgDB::readImageFile(sWorkDir + "Data/SkyBox/0.jpg");
	osg::ref_ptr<osg::Image> imagePosZ = osgDB::readImageFile(sWorkDir + "Data/SkyBox/3.jpg");
	osg::ref_ptr<osg::Image> imageNegZ = osgDB::readImageFile(sWorkDir + "Data/SkyBox/2.jpg");

	if (imagePosX.get() && imageNegX.get() && imagePosY.get() && imageNegY.get() && imagePosZ.get() && imageNegZ.get())
	{
		//��������ͼ�����������ͼ
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ.get());

		//����������ģʽ
		cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

		//�����˲������κ�mipmap
		cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	}

	return cubemap.get();
}

//����������ͼ����
struct TexMatCallback : public osg::NodeCallback
{
public:

	TexMatCallback(osg::TexMat& tm) :
	  _texMat(tm)
	  {
		  //
	  }

	  virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	  {
		  osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
		  if (cv)
		  {
			  //�õ�ģ����ͼ����������ת�Ƕ�
			  const osg::Matrix& MV = *(cv->getModelViewMatrix());
			  const osg::Matrix R = osg::Matrix::rotate( osg::DegreesToRadians(112.0f), 0.0f,0.0f,1.0f)*
				  osg::Matrix::rotate( osg::DegreesToRadians(90.0f), 1.0f,0.0f,0.0f);

			  osg::Quat q = MV.getRotate();
			  const osg::Matrix C = osg::Matrix::rotate( q.inverse() );

			  //�����������
			  _texMat.setMatrix( C*R );
		  }

		  traverse(node,nv);
	  }

	  //�������
	  osg::TexMat& _texMat;
};

//һ���任�࣬ʹ��պ����ӵ���ת
class MoveEarthySkyWithEyePointTransform : public osg::Transform
{
public:
	//�ֲ����������������
	virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const 
	{
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
		if (cv)
		{
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.preMult(osg::Matrix::translate(eyePointLocal));
		}
		return true;
	}

	//����������Ϊ�ֲ�����
	virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const
	{
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
		if (cv)
		{
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.postMult(osg::Matrix::translate(-eyePointLocal));
		}
		return true;
	}
};

//������պ�
osg::ref_ptr<osg::Node> createSkyBox(std::string sWorkDir)
{
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//��������ӳ�䷽ʽ��ָ��Ϊ�����ʽ���������е���ɫ����ԭ������ɫ
	osg::ref_ptr<osg::TexEnv> te = new osg::TexEnv;
	te->setMode(osg::TexEnv::REPLACE);
	stateset->setTextureAttributeAndModes(0, te.get(), osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);

	//�Զ������������꣬���䷽ʽ(REFLECTION_MAP)
	/*
	NORMAL_MAP����׼ģʽ������ͼ����
	REFLECTION_MAP������ģʽ����������
	SPHERE_MAP������ģ�ͣ���������
	*/
	osg::ref_ptr<osg::TexGen> tg = new osg::TexGen;
	tg->setMode(osg::TexGen::NORMAL_MAP);
	stateset->setTextureAttributeAndModes(0, tg.get(), osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);

	//�����������
	osg::ref_ptr<osg::TexMat> tm = new osg::TexMat;
	stateset->setTextureAttribute(0, tm.get());

	//��������ͼ����
	osg::ref_ptr<osg::TextureCubeMap> skymap = readCubeMap(sWorkDir);
	stateset->setTextureAttributeAndModes(0, skymap.get(), osg::StateAttribute::ON| osg::StateAttribute::OVERRIDE);

	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	stateset->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );

	//���������ΪԶƽ��
	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(1.0,1.0);//Զƽ��   
	stateset->setAttributeAndModes(depth, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//������Ⱦ˳��Ϊ-1������Ⱦ
	stateset->setRenderBinDetails(-1,"RenderBin");

	osg::ref_ptr<osg::Drawable> drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),1000));

	//��������뵽Ҷ�ڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->setCullingActive(false);
	geode->setStateSet( stateset.get());
	geode->addDrawable(drawable.get());

	//���ñ任
	osg::ref_ptr<osg::Transform> transform = new MoveEarthySkyWithEyePointTransform();
	transform->setCullingActive(false);
	transform->addChild(geode.get());

	osg::ref_ptr<osg::ClearNode> clearNode = new osg::ClearNode;
	clearNode->setCullCallback(new TexMatCallback(*tm));
	clearNode->addChild(transform.get());

	return clearNode.get();
}

CStatesetSkyBox::CStatesetSkyBox()
{
	std::wstring app = L"View";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"�������"));
	Name(GETSTRINGW(app.c_str(), L"SkyBox_Name", L"��պ�"));
	Tooltip(GETSTRINGW(app.c_str(), L"SkyBox_Tooltip", L"��պ�"));
	Description(GETSTRINGW(app.c_str(), L"SkyBox_Description", L"��պ�"));
	BitmapName(GETSTRINGW(app.c_str(), L"SkyBox_BitmapName", L"StatesetSkyBox"));

	x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
	std::wstring path = spAppWorkPath->getWorkPath();
	m_sWorkDir = x3::w2a(path);

	m_bShow = true;

	IUICommand::Checked(true);
}

CStatesetSkyBox::~CStatesetSkyBox()
{

}

bool CStatesetSkyBox::SetBuddy(x3::IObject* val)
{
	CStatesetCommand::SetBuddy(val);

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	//spViewer3D->getSceneData()->asGroup()->addChild(createSkyBox(m_sWorkDir));
	skynode = createSkyBox(m_sWorkDir);
	spViewer3D->getSceneData()->asGroup()->addChild(skynode);
	return true;
}

bool CStatesetSkyBox::Checked()
{
	return skynode == nullptr ? false : (skynode->getNodeMask() != 0);
}

bool CStatesetSkyBox::OnClick()
{
	if (skynode != nullptr)
	{
		if (skynode->getNodeMask() == 0)
			skynode->setNodeMask(1);
		else
			skynode->setNodeMask(0);
	}

	return true;
}

bool CStatesetSkyBox::UnInitialize()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	return true;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CStatesetManipulator::CStatesetManipulator()
{
	std::wstring app = L"View";
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"������"));
	Name(GETSTRINGW(app.c_str(), L"Manipulator_Name", L"������"));
	Tooltip(GETSTRINGW(app.c_str(), L"Manipulator_Tooltip", L"������"));
	Description(GETSTRINGW(app.c_str(), L"Manipulator_Description", L"������"));
	BitmapName(GETSTRINGW(app.c_str(), L"Manipulator_BitmapName", L"StatesetManipulator"));

	x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
	std::wstring path = spAppWorkPath->getWorkPath();
	m_sWorkDir = x3::w2a(path);

	m_bShow = true;

	IUICommand::Checked(true);
}

CStatesetManipulator::~CStatesetManipulator()
{

}

bool CStatesetManipulator::SetBuddy(x3::IObject* val)
{
	CStatesetCommand::SetBuddy(val);

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	spViewer3D->getSceneData()->asGroup()->addChild(createSkyBox(m_sWorkDir));

	return true;
}

bool CStatesetManipulator::Checked()
{
	return m_bShow;
}

bool CStatesetManipulator::OnClick()
{

	return true;
}

bool CStatesetManipulator::UnInitialize()
{
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	return true;
}