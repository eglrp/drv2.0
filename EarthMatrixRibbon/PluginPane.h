#pragma once
#include "UIPlugins.h"
USING_NAMESPACE_EARTHMATRIX

// CPluginPane

class CPluginPane : public CDockablePane
{
	DECLARE_DYNAMIC(CPluginPane)

public:
	CPluginPane(IUIWnd* pPlugin);
	virtual ~CPluginPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

protected:
	x3::Object<IUIWnd> m_spPlugin;
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


