/*!
* \file Arisha.h
* \date 2017/06/09 15:30
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

#ifndef Arisha_h__
#define Arisha_h__

#include "LandObject.h"
#include "Include.h"
#include "UIObserver.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CDynamicMesh;
};

class CArisha
	: public CLandObject
{
private:
	CArisha(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CArisha(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void KeyCheck(void);
	void SetConstantTable(void);
	void StateMove(void);
	void StateChange(void);
	void DirMove(float _fSpeedWeight, float _fTime);
	void HitCheck(void);
	void MouseMove(void);
	void Release(void);
public:
	static CArisha* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	const D3DXMATRIX* GetWeaponRef(void);
	const D3DXMATRIX* GetArishaInfo(void);
	const D3DXMATRIX* GetCollisionRef(int iIndex);
	const PLAYERSTATE* GetPlayerState(void);
	const D3DXVECTOR3* GetPlayerPos(void);
public:
	void SetCheckCollision(const bool* bCheckCollision, int iIndex)
	{	m_pCheckCollsion[iIndex] = bCheckCollision;	}
private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CDynamicMesh*	m_pMesh;
	
	CUIObserver*		m_pObserver;
	int					m_iHp;
	int					m_iHpMax;
	int					m_iDamage;
private:
	bool				m_bPush[7];
	vector<Engine::ANI_INFO> m_vecAniInfo;
	float				m_fSpeed;
	UINT				m_iAniCount;
	bool				m_bDesh;
	float				m_fDamageSpeed;
	float				m_fMoveSpeed;
	float				m_fDelay;
	float				m_fDelayCount;
	float				m_fHitCheck;

	float				m_fEffectDelay;
	float				m_fEffectCount[10];
	bool				m_bEffectRender[10];
private:
	DWORD				m_dwMeshIndex;
	const D3DXMATRIX*	m_pArishaInfo;
	const D3DXMATRIX*	m_pmatWeaponRef;
	const D3DXMATRIX*	m_pCollisionRef[8];
	const bool*			m_pCheckCollsion[8];
	PLAYERSTATE			m_ePlayerState;
	MAIN_PLAYERSTATE	m_eMainPlayerState;

	const D3DXMATRIX*	m_pHandLMatrix;
};

#endif // Arisha_h__