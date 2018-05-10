#include "stdafx.h"
#include "CDataShowInListView.h"
#include "DataManager.h"
#include "x3py/manager/iworkpath.h"
#include "EarthMatrixFunctions.h"
#include "DLGShowRealData.h"
#include "BackgroundDlg.h"
using namespace osg;
using namespace osgDB;

extern CDlgDB_Connect* g_DBconnector;

CDataShowInListView::CDataShowInListView(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"实时状态"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"物联网"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"实时数据展示"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"实时数据展示"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"StatesetPolygonModePoint"));
}

CDataShowInListView::~CDataShowInListView(void)
{
}

bool CDataShowInListView::Initialize()
{
	return true;
}

bool CDataShowInListView::UnInitialize()
{
	return true;
}


bool CDataShowInListView::OnClick()
{
	CDLGShowRealData* m_dlg = new CDLGShowRealData();
	m_dlg->Create(IDD_DIALOG_REALDATA);

	CBackgroundDlg* bdlg = new CBackgroundDlg();
	bdlg->Create(IDD_DIALOG_BACKGROUND);

	m_dlg->parent = (CObject*)bdlg;
	bdlg->ShowWindow(SW_SHOW);
	m_dlg->ShowWindow(SW_SHOW);
	return true;
}

bool CDataShowInListView::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	return true;
}

