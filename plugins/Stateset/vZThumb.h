#pragma once
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgText/Font>
#include <osgViewer/View>
#include <osgText/Text>

using namespace osg;

class CvZThumb :
    public osg::Camera
{
public:
    CvZThumb(void);
    ~CvZThumb(void);

    CvZThumb( const CvZThumb& copy, osg::CopyOp copyop = osg::CopyOp::SHALLOW_COPY );
    META_Node( osg, CvZThumb );
    void setThumb( osg::MatrixTransform* thumb )
    {
        _mapTransform = thumb;
    }
    void setBox( osg::MatrixTransform* box )
    {
        _boxTransform = box;
    }
    void setMainCamera( osg::Camera* camera )
    {
        _mainCamera = camera;
    }
    virtual void traverse( osg::NodeVisitor& nv );

	osg::MatrixTransform* createMap( const std::string& image);
	osg::MatrixTransform* createBox( );
	osg::ref_ptr<osg::Geometry> CreatePolyline3D(osg::Vec3dArray* arr);

	void ChangePosition(int nWidth, int nHeight);

	double Lon,Lat,Height;

protected:
    osg::ref_ptr<osg::MatrixTransform> _mapTransform;
    osg::ref_ptr<osg::MatrixTransform> _boxTransform;
    osg::observer_ptr<osg::Camera> _mainCamera;
	osg::ref_ptr<Geometry> polygon;
};
