#include "stdafx.h"
#include "vlOpenProject.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgUtil/Optimizer"
#include "IViewer3D.h"
#include "osgDB/FileUtils"
#include "afxinet.h"
#include "vhReadShapeFile.h"
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>

std::string g_projectPath;
using namespace osgEarth::Features;
int DF(CString strFileLocal, CString strFileHttp)
{
	CInternetSession sess;
	CHttpFile* pHttpFile;
	int nSize;

	//向图片服务器请求读取文件
	pHttpFile=(CHttpFile*)sess.OpenURL(strFileHttp);

	DWORD dwStatusCode = -1;
	pHttpFile->QueryInfoStatusCode(dwStatusCode);
	if (dwStatusCode == 400)
	{
		//UpdateLogError(_T("400：") + strFileHttp);
	}

	CFile   m_File;
	TCHAR szBuffer[1024] = {0};
	DWORD dwRead = 0;

	m_File.Open(strFileLocal, CFile::modeWrite|CFile::modeCreate|CFile::typeBinary); 
	while(dwRead = pHttpFile->Read(szBuffer,1024))
	{
		m_File.Write(szBuffer,dwRead);
	}
	nSize = m_File.GetLength();
	m_File.Close();    
	pHttpFile-> Close(); 

	if(pHttpFile!=NULL) 
	{ 
		delete pHttpFile; 
	} 
	sess.Close(); 

	return nSize;
}

CvlOpenProject::CvlOpenProject(void)
{
    std::wstring app = x3::a2w(_getClassName());
    Name(GETSTRINGW(app.c_str(), L"Name", L"打开项目"));
    Caption(GETSTRINGW(app.c_str(), L"Caption", L"配置管理"));
    Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
    Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"打开项目"));
    Description(GETSTRINGW(app.c_str(), L"Description", L"打开项目"));
    BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"OpenProject"));
}

CvlOpenProject::~CvlOpenProject(void)
{
}

bool CvlOpenProject::Initialize()
{
    return true;
}

bool CvlOpenProject::UnInitialize()
{
    return true;
}

bool CvlOpenProject::SetBuddy(x3::IObject* val)
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
bool CvlOpenProject::OnClick()
{
    USES_CONVERSION;
    IUICommand::Enable(false);
    Checked(true);

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if( !spViewer3D.valid() )
        return false;

    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"DRV Project File(*.drvp)|*.drvp||", NULL);

    if(dlg.DoModal() == IDOK)
    {
        CString sProjPath = dlg.GetPathName();
        if ( !osgDB::fileExists( W2A(sProjPath.GetBuffer(0)) ) )
        {
            CString strFileNotExist(GETSTRINGT(L"Project", _T("OpenFileNotExist"), _T("项目文件不存在，请重新选择！")));
            AfxMessageBox(strFileNotExist);
        }
        else
        {
            spViewer3D->SetProjectFilePath(sProjPath);
			std::string sPath = W2A(sProjPath.GetBuffer(0));
			g_projectPath = sPath.substr(0,sPath.find_last_of('\\')+1);
            OpenProject();
        }
    }
    else
    {

    }

    Checked(false);
    IUICommand::Enable(true);
    return true;
}

