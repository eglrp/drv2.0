#include "stdafx.h"
#include "vlSwitchDisp.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "x3py/manager/iworkpath.h"
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include "DataManager.h"

using namespace osg;
using namespace osgDB;

extern CDataManager* m_pDM;

CvlSwitchDisp::CvlSwitchDisp(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"切换显示"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"数据管理"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"切换模型的显示与隐藏状态"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"切换模型的显示与隐藏状态"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"SwitchDisp"));
}

CvlSwitchDisp::~CvlSwitchDisp(void)
{
}

bool CvlSwitchDisp::Initialize()
{
	supportsEvent("PROJECTOPEN");
	return true;
}

bool CvlSwitchDisp::UnInitialize()
{
	return true;
}

bool CvlSwitchDisp::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	m_spViewer3D = spViewer3D;

	Enable(false);

	registerHandlers();

	return true;
}

void CvlSwitchDisp::SetLayerCheck(CString sName, bool bCheck)
{
	USES_CONVERSION;
	//////////////////////////////////////////////////////////////////////////
	CStringArray* aLayers = m_spViewer3D->GetLayersArray();
	CStringArray* aLayersCheck = m_spViewer3D->GetLayerCheck();
	for (int i=0; i < aLayers->GetCount();i++)
	{
		CString sL = aLayers->GetAt(i);
		if (sL == sName)
		{
			std::string fp = T2A(sL);
			std::string filename = osgDB::getStrippedName(fp);
			HTREEITEM hRoot = m_pDM->m_wndTreeCtrl.GetRootItem();
			HTREEITEM item = m_pDM->FindItem(hRoot, CString(filename.c_str()));
			osg::Node* pNode = (osg::Node*)m_pDM->m_wndTreeCtrl.GetItemData(item);

			if (bCheck)
			{
				pNode->setNodeMask(1);
				aLayersCheck->SetAt(i,L"1");
			}
			else
			{
				pNode->setNodeMask(0);
				aLayersCheck->SetAt(i,L"0");
			}

			m_pDM->m_wndTreeCtrl.SetCheck(item, bCheck);
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

// IUICommand
bool CvlSwitchDisp::OnClick()
{
	USES_CONVERSION;
	IUICommand::Enable(false);
	Checked(true);
	
	CStringArray* aLayers = m_spViewer3D->GetLayersArray();
	CStringArray* aLayersCheck = m_spViewer3D->GetLayerCheck();
	for (int i = 0; i < aLayers->GetCount(); i++)
	{
		CString sL = aLayers->GetAt(i);
		CString sLCheck = aLayersCheck->GetAt(i);

		if (sL.Find(_T("vt-rmyy")) < 0)
		{
			continue;
		}

		if (sLCheck == _T("1"))
		{
			SetLayerCheck(sL, false);
		}
		else
			SetLayerCheck(sL, true);
	}

	Checked(false);
	IUICommand::Enable(true);
	return true;
}

bool CvlSwitchDisp::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
{
	if (eventKey == "PROJECTOPEN")
	{
		if (filename.length() > 0)
		{
			Enable(true);
		}
		else
			Enable(false);
	}
	return true;
}