#include "stdafx.h"
#include <vector>
#include <stdlib.h>
#include <memory.h>

using namespace std;

bool StrToBool(CString str);

CString BoolToStr(bool b);

//十六进制转为十进制
CString HexToDec(CString strHex);

//char转为十六进制
CString Char2Hex(char c);

//将时间转为字符串
CString OleTime2Str(CTime oleTime);

//将字符串转为时间
CTime Str2Time(CString strTime);

void AnalyzeData(CStringArray& aResult, CString sTarget, CString sDivider);

CString GetFileName(CString filepath);

bool IsRootDir(CString BrowerFile);

void getFiles(const std::string &file, const std::vector<std::string> &exts, const std::vector<std::string> &blackExts, std::vector<std::string> &files);

CString getFileContent(CString sFileName);

string int2str(int n);