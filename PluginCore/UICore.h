#pragma once
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "x3py/utilfunc/lockrw.h"

#if defined(_MSC_VER) && _MSC_VER > 1200    // VC8/9
	#include <hash_map>
	using stdext::hash_multimap;
	using stdext::hash_map;
#else                                       // VC6, GCC or others
	#define hash_multimap std::multimap
	#define hash_map std::map
#endif

BEGIN_NAMESPACE_EARTHMATRIX

class CUICore : public IUICore
{
	X3BEGIN_CLASS_DECLARE(CUICore, clsidUICore)		
		X3DEFINE_INTERFACE_ENTRY(IUICore)				
	X3END_CLASS_DECLARE()

public:
	CUICore();
	virtual ~CUICore();

public:
	virtual long size();	
	virtual x3::IObject* Find(const char* key);
	virtual bool Add(const char* key, x3::IObject* obj);
	virtual bool Remove(const char* key);
	virtual bool Remove(x3::IObject* obj);
	virtual void clear();

protected:
	x3::LockRW_<hash_map<std::string, x3::AnyObject> >  m_mapData;
};

END_NAMESPACE_EARTHMATRIX