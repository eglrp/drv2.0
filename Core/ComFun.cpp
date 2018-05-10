#include "ComFun.h"
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>

using namespace std;

#pragma comment(lib,"Shlwapi.lib")

//分析用特定符号分隔的数据
void AnalyzeData(CStringArray& aResult, CString sTarget, CString sDivider)
{
	aResult.RemoveAll();
	if(sTarget.GetLength() == 0) return;

	int nPos = 0, nLastPos = 0;
	while((nPos = sTarget.Find(sDivider, nLastPos)) != -1)
	{
		aResult.Add(sTarget.Mid(nLastPos, nPos - nLastPos));
		nLastPos = nPos + sDivider.GetLength();
	}
	if (nPos == -1)
	{
		aResult.Add(sTarget.Mid(nLastPos, sTarget.GetLength() - nLastPos));
	}
}

CString GetFileName(CString sFilePath)
{
	CString fileName = _T("");
	CFileFind ff;
	BOOL bFind;
	bFind = ff.FindFile(sFilePath);
	if (bFind)
	{
		bFind = ff.FindNextFile();
		fileName = ff.GetFileName();
	}
	ff.Close();
	return fileName;
}

CString OleTime2Str(CTime oleTime)
{
	CString  m_szDateTime;
	m_szDateTime = _T("2007-5-17");
	m_szDateTime = oleTime.Format(_T("%Y-%m-%d"));
	return m_szDateTime;
}

CTime Str2Time(CString strTime)
{
	COleVariant   vtime(strTime);

	vtime.ChangeType(VT_DATE);

	COleDateTime   time4 = vtime;

	SYSTEMTIME   systime;

	VariantTimeToSystemTime(time4,   &systime);
	CTime tm(systime);
	return tm;
}

bool StrToBool(CString str)
{
	if (str == _T("0") || str == _T(""))
	{
		return false;
	}
	return true;
}

CString BoolToStr(bool b)
{
	if (b)
	{
		return(_T("1"));
	}
	return(_T("0"));
}

CString GetCurrentDate()
{
	time_t now;
	time(&now);
	struct tm* tm = NULL;
	gmtime_s(tm, &now);
	int nian = tm->tm_year + 1900;
	int yue = tm->tm_mon + 1;
	int ri = tm->tm_mday;
	CString strTime = _T("");
	strTime.Format(_T("%d-%02d-%02d"), nian, yue, ri);
	return strTime;
}

CString Char2Hex(char c)
{
	CString str = L"";
	char ch, cl;
	ch = ((c & 0xF0) >> 4) + '0'; //不能用char char ch = c/16 + '0'; 否则汉字会出错
	if(ch > '9')ch += ('A' - '9' - 1);
	cl = (c & 0x0f) + '0';
	if(cl > '9') cl += ('A' - '9' - 1);
	str = ch;
	str = str + cl;
	return str;
}


CString HexToDec(CString strHex)
{
	USES_CONVERSION;
	int nLen = strHex.GetLength();

	TCHAR* p = strHex.GetBuffer(0);

	CString strDec;
	int  val = 0;
	int n = 0, nPower = 1;

	while(nLen > 0)
	{
		nLen--;
		n = strHex.GetLength() - 1 - nLen;
		nPower = 1;
		for(; n > 0; n--)
		{
			nPower = nPower * 16;
		}
		if(p[nLen] >= 'A' && p[nLen] <= 'F')
			val += (p[nLen] - 'A' + 10) * nPower;
		else
		{
			strDec = p[nLen];
			val += _ttoi(strDec) * nPower;
		}
	}
	strDec.Format(TEXT("%d"), val);
	return strDec;
}

bool IsRootDir(CString BrowerFile)
{
	if(BrowerFile.GetLength() <= 3)
	{
		return true;
	}
	return false;
}

LPCTSTR  bytesToHexStr(char* buffer, int len, CString& hexStr)
{
	unsigned char k;
	int j;
	CString tmpStr;

	hexStr.Empty();    // make sure nothing in it yet since we are concatenating it

	for (j = 0; j < len; j++)
	{
		k = static_cast<unsigned char>(buffer[j]);
		tmpStr.Format(_T("%02hX"), k);
		hexStr += tmpStr;
	}
	return hexStr;
}

void getFiles(const std::string &file, const std::vector<std::string> &exts, const std::vector<std::string> &blackExts, std::vector<std::string> &files)
{
	if (osgDB::fileType(file) == osgDB::DIRECTORY)
	{
		osgDB::DirectoryContents contents = osgDB::getDirectoryContents(file);
		for (osgDB::DirectoryContents::iterator itr = contents.begin(); itr != contents.end(); ++itr)
		{
			if (*itr == "." || *itr == "..") continue;

			std::string f = osgDB::concatPaths(file, *itr);

			if (osgDB::fileType(f) == osgDB::DIRECTORY) continue;

			getFiles(f, exts, blackExts, files);
		}
	}
	else
	{
		std::string ext = osgDB::getFileExtension(file);
		bool fileValid = false;
		//If we have no _extensions specified, assume we should try everything
		if (exts.size() == 0)
		{
			fileValid = true;
		}
		else
		{
			//Only accept files with the given _extensions
			for (unsigned int i = 0; i < exts.size(); ++i)
			{
				if (osgDB::equalCaseInsensitive(ext, exts[i]))
				{
					fileValid = true;
					break;
				}
			}
		}

		//Ignore any files that have blacklisted extensions
		for (unsigned int i = 0; i < blackExts.size(); ++i)
		{
			if (osgDB::equalCaseInsensitive(ext, blackExts[i]))
			{
				fileValid = false;
				break;
			}
		}

		if (fileValid)
		{
			files.push_back(osgDB::convertFileNameToNativeStyle(file));
		}
	}
}

CString getFileContent(CString sFileName)
{
	//不能用ifstream，osgdb跟mygui的OpenGL.lib冲突了，重复定义错误
	fstream fs(sFileName);
	string line;
	CString sContent;
	CString sLine;
	while(getline(fs,line))
	{
		sLine = line.c_str();
		sContent += sLine + L"\r\n";
	}
	return sContent;
}

string int2str(int n) {

	char t[24];
	int i = 0;

	while (n) {
		t[i++] = (n % 10) + '0';
		n /= 10;
	}
	t[i] = 0;

	return string(strrev(t));
}