// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "Viewer3D.h"

USING_NAMESPACE_EARTHMATRIX

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CViewer3D)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}
