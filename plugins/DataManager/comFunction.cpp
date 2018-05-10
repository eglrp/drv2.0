#include "stdafx.h"
#include "comFunction.h"

namespace COMFUNC
{
	bool allAlpha(std::string str)
	{
		if (str.empty())
		{
			return true;
		}
		bool result = false;
		for (size_t i = 0;i<str.size();++i)
		{
			if (!isalpha(str[i]) && str[i] != '_' && str[i] != ' ')
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>  
	bool isStringInVector(T s ,std::vector<T>& svec)
	{
		return find(svec.begin(),svec.end(),s) != svec.end();
	}
	template bool isStringInVector(std::string ,std::vector<std::string>& );


	//将F M解释为中文
	CString& getChineseSex(CString& s)
	{
		if (s.Compare(_T("F")) == 0)
		{
			s = _T("女");
		}
		else if (s.Compare(_T("M")) == 0)
		{
			s = _T("男");
		}
		return s;
	}

	//按指定分隔符将字符串分割
	void split(std::string s,std::string sflag,std::vector<CString>& vec)
	{
		size_t pos;
		vec.clear();
		while( (pos=s.find_first_of(sflag)) != -1)
		{
			vec.push_back(CString((s.substr(0,pos)).c_str()));
			s = s.substr(pos + sflag.size());
		}
		if (!s.empty())
		{
			vec.push_back(CString(s.c_str()));
		}
	}

	int findOlder(std::vector<std::string>& vec)
	{
		int maxAge = 9999;
		for (int i = 0;i<vec.size();++i)
		{
			std::string s = (vec[i]).substr(6,4);
			int a = atoi(s.c_str());
			if (a<maxAge)
			{
				maxAge = a;
			}
		}
		CTime t = CTime::GetCurrentTime(); //获取系统日期
		int y=t.GetYear(); //获取年份
		return y - maxAge;
	}

	std::string getDoorName(std::string& first,std::string& second)
	{
		if (first.find("单元") == -1)
		{
			return second;
		}
		std::string sD = first.substr(0,2);
		std::string sU = first.substr(first.find("单元") - 1,1);

		return sD + "-0" + sU + "-" + second; 
	}

	void getLastNRecord(std::list<std::string>& ls,std::list<std::string>& lt,int n )
	{
		lt.clear();
		if (ls.size()<=n)
		{
			lt.assign(ls.begin(),ls.end());
		}
		else
		{
			int i = 0;
			for (std::list<std::string>::reverse_iterator ite = ls.rbegin();ite != ls.rend();++ite)
			{
				i++;
				lt.push_front(*ite);

				if (i == n)
				{
					break;
				}
			}
		}
	}

	void dateTrans(std::string s1,std::string & s2)
	{
		if(s1.size() != 14)
			return;
		std::string syear = s1.substr(0,4);
		std::string smonth = s1.substr(4,2);
		std::string sday = s1.substr(6,2);
		std::string shour = s1.substr(8,2);
		std::string smin = s1.substr(10,2);
		std::string ssec = s1.substr(12);
		s2 = syear + "-" + smonth + "-" + sday +" "+ shour+":"+smin+":"+ssec;
	}
}

