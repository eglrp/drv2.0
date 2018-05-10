#pragma once
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgText/Font>
#include <osgViewer/View>
#include <osgText/Text>

using namespace osg;

class CvZCompass :
    public osg::Camera
{
public:
    CvZCompass(void);
    ~CvZCompass(void);

    CvZCompass( const CvZCompass& copy, osg::CopyOp copyop = osg::CopyOp::SHALLOW_COPY );
    META_Node( osg, CvZCompass );
    void setPlate( osg::MatrixTransform* plate )
    {
        _plateTransform = plate;
    }
    void setNeedle( osg::MatrixTransform* needle )
    {
        _needleTransform = needle;
    }
    void setMainCamera( osg::Camera* camera )
    {
        _mainCamera = camera;
    }
    virtual void traverse( osg::NodeVisitor& nv );

	osg::MatrixTransform* createCompass( const std::string& image, float radius, float height );

	void ChangePosition(int nWidth, int nHeight);


protected:
    osg::ref_ptr<osg::MatrixTransform> _plateTransform;
    osg::ref_ptr<osg::MatrixTransform> _needleTransform;
    osg::observer_ptr<osg::Camera> _mainCamera;
};
