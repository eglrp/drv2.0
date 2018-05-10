#include "stdafx.h"
#include "vlOpenFile.h"
#include "EarthMatrixFunctions.h"
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "osgUtil/Optimizer"
#include <osgEarthDrivers/model_simple/SimpleModelOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include "IViewer3D.h"
#include "x3py/manager/iworkpath.h"
#include "DlgAddData.h"
#include "afxinet.h"
#include "vhReadShapeFile.h"
#include "osg/LineWidth"
#include "osg/Point"

extern std::string g_sDataPath;
extern std::vector<CString> vecPaths;

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

CvlOpenFile::CvlOpenFile(void)
{
	std::wstring app = x3::a2w(_getClassName());
	Name(GETSTRINGW(app.c_str(), L"Name", L"增加图层"));
	Caption(GETSTRINGW(app.c_str(), L"Caption", L"图层管理"));
	Category(GETSTRINGW(app.c_str(), L"Category", L"主页"));
	Tooltip(GETSTRINGW(app.c_str(), L"Tooltip", L"添加图层"));
	Description(GETSTRINGW(app.c_str(), L"Description", L"添加图层"));
	BitmapName(GETSTRINGW(app.c_str(), L"BitmapName", L"Open"));

	registerHandlers();
}

CvlOpenFile::~CvlOpenFile(void)
{
}

bool CvlOpenFile::Initialize()
{
	return true;
}

bool CvlOpenFile::UnInitialize()
{
	return true;
}

bool CvlOpenFile::SetBuddy(x3::IObject* val)
{	
	x3::Object<IViewer3D> spViewer3D(val);
	if( !spViewer3D.valid() )
		return false;

	if( m_spBuddy==spViewer3D )
		return true;

	m_spBuddy = spViewer3D;
	if( spViewer3D->getViewer()==nullptr )
		return false;

	Enable(false);

	registerHandlers();

	return true;
}

