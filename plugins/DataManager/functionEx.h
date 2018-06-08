#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "IViewer3D.h"
#include "ExplosiveDisposalDlg.h"
#include "CommunityDlg.h"
#include "FireControlDlg.h"

USING_NAMESPACE_EARTHMATRIX

const char* const clsiddataAnalysis = "00000000-7000-0000-0000-000000000000";
class CdataAnalysis : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CdataAnalysis, clsiddataAnalysis)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CdataAnalysis(void);
	~CdataAnalysis(void);

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

public:
	bool bActive;

	CCommunityDlg* m_pCommunityDlg;
};

const char* const clsidfireControl = "00000000-7000-0000-0000-000000000001";
class CfireControl : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CfireControl, clsidfireControl)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CfireControl(void);
	~CfireControl(void);

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

	std::string getPrj()
	{
		x3::Object<IViewer3D> spViewer3D(m_spBuddy);
		return spViewer3D->GetPrj();
	}
public:
	bool bActive;

	CFireControlDlg* m_pFireControlDlg;
};

const char* const clsidViolenceDisposal = "00000000-7000-0000-0000-000000000002";
class CviolenceDisposal : public IUICommon, public IUICommand, public IAnythingEventObserver
{
	X3BEGIN_CLASS_DECLARE(CviolenceDisposal, clsidViolenceDisposal)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUICommand)		
		X3END_CLASS_DECLARE()

public:
	CviolenceDisposal(void);
	~CviolenceDisposal(void);

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

public:
	bool bActive;
	CExplosiveDisposalDlg* m_pExplosiveDisposalDlg;
};