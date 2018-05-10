#pragma once
#include <algorithm>
#include "osg/Node"
#include "osgText/Font"  
#include "osgText/Text"
#include "osg/PositionAttitudeTransform"
#include <osg/Billboard>
#include "osg/LineWidth"
#include <osgText/Text3D>
#include <osg/Material>
#include <list>

namespace COMFUNC
{
	class PAIR1
	{
	public:
		int n;
		std::string str;
	};
	//osg::Billboard* createText(osg::Vec3d pos,osg::Vec4 textColor,osg::Vec4 outlineColor, float layoutCharacterSize, std::string title);
	bool allAlpha(std::string str);

	template<typename T>
	bool isStringInVector(T,std::vector<T>&);

	CString& getChineseSex(CString& s);

	void split(std::string s,std::string sflag,std::vector<CString>& vec);

	int findOlder(std::vector<std::string>& vec);//�������֤��ѡ��������

	std::string getDoorName(std::string& first,std::string& second);//�����Ŷ���Ԫ������������

	void getLastNRecord(std::list<std::string>& ls,std::list<std::string>& lt,int n = 5);//��ȡlist���n������

	void dateTrans(std::string s1,std::string & s2);//ת�����ڸ�ʽ
}