// IUICommand
bool CvlOpenFile::OnClick()
{
	USES_CONVERSION;
	IUICommand::Enable(false);
	Checked(true);

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	//////////////////////////////////////////////////////////////////////////
	AFX_MANAGE_STATE_EX;
	CDlgAddData* dlg = new CDlgAddData();
	if( dlg->DoModal()!=IDOK )
	{
		IUICommand::Enable(true);
		Checked(false);
		return false;
	}
	
	//////////////////////////////////////////////////////////////////////////
	if (vecPaths.empty())
	{
		IUICommand::Enable(true);
		Checked(false);
		return false;
	}
	for (int i = 0;i<vecPaths.size();++i)
	{
		std::string filename = T2A(vecPaths[i]);
		std::string webfilename = T2A(vecPaths[i]);

		

		if( filename.size()<=0 )
		{
			AfxMessageBox(_T("未获得文件路径！"));
			continue;
		}


		std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
		std::string FileNameEx = filename.substr(filename.find_last_of( '\\' )+1);
		std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));


		//检测文件是否存在
		CStringArray* aLayers0 = spViewer3D->GetLayersArray();
		bool bExist = false;
		for (int j = 0; j < aLayers0->GetCount(); j++)
		{
			CString sL = aLayers0->GetAt(j);
			std::string fp = T2A(sL);
			std::string filename0 = osgDB::getStrippedName(fp);
			if (filename0.find(FileNameNoEx) != -1)
			{
				bExist = true;
				break;
			}
		}
		if (bExist)
		{
			CString str = CString(FileNameEx.c_str()) + _T("已存在！");
			AfxMessageBox(str);
			continue;
		}


		if ( int(filename.find('/')) > -1)
		{
			FilePath = filename.substr( 0, filename.find_last_of( '/' ) );
			FileNameEx = filename.substr(filename.find_last_of( '/' )+1);
			FileNameNoEx = filename;//显示完整的网络路径
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

		osgDB::Registry::instance()->setDataFilePathList( FilePath );

#ifdef _2D

		osg::ref_ptr<osg::Node> spNode = osgDB::readNodeFile(filename);

		if( !spNode.valid() )
		{
			CString err = vecPaths[i]+_T("打开失败！");
			AfxMessageBox(err);
			continue;
		}
		spNode->getOrCreateStateSet()->setAttributeAndModes( new osg::Point(2.0f) );

		if (int(filename.find(".shp")) == -1 )
		{
			spNode->setName(FileNameEx);
			
			if ( (int(webfilename.find(".shp")) > -1 && int(webfilename.find("/")) > -1)
				|| (int(webfilename.find(".dxf")) > -1 && int(webfilename.find("/")) > -1)
				)
			{
				spViewer3D->GetLayersArray()->Add(CString(webfilename.c_str()));
			}
			else
				spViewer3D->GetLayersArray()->Add(CString(filename.c_str()));
			spViewer3D->GetLayerCheck()->Add(_T("1"));
			spViewer3D->AddNode(spNode.get(),nullptr);
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
							std::string sRS = feature->getString("进出人口数");

							if (!sDH.empty())
							{
								vecLab.push_back(sDH);
							}
							else if (!sJZ.empty())
							{
								vecLab.push_back(sJZ);
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
			////////////////////////////////////
			for (int i = 0;i<vecPt.size();++i)
			{	
				osg::ref_ptr<osg::Geode> geode = new osg::Geode; 
				osg::ref_ptr<osg::Geode> geode1 = new osg::Geode; 
				osg::ref_ptr<osg::LOD> lode = new osg::LOD();
				lode->setName("热点");

				//创建文字
				CReadShapeFile cf;
				float hAdd = 8;
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
				osg::ref_ptr<osg::Group> tGroup = new osg::Group;
				if (isPoints)
				{
					//热点box
					osg::Billboard* geodeb = new osg::Billboard();
					geodeb->setMode(osg::Billboard::AXIAL_ROT);

					osg::ref_ptr<osg::DrawPixels> bmp= new osg::DrawPixels;
					//bmp->setPosition( osg::Vec3( vecPt[i].x(),vecPt[i].y(),vecPt[i].z()+3));
					bmp->setImage( osgDB::readImageFile("F:\\2_DRV开发\\新建文件夹\\EarthMatrixRibbon\\res\\MAIN.png"));
					bmp->setStateSet(stateset1);
					geodeb->addDrawable(bmp);
					geodeb->setPosition(0,osg::Vec3( vecPt[i].x(),vecPt[i].y(),vecPt[i].z()+3));
					/*osg::ref_ptr< osg::Geode> geode= new osg::Geode;
					geode->addDrawable( bmp.get());*/

					//bmp->setImage(osgDB::readImageFile("F:\\2_DRV开发\\新建文件夹\\EarthMatrixRibbon\\MAIN.png"));
					/*osg::ref_ptr<osg::ShapeDrawable> box = new osg::ShapeDrawable(new osg::Box(osg::Vec3(vecPt[i].x(),vecPt[i].y(),vecPt[i].z()+3), 2 ));
					box->setColor(osg::Vec4(14/255.0,108/255.0,195/255.0,0.3));
					box->setStateSet(stateset1);
					osg::ref_ptr<osg::ShapeDrawable> box1 = new osg::ShapeDrawable(new osg::Box(osg::Vec3(vecPt[i].x(),vecPt[i].y(),vecPt[i].z()+3), 2 ));
					box1->setColor(osg::Vec4(14/255.0,108/255.0,195/255.0,0.3));
					box1->setStateSet(stateset1);*/
					tGroup->addChild(geodeb);
					//geode->addDrawable(geodeb);
					//geode1->addDrawable(geodeb);
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

				tGroup->setName("物联设备");
				lode->addChild(tGroup,0,FLT_MAX);
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
				spViewer3D->GetLayersArray()->Add(CString(filename.c_str()));
			spViewer3D->GetLayerCheck()->Add(_T("1"));
			spViewer3D->AddNode(group,nullptr);

			osg::Group* pGroup = spViewer3D->getSceneData()->asGroup();
			int n = pGroup->getNumChildren();
			for (int i = 0;i<n;++i)
			{
				std::string s = pGroup->getChild(i)->getName();
				if (s.find("热点.shp") != -1)
				{
					int n1 = pGroup->getChild(i)->asGroup()->getNumChildren();
					for (int i1 = 0;i1 < n1;++i1)
					{
						osg::LOD* lod = dynamic_cast<osg::LOD*>(pGroup->getChild(i)->asGroup()->getChild(i1));
						if (lod)
						{
							for (int j = 0;j<lod->getNumChildren();++j)
							{
								if (lod->getChild(j)->getName() == "物联设备")
								{
									osg::Group* group1 = lod->getChild(j)->asGroup();
									for (int k = 0;k<group1->getNumChildren();k++)
									{
										osg::Billboard* bb = dynamic_cast<osg::Billboard*>(group1->getChild(k));
										osg::ref_ptr<osg::DrawPixels> bmp = dynamic_cast<osg::DrawPixels*>(bb->getDrawable(0));
										bmp->setImage(osgDB::readImageFile("F:\\2_DRV开发\\新建文件夹\\EarthMatrixRibbon\\MAIN.png"));
									}
									break;
								}
							}
						}
					}
					break;
				}
			}
		}	
		
#else
		spViewer3D->OpenFile(filename, nullptr);
#endif // _2D
	}
	
	/*

	std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
	std::string FileNameEx = filename.substr(filename.find_last_of( '\\' )+1);
	std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));
	osgDB::Registry::instance()->setDataFilePathList( FilePath );

	setlocale( LC_ALL, "chs" );

	x3::Object<IViewer3D> spViewer3D(m_spBuddy);

	osg::ref_ptr<osg::Node> spNode = osgDB::readNodeFile(filename);
	setlocale( LC_ALL, "C" );

	spNode->setName(FileNameNoEx);

	if( !spNode.valid() )
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	const osg::BoundingSphere& bs = spNode->getBound();
	if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
	{
		Checked(false);
		IUICommand::Enable(true);
		return false;
	}

	osg::Vec3d v3f = bs.center();
	osg::Vec3d v3dOut;

	//double lat,lon,h;

	const SpatialReference* epsg2382;
	std::string ProjFile = FilePath + "\\" + FileNameNoEx + ".prj";
	
	if (osgDB::fileExists(ProjFile))
	{
		std::string sContent = spViewer3D->GetFileContent(ProjFile);
		epsg2382 = SpatialReference::create(sContent);

	}
	else
		epsg2382 = SpatialReference::create("wgs84");
	const SpatialReference* wgs84 = spViewer3D->getMapNode()->getMapSRS();
	epsg2382->transform(v3f, wgs84, v3dOut);

	osg::Vec3d location = osg::Vec3d(v3dOut.x(), v3dOut.y(), v3dOut.z());    

	//////////////////////////////////////////////////////////////////////////
	osgEarth::Drivers::SimpleModelOptions simpleModelOptions;
	simpleModelOptions.url() = osgEarth::URI(filename);
	simpleModelOptions.location() = location;
	simpleModelOptions.node() = spNode.get();
	osgEarth::ModelLayerOptions layerOptions(FileNameNoEx, simpleModelOptions);    
	osg::ref_ptr<osgEarth::ModelLayer> mLayer = new osgEarth::ModelLayer(layerOptions);
	spViewer3D->getMapNode()->getMap()->addModelLayer(mLayer.get());
	//////////////////////////////////////////////////////////////////////////

	spViewer3D->FlyToViewport(v3dOut.x(), v3dOut.y(), v3dOut.z());
	*/
	vecPaths.clear();
	Checked(false);
	IUICommand::Enable(true);
	return true;
}


bool CvlOpenFile::OnAnything2(x3::IObject* sender, const std::string& eventKey, const std::string& filename)
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