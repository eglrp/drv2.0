#ifndef _PICKMODELHANDLER_H_
#define _PICKMODELHANDLER_H_

#include <osgGA/GUIEventHandler>
#include <osgManipulator/Dragger>
#include <osgManipulator/CommandManager>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/RotateSphereDragger>
#include <osgManipulator/ScaleAxisDragger>

class PickModelHandler : public osgGA::GUIEventHandler{
public:
	PickModelHandler() : _activeDragger( 0 ), _enablePick( false ), _hasSelected( false ){
		Init();
	}

	bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );

protected:
	osgManipulator::Dragger* _activeDragger;
	osgManipulator::PointerInfo _pointer;
	osg::ref_ptr<osgManipulator::Selection> _selection;
	osg::ref_ptr<osgManipulator::TranslateAxisDragger> _draggerTranslate;
	osg::ref_ptr<osgManipulator::RotateSphereDragger> _draggerRotate;
	osg::ref_ptr<osgManipulator::ScaleAxisDragger> _draggerScale;
	osg::ref_ptr<osgManipulator::Dragger> _dragger;
	osg::ref_ptr<osgManipulator::CommandManager> _manager;

	bool _enablePick;
	bool _hasSelected;

	void Init();
	bool SelectNode( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );
	bool DragNode( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa );

public:
	enum SELECT{ MODEL, GEOD };
	enum OPERATION{ TRANSLATE, ROTATE, SCALE };
	void EnableSelect( bool flag = false ){ _enablePick = flag; }
	bool GetEnableSelect(){ return _enablePick; }
	void SetSelectType( SELECT type ){ _selectType = type; }
	SELECT GetSelectType(){ return _selectType; }
	void SetOperationType( OPERATION type );//{ _operationType = type; }
	OPERATION GetOperationType(){ return _operationType; }
	void TestOperationType();

protected:
	SELECT _selectType;
	OPERATION _operationType;
};

#endif