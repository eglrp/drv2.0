#include "stdafx.h"
#include "vhReadShapeFile.h"
#include <map>

unsigned long CReadShapeFile::OnChangeByteOrder (int indata)
{
    char ss[8];
    char ee[8];
    unsigned long val = unsigned long(indata);
    _ultoa( val, ss, 16 );// 将十六进制的数 (val) 转到一个字符串 (ss) 中
    int i;
    int length=strlen(ss);
    if(length!=8)
    {
        for(i=0;i<8-length;i++)
                ee[i]='0';
        for(i=0;i<length;i++)
                ee[i+8-length]=ss[i];
        for(i=0;i<8;i++)
                ss[i]=ee[i];
	}
    ////****** 进行倒序
	int t;
	t =ss[0];
	ss[0] =ss[6];
	ss[6] =t;
	t =ss[1];
	ss[1]       =ss[7];
	ss[7]       =t;
	t      =ss[2];
	ss[2]       =ss[4];
	ss[4]       =t;
	t      =ss[3];
	ss[3]       =ss[5];
	ss[5]       =t;
    //****** 将存有十六进制数 (val) 的字符串 (ss) 中的十六进制数转成十进制数
    int value=0;
	sscanf(ss,"%x",&value);  
    return (value);
}

string CReadShapeFile::OnReadShp(string ShpFileName,std::vector<myPoint>& vecPt)
{
	vecPt.clear();
	//****Shp 文件指针
    FILE*   m_ShpFile_fp;      
	// 打开坐标文件
    if((m_ShpFile_fp=fopen(ShpFileName.c_str(),"rb"))==NULL)
	{
        return " ";
    }

    // 读取坐标文件头的内容开始
    int FileCode;
    int Unused;
    int FileLength;
    int Version;
    int ShapeType;
    double Xmin;
    double Ymin;
    double Xmax;
    double Ymax;
    double Zmin;
    double Zmax;
    double Mmin;
    double Mmax;
    fread(&FileCode,     sizeof(int),   1,m_ShpFile_fp);
    for(int i=0;i<5;i++)
        fread(&Unused,sizeof(int),   1,m_ShpFile_fp);
    fread(&FileLength,   sizeof(int),   1,m_ShpFile_fp);
    fread(&Version,          sizeof(int),   1,m_ShpFile_fp);
    fread(&ShapeType,    sizeof(int),   1,m_ShpFile_fp);
    fread(&Xmin,         sizeof(double),1,m_ShpFile_fp);
    fread(&Ymin,         sizeof(double),1,m_ShpFile_fp);
    fread(&Xmax,         sizeof(double),1,m_ShpFile_fp);
    fread(&Ymax,         sizeof(double),1,m_ShpFile_fp);
    fread(&Zmin,         sizeof(double),1,m_ShpFile_fp);
    fread(&Zmax,        sizeof(double),1,m_ShpFile_fp);
    fread(&Mmin,         sizeof(double),1,m_ShpFile_fp);
    fread(&Mmax,         sizeof(double),1,m_ShpFile_fp);
    // 读取坐标文件头的内容结束



    // 根据几何类型读取实体信息
	string sgeoType;
	int RecordNumber;
	int ContentLength;
	//面状信息
	if (ShapeType==5)
	{
		sgeoType = "polygon";
		while((fread(&RecordNumber,    sizeof(int),   1,m_ShpFile_fp)!=0))
		{
			fread(&ContentLength,sizeof(int),   1,m_ShpFile_fp);
			int shapeType;
			double Box[4];
			int NumParts;
			int NumPoints;
			int *Parts;
			fread(&shapeType,    sizeof(int),   1,m_ShpFile_fp);
			// 读 Box
			for(int i=0;i<4;i++)
			{
				fread(Box+i,     sizeof(double),1,m_ShpFile_fp);
			}
			myPoint pt;
			pt.x = (*Box+*(Box+2))/2;
			pt.y = (*(Box+1)+*(Box+3))/2;
			vecPt.push_back(pt);
			// 读 NumParts 和 NumPoints
			fread(&NumParts,     sizeof(int),   1,m_ShpFile_fp);
			fread(&NumPoints,    sizeof(int),   1,m_ShpFile_fp);
			// 读 Parts 和 Points
			Parts =new int[NumParts];
			for(int i=0;i<NumParts;i++)
			{
				fread(Parts+i,   sizeof(int),   1,m_ShpFile_fp);
			}
			int pointNum;
			int xx;
			int yy;
			for(int i=0;i<NumParts;i++)
			{
				if(i!=NumParts-1)
					pointNum =Parts[i+1]-Parts[i];
				else
					pointNum =NumPoints-Parts[i];
				double *PointsX;
				double *PointsY;
				PointsX =new double[pointNum];
				PointsY =new double[pointNum];
				for(int j=0;j<pointNum;j++)
				{
					fread(PointsX+j, sizeof(double),1,m_ShpFile_fp);
					fread(PointsY+j, sizeof(double),1,m_ShpFile_fp);
				}
				delete[] PointsX;
				delete[] PointsY;
			}
			delete[] Parts;	
		}	
	}
	//点状信息
	else if (ShapeType == 1)
	{
		sgeoType = "point";
		while((fread(&RecordNumber,    sizeof(int),   1,m_ShpFile_fp)!=0))
		{
			fread(&ContentLength,sizeof(int),   1,m_ShpFile_fp);
			int shapeType;
			double x;
			double y;
			fread(&shapeType, sizeof(int),   1,m_ShpFile_fp);
			fread(&x, sizeof(double),   1,m_ShpFile_fp);
			fread(&y, sizeof(double),   1,m_ShpFile_fp);
			vecPt.push_back(myPoint(x,y,0));
		}
	}
	fclose(m_ShpFile_fp);
	return sgeoType;
}

