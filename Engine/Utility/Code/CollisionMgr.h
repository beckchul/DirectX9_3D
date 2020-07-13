/*!
* \file CollisionMgr.h
* \date 2017/06/21 15:18
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CCollision;

class ENGINE_DLL CCollisionMgr
{
public:
	enum COLLISIONID { COLLISION_OBB, COLLISION_AABB };

public:
	DECLARE_SINGLETON(CCollisionMgr)

private:
	CCollisionMgr(void);
	~CCollisionMgr(void);

public:
	CCollision* CloneColObject(COLLISIONID eCollisionID);

public:
	HRESULT AddColObject(COLLISIONID eCollisionID);

public:
	HRESULT InitCollison(LPDIRECT3DDEVICE9 pDevice);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	typedef map<COLLISIONID, CCollision*>		MAPCOLLISION;
	MAPCOLLISION		m_mapCollision;
};

END

#endif // CollisionMgr_h__