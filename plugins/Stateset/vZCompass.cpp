#include "StdAfx.h"
#include "vZCompass.h"
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include "x3py/module/plugininc.h"
#include "x3py/manager/iworkpath.h"
#include "x3py/utilfunc/convstr.h"

CvZCompass::CvZCompass(void)
{
	x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
	std::wstring path = spAppWorkPath->getWorkPath();

	std::string compassPath = x3::w2a(path) + "Skins\\Default\\Compass.png";

    setPlate( createCompass(compassPath, 1.5f, -1.0f) );
    setRenderOrder( osg::Camera::POST_RENDER );
    setClearMask( GL_DEPTH_BUFFER_BIT );
    setAllowEventFocus( false );
    setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
    setViewport( 1600 - 64 - 10, 800 - 64 - 10, 64.0, 64.0 );
    setProjectionMatrix( osg::Matrixd::ortho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0) );
}

void CvZCompass::ChangePosition(int nWidth, int nHeight)
{
	setViewport( nWidth - 64 - 10, nHeight - 64 - 10, 64.0, 64.0 );
}


CvZCompass::~CvZCompass(void)
{
}

CvZCompass::CvZCompass( const CvZCompass& copy, osg::CopyOp copyop ):
    osg::Camera(copy, copyop),
    _plateTransform(copy._plateTransform),
    _needleTransform(copy._needleTransform),
    _mainCamera(copy._mainCamera)
{
}

void CvZCompass::traverse( osg::NodeVisitor& nv )
{
    if ( _mainCamera.valid() &&
            nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR )
    {
        osg::Matrix matrix = _mainCamera->getViewMatrix();
        matrix.setTrans( osg::Vec3() );
        osg::Vec3 northVec = osg::Y_AXIS * matrix;
        northVec.z() = 0.0f;
        northVec.normalize();
        osg::Vec3 axis = osg::Y_AXIS ^ northVec;
        float angle = atan2(axis.length(), osg::Y_AXIS * northVec);
        axis.normalize();
        if ( _plateTransform.valid() )
            _plateTransform->setMatrix( osg::Matrix::rotate(angle, axis) );
    }
    _plateTransform->accept( nv );
    //_needleTransform->accept( nv );
    osg::Camera::traverse( nv );
}

osg::MatrixTransform* CvZCompass::createCompass( const std::string& image, float radius, float height )
{
    osg::Vec3 center(-radius, -radius, height);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(createTexturedQuadGeometry(center, osg::Vec3(radius * 2.0f, 0.0f, 0.0f), osg::Vec3(0.0f, radius * 2.0f, 0.0f)) );
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage( osgDB::readImageFile(image) );
    osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
    part->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get() );
    part->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    part->addChild( geode.get() );
    return part.release();
}