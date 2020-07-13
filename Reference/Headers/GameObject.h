/*!
 * \file GameObject.h
 * \date 2015/04/03 18:11
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
#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CComponent;
class CShader;

class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGameObject(void);

public:
	const CComponent* GetComponent(const TCHAR* pComponentKey);
	const float& GetViewZ(void) {return m_fViewZ;}

public:
	virtual void Update(void);
	virtual void Render(void){}

private:
	void Release(void);

protected:
	void Compute_ViewZ(const D3DXVECTOR3* pPos);

protected:
	CShader*		m_pShader;

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	float		m_fViewZ;

protected:
	typedef map<const TCHAR*, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;
};

END

#endif // GameObject_h__