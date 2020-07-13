#include "CollisionMgr.h"
#include "Collision_OBB.h"
#include "Collision.h"
#include "Collision_AABB.h"

using namespace Engine;

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr(void)
	: m_pDevice(NULL)
{

}

CCollisionMgr::~CCollisionMgr(void)
{
	Release();
}

CCollision* Engine::CCollisionMgr::CloneColObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator	iter = m_mapCollision.find(eCollisionID);
	if (iter == m_mapCollision.end())
		return NULL;

	return iter->second->CloneColObject();
}

HRESULT Engine::CCollisionMgr::AddColObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator	iter = m_mapCollision.find(eCollisionID);
	if (iter != m_mapCollision.end())
		return E_FAIL;

	CCollision*		pCollision = NULL;

	switch (eCollisionID)
	{
	case COLLISION_AABB:
		pCollision = CCollision_AABB::Create(m_pDevice);
		break;

	case COLLISION_OBB:
		pCollision = CCollision_OBB::Create(m_pDevice);
		break;
	}
	NULL_CHECK_RETURN(pCollision, E_FAIL);

	m_mapCollision.insert(MAPCOLLISION::value_type(eCollisionID, pCollision));
	return S_OK;
}

HRESULT Engine::CCollisionMgr::InitCollison(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	return S_OK;
}

void Engine::CCollisionMgr::Release(void)
{

	MAPCOLLISION::iterator	iter = m_mapCollision.begin();
	MAPCOLLISION::iterator	iter_end = m_mapCollision.end();

		DWORD		dwRefCnt = 0;
		for (; iter != iter_end; ++iter)
		{
			dwRefCnt = iter->second->Release();
			if (dwRefCnt == 0)
				Safe_Delete(iter->second);
		}
		m_mapCollision.clear();
}
