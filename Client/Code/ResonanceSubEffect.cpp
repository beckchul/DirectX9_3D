#include "stdafx.h"
#include "ResonanceSubEffect.h"
#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "RcTex.h"
#include "SoundMgr.h"

CResonanceSubEffect::CResonanceSubEffect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_bRender(false)
, m_fCount(0.f)
, m_fTimeCount(0.f)
, m_vEffectPos(0.f, 0.f, 0.f)
{
}

CResonanceSubEffect::~CResonanceSubEffect(void)
{
	Release();
}

HRESULT CResonanceSubEffect::Initialize(void)
{
	AddComponent();

	m_pInfo->m_vScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(90.f);
	//m_pInfo->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian(90.f);
	return S_OK;
}

void CResonanceSubEffect::Update(void)
{
	m_pInfo->m_vDir = m_vEffectDir;
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);
	if (m_fTimeCount >= 210.f)
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian((m_fTimeCount - 210.f) * 5.f);
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian((m_fTimeCount - 120.f) * 5.f);
	}
	Engine::CGameObject::Update();
	State();
}

void CResonanceSubEffect::Render(void)
{
	if (!m_bRender)
		return;

	SetConstantTable(m_pShader->GetEffectHandle());

	m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture->GetTexture(2));

	m_pShader->BeginShader(1);
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();
}

void CResonanceSubEffect::Release(void)
{
}

HRESULT CResonanceSubEffect::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"Resonance");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CResonanceSubEffect::SetConstantTable(LPD3DXEFFECT pEffect)
{
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));




	//D3DXMATRIX		matBill, matWorld;
	//D3DXMatrixIdentity(&matBill);

	//m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	//ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	//D3DXMatrixInverse(&matBill, NULL, &matBill);

	//D3DXMatrixIdentity(&matWorld);
	D3DXVECTOR3 vPos = m_vEffectPos + D3DXVECTOR3(0.f, 2.f, 0.f);
	memcpy(&m_pInfo->m_matWorld.m[3][0], &vPos, sizeof(D3DXVECTOR3));

	//matWorld = matBill * m_pInfo->m_matWorld;

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);
	return S_OK;
}

void CResonanceSubEffect::State(void)
{
	if (!m_bRender)
		return;

	m_fCount += Engine::Get_TimeMgr()->GetTime() * 100.f;

	if (m_fCount >= 1.f)
	{
		m_fCount = 0.f;
		++m_fTimeCount;

		if (m_fTimeCount <= 100.f)
			m_pInfo->m_vScale = D3DXVECTOR3(m_fTimeCount * 0.01f, m_fTimeCount * 0.01f, m_fTimeCount * 0.01f);
		else if (m_fTimeCount < 240.f)
			m_pInfo->m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
		else
			m_pInfo->m_vScale = D3DXVECTOR3(1.f - (m_fTimeCount - 210.f) * 0.1f
				, 1.f - (m_fTimeCount - 210.f) * 0.1f, 1.f - (m_fTimeCount - 210.f) * 0.1f);
		if (m_fTimeCount >= 250.f)
		{
			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_resonance_hand_end.wav", CHANNEL_PLAYER_EFFECT2, 0.5f);
			m_fTimeCount = 0.f;
			m_bRender = false;
			m_pInfo->m_vScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
			m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(90.f);
			m_pInfo->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian(0.f);
		}
	}
}

CResonanceSubEffect* CResonanceSubEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CResonanceSubEffect*	pGameObject = new CResonanceSubEffect(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CResonanceSubEffect::SetEffectPos(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir)
{
	m_vEffectPos = _vPos;
	m_vEffectDir = _vDir;
}

void CResonanceSubEffect::SetRender(bool _bRender)
{
	m_bRender = _bRender;
}
