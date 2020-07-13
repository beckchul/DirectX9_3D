#include "Scene.h"
#include "Layer.h"

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CScene::~CScene(void)
{
	Release();
}

void Engine::CScene::Update(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}
}

void Engine::CScene::Render(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}

void Engine::CScene::Release(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}

const Engine::CComponent* Engine::CScene::GetComponent(WORD LayerID , const TCHAR* pObjectKey , const TCHAR* pComponentKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);
	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetComponent(pObjectKey, pComponentKey);
}

const list<Engine::CGameObject*> Engine::CScene::GetGameObjectlist(WORD LayerID, const TCHAR * pObjectKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);
	if (iter == m_mapLayer.end())
		return list<Engine::CGameObject*>();

	return iter->second->GetGameObjectlist(pObjectKey);
}

list<Engine::CGameObject*> Engine::CScene::GetNotConstGameObjectlist(WORD LayerID, const TCHAR* pObjectKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);
	if (iter == m_mapLayer.end())
		return list<Engine::CGameObject*>();

	return iter->second->GetNotConstGameObjectlist(pObjectKey);
}

