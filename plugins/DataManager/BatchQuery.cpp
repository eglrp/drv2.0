#include "stdafx.h"
#include "BatchQuery.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "DataManager.h"

extern CDataManager* m_pDM;
CBatchQuery::CBatchQuery(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"批量查询"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"查询搜索"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"批量查询"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"批量查询"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"StatesetTexture"));

	bActive = false;

}

CBatchQuery::~CBatchQuery(void)
{
}

bool CBatchQuery::Initialize()
{
	return true;
}

bool CBatchQuery::UnInitialize()
{
	return true;
}

bool CBatchQuery::Checked()
{
	return bActive;
}

// IUICommand
bool CBatchQuery::OnClick()
{
	Activate();
	return true;
}

bool CBatchQuery::Activate()
{
	if (m_pDM != NULL)
	{
		m_pDM->OnBnClickedButtonSearchAdv();
		return true;
	}
	return false;
}
bool CBatchQuery::Deactivate()
{
	return true;
}

bool CBatchQuery::SetBuddy(x3::IObject* val)
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

