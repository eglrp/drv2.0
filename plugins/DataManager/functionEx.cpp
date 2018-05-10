#include "stdafx.h"
#include "functionEx.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "DlgDB_Connect.h"
#include "DataManager.h"
#include "sniperToolHandle.h"

extern CDlgDB_Connect* g_DBconnector;
extern CDataManager* m_pDM;
CdataAnalysis::CdataAnalysis(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"辖区管控"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"预案处置"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"辖区管控"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"辖区管控"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"AttShow"));

	bActive = false;
	m_pCommunityDlg = NULL;
	
}

CdataAnalysis::~CdataAnalysis(void)
{
}

bool CdataAnalysis::Initialize()
{
	return true;
}

bool CdataAnalysis::UnInitialize()
{
	return true;
}

bool CdataAnalysis::Checked()
{
	return bActive;
}

bool CdataAnalysis::SetBuddy(x3::IObject* val)
{
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy == spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer() == nullptr )
		return false;

	return true;
}

bool CdataAnalysis::OnClick()
{
	if (bActive)
	{
		Deactivate();
	}
	else
	{
		if (!g_DBconnector->bConnect)
		{
			AfxMessageBox(_T("服务器未连接,请先连接服务器!"));
			return false;
		}
		Activate();
	}
	return true;
}

bool CdataAnalysis::Activate()
{
	int H = m_pDM->m_spViewer3D->rectView3D.bottom - m_pDM->m_spViewer3D->rectView3D.top;
	if (m_pCommunityDlg == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pCommunityDlg = new CCommunityDlg(H);
		m_pCommunityDlg->parent = (CObject*)this;
		m_pCommunityDlg->Create(IDD_DIALOG_Community);
		//m_pCommunityDlg->ShowWindow(SW_SHOW);
	}
	/*else
		m_pCommunityDlg->ShowWindow(SW_SHOW);*/

	RECT r;
	m_pCommunityDlg->GetWindowRect(&r);
	int width = r.right - r.left;
	int height = r.bottom - r.top;
	
	r.left = m_pDM->m_spViewer3D->rectView3D.right /*- 10 - width*/;
	r.right = r.left +width;
	r.top = m_pDM->m_spViewer3D->rectView3D.top + H/2 - height/2;
	r.bottom = r.top + height;
	m_pCommunityDlg->MoveWindow(&r);
	m_pCommunityDlg->ShowWindow(SW_SHOW);
	m_pCommunityDlg->play();
	bActive = true;
	return true;
}

bool CdataAnalysis::Deactivate()
{
	if(m_pCommunityDlg != NULL)
	{
		m_pCommunityDlg->DestroyWindow();
		m_pCommunityDlg = NULL;
	}
	bActive = false;
	return true;
}


CfireControl::CfireControl(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"消防指挥"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"预案处置"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"消防指挥"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"消防指挥"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"StatesetTexture"));

	bActive = false;
	m_pFireControlDlg = NULL;
}

CfireControl::~CfireControl(void)
{
}

bool CfireControl::Initialize()
{
	return true;
}

bool CfireControl::UnInitialize()
{
	return true;
}

bool CfireControl::SetBuddy(x3::IObject* val)
{
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy == spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer() == nullptr )
		return false;

	return true;
}

bool CfireControl::Activate()
{
	int H = m_pDM->m_spViewer3D->rectView3D.bottom - m_pDM->m_spViewer3D->rectView3D.top;
	if (m_pFireControlDlg == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pFireControlDlg = new CFireControlDlg(H);
		m_pFireControlDlg->parent = (CObject*)this;
		m_pFireControlDlg->Create(IDD_DIALOG_firecontrol);
		m_pFireControlDlg->m_pGetPosHandler = new GetPosHandler(m_pDM->m_spViewer3D->getRootNode()->asGroup());
		m_pDM->m_spViewer3D->getViewer()->addEventHandler(m_pFireControlDlg->m_pGetPosHandler);
	}
	RECT r;
	m_pFireControlDlg->GetWindowRect(&r);
	int width = r.right - r.left;
	int height = r.bottom - r.top;

	r.left = m_pDM->m_spViewer3D->rectView3D.right /*- 10 - width*/;
	r.right = r.left +width;
	r.top = m_pDM->m_spViewer3D->rectView3D.top + H/2 - height/2;
	r.bottom = r.top + height;
	m_pFireControlDlg->MoveWindow(&r);
	m_pFireControlDlg->ShowWindow(SW_SHOW);
	m_pFireControlDlg->play();
	bActive = true;
	return true;
}

