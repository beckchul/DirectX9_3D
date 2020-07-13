#include "stdafx.h"
#include "ResonanceBoomEffect.h"
#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "RcTex.h"
#include "Stage.h"
#include "Stone.h"
#include "ArishaHitBox.h"
#include "SoundMgr.h"

CResonanceBoomEffect::CResonanceBoomEffect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_vEffectPos(0.f, 0.f, 0.f)
{
	m_bRender[0] = false;
	m_bRender[1] = false;
	m_fCount[0] = 0.f;
	m_fCount[1] = 0.f;
	m_fTimeCount[0] = 0.f;
	m_fTimeCount[1] = 0.f;
}

CResonanceBoomEffect::~CResonanceBoomEffect(void)
{
	Release();
}

HRESULT CResonanceBoomEffect::Initialize(void)
{
	AddComponent();

	m_pInfo->m_vScale = D3DXVECTOR3(10.f, 10.f, 10.f);
	return S_OK;
}

void CResonanceBoomEffect::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);

	Engine::CGameObject::Update();
	State();
}

void CResonanceBoomEffect::Render(void)
{
	RenderFirst();
	RenderSecond();
}

void CResonanceBoomEffect::RenderFirst(void)
{
	if (!m_bRender[0])
		return;

	int iPosX = ((int)m_fTimeCount[0] / 3) % 5;
	int iPosY = ((int)m_fTimeCount[0] / 3) / 5;

	((Engine::CRcTex*)m_pBuffer)->SetEffectUV(5.f, 10.f, static_cast<float>(iPosX), static_cast<float>(iPosY));

	SetConstantTable(m_pShader->GetEffectHandle());

	m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture[0]->GetTexture(0));

	m_pShader->BeginShader(1);
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();

	((Engine::CRcTex*)m_pBuffer)->ReSetUV();
}

void CResonanceBoomEffect::RenderSecond(void)
{
	if (!m_bRender[1])
		return;

	SetConstantTableSecond(m_pShader->GetEffectHandle());

	m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture[1]->GetTexture(((DWORD)m_fTimeCount[1]) / 2));

	m_pShader->BeginShader(1);
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();
}

void CResonanceBoomEffect::Release(void)
{
}

HRESULT CResonanceBoomEffect::AddComponent(void)
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
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"ResonanceBoomBegin");
	m_pTexture[0] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"ResonanceBoomEnd");
	m_pTexture[1] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CResonanceBoomEffect::SetConstantTable(LPD3DXEFFECT pEffect)
{
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMATRIX		matBill, matWorld;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	D3DXMatrixIdentity(&matWorld);
	D3DXVECTOR3 vPos = m_vEffectPos + D3DXVECTOR3(0.f, 4.f, 0.f);
	memcpy(&m_pInfo->m_matWorld.m[3][0], &vPos, sizeof(D3DXVECTOR3));

	matWorld = matBill * m_pInfo->m_matWorld;

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);
	return S_OK;
}


HRESULT CResonanceBoomEffect::SetConstantTableSecond(LPD3DXEFFECT pEffect)
{
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMATRIX		matBill, matWorld;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	D3DXMatrixIdentity(&matWorld);
	D3DXVECTOR3 vPos = m_vEffectPos + D3DXVECTOR3(0.f, 4.f, 0.f);
	memcpy(&m_pInfo->m_matWorld.m[3][0], &vPos, sizeof(D3DXVECTOR3));

	matWorld = matBill * m_pInfo->m_matWorld;

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);
	return S_OK;
}

void CResonanceBoomEffect::State(void)
{
	if (m_bRender[0])
	{
		m_fCount[0] += Engine::Get_TimeMgr()->GetTime() * 100.f;

		if (m_fCount[0] >= 1.f)
		{
			m_fCount[0] = 0.f;
			++m_fTimeCount[0];

			if (m_fTimeCount[0] >= 150.f)
			{
				m_fTimeCount[0] = 0.f;
				m_bRender[0] = false;
				m_bRender[1] = true;
				m_pInfo->m_vScale = D3DXVECTOR3(30.f, 30.f, 30.f);

				const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ArishaHitBox");
				((CArishaHitBox*)pObjectlist.front())->SetCollision(true);

				((CArishaHitBox*)pObjectlist.front())->SetColPos(m_vEffectPos);
				((CArishaHitBox*)pObjectlist.front())->SetColScale(D3DXVECTOR3(5.f, 5.f, 5.f));

				CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_resonance_attack_end.wav", CHANNEL_PLAYER_EFFECT2, 0.5f);
			}
		}
	}

	if (m_bRender[1])
	{
		m_fCount[1] += Engine::Get_TimeMgr()->GetTime() * 100.f;

		if (m_fCount[1] >= 1.f)
		{
			m_fCount[1] = 0.f;
			++m_fTimeCount[1];

			if (m_fTimeCount[1] >= 130.f)
			{
				m_fTimeCount[1] = 0.f;
				m_bRender[1] = false;
				m_pInfo->m_vScale = D3DXVECTOR3(10.f, 10.f, 10.f);

				const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ArishaHitBox");
				((CArishaHitBox*)pObjectlist.front())->SetCollision(false);
			}
		}
	}
}

CResonanceBoomEffect * CResonanceBoomEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CResonanceBoomEffect*	pGameObject = new CResonanceBoomEffect(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CResonanceBoomEffect::SetEffectPos(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir)
{
	m_vEffectPos = _vPos;
	m_vHitBoxDir = _vDir;
}

void CResonanceBoomEffect::SetRender(bool _bRender)
{
	m_bRender[0] = _bRender;
}