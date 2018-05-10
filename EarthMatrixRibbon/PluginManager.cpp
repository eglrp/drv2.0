#include "StdAfx.h"
#include "PluginManager.h"
#include "x3py/nonplugin/scanplugins.h"
#include "x3py/manager/x3manager.h"
#include "x3py/manager/iplugins.h"
USING_NAMESPACE_EARTHMATRIX

int CPluginManager::MINID = 3000;
int CPluginManager::MAXID = 3999;

CPluginManager::CPluginManager(void)
{
	m_nNextID = MINID;
	m_nLastTool = -1;
}

CPluginManager::~CPluginManager(void)
{
	UnloadPlugins();
}

bool CPluginManager::Initialize(const char* folder)
{
	return x3::loadScanPlugins(folder)>0;
}

bool CPluginManager::UnInitialize()
{
	x3::unloadScanPlugins();
	return true;
}

bool CPluginManager::LoadPlugins()
{
	UnloadPlugins();

	x3::Object<x3::IPlugins> spPlugins(x3::clsidManager);
	if( !spPlugins.valid() || spPlugins->getPluginCount()<=0 )
		return false;

	x3::Object<IUICore> spUICore(clsidUICore);	
	x3::LockRW lockcls(m_mapPlugin.locker, true);
	bool ret = false;
	if( lockcls.canWrite() )
	{
		int nCount = spPlugins->getCLSIDCount();
		for( int i=0; i<nCount; i++ )
		{
			int nNextID = GetNextID();
			if( nNextID==-1 )
				break;

			const char* clsid = spPlugins->getCLSID(i);
			if( clsid==NULL )
				continue;

			x3::Object<IUICommon> spCommon(clsid);
			if( !spCommon.valid() )
				continue;

			spCommon->Initialize();	
			m_mapPlugin[nNextID] = spCommon.p();
			if( spUICore.valid() )
				spUICore->Add(spCommon->getClassName(), spCommon.p());
			ret = true;
		}
	}

	return ret;
}

bool CPluginManager::UnloadPlugins()
{
	x3::Object<IUICore> spUICore(clsidUICore);
	x3::LockRW lockcls(m_mapPlugin.locker, true);
	if( lockcls.canWrite() )
	{
		hash_map<int, x3::AnyObject>::iterator it = m_mapPlugin.begin();
		for( ; it!=m_mapPlugin.end(); it++ )
		{
			if( it->second.valid() )
			{
				x3::Object<IUICommon> spCommond(it->second);
				if( spCommond.valid() )
				{
					if( spUICore.valid() )
						spUICore->Remove(spCommond->getClassName());
					spCommond->UnInitialize();
				}
			}
		}
		m_mapPlugin.clear();
		m_nNextID = MINID;
		m_nLastTool = -1;

		x3::LockRW lockcls2(m_mapIdleID.locker, true);
		if( lockcls2.canWrite() )
			m_mapIdleID.clear();
	}

	return true;
}

int  CPluginManager::GetNextID()
{
	x3::LockRW lockcls(m_mapIdleID.locker, true);
	int ret = -1;
	if( lockcls.canWrite() )
	{
		std::vector<int>::iterator it = m_mapIdleID.begin();
		if( it!=m_mapIdleID.end() )
		{
			ret = *it;
			m_mapIdleID.erase(it);
		}
	}

	if( ret!=-1 )
		return ret;

	if( m_nNextID>MAXID )
		return ret;

	ret = m_nNextID++;
	return ret;
}

void CPluginManager::FreeID(int id)
{
	if( id==m_nLastTool )
		m_nLastTool = -1;

	if( id<MINID || id>MAXID || id>=m_nNextID )
		return;

	x3::LockRW lockcls(m_mapIdleID.locker, true);	
	if( lockcls.canWrite() )
	{
		std::vector<int>::iterator it = m_mapIdleID.begin();
		for( ; it!=m_mapIdleID.end(); it++ )
		{
			if( *it==id )
				return;
		}

		m_mapIdleID.push_back(id);
	}
}

int CPluginManager::GetPluginCount()
{
	x3::LockRW lockcls(m_mapPlugin.locker);
	int ret = 0;
	if( lockcls.canRead() )
		ret = m_mapPlugin.size();

	return ret;
}

IUICommon* CPluginManager::FindPlugin(int id)
{
	x3::LockRW lockcls(m_mapPlugin.locker);
	IUICommon* ret = nullptr;
	if( lockcls.canRead() )
	{
		hash_map<int, x3::AnyObject>::iterator it = m_mapPlugin.find(id);
		if( it!=m_mapPlugin.end() )
		{
			x3::Object<IUICommon> spCommon(it->second);
			ret = spCommon.p();
		}
	}

	return ret;
}

bool CPluginManager::GetPlugin(int index, int& id, IUICommon** ppICommon)
{
	if( ppICommon==NULL )
		return false;

	x3::LockRW lockcls(m_mapPlugin.locker);
	bool ret = false;
	if( lockcls.canRead() )
	{
		if( index>=0 && index<m_mapPlugin.size() )
		{
			hash_map<int, x3::AnyObject>::iterator it = m_mapPlugin.begin();
			for( int i=0; i<index; i++ )
				it++;

			if( it!=m_mapPlugin.end() )
			{
				id = it->first;
				if( *ppICommon!=NULL )
				{
					(*ppICommon)->releaseObject();
					*ppICommon = NULL;
				}

				x3::Object<IUICommon> spCommon(it->second);
				*ppICommon = spCommon.p();
				if( *ppICommon!=NULL )
					ret = true;
			}
		}
	}

	return ret;
}

void CPluginManager::SetLastTool(int id) 
{
	m_nLastTool = id; 
}

IUITool* CPluginManager::LastTool()
{
	x3::Object<IUITool> spTool(FindPlugin(m_nLastTool));
	return spTool.p();
}