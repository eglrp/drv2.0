#pragma once
#include "osgGA/GUIEventHandler"
#include "osgViewer/Viewer"
#include "osg/Geode"
#include "osg/Geometry"
#include "osg/PositionAttitudeTransform"
#include "osgText/Text"
#include "osg/Camera"

class CvlMeasureToolHandler : public osgGA::GUIEventHandler
{
public:
	class MeasureToolEventHandler : public osg::Referenced
	{
	public:
		virtual void onDistanceChanged(CvlMeasureToolHandler* sender, double distance, double distance1, double distance2) {}
		virtual ~MeasureToolEventHandler() { }
	};

	typedef std::list< osg::ref_ptr< MeasureToolEventHandler > > MeasureToolEventHandlerList;


	CvlMeasureToolHandler( osg::Group* group );        
	virtual ~CvlMeasureToolHandler();

	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );        

	bool getLocationAt(const osgGA::GUIEventAdapter& ea, osgViewer::Viewer* viewer, osg::Vec3d &outPt);

	void clear();
	void rebuildGeometry();

	void addEventHandler(MeasureToolEventHandler* handler );

	void setMouseButton(int mouseButton);
	int getMouseButton() const;

	void setIsPath( bool path );
	bool getIsPath() const;

	void setIsPoint(bool b){_isPoint = b;}

	int getMaxNum() { return _maxNum; }
	void setMaxNum(int num) { _maxNum = num; }

	void CreateViewportInfoWin();

	/** Sets the style of the measurment line (to something other than the default) */
	//void setLineStyle( const Style& style );
	//const Style& getLineStyle() const { return _feature->style().value(); }

	void setIntersectionMask( osg::Node::NodeMask intersectionMask ) { _intersectionMask = intersectionMask; }
	osg::Node::NodeMask getIntersectionMask() const { return _intersectionMask;}

	//osgEarth::Features::Feature* getFeature() const { return _feature.get(); }

protected:
	//GeoInterpolation _geoInterpolation;
	void fireDistanceChanged();
	bool _lastPointTemporary;
	bool _gotFirstLocation;
	bool _finished;
	bool _mouseDown;
	int  _maxNum;
	float _mouseDownX, _mouseDownY;
	int _mouseButton;
	int _mouseButtonRight;
	osg::ref_ptr< osg::Group > _group;

	osg::Vec3 _originPos;
	osg::ref_ptr<osg::Geode> _geode;
	osg::ref_ptr<osg::PositionAttitudeTransform> _positionAttitudeTransform;
	osg::ref_ptr<osg::Geometry> _geometry;
	osg::ref_ptr<osg::Vec3Array> _vertices;
	osg::Vec3 _lastPt;

	MeasureToolEventHandlerList _eventHandlers;
	bool _isPath;
	bool _isPoint;
	osg::Node::NodeMask _intersectionMask;

	osg::ref_ptr<osg::Camera> m_spViewportInfo;
	std::string m_sViewportInfo;
	osg::ref_ptr<osgText::Text> m_spText;

	void rebuild();
};

