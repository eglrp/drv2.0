#include "stdafx.h"
#include "PickModelHandler.h"
#include <osgViewer/Viewer>

void PickModelHandler::Init(){
	_selection = new osgManipulator::Selection;

	_draggerTranslate = new osgManipulator::TranslateAxisDragger;
	_draggerScale = new osgManipulator::ScaleAxisDragger;
	_draggerRotate = new osgManipulator::RotateSphereDragger;
	_dragger = _draggerTranslate.get();

	_draggerTranslate->setupDefaultGeometry();
	_draggerScale->setupDefaultGeometry();
	_draggerRotate->setupDefaultGeometry();

	_draggerTranslate->setNodeMask( 0 );
	_draggerScale->setNodeMask( 0 );
	_draggerRotate->setNodeMask( 0 );

	_manager = new osgManipulator::CommandManager;	
	_manager->connect( *_dragger, *_selection );

	_selectType = MODEL;
	_operationType = TRANSLATE;
}

bool PickModelHandler::handle( const osgGA::GUIEventAdapter &ea, 
							  osgGA::GUIActionAdapter &aa ){
	osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
	if( !view )
		return false;

	if( !_enablePick )
		return view->getCameraManipulator()->handle( ea, aa );

	switch( ea.getEventType() ){
		case osgGA::GUIEventAdapter::PUSH:
			return SelectNode( ea, aa );
			break;

		case osgGA::GUIEventAdapter::DRAG:
		case osgGA::GUIEventAdapter::RELEASE:
			return DragNode( ea, aa );
			break;

		default:
			break;
	}
	return false;
}

bool PickModelHandler::SelectNode( const osgGA::GUIEventAdapter &ea, 
								  osgGA::GUIActionAdapter &aa ){
    osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
	_pointer.reset();
	TestOperationType();
	osgUtil::LineSegmentIntersector::Intersections hits;
	if( view->computeIntersections( ea.getX(), ea.getY(), hits ) ){
		_pointer.setCamera( view->getCamera() );
		_pointer.setMousePosition( ea.getX(), ea.getY() );	
		osgUtil::LineSegmentIntersector::Intersections::iterator hitr;
		for( hitr = hits.begin(); hitr != hits.end(); ++hitr ){
			_pointer.addIntersection( hitr->nodePath, hitr->getLocalIntersectPoint() );
		}
		osg::NodePath::iterator itr;
		for( itr = _pointer._hitList.front().first.begin();
			itr != _pointer._hitList.front().first.end(); ++itr ){
				if( !_hasSelected ){
					switch( _selectType ){
						case GEOD:
							{
								osg::ref_ptr<osg::Geode> gode = dynamic_cast<osg::Geode*>( *itr );
								if( gode ){
									float scale = gode->computeBound().radius() * 16;
									_selection->setMatrix( osg::Matrix::identity() );
									_dragger->setMatrix( osg::Matrix::scale( scale, scale, scale ) * 
										osg::Matrix::translate( gode->computeBound().center() ) );
									gode->getParent( 0 )->addChild( _selection.get() );
									gode->getParent( 0 )->addChild( _dragger.get() );
									_selection->addChild( gode.get() );
									gode->getParent( 0 )->removeChild( gode.get() );
									_dragger->setNodeMask( 1 );
									_hasSelected = true;
									return true;
								}
							}
							break;

						case MODEL:
							{
								osg::ref_ptr<osg::Group> group = dynamic_cast<osg::Group*>( *itr );
								if( group ){
									if( group->getName() == "MODEL" ){
										float scale = group->computeBound().radius() * 1.6;
										_selection->setMatrix( osg::Matrix::identity() );
										_dragger->setMatrix( osg::Matrix::scale( scale, scale, scale ) * 
											osg::Matrix::translate( group->computeBound().center() ) );
										group->getParent( 0 )->addChild( _selection.get() );
										group->getParent( 0 )->addChild( _dragger.get() );
										_selection->addChild( group.get() );
										group->getParent( 0 )->removeChild( group.get() );
										_dragger->setNodeMask( 1 );
										_hasSelected = true;
										return true;
									}
								}
							}
							break;
					}
				}else{
					osgManipulator::Dragger* dragger = dynamic_cast<osgManipulator::Dragger*>( *itr );
					if( dragger ){
						dragger->handle( _pointer, ea, aa );
						_activeDragger = dragger;
						return true;
					}
				}
		}
	}else{
		_draggerTranslate->setNodeMask( 0 );
		_draggerRotate->setNodeMask( 0 );
		_draggerScale->setNodeMask( 0 );
		_hasSelected = false;
		if( _selection->getNumChildren() != 0 ){
			osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>( _selection->getParent( 0 ) );
			if( mt ){
				mt->setMatrix( mt->getMatrix() * _selection->getMatrix() );
				mt->removeChild( _selection.get() );
				mt->removeChild( _dragger.get() );
				mt->addChild( _selection->getChild( 0 ) );
				_selection->removeChild( _selection->getChild( 0 ) );
			}else{
				osg::MatrixTransform* mt = new osg::MatrixTransform;
				mt->setMatrix( _selection->getMatrix() );
				_selection->getParent( 0 )->addChild( mt );
				_selection->getParent( 0 )->removeChild( _dragger.get() );
				_selection->getParent( 0 )->removeChild( _selection.get() );
				mt->addChild( _selection->getChild( 0 ) );
				_selection->removeChild( _selection->getChild( 0 ) );
			}
		}
		return view->getCameraManipulator()->handle( ea, aa );
	}
	return false;
}

bool PickModelHandler::DragNode( const osgGA::GUIEventAdapter &ea, 
								osgGA::GUIActionAdapter &aa ){
	osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
	if( _activeDragger ){
		_pointer._hitIter = _pointer._hitList.begin();
		_pointer.setCamera( view->getCamera() );
		_pointer.setMousePosition( ea.getX(), ea.getY() );
		_activeDragger->handle( _pointer, ea, aa );
		if( ea.getEventType() == osgGA::GUIEventAdapter::RELEASE ){
			_activeDragger = NULL;
			_pointer.reset();
		}
		return true;
	}else
		return view->getCameraManipulator()->handle( ea, aa );
}

void PickModelHandler::SetOperationType( OPERATION type ){
	if( !_hasSelected )
		_operationType = type;
}

void PickModelHandler::TestOperationType(){
	_manager->disconnect( *_dragger );
	switch( _operationType ){
		case TRANSLATE:
			_dragger = _draggerTranslate.get();
			break;
		case ROTATE:
			_dragger = _draggerRotate.get();
			break;
		case SCALE:
			_dragger = _draggerScale.get();
			break;
	}
	_manager->connect( *_dragger, *_selection );
}