void CvlOpenProject::OpenProject()
{
    USES_CONVERSION;

    x3::Object<IViewer3D> spViewer3D(m_spBuddy);
    if( !spViewer3D.valid() )
        return;

	spViewer3D->Lock();
	
    rapidxml::file<> fdoc(W2A(spViewer3D->GetProjectFilePath().GetBuffer(0)));

    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());

    //! 获取根节点
    rapidxml::xml_node<>* root = doc.first_node();

	//! 获取根节点第一个节点
	rapidxml::xml_node<>* node = root->first_node("DefLayer");
	if (node)
	{
		spViewer3D->SetDefLayer(CString(node->value()));
	}

	node = root->first_node("Viewport");
	if (node)
	{
		spViewer3D->SetViewport(CString(node->value()));
	}

    //! 获取根节点第一个节点
    node = root->first_node("Layers");

    xml_node<>* nodeLayer = node->first_node();
    while (nodeLayer)
    {
		
        CString sLayerPath =  CString(std::string(g_projectPath + nodeLayer->value()).c_str());
		CString sCheck = (nodeLayer->first_attribute("Checked")!= NULL)?CString(nodeLayer->first_attribute("Checked")->value()):L"1";
#ifdef _2D

        std::string filename = g_projectPath + nodeLayer->value();
		std::string webfilename = filename;
        std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
        std::string FileNameEx = filename.substr(filename.find_last_of( '\\' ) + 1);
        std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));

		if (int(filename.find("/")) > -1)
		{
			FilePath = filename.substr( 0, filename.find_last_of( '/' ) );
			FileNameEx = filename.substr(filename.find_last_of( '/' )+1);
			FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));
		}

		if ( int(filename.find(".shp")) > -1 && int(filename.find("/")) > -1)
		{
			FilePath = filename.substr( 0, filename.find_last_of( '/' ) );
			FileNameEx = filename.substr(filename.find_last_of( '/' )+1);
			FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));

			CString sProjPath = spViewer3D->GetProjectFilePath();
			CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));

			DF(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".shp"), CString(FilePath.c_str()) + _T("/") + CString(FileNameNoEx.c_str()) + _T(".shp"));
			DF(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".prj"), CString(FilePath.c_str()) + _T("/") + CString(FileNameNoEx.c_str()) + _T(".prj"));
			DF(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".dbf"), CString(FilePath.c_str()) + _T("/") + CString(FileNameNoEx.c_str()) + _T(".dbf"));
			DF(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".shp.xml"), CString(FilePath.c_str()) + _T("/") + CString(FileNameNoEx.c_str()) + _T(".shp.xml"));
			DF(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".shx"), CString(FilePath.c_str()) + _T("/") + CString(FileNameNoEx.c_str()) + _T(".shx"));

			filename = T2A(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".shp"));

			FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
			FileNameEx = filename.substr(filename.find_last_of( '\\' )+1);
			FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));
		}
		
		if ( int(filename.find(".dxf")) > -1 && int(filename.find("/")) > -1)
		{
			FilePath = filename.substr( 0, filename.find_last_of( '/' ) );
			FileNameEx = filename.substr(filename.find_last_of( '/' )+1);
			FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));

			CString sProjPath = spViewer3D->GetProjectFilePath();
			CString sProjDir = sProjPath.Left(sProjPath.ReverseFind('\\'));

			DF(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".dxf"), CString(FilePath.c_str()) + _T("/") + CString(FileNameNoEx.c_str()) + _T(".dxf"));

			filename = T2A(sProjDir + _T("\\") + CString(FileNameNoEx.c_str()) + _T(".dxf"));

			FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
			FileNameEx = filename.substr(filename.find_last_of( '\\' )+1);
			FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));
		}

        osg::ref_ptr<osg::Node> spNode = osgDB::readNodeFile(filename);

        if( spNode.valid() )
        {
			if (int(filename.find(".shp")) == -1 )
			{
				spViewer3D->Lock();
				spNode->setName(FileNameEx);
				if ( (int(webfilename.find(".shp")) > -1 && int(webfilename.find("/")) > -1)
					|| (int(webfilename.find(".dxf")) > -1 && int(webfilename.find("/")) > -1)
					)
				{
					spViewer3D->GetLayersArray()->Add(CString(webfilename.c_str()));
				}
				else
					spViewer3D->GetLayersArray()->Add(sLayerPath);
				spViewer3D->GetLayerCheck()->Add(sCheck);
				spViewer3D->AddNode(spNode.get(),nullptr);
				spViewer3D->Unlock();
			}
			//如果加载的shp文件
			else
			{
				std::vector<osg::Vec3d> vecPt;
				std::vector<string> vecLab;
				std::vector<string> vecPerson;

				bool isPoints = false;
				bool isBuilding = false;
				bool buildingFlag = false;
				osgEarth::Drivers::OGRFeatureOptions ogrOpt;
				ogrOpt.url() = filename;
				ogrOpt.openWrite() = true;

				osg::ref_ptr<osgEarth::Features::FeatureSource> features = osgEarth::Features::FeatureSourceFactory::create(ogrOpt);
				features->initialize();
				osg::ref_ptr<osgEarth::Features::FeatureCursor> cursor = features->createFeatureCursor();
				osgEarth::Features::Feature* feature;

				while(cursor->hasMore())
				{
					//输出feature中的信息
					feature = cursor->nextFeature();
					osgEarth::Symbology::GeometryIterator parts (feature->getGeometry(), false);

					while(parts.hasMore())
					{
						osgEarth::Symbology::Geometry* part = parts.next();
						osg::ref_ptr<osg::Vec3Array> allPoints = new osg::Vec3Array;

						allPoints = part->toVec3Array();
						if (allPoints->empty())
						{
							continue;
						}
						Vec3dVector* vv = new Vec3dVector();
						//osg::ref_ptr< osg::Vec3Array > v3 = new osg::Vec3Array;

						for( osg::Vec3Array::iterator i = allPoints->begin(); i != allPoints->end(); ++i )
						{
							osg::Vec3d v(*i);
							vv->push_back(v);

							/*v.z() = v.z() + 1;
							v3->push_back(v);*/
						}
					
						if (part->getType() == Geometry::TYPE_POINTSET)//点状信息
						{
							isPoints = true;
							osg::Vec3d v(*(allPoints->begin()));
							std::string sZ = feature->getString("z");
							if (!sZ.empty())
							{
								v.z() = atof(sZ.data());
							}
							vecPt.push_back(v);//保存坐标
							std::string sNm1 = feature->getString("name");
							std::string sNm2 = feature->getString("NAME");
							if (!sNm1.empty())
							{
								vecLab.push_back(sNm1);
							}
							else if (!sNm2.empty())
							{
								vecLab.push_back(sNm2);
							}
							else
								vecLab.push_back("null");
						}
						else if (part->getType() == Geometry::TYPE_POLYGON)//面状信息
						{
							osgEarth::Symbology::Ring r(vv);
							osgEarth::Bounds bd = r.getBounds();
							vecPt.push_back(bd.center() );//保存中心坐标

							std::string sTH = feature->getString("TH");//根据th字段判断是否为建筑
							if (!sTH.empty())
							{
								std::string sDH = feature->getString("栋号");
								std::string sJZ = feature->getString("建筑名称");
								std::string sSB = feature->getString("设备名称");
								std::string sRS = feature->getString("进出人口数");

								if (!sDH.empty())
								{
									vecLab.push_back(sDH);
								}
								else if (!sJZ.empty())
								{
									vecLab.push_back(sJZ);
								}
								else if (!sSB.empty())
								{
									vecLab.push_back(sSB);
								}
								else
									vecLab.push_back("null");
								if (!sRS.empty())
								{
									vecPerson.push_back(sRS);
									buildingFlag = true;
								}
								else
									vecPerson.push_back("0");

								vecPt.back().z() = atof(sTH.data());
								isBuilding = true;
							}
						}	
					}
				}

				if (isPoints)//点信息 碰撞更新高度
				{
					if (!vecPt.empty() && vecPt[0].z() == 0)
					{
						CReadShapeFile cf;
						if (spViewer3D->getRootNode()->asGroup() != nullptr)
						{
							cf.updateHbyImpact(vecPt,spViewer3D->getRootNode()->asGroup());
						}	
					}
				}

				if (vecLab.size() != vecPt.size())//校验标签和坐标数据一致性
				{
					vecLab.clear();
					vecPt.clear();
				}
				osg::ref_ptr<osgText::Font> font = osgText::readFontFile( "fonts/simhei.ttf" );
				osg::Vec4 txtColor( 46/255.0, 211/255.0, 250/255.0, 1.f );  
				osg::ref_ptr<osg::Group> group = new osg::Group;

				//计算出入口人数最大值
				int maxV = 0;
				if (vecPerson.size()>0)
				{
					for (int i = 0;i< vecPerson.size();++i)
					{
						int k = atoi(vecPerson[i].c_str());
						if (maxV< k)
						{
							maxV = k;
						}
					}
				}

				for (int i = 0;i<vecPt.size();++i)
				{	
					osg::ref_ptr<osg::Geode> geode = new osg::Geode; 
					osg::ref_ptr<osg::Geode> geode1 = new osg::Geode; 
					osg::ref_ptr<osg::LOD> lode = new osg::LOD();


					//创建文字
					CReadShapeFile cf;
					float hAdd = 6;
					if (isBuilding)
					{
						hAdd = 3;
					}
					if (isPoints)
					{
						txtColor = osg::Vec4(0,1,0,1);
					}
					
					osg::Billboard* text = cf.createText(osg::Vec3d(vecPt[i].x(),vecPt[i].y(),vecPt[i].z() + hAdd),
															txtColor,
															osg::Vec4(0,0,0,1),
															20,
															cf.multiLine(vecLab[i]));
					osg::Billboard* text1 = cf.createText(osg::Vec3d(vecPt[i].x(),vecPt[i].y(),vecPt[i].z() + hAdd),
															txtColor,
															osg::Vec4(0,0,0,1),
															50,
															cf.multiLine(vecLab[i]));  
				
					osg::Billboard* textTop = NULL;
					//设置样式
					osg::ref_ptr<osg::StateSet> stateset1 = new osg::StateSet();
					stateset1->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
					stateset1->setMode(GL_BLEND,osg::StateAttribute::ON);  
					stateset1->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		
					//点信息 添加热点box
					if (isPoints)
					{
						//热点box
						osg::ref_ptr<osg::ShapeDrawable> box = new osg::ShapeDrawable(new osg::Box(osg::Vec3(vecPt[i].x(),vecPt[i].y(),vecPt[i].z()+3), 2 ));
						box->setColor(osg::Vec4(14/255.0,108/255.0,195/255.0,0.3));
						box->setStateSet(stateset1);
						osg::ref_ptr<osg::ShapeDrawable> box1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(vecPt[i].x(),vecPt[i].y(),vecPt[i].z()+3), 2 ));
						box1->setColor(osg::Vec4(14/255.0,108/255.0,195/255.0,0.3));
						box1->setStateSet(stateset1);

						geode->addDrawable(box.get());
						geode1->addDrawable(box1.get());
					}
					else if (isBuilding && buildingFlag)//添加柱状图信息
					{
						//柱状图box
						float H = 8.0 * atoi(vecPerson[i].c_str())/float(maxV) + 0.5;//柱高
						float dH = vecPt[i].z()+hAdd + 6;
						osg::ref_ptr<osg::ShapeDrawable> boxTop = new osg::ShapeDrawable(new osg::Box(osg::Vec3(vecPt[i].x(),vecPt[i].y(),dH + H/2),2,2, H ));
						boxTop->setColor(osg::Vec4(14/255.0,108/255.0,195/255.0,0.6));
						boxTop->setStateSet(stateset1);

						osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
						shared_colors->push_back(osg::Vec4(0,0,1,1));
						osg::ref_ptr<osg::Vec3Array> point = new osg::Vec3Array; 
						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() - 1,dH));
						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() - 1,dH));
						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() - 1,dH+H));
						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() - 1,dH + H));

						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() - 1,dH + H));
						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() - 1,dH));
						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() + 1,dH));
						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() + 1,dH + H));

						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() + 1,dH));
						point->push_back(osg::Vec3d(vecPt[i].x() - 1,vecPt[i].y() + 1,dH + H));
						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() + 1,dH + H));
						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() + 1,dH));

						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() + 1,dH + H));
						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() + 1,dH));
						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() - 1,dH));
						point->push_back(osg::Vec3d(vecPt[i].x() + 1,vecPt[i].y() - 1,dH + H));

						osg::Geometry* lineGeom = new osg::Geometry();
						lineGeom->setVertexArray(point.get());
						lineGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);
						lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,0,4));
						lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,4,4));
						lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,8,4));
						lineGeom->addPrimitiveSet(new  osg::DrawArrays(osg::DrawArrays::LINE_LOOP,12,4));
						osg::StateSet* ss = lineGeom->getOrCreateStateSet();
						ss->setAttributeAndModes( new osg::LineWidth(2.5f) );
						std::string sTitle("进出人数：");
						sTitle = sTitle + vecPerson[i] + " (人)";
						osg::Billboard* text = cf.createText(osg::Vec3d(vecPt[i].x(),vecPt[i].y(),vecPt[i].z() + hAdd + 8 + H),
							osg::Vec4(1,0,0,1),
							osg::Vec4(1,1,1,1),
							12,
							sTitle);   

						textTop = dynamic_cast<osg::Billboard*>(text->clone(osg::CopyOp::DEEP_COPY_ALL));
						geode->addDrawable(boxTop.get());
						geode->addDrawable(lineGeom);
						geode1->addDrawable(boxTop.get());
						geode1->addDrawable(lineGeom);
					}
				

					lode->addChild(geode,0.0f,500.0f);
					lode->addChild(text,0.0f,500.0f);
					
					lode->addChild(geode1,500.0f,FLT_MAX);
					lode->addChild(text1,500.0f,FLT_MAX);

					if (textTop != NULL)
					{
						lode->addChild(textTop,0,FLT_MAX);
					}
					group->addChild(lode);
				}
				group->setName(FileNameEx);
				
				if ( (int(webfilename.find(".shp")) > -1 && int(webfilename.find("/")) > -1)
					|| (int(webfilename.find(".dxf")) > -1 && int(webfilename.find("/")) > -1)
					)
				{
					spViewer3D->GetLayersArray()->Add(CString(webfilename.c_str()));
				}
				else
					spViewer3D->GetLayersArray()->Add(sLayerPath);
				spViewer3D->GetLayerCheck()->Add(sCheck);
				spViewer3D->AddNode(group,nullptr);	
			}
