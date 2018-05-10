#include "stdafx.h"
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "vlWalkPlay.h"
#include "vlWalkMana.h"
#include "vlWalkRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CvlWalkPlay)
	XDEFINE_CLASSMAP_ENTRY(CvlWalkMana)
	XDEFINE_CLASSMAP_ENTRY(CvlWalkRec)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}