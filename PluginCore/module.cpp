#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "UICore.h"
#include "UILanguages.h"

USING_NAMESPACE_EARTHMATRIX

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY_Singleton(CUICore)
	XDEFINE_CLASSMAP_ENTRY_Singleton(UILanguages)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}