#else
			spViewer3D->OpenFile(nodeLayer->value(), nullptr);
#endif // _2D
        }

        nodeLayer = nodeLayer->next_sibling();
    }

    //! 获取根节点第一个节点
    node = root->first_node("Paths");
    if (node)
    {
        nodeLayer = node->first_node();
        while (nodeLayer)
        {
            CString sLayerPath =  CString(nodeLayer->value());

			if (osgDB::fileExists(W2A(sLayerPath)))
			{
				spViewer3D->GetPathsArray()->Add(sLayerPath);
			}

            nodeLayer = nodeLayer->next_sibling();
        }
    }

    node = root->first_node("DisplaySetting");
    if (node)
    {
        nodeLayer = node->first_node("OSG_MAX_PAGEDLOD");
        if (nodeLayer != NULL)
        {
            int nNum = atoi(nodeLayer->value());
            if (nNum != 0)
            {
                spViewer3D->SetOSG_MAX_PAGEDLOD(nNum);
            }
        }

        nodeLayer = node->first_node("OSG_NUM_DATABASE_THREADS");
        if (nodeLayer != NULL)
        {
            int nNum = atoi(nodeLayer->value());
            if (nNum != 0)
            {
                spViewer3D->SetOSG_NUM_DATABASE_THREADS(nNum);
            }
        }

        nodeLayer = node->first_node("SCREEN_NUM");
        if (nodeLayer != NULL)
        {
            int nNum = atoi(nodeLayer->value());
            if (nNum != 0)
            {
                spViewer3D->SetScreenNum(nNum);
            }
        }
    }
	node = root->first_node("Projection");
	if (node)
	{
		nodeLayer = node->first_node("PROJECTION");
		if (nodeLayer != NULL)
		{
			std::string s(nodeLayer->value());
			spViewer3D->SetPrj(s);	
		}
	}
	spViewer3D->Unlock();
}

