#include "Layer.h"
#include "GameObject.h"

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CLayer::~CLayer(void)
{
	Release();
}

HRESULT Engine::CLayer::AddObject(const TCHAR* pObjectKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		MAPOBJLIST::iterator	iter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), CTagFinder(pObjectKey));
		if(iter == m_mapObjlist.end())
		{
			m_mapObjlist[pObjectKey] = OBJECTLIST();
		}
		m_mapObjlist[pObjectKey].push_back(pGameObject);
	}

	return S_OK;
}

void Engine::CLayer::Update(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
		{
			(*iterList)->Update();
		}		
	}	
}

void Engine::CLayer::Render(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
		{
			(*iterList)->Render();
		}		
	}	
}

Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);
}

void Engine::CLayer::Release(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
			Engine::Safe_Delete((*iterList));

		iter->second.clear();
	}
	m_mapObjlist.clear();
}

const Engine::CComponent* Engine::CLayer::GetComponent(const TCHAR* pObjectKey, const TCHAR* pComponentKey)
{
	MAPOBJLIST::iterator	mapiter = find_if(m_mapObjlist.begin(), m_mapObjlist.end()
		, CTagFinder(pObjectKey));
	if(mapiter == m_mapObjlist.end())
		return NULL;

	OBJECTLIST::iterator	iterlist = mapiter->second.begin();
	OBJECTLIST::iterator	iterlist_end = mapiter->second.end();
	for( ; iterlist != iterlist_end; ++iterlist)
	{
		const CComponent* pComponent = (*iterlist)->GetComponent(pComponentKey);
		if(pComponent != NULL)
			return pComponent;
	}
	return NULL;
}

const list<Engine::CGameObject*> Engine::CLayer::GetGameObjectlist(const TCHAR* pObjectKey)
{
	MAPOBJLIST::iterator	mapiter = find_if(m_mapObjlist.begin(), m_mapObjlist.end()
		, CTagFinder(pObjectKey));
	if (mapiter == m_mapObjlist.end())
		return list<Engine::CGameObject*>();

	return mapiter->second;
}

std::list<Engine::CGameObject*> Engine::CLayer::GetNotConstGameObjectlist(const TCHAR* pObjectKey)
{
	MAPOBJLIST::iterator	mapiter = find_if(m_mapObjlist.begin(), m_mapObjlist.end()
		, CTagFinder(pObjectKey));
	if (mapiter == m_mapObjlist.end())
		return list<Engine::CGameObject*>();

	return mapiter->second;
}
