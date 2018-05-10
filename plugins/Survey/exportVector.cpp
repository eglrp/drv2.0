#include "stdafx.h"
#include "exportVector.h"
#include "EarthMatrixFunctions.h"
#include "toShpAndDxf.h"
CExportVector::CExportVector(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"ʸ������"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"�ռ�����"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"��ҳ"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"ʸ������"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"ʸ������"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"PointSurvey"));

	
}

CExportVector::~CExportVector(void)
{
}

bool CExportVector::SetBuddy(x3::IObject* val)
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

// IUICommand
bool CExportVector::OnClick()
{
	USES_CONVERSION;
	x3::Object<IViewer3D> spViewer3D(m_spBuddy);
	osg::ref_ptr<osg::Group> root = spViewer3D->getRootNode()->asGroup();
	if (!root)
	{
		return false;
	}
	CFindVertex fv;
	for (int i = 0;i<root->getNumChildren();++i)
	{
		osg::Node* node = root->getChild(i);
		if (node->getName() == "vector")
		{	
			node->accept(fv);
		}
	}
	if (fv.mvecVertex.empty())
	{
		AfxMessageBox(L"δ���ֿɵ���ʸ����");
		return false;
	}
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,L"DXF(*.dxf)|*.dxf||",NULL);  

	if(dlg.DoModal() == IDOK)  
	{  
		CString fileOut = dlg.GetPathName();
		if (fileOut.Find(L".dxf") == -1 &&
			fileOut.Find(L".DXF") == -1)
		{
			fileOut += L".dxf";
		}
		toShpAndDxf::convertToDxf(fv.mvecVertex,W2A(fileOut),"hatching");
		AfxMessageBox(L"������ɣ�");
	}  	
	return true;
}

bool CExportVector::Checked()
{
	return false;
}

bool CExportVector::Initialize()
{
	
	return true;
}

bool CExportVector::UnInitialize()
{
	return true;
}