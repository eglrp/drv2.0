#include "stdafx.h"
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "vlAddModel.h"
#include "vlAddVideo.h"
#include "vlModelLib.h"
#include "vlModModel.h"
#include "vlAddAnno.h"
#include "vlAddDynamicModel.h"
#include "vlAddVideoReal.h"
#include "vlPlayVideo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CvlAddModel)
	//XDEFINE_CLASSMAP_ENTRY(CvlAddVideo)
	XDEFINE_CLASSMAP_ENTRY(CvlAddVideoReal)
	//XDEFINE_CLASSMAP_ENTRY(CvlModelLib)
	XDEFINE_CLASSMAP_ENTRY(CvlPlayVideo)
	//XDEFINE_CLASSMAP_ENTRY(CvlAddAnno)
	XDEFINE_CLASSMAP_ENTRY(CvlAddDynamicModel)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}