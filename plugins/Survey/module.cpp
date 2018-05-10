#include "stdafx.h"
#include "x3py/module/plugininc.h"
#include "x3py/module/pluginimpl.h"
#include "x3py/module/modulemacro.h"
#include "x3py/observer/observerimpl.h"
#include "vlLinetSurvey.h"
#include "vlPointSurvey.h"
#include "vlLineSurveyTri.h"
#include "vlPOISearch.h"
#include "vlPointSelect.h"
#include "vlLineSurveyArea.h"
#include "vlRefetchBuildingH.h"
#include "SniperLineAnalysis.h"
#include "exportVector.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

XBEGIN_DEFINE_MODULE()
	XDEFINE_CLASSMAP_ENTRY(CvlPointSurvey)
	//XDEFINE_CLASSMAP_ENTRY(CvlLineSurvey)
	XDEFINE_CLASSMAP_ENTRY(CvlLineSurveyTri)
	XDEFINE_CLASSMAP_ENTRY(CvlLineSurveyArea)
	XDEFINE_CLASSMAP_ENTRY(CSniperLineAnalysis)
	//XDEFINE_CLASSMAP_ENTRY(CvlPOISearch)
	//XDEFINE_CLASSMAP_ENTRY(CvlRefetchBuildingH)
	XDEFINE_CLASSMAP_ENTRY(CvlPointSelect)
	XDEFINE_CLASSMAP_ENTRY(CExportVector)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
	return true;
}

OUTAPI void x3UninitializePlugin()
{
}