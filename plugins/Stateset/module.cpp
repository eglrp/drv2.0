// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "Stateset.h"
#include "vlToggleFullscreen.h"
#include "vZToggle3D.h"

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CStatesetBackface)
	XDEFINE_CLASSMAP_ENTRY(CStatesetLighting)
	XDEFINE_CLASSMAP_ENTRY(CStatesetTexture)
	XDEFINE_CLASSMAP_ENTRY(CStatesetPolygonModePoint)
	XDEFINE_CLASSMAP_ENTRY(CStatesetPolygonModeLine)
	XDEFINE_CLASSMAP_ENTRY(CStatesetPolygonModeFill)
	XDEFINE_CLASSMAP_ENTRY(CStatesetNorthPointer)
	XDEFINE_CLASSMAP_ENTRY(CStatesetViewportInfo) 
	XDEFINE_CLASSMAP_ENTRY(CStatesetThumb)
	XDEFINE_CLASSMAP_ENTRY(CStatesetSkyBox)
	XDEFINE_CLASSMAP_ENTRY(CvlToggleFullscreen)
	XDEFINE_CLASSMAP_ENTRY(CvZToggle3D)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}
