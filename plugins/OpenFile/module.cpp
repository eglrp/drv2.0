// OpenFile.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "vlOpenFile.h"
#include "vlOpenFileDem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CvlOpenFile)
	//XDEFINE_CLASSMAP_ENTRY(CvlOpenFileDem)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}