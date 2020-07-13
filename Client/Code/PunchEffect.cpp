#include "stdafx.h"
#include "PunchEffect.h"
#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "RcTex.h"

CPunchEffect::CPunchEffect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_bRender(false)
, m_fCount(0.f)
, m_fTimeCount(0.f)
, m_fRenderCount(0.f)
{
}

CPunchEffect::~CPunchEffect(void)
{
	Release();
}

HRESULT CPunchEffect::Initialize(void)
{
	AddComponent();

	m_pInfo->m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	return S_OK;
}

void CPunchEffect::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);

	Engine::CGameObject::Update();
	State();
}

void CPunchEffect::Render(void)
{
	if (!m_bRender)
		return;

	int iPosX = static_cast<int>(m_fTimeCount) % 4;
	int iPosY = static_cast<int>(m_fTimeCount) / 4;

	((Engine::CRcTex*)m_pBuffer)->SetEffectUV(4.f, 4.f, static_cast<float>(iPosX), static_cast<float>(iPosY));

	SetConstantTable(m_pShader->GetEffectHandle());

	m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture->GetTexture(0));

	m_pShader->BeginShader(1);
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();

	((Engine::CRcTex*)m_pBuffer)->ReSetUV();
}



void CPunchEffect::Release(void)
{
}

HRESULT CPunchEffect::AddComponent(void)
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
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"PunchEffect");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"ShaderUV");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CPunchEffect::SetConstantTable(LPD3DXEFFECT pEffect)
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
	D3DXVECTOR3 vPos = m_vEffectPos;
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

void CPunchEffect::State(void)
{
	if (!m_bRender)
		return;

	m_fCount += Engine::Get_TimeMgr()->GetTime() * 32.f;

	if (m_fCount >= 1.f)
	{
		m_fCount = 0.f;
		++m_fTimeCount;

		if (m_fTimeCount >= 16.f)
		{
			m_fTimeCount = 0.f;
			++m_fRenderCount;

			if(m_fRenderCount >= 7)
				m_bRender = false;
		}
	}
}

CPunchEffect * CPunchEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPunchEffect*	pGameObject = new CPunchEffect(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CPunchEffect::SetRender(bool _bRender)
{
	m_bRender = _bRender;
	m_fRenderCount = 0.f;
}

void CPunchEffect::SetEffectPos(D3DXVECTOR3 _vPos)
{
	m_vEffectPos = _vPos;
}