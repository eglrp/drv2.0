#pragma once
#include "CbersExports.h"
#include "x3py/module/plugininc.h"
#include "x3py/objptr.h"
#include <osgGA/CameraManipulator>

BEGIN_NAMESPACE_Cbers3D

class ICameraManipulator : public x3::IObject
{
	X3DEFINE_IID(ICameraManipulator);

public:
	ICameraManipulator() { m_spCameraManipulator = NULL; }

public:
	virtual osgGA::CameraManipulator* CameraManipulator() { return m_spCameraManipulator.get(); };
	virtual void CameraManipulator(osgGA::CameraManipulator* val) { m_spCameraManipulator = val; };	

protected:
	osg::ref_ptr<osgGA::CameraManipulator> m_spCameraManipulator;
};

END_NAMESPACE_Cbers3D