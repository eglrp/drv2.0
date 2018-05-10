#pragma once
#include "osgGA/KeySwitchMatrixManipulator"

class CSelfKeySwitchMatrixManipulator : public osgGA::KeySwitchMatrixManipulator
{
public:
	CSelfKeySwitchMatrixManipulator()
	{
		selectIndex = 0;
	}
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
	
	int selectIndex;
};
