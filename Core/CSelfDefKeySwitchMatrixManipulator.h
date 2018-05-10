#pragma once
#include "osgGA/KeySwitchMatrixManipulator"

class CSelfDefKeySwitchMatrixManipulator : public osgGA::KeySwitchMatrixManipulator
{
public:
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
};