/*!
 * \file Scene.h
 * \date 2015/04/02 23:10
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
#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;

class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CScene(void);

public:
	const CComponent* GetComponent(WORD LayerID
		, const TCHAR* pObjectKey
		, const TCHAR* pComponentKey);

	const list<CGameObject*> GetGameObjectlist(WORD LayerID
		, const TCHAR* pObjectKey);

	list<CGameObject*> GetNotConstGameObjectlist(WORD LayerID
		, const TCHAR* pObjectKey);
	
public:
	virtual HRESULT InitScene(void) PURE;
	virtual void Update(void);
	virtual void Render(void);

private:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	typedef map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;
};

END

#endif // Scene_h__