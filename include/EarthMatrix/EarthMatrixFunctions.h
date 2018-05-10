#pragma once
#include "UIPlugins.h"
#include "x3py/utilfunc/convstr.h"
USING_NAMESPACE_EARTHMATRIX

#ifdef _AFX
// Load String From Resource
inline const wchar_t* LoadStringW(const wchar_t* app, int id, const wchar_t* key)
{
	AFX_MANAGE_STATE_EX
	if( app==NULL || id<0 )
		return NULL;

	x3::Object<IUILanguages> spLanguages(clsidLanguages);

	CString strValue;
	strValue.LoadString(id);
	if( spLanguages!=NULL )
		return spLanguages->GetString(app, key, x3::t2w(strValue).c_str());
	else
		return x3::t2w(strValue).c_str();
}

#define LOADSTRINGA(app, id) x3::w2a(LoadStringW(x3::a2w(app).c_str(), id, x3::a2w(#id).c_str())).c_str()
#define LOADSTRINGW(app, id) LoadStringW(app, id, x3::a2w(#id).c_str())
#define LOADSTRINGT(app, id) x3::w2t(LoadStringW(x3::t2w(app).c_str(), id, x3::a2w(#id).c_str())).c_str()
#endif

inline const wchar_t* GetStringW(const wchar_t* app, const wchar_t* key, const wchar_t* default)
{
	if( app==NULL || key==NULL )
		return default;

	x3::Object<IUILanguages> spLanguages(clsidLanguages);
	if( spLanguages==NULL )
		return default;

	return spLanguages->GetString(app, key, default);
}

#define GETSTRINGA(app, key, default) x3::w2a(GetStringW(x3::a2w(app).c_str(), x3::a2w(key).c_str(), x3::a2w(default).c_str())).c_str()
#define GETSTRINGW(app, key, default) GetStringW(app, key, default)
#define GETSTRINGT(app, key, default) x3::w2t(GetStringW(x3::t2w(app).c_str(), x3::t2w(key).c_str(), x3::t2w(default).c_str())).c_str()