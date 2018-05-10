#pragma once

#define BEGIN_NAMESPACE_EARTHMATRIX  namespace EarthMatrix {
#define END_NAMESPACE_EARTHMATRIX    }
#define USING_NAMESPACE_EARTHMATRIX  using namespace EarthMatrix;

#ifdef _AFXEXT
#define AFX_MANAGE_STATE_EX     AFX_MANAGE_STATE(AfxGetAppModuleState())
#else
#define AFX_MANAGE_STATE_EX     AFX_MANAGE_STATE(AfxGetStaticModuleState())
#endif