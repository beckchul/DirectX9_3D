/*!
 * \file Collision.h
 * \date 2017/06/14 15:19
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
#ifndef Collision_h__
#define Collision_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision
	: public CComponent
{
protected:
	CCollision(void);
	CCollision(const CCollision& rhs);

public:
	virtual ~CCollision(void);

public:
	virtual CCollision* CloneColObject(void) PURE;

public:
	virtual DWORD Release(void) PURE;
};

END

#endif // Collision_h__