bool CfireControl::Deactivate()
{
	if(m_pFireControlDlg != NULL)
	{
		if (m_pDM->gTemp != nullptr)
		{
			m_pDM->m_spViewer3D->RemoveNode(m_pDM->gTemp);
			m_pDM->gTemp = nullptr;
		}
		m_pDM->m_spViewer3D->getViewer()->removeEventHandler(m_pFireControlDlg->m_pGetPosHandler);
		m_pFireControlDlg->DestroyWindow();
		m_pFireControlDlg = NULL;
	}
	bActive = false;
	return true;
}

bool CfireControl::OnClick()
{
	if (bActive)
	{
		Deactivate();
	}
	else
	{
		if (!g_DBconnector->bConnect)
		{
			AfxMessageBox(_T("服务器未连接,请先连接服务器!"));
			return false;
		}
		Activate();
	}
	return true;
}

bool CfireControl::Checked()
{
	return bActive;
}


CviolenceDisposal::CviolenceDisposal(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"应急维稳"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"预案处置"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"应急维稳"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"应急维稳"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"StatesetTexture"));

	bActive = false;
	m_pExplosiveDisposalDlg = NULL;

}

CviolenceDisposal::~CviolenceDisposal(void)
{
}

bool CviolenceDisposal::Initialize()
{
	return true;
}

bool CviolenceDisposal::UnInitialize()
{
	return true;
}

bool CviolenceDisposal::Checked()
{
	return bActive;
}

bool CviolenceDisposal::SetBuddy(x3::IObject* val)
{
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy == spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer() == nullptr )
		return false;

	return true;
}

bool CviolenceDisposal::Activate()
{
	int H = m_pDM->m_spViewer3D->rectView3D.bottom - m_pDM->m_spViewer3D->rectView3D.top;
	if (m_pExplosiveDisposalDlg == NULL)
	{
		AFX_MANAGE_STATE_EX;
		m_pExplosiveDisposalDlg = new CExplosiveDisposalDlg(H);
		m_pExplosiveDisposalDlg->parent = (CObject*)this;
		m_pExplosiveDisposalDlg->Create(IDD_DIALOG_Explosive);
		/*CRect rc;  
		m_pExplosiveDisposalDlg->GetWindowRect(&rc);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		rc.left = x/2 - 400;
		rc.right = rc.left + 800;
		rc.top = y/2 - 300;
		rc.bottom = rc.top + 600;
		m_pExplosiveDisposalDlg->MoveWindow(&rc);
		m_pExplosiveDisposalDlg->ShowWindow(SW_SHOW);*/
	}
	/*else
		m_pExplosiveDisposalDlg->ShowWindow(SW_SHOW);*/

	RECT r;
	m_pExplosiveDisposalDlg->GetWindowRect(&r);
	int width = r.right - r.left;
	int height = r.bottom - r.top;

	r.left = m_pDM->m_spViewer3D->rectView3D.right /*- 10 - width*/;
	r.right = r.left +width;
	r.top = m_pDM->m_spViewer3D->rectView3D.top + H/2 - height/2;
	r.bottom = r.top + height;
	m_pExplosiveDisposalDlg->MoveWindow(&r);
	m_pExplosiveDisposalDlg->ShowWindow(SW_SHOW);
	m_pExplosiveDisposalDlg->play();

	bActive = true;
	return true;
}

bool CviolenceDisposal::Deactivate()
{
	if(m_pExplosiveDisposalDlg != NULL)
	{
		m_pExplosiveDisposalDlg->DestroyWindow();
		m_pExplosiveDisposalDlg = NULL;
	}
	bActive = false;
	return true;
}

bool CviolenceDisposal::OnClick()
{
	if (bActive)
	{
		Deactivate();
	}
	else
	{
		if (!g_DBconnector->bConnect)
		{
			AfxMessageBox(_T("服务器未连接,请先连接服务器!"));
			return false;
		}
		Activate();
	}
	return true;
}