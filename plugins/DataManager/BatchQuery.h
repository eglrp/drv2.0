#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsidBatchQuery = "00000000-3000-0000-0000-000000000002";
class CBatchQuery : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CBatchQuery, clsidBatchQuery)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CBatchQuery(void);
	~CBatchQuery(void);

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	x3::Object<IViewer3D> m_spViewer3D;
	// IUICommand
	virtual bool OnClick();
	virtual bool Checked();

	virtual bool Activate();
	virtual bool Deactivate();

	bool bActive;
};