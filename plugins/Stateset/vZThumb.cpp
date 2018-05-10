#include "stdafx.h"
#include "vZThumb.h"
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include "x3py/module/plugininc.h"
#include "x3py/manager/iworkpath.h"
#include "x3py/utilfunc/convstr.h"


CvZThumb::CvZThumb(void)
{
    x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
    std::wstring path = spAppWorkPath->getWorkPath();

    setThumb( createMap(x3::w2a(path) + "Skins\\Default\\Thumb.png") );
    setBox( createBox() );

    setRenderOrder( osg::Camera::POST_RENDER );
    setAllowEventFocus( false );
    setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
    setViewport( 1600 - 256 - 10, 10, 256, 128 );
    setProjectionMatrix( osg::Matrixd::ortho(-1.0, 1.0, -0.5, 0.5, -10.0, 10.0) );
}


CvZThumb::~CvZThumb(void)
{
}

CvZThumb::CvZThumb( const CvZThumb& copy, osg::CopyOp copyop ):
    osg::Camera(copy, copyop),
    _mapTransform(copy._mapTransform),
    _boxTransform(copy._boxTransform),
    _mainCamera(copy._mainCamera)
{
}

void CvZThumb::ChangePosition(int nWidth, int nHeight)
{
    setViewport( nWidth - 256 - 10, 10, 256, 128 );
}

void CvZThumb::traverse( osg::NodeVisitor& nv )
{
    if ( _mainCamera.valid() &&
            nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR )
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();

        double lon = 0, lat = 0, dWidth = 0.5, dHeight = -0.99;

        lon = Lon / 180.0;
        dWidth = Height / 1000.0 / 1000.0 / 22.0 / 2.0;
        lat = Lat / 90.0;

        vertices->push_back (osg::Vec3d ( lon - dWidth, lat + dWidth, dHeight));
        vertices->push_back (osg::Vec3d ( lon + dWidth, lat + dWidth, dHeight));
        vertices->push_back (osg::Vec3d ( lon + dWidth, lat - dWidth, dHeight));
        vertices->push_back (osg::Vec3d ( lon - dWidth, lat - dWidth, dHeight));

        polygon->setVertexArray(vertices);
    }
    _mapTransform->accept( nv );
    _boxTransform->accept( nv );
    osg::Camera::traverse( nv );
}

osg::MatrixTransform* CvZThumb::createMap( const std::string& image)
{
    osg::Vec3 center(-1, -0.5, -1);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(createTexturedQuadGeometry(center, osg::Vec3(2.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, 0.0f)) );
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage( osgDB::readImageFile(image) );
    osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
    part->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get() );
    part->addChild( geode.get() );
    return part.release();
}

osg::MatrixTransform* CvZThumb::createBox()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    double lon = 0, lat = 0, dWidth = 0.5, dHeight = -0.99;
    osg::ref_ptr<osg::Vec3dArray> vertices (new osg::Vec3dArray());
    vertices->push_back (osg::Vec3d ( lon - dWidth, lat + dWidth, dHeight));
    vertices->push_back (osg::Vec3d ( lon + dWidth, lat + dWidth, dHeight));
    vertices->push_back (osg::Vec3d ( lon + dWidth, lat - dWidth, dHeight));
    vertices->push_back (osg::Vec3d ( lon - dWidth, lat - dWidth, dHeight));

    geode->addDrawable(CreatePolyline3D(vertices.get()));

    osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
    part->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    part->addChild( geode.get() );

    return part.release();
}

osg::ref_ptr<osg::Geometry> CvZThumb::CreatePolyline3D(osg::Vec3dArray* arr)
{
    polygon = new osg::Geometry;

    //设置顶点
    polygon->setVertexArray(arr);

    //设置法线
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
    (*normals)[0].set( 0.0f, -1.0f, 0.0f );
    polygon->setNormalArray( normals.get() );
    polygon->setNormalBinding( osg::Geometry::BIND_OVERALL );

    //设置颜色
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
    (*colors)[0].set( 1.0f, 1.0f, 0.0f, 1.0f );
    polygon->setColorArray( colors.get() );
    polygon->setColorBinding( osg::Geometry::BIND_OVERALL );

    //设置几何体类型
    polygon->addPrimitiveSet( new osg::DrawArrays(GL_LINE_LOOP, 0, 4) );

    //设置线宽
    polygon->getOrCreateStateSet()->setAttribute( new osg::LineWidth(0.5f) );

    return polygon.get();
}