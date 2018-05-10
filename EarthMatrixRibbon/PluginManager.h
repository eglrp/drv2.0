#pragma once
#include "x3py/module/plugininc.h"
#include "x3py/objptr.h"
#include "x3py/utilfunc/lockrw.h"
#include "UIPlugins.h"
USING_NAMESPACE_EARTHMATRIX

#if defined(_MSC_VER) && _MSC_VER > 1200    // VC8/9
	#include <hash_map>
	using stdext::hash_multimap;
	using stdext::hash_map;
#else                                       // VC6, GCC or others
	#define hash_multimap std::multimap
	#define hash_map std::map
#endif

class CPluginManager
{
public:
	CPluginManager(void);
	~CPluginManager(void);

public:
	static bool Initialize(const char* folder = "plugins");
	static bool UnInitialize();

public:
	bool LoadPlugins();
	bool UnloadPlugins();
	int  GetPluginCount();
	bool GetPlugin(int index, int& id, IUICommon** ppICommon);
	IUICommon* FindPlugin(int id);
	void SetLastTool(int id);
	IUITool* LastTool();

protected:
	int  GetNextID();
	void FreeID(int id);

public:
	static int MINID;
	static int MAXID;
	
protected:
	x3::LockRW_<hash_map<int, x3::AnyObject>> m_mapPlugin;
	x3::LockRW_<std::vector<int>> m_mapIdleID;
	int m_nNextID;
	int m_nLastTool;
};