void CReadShapeFile::OnReadDbf(string DbfFileName,vector<string>& vecLab,vector<myPoint>& vecPt)
{
	vecLab.clear();
	//****Dbf 文件指针
	FILE*   m_DbfFile_fp;        
    // 打开 dbf 文件
    if((m_DbfFile_fp=fopen(DbfFileName.c_str(),"rb"))==NULL)
    {
        return;
    }
    int i,j;
    //////**** 读取 dbf 文件的文件头  开始
	vector<CKeyValue> mfiled;
	short lenMax = 0;;
    BYTE version;
	int Unused;
    fread(&version,     1,   1,m_DbfFile_fp);
    BYTE date[3];
    for(i=0;i<3;i++)
    {
        fread(date+i,     1,   1,m_DbfFile_fp);		
    }
    int RecordNum;            //******
    fread(&RecordNum,         sizeof(int),   1,m_DbfFile_fp);
    short HeaderByteNum;
    fread(&HeaderByteNum, sizeof(short), 1,m_DbfFile_fp);
    short RecordByteNum;
    fread(&RecordByteNum, sizeof(short), 1,m_DbfFile_fp);
    short Reserved1;          
    fread(&Reserved1, sizeof(short), 1,m_DbfFile_fp);	
    BYTE Flag4s;
    fread(&Flag4s, sizeof(BYTE), 1,m_DbfFile_fp);	
    BYTE EncrypteFlag;
    fread(&EncrypteFlag, sizeof(BYTE), 1,m_DbfFile_fp);      
    for(i=0;i<3;i++)
    {
        fread(&Unused,        sizeof(int),   1,m_DbfFile_fp);
    }
    BYTE MDXFlag;
    fread(&MDXFlag,    sizeof(BYTE), 1,m_DbfFile_fp);
    BYTE LDriID;
    fread(&LDriID,                sizeof(BYTE), 1,m_DbfFile_fp);
    short Reserved2;
    fread(&Reserved2,    sizeof(short), 1,m_DbfFile_fp);
    BYTE name[11];
    BYTE fieldType;
    int Reserved3;
    BYTE fieldLength;
    BYTE decimalCount;
    short Reserved4;
    BYTE workID;
    short Reserved5[5];
    BYTE mDXFlag1;
    int fieldscount;
    fieldscount = (HeaderByteNum - 32) / 32;
	for (i=0;i< fieldscount;i++)
	{
		fread(name,    11, 1,m_DbfFile_fp);
		char* buffer=new char[12];
		for (int k = 0;k<11;k++)
		{
			buffer[k] = name[k];
		}
		buffer[11] = '\0';
		fread(&fieldType,   sizeof(BYTE), 1,m_DbfFile_fp);			
		//Reserved3----4     bytes
		Reserved3      =0;
		fread(&Reserved3, sizeof(int), 1,m_DbfFile_fp);
		//FieldLength--1     bytes
		fread(&fieldLength,sizeof(BYTE), 1,m_DbfFile_fp);
		mfiled.push_back(CKeyValue(string(buffer),short(fieldLength)));
		if (short(fieldLength)>lenMax)
		{
			lenMax = short(fieldLength);
		}
		//DecimalCount-1   bytes
		fread(&decimalCount,sizeof(BYTE), 1,m_DbfFile_fp);	
		//Reserved4----2     bytes
		Reserved4      =0;
		fread(&Reserved4, sizeof(short), 1,m_DbfFile_fp);
		//WorkID-------1    bytes
		fread(&workID,            sizeof(BYTE), 1,m_DbfFile_fp);	
		//Reserved5----10   bytes
		for(j=0;j<5;j++)
		{
			fread(Reserved5+j,sizeof(short), 1,m_DbfFile_fp);
		}
		//MDXFlag1-----1 bytes
		fread(&mDXFlag1,       sizeof(BYTE), 1,m_DbfFile_fp);	
		delete[] buffer;
	}
	BYTE terminator;
	fread(&terminator, sizeof(BYTE), 1,m_DbfFile_fp);  
	//文件头读取完毕
	//
	//
	//开始读记录
	BYTE   deleteFlag;
	char* text = new char[lenMax];
	for(j=0;j<lenMax;j++)
		strcpy(text+j,"");
	for(i=0;i<RecordNum;i++)
	{
		fread(&deleteFlag, sizeof(BYTE), 1,m_DbfFile_fp);
		for (vector<CKeyValue>::iterator ite = mfiled.begin();ite!=mfiled.end();++ite)
		{
			//读字段
			for(j=0;j<(*ite).val;j++)
			{
				fread(text+j, sizeof(char), 1,m_DbfFile_fp);
			}
			if ((*ite).key == "NAME" || (*ite).key == "建筑名称" || (*ite).key == "栋号" || (*ite).key == "name")
			{
				text[lenMax-1] = '\0';
				
				string str(text);
				int pos = str.find(" ");
				if (pos!=-1)
				{
					str = str.substr(0,pos+1);
				}
				vecLab.push_back(str);
			}
			else if ((*ite).key == "TH")
			{
				text[lenMax-1] = '\0';
				double th = atof(text);
				vecPt[i].z = th;
			}
			for(j=0;j<lenMax;j++)
				strcpy(text+j,"");
		}
	}
	fclose(m_DbfFile_fp);
}

