#include "stdafx.h"
#include "Terrain.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "..\..\Engine\Utility\Code\Shader.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTerrainVtx(NULL)
, m_pIndex(NULL)
, m_dwTriCnt(0)
, m_fDetail(0.f)
{
	ZeroMemory(&m_pMtrl, sizeof(D3DMATERIAL9));
	ZeroMemory(m_pTexture, 8);
}

CTerrain::~CTerrain(void)
{
	Release();
}

HRESULT CTerrain::Initialize(void)
{
	HRESULT		hr = AddComponent();
	FAILED_CHECK(hr);

	ZeroMemory(&m_pMtrl, sizeof(D3DMATERIAL9));
	m_pMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pMtrl.Power = 5.f;

	m_pTerrainVtx = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];
	m_pBuffer->GetVtxInfo(m_pTerrainVtx);

	m_fDetail = 20.f;

	hr = D3DXCreateTexture(m_pDevice
		, VTXCNTX
		, VTXCNTZ
		, 1
		, 0
		, D3DFMT_A8R8G8B8
		, D3DPOOL_MANAGED
		, &m_pCreateTexture);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	D3DLOCKED_RECT		LockRect;
	m_pCreateTexture->LockRect(0, &LockRect, NULL, 0);

	for (UINT i = 0; i < VTXCNTZ; ++i)
	{
		for (UINT j = 0; j < VTXCNTX; ++j)
		{
			UINT		iIndex = i * VTXCNTX + j;
			if (j > 64)
				((DWORD*)LockRect.pBits)[iIndex] = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
			else
				((DWORD*)LockRect.pBits)[iIndex] = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		}
	}

	m_pCreateTexture->UnlockRect(0);

	hr = D3DXSaveTextureToFile(L"../bin/SaveTexture.bmp", D3DXIFF_BMP, m_pCreateTexture
		, NULL);
	FAILED_CHECK(hr);

	return S_OK;
}

void CTerrain::Update(void)
{
	Engine::CGameObject::Update();
}

void CTerrain::Render(void)
{
	LPD3DXEFFECT	pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK(pEffect);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	pEffect->SetVector("g_vMtrlDiffuse", (D3DXVECTOR4*)(&m_pMtrl.Diffuse));
	pEffect->SetVector("g_vMtrlAmbient", (D3DXVECTOR4*)(&m_pMtrl.Ambient));
	pEffect->SetVector("g_vMtrlSpecular", (D3DXVECTOR4*)(&m_pMtrl.Specular));
	pEffect->SetFloat("g_fPower", m_pMtrl.Power);

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	pEffect->SetTexture("g_SrcTexture", m_pTexture[0]->GetTexture(0));
	pEffect->SetTexture("g_DestTexture", m_pTexture[0]->GetTexture(1));
	pEffect->SetTexture("g_MultiTexture", m_pCreateTexture);
	pEffect->SetTexture("g_BrushTexture", m_pTexture[1]->GetTexture(0));

	pEffect->SetFloat("g_fDetail", m_fDetail);

	pEffect->SetVector("g_vBrushPos", &D3DXVECTOR4(30.f, 0.f, 10.f, 1.f));
	pEffect->SetFloat("g_fBrushSize", 10.f);

	m_pShader->BeginShader();
	m_pBuffer->Render(NULL);
	m_pShader->EndShader();
}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_TerrainTex");
	m_pTexture[0] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Multi", pComponent));

	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Texture_Brush");
	m_pTexture[1] = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Brush", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_Terrain");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain*		pGameObject = new CTerrain(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CTerrain::Release(void)
{
	Engine::Safe_Delete_Array(m_pTerrainVtx);
	//Engine::Safe_Delete_Array(m_pIndex);
}



