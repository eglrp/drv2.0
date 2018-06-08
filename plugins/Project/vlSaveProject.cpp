#include "stdafx.h"
#include "vlSaveProject.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "ComFun.h"

extern std::string g_projectPath;
CvlSaveProject::CvlSaveProject(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"保存项目"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"配置管理"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"保存项目"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"保存项目"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"SaveProject"));

    registerHandlers();
}

CvlSaveProject::~CvlSaveProject(void)
{
}

bool CvlSaveProject::Initialize()
{
	supportsEvent("PROJECTOPEN");
	supportsEvent("PROJECTCHANGED");
    return true;
}

bool CvlSaveProject::UnInitialize()
{
    return true;
}

bool CvlSaveProject::SetBuddy(x3::IObject* val)
{
    x3::Object<IViewer3D> spViewer3D(val);
    if( !spViewer3D.valid() )
        return false;

    if( m_spBuddy == spViewer3D )
        return true;

    m_spBuddy = spViewer3D;
    if( spViewer3D->getViewer() == nullptr )
        return false;

    Enable(false);

    return true;
}

// IUICommand
bool CvlSaveProject::OnClick()
{
    USES_CONVERSION;
    IUICommand::Enable(false);
    Checked(true);

    CString strSaveSuccess(GETSTRINGT(L"Project", _T("SaveSuccess"), _T("保存成功！")));
    SaveProject();
    AfxMessageBox(strSaveSuccess);

    Checked(false);
    IUICommand::Enable(true);
    return true;
}

void CvlSaveProject::SaveProject()
{
    USES_CONVERSION;

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if( !spViewer3D.valid() )
        return;	

    rapidxml::xml_document<> doc;

    rapidxml::xml_node<>* root = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
    doc.append_node(root);

    rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, "Config", "Information");
    doc.append_node(node);

	//////////////////////////////////////////////////////////////////////////
	rapidxml::xml_node<>* layers = doc.allocate_node(rapidxml::node_element, "DefLayer", W2A(spViewer3D->GetDefLayer()));
	node->append_node(layers);

	layers = doc.allocate_node(rapidxml::node_element, "Viewport", W2A(spViewer3D->GetViewportPos()));
	node->append_node(layers);

    //////////////////////////////////////////////////////////////////////////
    layers = doc.allocate_node(rapidxml::node_element, "Layers", NULL);
    node->append_node(layers);

	CStringArray* aLayers = spViewer3D->GetLayersArray();
	CStringArray* aLayersCheck = spViewer3D->GetLayerCheck();
	for (int i = 0; i < aLayers->GetCount(); i++)
	{
		CString sL = aLayers->GetAt(i);
		std::string sl = W2A(sL.GetBuffer(0));
		if (sl.find(g_projectPath) == -1)
		{
			continue;
		}
		sl = sl.substr(g_projectPath.size());
		sL = CString(sl.c_str());
		CString sLCheck = aLayersCheck->GetAt(i);

		rapidxml::xml_node<>* nodeLayer = doc.allocate_node(rapidxml::node_element, "Layer", W2A(sL.GetBuffer()));

		xml_attribute<> *attr = new xml_attribute<>();
		attr->name("Checked");
		attr->value(W2A(sLCheck.GetBuffer()));
		
		nodeLayer->append_attribute(attr);
		layers->append_node(nodeLayer);
	}

    //////////////////////////////////////////////////////////////////////////
    rapidxml::xml_node<>* paths = doc.allocate_node(rapidxml::node_element, "Paths", NULL);
    node->append_node(paths);

    CStringArray* aPaths = spViewer3D->GetPathsArray();
    for (int i = 0; i < aPaths->GetCount(); i++)
    {
        CString sL = aPaths->GetAt(i);
        paths->append_node(doc.allocate_node(rapidxml::node_element, "Path", W2A(sL.GetBuffer())));
    }

    //////////////////////////////////////////////////////////////////////////
    rapidxml::xml_node<>* settings = doc.allocate_node(rapidxml::node_element, "DisplaySetting", NULL);
    node->append_node(settings);
    std::string sNum1 = int2str(spViewer3D->GetOSG_MAX_PAGEDLOD());
    settings->append_node(doc.allocate_node(rapidxml::node_element, "OSG_MAX_PAGEDLOD", sNum1.c_str()));

	std::string sNum2 = int2str(spViewer3D->GetOSG_NUM_DATABASE_THREADS());
	settings->append_node(doc.allocate_node(rapidxml::node_element, "OSG_NUM_DATABASE_THREADS", sNum2.c_str()));

	std::string sNum3 = int2str(spViewer3D->GetScreenNum());
	settings->append_node(doc.allocate_node(rapidxml::node_element, "SCREEN_NUM", sNum3.c_str()));
    //////////////////////////////////////////////////////////////////////////
	rapidxml::xml_node<>* prj = doc.allocate_node(rapidxml::node_element, "Projection", NULL);
	node->append_node(prj);
	std::string s = spViewer3D->GetPrj();
	prj->append_node(doc.allocate_node(rapidxml::node_element, "PROJECTION", s.c_str()));
    /*rapidxml::xml_node<>* mode = doc.allocate_node(rapidxml::node_element,"mode","screen mode");
    mode->append_attribute(doc.allocate_attribute("fullscreen","false"));
    node->append_node(mode);*/

    CString sPath = spViewer3D->GetProjectFilePath();
    if (sPath.GetLength() == 0)
    {
        CString strError(GETSTRINGT(L"Project", _T("ProjectNotExist"), _T("项目文件不存在，无法保存项目！")));
        AfxMessageBox(strError);
        return;
    }

    std::ofstream out(sPath);
    out << doc;
}

bool CvlSaveProject::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
{
    if (eventKey == "PROJECTOPEN")
    {
        if (filename.length() > 0)
        {
            Enable(true);
        }
    }

	if (eventKey == "PROJECTCHANGED")
	{
		//SaveProject();
	}
    return true;
}