void CReadShapeFile::updateHbyImpact(vector<osg::Vec3d>& vecPt,osg::Group* root)
{
	osg::Vec3d p;
	for (int i = 0;i<vecPt.size();++i)
	{
		osg::Vec3d p1 = vecPt[i];
		osg::Vec3d p2 = p1+osg::Vec3d(0,0,1000);
		osg::ref_ptr<osgUtil::LineSegmentIntersector> ls = new osgUtil::LineSegmentIntersector(p1, p2);
		osgUtil::IntersectionVisitor iv(ls);
		root->accept(iv);
		if (ls.valid())
		{
			if (ls->containsIntersections())
			{
				p =ls->getFirstIntersection().getWorldIntersectPoint();
				vecPt[i].z() = p.z();
			}
		}

		/*if (ls.valid() && ls->containsIntersections())
		{
			p1 =ls->getFirstIntersection().getWorldIntersectPoint();
			vecPt[i].z = p1.z();
		}*/
	}	
}

string CReadShapeFile::multiLine(std::string s,int k)
{
	string s1,s2;
	int n = s.size();
	if (n<2*k+1)
	{
		return s;
	}
	if ((n/2)%2 == 0)
	{
		s1 = s.substr(0,n/2);
		s2 = s.substr(n/2);
		return s1+"\n"+s2;
	}
	else
	{
		s1 = s.substr(0,n/2+1);
		s2 = s.substr(n/2+1);
		return s1+"\n"+s2;
	}
}

