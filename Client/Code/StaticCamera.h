#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"

namespace Engine
{
	class CTransform;
}

class CStaticCamera
	: public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStaticCamera(void);

public:
	virtual void Update(void);

public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pTargetInfo
		, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
	void KeyCheck(void);
	void TargetRenewal(void);
	void CharacterRender(void);
	void ResonanceRender(void);
	void RordOfManaOneRender(void);
	void RordOfManaTwoRender(void);
	void RordOfManaThreeRender(void);
	void RordOfManaFourRender(void);
public:
	void SetCameraTarget(const Engine::CTransform* pTargetInfo);

private:
	const Engine::CTransform*		m_pTargetInfo;
private:
	float		m_fTargetDistance;
	float		m_fCamSpeed;
	bool		m_bRenderCharacter;
};

#endif // StaticCamera_h__