#include "stdafx.h"
#include <vector>
#include <stdlib.h>
#include <memory.h>

using namespace std;

bool StrToBool(CString str);

CString BoolToStr(bool b);

//ʮ������תΪʮ����
CString HexToDec(CString strHex);

//charתΪʮ������
CString Char2Hex(char c);

//��ʱ��תΪ�ַ���
CString OleTime2Str(CTime oleTime);

//���ַ���תΪʱ��
CTime Str2Time(CString strTime);

void AnalyzeData(CStringArray& aResult, CString sTarget, CString sDivider);

CString GetFileName(CString filepath);

bool IsRootDir(CString BrowerFile);

void getFiles(const std::string &file, const std::vector<std::string> &exts, const std::vector<std::string> &blackExts, std::vector<std::string> &files);

CString getFileContent(CString sFileName);

string int2str(int n);