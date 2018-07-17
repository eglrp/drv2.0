#include "stdafx.h"
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "DataManager.h"
#include "vlSwitchDisp.h"
#include "serverConnect.h"
#include "CDataShowInListView.h"
#include "BatchQuery.h"
#include "functionEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CDataManager)
	//XDEFINE_CLASSMAP_ENTRY(CvlSwitchDisp)
	XDEFINE_CLASSMAP_ENTRY(CDataShowInListView)
	XDEFINE_CLASSMAP_ENTRY(CServerConnect)
	XDEFINE_CLASSMAP_ENTRY(CBatchQuery)
	//XDEFINE_CLASSMAP_ENTRY(CdataAnalysis)
	//XDEFINE_CLASSMAP_ENTRY(CfireControl)
	//XDEFINE_CLASSMAP_ENTRY(CviolenceDisposal)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}