std::vector<int>& CReadShapeFile::vacuate(vector<myPoint>& vecPt,double dis)
{
//	assert(!vecPt.empty());
	std::vector<int> vResult;
	return vResult;
}

//osg::ref_ptr<osgText::Text> CReadShapeFile::createText(osg::Vec3d pos,osg::Vec4 color,osgText::Font* font,std::string title,float size)
//{
//	osg::ref_ptr<osgText::Text> text = new osgText::Text; 
//	text->setFont( font );  
//	text->setColor( color );  
//	text->setCharacterSize( size );  
//	text->setPosition( pos);  
//	text->setAxisAlignment( osgText::Text::SCREEN );
//	text->setText(title.c_str());
//	text->setAlignment(osgText::TextBase::CENTER_BOTTOM);
//	//text->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
//	text->setBackdropType(osgText::Text::OUTLINE);
//	//text->setBackdropColor(osg::Vec4(0,0,0,1));
//	//text->setBoundingBoxColor(osg::Vec4(1,1,0,1));
//	text->setDrawMode(osgText::Text::TEXT);
//
//	
//	return text;
//}

osg::PositionAttitudeTransform* CReadShapeFile::create3DText(osg::Vec3d pos,osg::Vec4 color, float radius, std::string title)
{
	osg::Billboard* geode = new osg::Billboard();
	geode->setMode(osg::Billboard::POINT_ROT_EYE);
	float characterSize = radius * 0.2f;
	float characterDepth = characterSize * 0.2f;

	osgText::Text3D* text1 = new osgText::Text3D;
	text1->setFont("fonts/simhei.ttf");
	text1->setColor(osg::Vec4(46/255.0, 211/255.0, 250/255.0,1));
	text1->setCharacterSize(characterSize);
	text1->setCharacterDepth(characterDepth);
	//text1->setPosition(pos);
	text1->setDrawMode(osgText::Text3D::TEXT);
	text1->setAxisAlignment(osgText::Text3D::XZ_PLANE);
	//std::string str =  sText.GetBuffer(0);
	text1->setText(title.c_str());
	//text1->setText(str.c_str());
	text1->setLineSpacing(0.25);
	text1->setBoundingBoxColor(osg::Vec4(0,0,0,1));
	text1->setBoundingBoxMargin(0.5);
	osg::ref_ptr<osgText::Style> style = new osgText::Style;
	osg::ref_ptr<osgText::Bevel> bevel = new osgText::Bevel;
	bevel->roundedBevel2(0.25);
	style->setBevel(bevel.get());
	style->setWidthRatio(0.2f);
	style->setThicknessRatio(0);
	text1->setStyle(style.get());

	geode->addDrawable(text1);

	osg::PositionAttitudeTransform* rootNode = new osg::PositionAttitudeTransform;
	rootNode->addChild(geode);
	rootNode->setPosition(pos);


	osg::Material* front = new osg::Material;
	front->setAlpha(osg::Material::FRONT_AND_BACK, 1);
	front->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2, 0.2, 0.2, 1.0));
	front->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(.0, .0, 1.0, 1.0));

	rootNode->getOrCreateStateSet()->setAttributeAndModes(front);


	return rootNode;
}

osg::Billboard* CReadShapeFile::createText(osg::Vec3d pos,osg::Vec4 textColor,osg::Vec4 outlineColor, float layoutCharacterSize, std::string title)
{
	osg::Billboard* geode = new osg::Billboard();
	geode->setMode(osg::Billboard::AXIAL_ROT);
	osgText::Text* text = new osgText::Text;
	osgText::Font* font = osgText::readFontFile("fonts/simhei.ttf");
	text->setFont(font);
	text->setColor(textColor);
	layoutCharacterSize *= 0.2f;
	text->setCharacterSize(layoutCharacterSize);
	text->setAlignment(osgText::TextBase::CENTER_BOTTOM);
	text->setBackdropType(osgText::Text::OUTLINE);
	text->setAxisAlignment(osgText::Text::SCREEN);
	text->setBackdropColor(outlineColor);
	text->setText(title.c_str());

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	text->setStateSet(stateset);

	geode->addDrawable(text);
	geode->setPosition(0,pos);
	return geode;
}