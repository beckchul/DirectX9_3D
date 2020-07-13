#include "stdafx.h"
#include "LightningEffect.h"
#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "RcTex.h"
#include "Stage.h"
#include "Stone.h"
#include "CromcruachHitBox.h"

CLightningEffect::CLightningEffect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_bRender(false)
, m_fCount(0.f)
, m_fTimeCount(0.f)
, m_vEffectPos(0.f, 0.f, 0.f)
, m_iIndex(0)
{
}

CLightningEffect::~CLightningEffect(void)
{
	Release();
}

HRESULT CLightningEffect::Initialize(void)
{
	AddComponent();

	m_pInfo->m_vScale = D3DXVECTOR3(6.f, 6.f, 6.f);
	m_pInfo->m_fAngle[Engine::ANGLE_Z] = D3DXToRadian(180.f);
	return S_OK;
}

void CLightningEffect::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);

	Engine::CGameObject::Update();
	State();
}

void CLightningEffect::Render(void)
{
	if (!m_bRender)
		return;

	int iPosX = static_cast<int>(m_fTimeCount) % 4;
	int iPosY = static_cast<int>(m_fTimeCount) / 4;

	((Engine::CRcTex*)m_pBuffer)->SetEffectUV(4.f, 4.f, static_cast<float>(iPosX), static_cast<float>(iPosY));

	SetConstantTable(m_pShader->GetEffectHandle());

	m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture->GetTexture(m_iIndex));

	m_pShader->BeginShader(1);
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();

	((Engine::CRcTex*)m_pBuffer)->ReSetUV();
}

void CLightningEffect::Release(void)
{
}

HRESULT CLightningEffect::AddComponent(void)
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
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"Lightning");
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

HRESULT CLightningEffect::SetConstantTable(LPD3DXEFFECT pEffect)
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
	D3DXVECTOR3 vPos = m_vEffectPos + D3DXVECTOR3((float)(rand() % 14 - 7), 2.f, (float)(rand() % 14 - 7));
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

void CLightningEffect::State(void)
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
			m_bRender = false;

			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"Stone");
			((CStone*)pObjectlist.front())->SetEffexct(D3DXVECTOR3(300.f, 0.f, 300.f), 7.f);


			const list<CGameObject*> pObjectlist3 = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"CromcruachHitBox");
			((CCromcruachHitBox*)pObjectlist3.front())->SetCollision(false);
		}
	}
}

CLightningEffect * CLightningEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLightningEffect*	pGameObject = new CLightningEffect(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CLightningEffect::SetEffectPos(D3DXVECTOR3 _vPos)
{
	m_vEffectPos = _vPos;
}

void CLightningEffect::SetRender(bool _bRender)
{
	m_bRender = _bRender;
	m_iIndex = rand() % 2;
}