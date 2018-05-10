#include "stdafx.h"
#include "CSelfKeySwitchMatrixManipulator.h"

bool CSelfKeySwitchMatrixManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
	KeySwitchMatrixManipulator::handle(ea,us);
	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
	{
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_P)
		{
			if (selectIndex == 0)
			{
				selectMatrixManipulator(1);
				selectIndex = 1;
			}
			else
			{
				selectMatrixManipulator(0);
				getCurrentMatrixManipulator()->handle(ea,us);
				selectIndex = 0;
			}
		}
	}		

	return true;;
}