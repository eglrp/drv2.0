#include "stdafx.h"
#include "serverConnect.h"
#include "DataManager.h"
#include "x3py/manager/iworkpath.h"
#include "EarthMatrixFunctions.h"
using namespace osg;
using namespace osgDB;

extern CDlgDB_Connect* g_DBconnector;
CServerConnect::CServerConnect(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"连接服务器"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"物联网"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"服务器连接设置"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"服务器连接设置"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"server"));
}

CServerConnect::~CServerConnect(void)
{
}
bool CServerConnect::Initialize()
{
	return true;
}

bool CServerConnect::UnInitialize()
{
	return true;
}


bool CServerConnect::OnClick()
{
	g_DBconnector->ShowWindow(SW_SHOW);	
	return true;
}

bool CServerConnect::SetBuddy(x3::IObject* val)
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