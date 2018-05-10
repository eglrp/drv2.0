#include "UILanguages.h"
#include "x3py/manager/iworkpath.h"
#include "x3py/manager/x3manager.h"
#include "x3py/utilfunc/convstr.h"

BEGIN_NAMESPACE_EARTHMATRIX
#define MAX_STRING_LENGTH 2048

UILanguages::UILanguages()
{
	LoadLanguages();
}

UILanguages::~UILanguages()
{
	clear();
}

void UILanguages::LoadLanguages()
{
	clear();

	x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
	if( spAppWorkPath!=NULL )
	{			
		std::wstring path = spAppWorkPath->getWorkPath();
		m_strConfigFile = path + L"config.ini";
		scanfiles((path + L"\\Language").c_str(), false);
		::CreateDirectoryW((path + L"\\Language").c_str(), NULL);
		
		std::wstring defaultName = spAppWorkPath->getLocaleName();
		if( size()>0 )
		{
			wchar_t szValue[MAX_STRING_LENGTH];
			memset(szValue, 0, sizeof(szValue));
			GetPrivateProfileStringW(L"Setting", L"Language", defaultName.c_str(), szValue, sizeof(szValue), m_strConfigFile.c_str());
			if( wcslen(szValue)>0 )
			{
				x3::LockRW locker(m_vtLanguage.locker);				
				if (locker.canRead())
				{
					std::vector<SLanguageItem>::iterator it = m_vtLanguage.begin();
					for( ; it!=m_vtLanguage.end(); it++ )
					{
						if( _wcsicmp(szValue, PathFindFileNameW(it->file.c_str()))==0 )
						{
							m_strFile = it->file;
							break;
						}
					}
				}
			}

			if( m_strFile.empty() )
			{
				if( size()==1 )
				{
					x3::LockRW locker(m_vtLanguage.locker);				
					if (locker.canRead())
					{
						std::vector<SLanguageItem>::iterator it = m_vtLanguage.begin();
						if( it!=m_vtLanguage.end() )
							m_strFile = it->file;
					}
				}
				else
				{
					x3::LockRW locker(m_vtLanguage.locker);				
					if (locker.canRead())
					{
						std::vector<SLanguageItem>::iterator it = m_vtLanguage.begin();
						for( ; it!=m_vtLanguage.end(); it++ )
						{
							if( _wcsicmp(L"en-US", PathFindFileNameW(it->file.c_str()))==0 )
							{
								m_strFile = it->file;
								break;
							}
						}
					}
				}
			}
		}	
		else
		{
			m_strFile = path + L"Language\\" + defaultName + L".ini";
		}
	}
}

bool UILanguages::loadfilter(const wchar_t* filename, const wchar_t* ext)
{
	if (_wcsicmp(ext, L".ini") == 0)
	{
		SLanguageItem item;
		item.file = filename;

		wchar_t szValue[MAX_STRING_LENGTH];
		memset(szValue, 0, sizeof(szValue));
		GetPrivateProfileStringW(L"Language", L"Name", L"", szValue, sizeof(szValue), filename);
		if( wcslen(szValue)<=0 )
			return true;
		
		item.name = szValue;
		memset(szValue, 0, sizeof(szValue));
		GetPrivateProfileStringW(L"Language", L"Code", L"", szValue, sizeof(szValue), filename);
		if( wcslen(szValue)<=0 )
			return true;

		item.code = _wtoi(szValue);
		x3::LockRW locker(m_vtLanguage.locker, true);
		if (locker.canWrite())
			m_vtLanguage.push_back(item);
	}

	return true;
}

int UILanguages::scanfiles(const wchar_t* path, bool recursive)
{
	int count = 0;
	wchar_t filename[MAX_PATH];
	bool cancel = false;

#ifdef _WIN32
	lstrcpynW(filename, path, MAX_PATH);
	PathAppendW(filename, L"*.*");

	WIN32_FIND_DATAW fd;
	HANDLE hfind = ::FindFirstFileW(filename, &fd);
	BOOL loop = (hfind != INVALID_HANDLE_VALUE);

	for (; loop && !cancel; loop = ::FindNextFileW(hfind, &fd))
	{
		if (fd.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
		{
		}
		else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (fd.cFileName[0] != '.')
			{
				lstrcpynW(filename, path, MAX_PATH);
				PathAppendW(filename, fd.cFileName);
				PathAddBackslashW(filename);

				cancel = !loadfilter(filename, L"");
				if (recursive && !cancel)
				{
					count += scanfiles(filename, recursive);
				}
			}
		}
		else
		{
			lstrcpynW(filename, path, MAX_PATH);
			PathAppendW(filename, fd.cFileName);
			cancel = !loadfilter(filename, PathFindExtensionW(filename));
			count++;
		}
	}

	::FindClose(hfind);
#else
	std::vector<std::wstring> dirs;
	struct dirent *dp;
	DIR *dirp;

	dirp = opendir(path);
	if (dirp)
	{
		while (!cancel && (dp = readdir(dirp)) != NULL)
		{
			if (DT_DIR == dp->d_type && dp->d_name[0] != '.')
			{
				dirs.push_back(dp->d_name);
			}
			else if (DT_REG == dp->d_type)
			{
				strncpy(filename, path, MAX_PATH);
				PathAppendA(filename, dp->d_name);
				cancel = !loadfilter(filename, "");
				count++;
			}
		}
		closedir(dirp);
	}

	std::vector<std::string>::const_iterator it = dirs.begin();
	for (; it != dirs.end() && !cancel; ++it)
	{
		strncpy(filename, path, MAX_PATH);
		PathAppendW(filename, it->c_str());
		PathAddBackslashW(filename);

		cancel = !loadfilter(filename, PathFindExtensionW(filename));
		if (recursive && !cancel)
		{
			count += scanfiles(filename, recursive);
		}
	}
#endif

	return count;
}

