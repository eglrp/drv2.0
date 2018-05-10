#pragma  once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "x3py/utilfunc/lockrw.h"
#include <vector>

BEGIN_NAMESPACE_EARTHMATRIX

class UILanguages : public IUILanguages
{
	X3BEGIN_CLASS_DECLARE(UILanguages, clsidLanguages)		
		X3DEFINE_INTERFACE_ENTRY(IUILanguages)				
	X3END_CLASS_DECLARE()

public:
	UILanguages();
	virtual ~UILanguages();

public:
	virtual int size();
	virtual const wchar_t* getName(int index);
	virtual bool select(int index);
	virtual const wchar_t* GetString(const wchar_t* app, const wchar_t* stringId, const wchar_t* defualt = NULL);
	virtual const wchar_t* GetString(const wchar_t* app, const int id, const wchar_t* defualt = NULL);
	virtual bool LocalizeDialog(size_t hwnd, const wchar_t* app, const wchar_t *stringId);
	virtual bool LocalizeDialog(size_t hwnd, const wchar_t* app, const int id);

protected:
	void LoadLanguages();	
	int scanfiles(const wchar_t* path, bool recursive);
	bool loadfilter(const wchar_t* filename, const wchar_t* ext);
	void clear();

	typedef struct
	{
		UILanguages* pLanguages;
		std::wstring app;
	}SLocalizeDialogEnumData, *LPLocalizeDialogEnumData;
	static BOOL CALLBACK LocalizeDialogEnum( HWND hwnd, LPARAM data);

	typedef struct  
	{
		std::wstring name;
		int code;
		std::wstring file;
	}SLanguageItem;
	x3::LockRW_<std::vector<SLanguageItem> >  m_vtLanguage;
	std::wstring m_strFile;
	std::wstring m_strConfigFile;
};

END_NAMESPACE_EARTHMATRIX