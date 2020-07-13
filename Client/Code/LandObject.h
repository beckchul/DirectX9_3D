/*!
 * \file LandObject.h
 * \date 2017/06/05 16:03
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

#ifndef LandObject_h__
#define LandObject_h__

#include "GameObject.h"

namespace Engine
{
	class CTransform;
}

class CLandObject
	: public Engine::CGameObject
{
public:
	explicit CLandObject(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLandObject(void);

public:
	virtual void Update(void){};
	virtual void Render(void){};

protected:
	Engine::CTransform*		m_pInfo;
};

#endif // LandObject_h__