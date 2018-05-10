#include "stdafx.h"
#include "CSelfDefKeySwitchMatrixManipulator.h"

bool CSelfDefKeySwitchMatrixManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
	
	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
	{
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_1)
		{
			selectMatrixManipulator(0);
		}
		else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_2)
		{
			selectMatrixManipulator(1);
		}
	}
	return KeySwitchMatrixManipulator::handle(ea,us);
}