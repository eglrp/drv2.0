#include "stdafx.h"
#include "vlNewProject.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "Project.h"

extern std::string g_projectPath;
CvlNewProject::CvlNewProject(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"新建项目"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"配置管理"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"新建项目"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"新建项目"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"NewProject")); 
}

CvlNewProject::~CvlNewProject(void)
{
}

bool CvlNewProject::Initialize()
{
	return true;
}

bool CvlNewProject::UnInitialize()
{
	return true;
}

bool CvlNewProject::SetBuddy(x3::IObject* val)
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

// IUICommand
bool CvlNewProject::OnClick()
{
	AFX_MANAGE_STATE_EX;
	USES_CONVERSION;
	CProject* p = new CProject();
	p->parent = (CObject*) this;
	p->DoModal();

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	if( !spViewer3D.valid() )
		return false;

	spViewer3D->SetProjectFilePath(p->m_strFilePath);
	g_projectPath = W2A(p->m_strFilePath.GetBuffer(0));
	g_projectPath = g_projectPath.substr(0,g_projectPath.find_last_of('\\')+1);
	return true;
}

