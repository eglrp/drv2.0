#include "stdafx.h"
#include "vlModelLib.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"

CvlModelLib::CvlModelLib(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"模型库"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"在线视频管理"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"模型库"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"模型库"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"ModelLib")); 
}

CvlModelLib::~CvlModelLib(void)
{
}

bool CvlModelLib::Initialize()
{
	return true;
}

bool CvlModelLib::UnInitialize()
{
	return true;
}

bool CvlModelLib::SetBuddy(x3::IObject* val)
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
bool CvlModelLib::OnClick()
{
	USES_CONVERSION;
	IUICommand::Enable(false);
	Checked(true);
	
	CString strFilter(GETSTRINGT(A2CT(_getClassName()), _T("Filter"), _T("模型文件(*.ive;*.osg;*.osgb;*.osgt)|*.ive;*.osg;*.osgb;*.osgt;*.cbs|所有文件(*.*)|*.*||")));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, strFilter);
	if( dlg.DoModal()!=IDOK )
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}
	
	std::string filename = T2A(dlg.GetPathName());
	if( filename.size()<=0 )
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

	std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
	osgDB::Registry::instance()->setDataFilePathList( FilePath );

	setlocale( LC_ALL, "chs" );
	osg::ref_ptr<osg::Node> spNode = osgDB::readNodeFile(filename);
	setlocale( LC_ALL, "C" );
	if( !spNode.valid() )
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

	// Optimize the model
	osgUtil::Optimizer optimizer;
	optimizer.optimize(spNode);
	optimizer.reset();

	if( m_spBuddy!=NULL )
	{
		x3::Object<IViewer3D> spViewer3D(m_spBuddy);
		if( spViewer3D.valid() )
		{
			spViewer3D->AddNode(spNode.get());
		}
	}

	Checked(false);
	IUICommand::Enable(true);
	return true;
}