void UILanguages::clear()
{
	x3::LockRW locker(m_vtLanguage.locker, true);
	if (locker.canWrite())
		m_vtLanguage.clear();
	m_strFile = L"";
	m_strConfigFile = L"";
}

int UILanguages::size()
{
	x3::LockRW locker(m_vtLanguage.locker);
	return locker.canRead() ? m_vtLanguage.size() : 0;
}

const wchar_t* UILanguages::getName(int index)
{
	x3::LockRW locker(m_vtLanguage.locker);
	const wchar_t* ret = NULL;
	if (locker.canRead())
	{
		if( index>=0 && index<m_vtLanguage.size() )
			ret = m_vtLanguage[index].name.c_str();
	}

	return ret;
}

bool UILanguages::select(int index)
{
	x3::LockRW locker(m_vtLanguage.locker);
	bool ret = false;
	if (locker.canRead())
	{
		if( index>=0 && index<m_vtLanguage.size() )
			ret = WritePrivateProfileStringW(L"Setting", L"Language", PathFindFileNameW(m_vtLanguage[index].file.c_str()), m_strConfigFile.c_str()) ? true : false;
	}

	return ret;
}

const wchar_t* UILanguages::GetString(const wchar_t* app, const wchar_t* stringId, const wchar_t* defualt)
{
	if( m_strFile.empty() || app==NULL || stringId==NULL )
		return NULL;

	wchar_t szValue[MAX_STRING_LENGTH];
	memset(szValue, 0, sizeof(szValue));
	GetPrivateProfileStringW(app, stringId, defualt, szValue, sizeof(szValue), m_strFile.c_str());

#ifdef _DEBUG
	WritePrivateProfileStringW(app, stringId, szValue, m_strFile.c_str());
#endif
	return szValue;
}

const wchar_t* UILanguages::GetString(const wchar_t* app, const int id, const wchar_t* defualt)
{
	if( m_strFile.empty() || app==NULL || id<0 )
		return NULL;

	wchar_t szKey[MAX_STRING_LENGTH];
	_itow_s(id, szKey, sizeof(szKey), 10);

	wchar_t szValue[MAX_STRING_LENGTH];
	memset(szValue, 0, sizeof(szValue));
	GetPrivateProfileStringW(app, szKey, defualt, szValue, sizeof(szValue), m_strFile.c_str());

#ifdef _DEBUG
	WritePrivateProfileStringW(app, szKey, szValue, m_strFile.c_str());
#endif
	return szValue;
}

bool UILanguages::LocalizeDialog(size_t hwnd, const wchar_t* app, const wchar_t *stringId)
{
#ifdef _WIN32
	//LastDialogId = stringId;
	SetWindowLongPtr ((HWND)hwnd, GWLP_USERDATA, (LONG_PTR) 'TRUE');
	if (stringId == NULL)
		SetWindowTextW((HWND)hwnd, L"");
	else
	{
		wchar_t szDefault[MAX_STRING_LENGTH];
		memset(szDefault, 0, sizeof(szDefault));
		GetWindowTextW((HWND)hwnd, szDefault, sizeof(szDefault));
		SetWindowTextW ((HWND)hwnd, GetString(app, stringId, szDefault));
	}

	SLocalizeDialogEnumData sData;
	sData.pLanguages = this;
	sData.app = app;
	EnumChildWindows ((HWND)hwnd, LocalizeDialogEnum, (LPARAM)(&sData));
	return true;
#else
	return false;
#endif
}

bool UILanguages::LocalizeDialog(size_t hwnd, const wchar_t* app, const int id)
{
#ifdef _WIN32
	//LastDialogId = stringId;
	SetWindowLongPtr ((HWND)hwnd, GWLP_USERDATA, (LONG_PTR) 'TRUE');
	if (id<0)
		SetWindowTextW((HWND)hwnd, L"");
	else
	{
		wchar_t szDefault[MAX_STRING_LENGTH];
		memset(szDefault, 0, sizeof(szDefault));
		GetWindowTextW((HWND)hwnd, szDefault, sizeof(szDefault));
		SetWindowTextW ((HWND)hwnd, GetString(app, id, szDefault));
	}

	SLocalizeDialogEnumData sData;
	sData.pLanguages = this;
	sData.app = app;
	EnumChildWindows ((HWND)hwnd, LocalizeDialogEnum, (LPARAM)(&sData));
	return true;
#else
	return false;
#endif
}

BOOL UILanguages::LocalizeDialogEnum( HWND hwnd, LPARAM data)
{
	LPLocalizeDialogEnumData pData = (LPLocalizeDialogEnumData)data;
	if( pData==NULL || pData->pLanguages==NULL )
		return FALSE;

	// Localization of controls
	int ctrlId = GetDlgCtrlID (hwnd);
	if (ctrlId != 0)
	{
		wchar_t name[10] = { 0 };
		GetClassNameW (hwnd, name, sizeof (name));

		if (_wcsicmp (name, L"Button") == 0 || _wcsicmp (name, L"Static") == 0)
		{
			wchar_t szDefault[MAX_STRING_LENGTH];
			memset(szDefault, 0, sizeof(szDefault));
			GetWindowTextW(hwnd, szDefault, sizeof(szDefault));
			SetWindowTextW (hwnd, pData->pLanguages->GetString(pData->app.c_str(), ctrlId, szDefault));
		}
	}

	return TRUE;
}


END_NAMESPACE_EARTHMATRIX