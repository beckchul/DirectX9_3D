#ifndef Cromcruach_h__
#define Cromcruach__

#include "LandObject.h"
#include "Include.h"
#include "UIObserver.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CDynamicMesh;
};

class CCromcruach
	: public CLandObject
{
private:
	CCromcruach(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCromcruach(void);

public:
	const D3DXMATRIX* GetCromcruachInfo(void);
	const D3DXMATRIX* GetWeaponRef(void);
	const D3DXMATRIX* GetCollisionRef(int iIndex);
	const MONSTERSTATE*	GetMonsterState(void);
public:
	void SetCheckCollision(const bool* bCheckCollision, int iIndex)
	{ m_pCheckCollision[iIndex] = bCheckCollision; }
	void SetPlayerPos(const D3DXVECTOR3* pPlayerPos) { m_pPlayerPos = pPlayerPos; }

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void SetConstantTable(void);
	void Patton(void);
	void StateMove(void);
	void StateChange(void);
	void DirMove(int _iIndex, float _fSpeedWeight, float _fTime);
	void JiggleMove(void);
	void HitCheck(void);
	void Release(void);
public:
	static CCromcruach* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CDynamicMesh*	m_pMesh;

	CUIObserver*		m_pObserver;
	int		m_iHp;
	int		m_iHpMax;
	int		m_iDamage;
private:
	vector<Engine::ANI_INFO> m_vecAniInfo;
	float				m_fSpeed;
	float				m_fMoveSpeed[3];
	float				m_fDelay;
	float				m_fCullTime;
	float				m_fDelayCount[3];
	bool				m_bDelayCheck[3];
	bool				m_bEffectRender[2];
	DWORD				m_dwMeshIndex;
	bool				m_bLeft;
	float				m_fLeftCount;
	
private:
	const D3DXMATRIX*	m_pCromcruachInfo;
	const D3DXMATRIX*	m_pmatWeaponRef;

	const D3DXMATRIX*	m_pCollisionRef[13];
	const bool*			m_pCheckCollision[13];
	float				m_fHitColor;

	const D3DXVECTOR3*	m_pPlayerPos;
	MONSTERSTATE		m_eMonsterState;
	MONSTERSTATE		m_ePreMonsterState;
	MAIN_MONSTERSTATE	m_eMainMonsterState;

	D3DXMATRIX*			m_JiggleOrignal[12];
	D3DXVECTOR3			m_vAIDir;
	bool				m_bDeadCheck;
	int					m_iDashCount;
};


#endif // Cromcruach_h__