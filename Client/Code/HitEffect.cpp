#include "stdafx.h"
#include "HitEffect.h"
#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "Stage.h"
#include "Collision_OBB.h"
#include "RcTex.h"

CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_bRender(false)
, m_fScale(0.f)
{
}

CHitEffect::~CHitEffect(void)
{
	Release();
}

HRESULT CHitEffect::Initialize(void)
{
	AddComponent();

	m_pInfo->m_vScale = D3DXVECTOR3(2.f, 2.f, 2.f);
	return S_OK;
}

void CHitEffect::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);
	Engine::CGameObject::Update();
	State();
}

void CHitEffect::Render(void)
{
	if (!m_bRender)
		return;

	memcpy(&m_pInfo->m_matWorld.m[3][0], &m_matEffectMatrix.m[3][0], sizeof(D3DXVECTOR3));

	SetConstantTable(m_pShader->GetEffectHandle());


	m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture->GetTexture(0));

	m_pShader->BeginShader(1);
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();

	m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture->GetTexture(1));

	m_pShader->BeginShader(1);
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();
}

void CHitEffect::SetEffectMatrix(D3DXMATRIX _Matrix)
{
	m_matEffectMatrix = _Matrix;
	m_pInfo->m_fAngle[Engine::ANGLE_X] = D3DXToRadian((float)(rand() % 180 - 90));
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian((float)(rand() % 180 - 90));
	m_pInfo->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian((float)(rand() % 180 - 90));
}

HRESULT CHitEffect::AddComponent(void)
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
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"HitEffect");
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

HRESULT CHitEffect::SetConstantTable(LPD3DXEFFECT	pEffect)
{
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

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

void CHitEffect::State(void)
{	
	if (m_bRender)
	{
		m_fScale += Engine::Get_TimeMgr()->GetTime() * 4.f;

		if (m_fScale >= 1.f)
			m_bRender = false;
	}
	else
	{
		m_fScale = 0.f;
	}
}

void CHitEffect::Release(void)
{
}

CHitEffect* CHitEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHitEffect*	pGameObject = new CHitEffect(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
