#include "stdafx.h"
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "vlNewProject.h"
#include "vlOpenProject.h"
#include "vlSaveProject.h"
#include "vlSetProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CvlNewProject)
	XDEFINE_CLASSMAP_ENTRY(CvlOpenProject)
	XDEFINE_CLASSMAP_ENTRY(CvlSaveProject)
	XDEFINE_CLASSMAP_ENTRY(CvlSetProject)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}