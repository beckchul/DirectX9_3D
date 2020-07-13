#include "stdafx.h"
#include "DamageFont.h"
#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "RcTex.h"

CDamageFont::CDamageFont(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_fX(0.f)
, m_fY(0.f)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
, m_iRenderIndex(0)
{
	ZeroMemory(m_vFontPos, sizeof(D3DXVECTOR3) * 8);
	ZeroMemory(m_iNumber, sizeof(int) * 8);
	ZeroMemory(m_fCount, sizeof(float) * 8);

	for (int i = 0; i < 8; ++i)
		m_fRenderTime[i] = 2.f;
}

CDamageFont::~CDamageFont(void)
{
	Release();
}

HRESULT CDamageFont::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_wVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_wVtxCnt];
	m_pInfo->m_vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	m_pBuffer->GetVtxInfo(m_pVertex);
	NULL_CHECK_RETURN(m_pVertex, E_FAIL);
	return S_OK;
}

void CDamageFont::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);
	Engine::CGameObject::Update();

	for (int i = 0; i < 8; ++i)
	{
		m_fCount[i] += Engine::Get_TimeMgr()->GetTime();

		if (m_fCount[i] >= 1.f)
		{
			m_fCount[i] = 0.f;
			++m_fRenderTime[i];
		}

		m_vFontPos[i].y += Engine::Get_TimeMgr()->GetTime();
	}
}

void CDamageFont::Render(void)
{

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < 8; ++i)
	{
		if (m_fRenderTime[i] >= 1.f)
			return;

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		D3DXMATRIX		matBill, matWorld;
		D3DXMATRIX		matView, matProj;

		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		D3DXMatrixIdentity(&matBill);
		m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
		ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
		D3DXMatrixInverse(&matBill, NULL, &matBill);

		D3DXMatrixIdentity(&matWorld);
		D3DXVECTOR3 vPos = m_vFontPos[i];
		memcpy(&m_pInfo->m_matWorld.m[3][0], &vPos, sizeof(D3DXVECTOR3));

		matWorld = matBill * m_pInfo->m_matWorld;

		//TenHun
		int iThousand = (m_iNumber[i] / 1000);

		if (iThousand != 0)
		{
			m_pVertex[0].vTexUV = D3DXVECTOR2(float(iThousand) * 0.1f, 0.f);
			m_pVertex[1].vTexUV = D3DXVECTOR2(float(iThousand) * 0.1f + 0.1f, 0.f);
			m_pVertex[2].vTexUV = D3DXVECTOR2(float(iThousand) * 0.1f + 0.1f, 1.f);
			m_pVertex[3].vTexUV = D3DXVECTOR2(float(iThousand) * 0.1f, 1.f);

			m_pVertex[0].vPos = D3DXVECTOR3(-1.5f, 0.5f, 0.f);
			m_pVertex[1].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
			m_pVertex[2].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
			m_pVertex[3].vPos = D3DXVECTOR3(-1.5f, -0.5f, 0.f);

			m_pBuffer->SetVtxInfo(m_pVertex);
			m_pTexture->Render(0, 0);
			m_pBuffer->Render(&matWorld);
		}


		//Hun
		int iHun = ((m_iNumber[i] % 1000) / 100);

		if (iHun != 0 || (iThousand != 0 && iHun == 0))
		{
			m_pVertex[0].vTexUV = D3DXVECTOR2(float(iHun) * 0.1f, 0.f);
			m_pVertex[1].vTexUV = D3DXVECTOR2(float(iHun) * 0.1f + 0.1f, 0.f);
			m_pVertex[2].vTexUV = D3DXVECTOR2(float(iHun) * 0.1f + 0.1f, 1.f);
			m_pVertex[3].vTexUV = D3DXVECTOR2(float(iHun) * 0.1f, 1.f);

			m_pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
			m_pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
			m_pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);
			m_pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);

			m_pBuffer->SetVtxInfo(m_pVertex);
			m_pTexture->Render(0, 0);
			m_pBuffer->Render(&matWorld);
		}
		
		//Ten
		//vPos = m_vFontPos[i] + D3DXVECTOR3(0.2f, 0.f, 0.2f);
		memcpy(&m_pInfo->m_matWorld.m[3][0], &vPos, sizeof(D3DXVECTOR3));

		matWorld = matBill * m_pInfo->m_matWorld;


		int iTen = ((m_iNumber[i] % 100) / 10);

		m_pVertex[0].vTexUV = D3DXVECTOR2(float(iTen) * 0.1f, 0.f);
		m_pVertex[1].vTexUV = D3DXVECTOR2(float(iTen) * 0.1f + 0.1f, 0.f);
		m_pVertex[2].vTexUV = D3DXVECTOR2(float(iTen) * 0.1f + 0.1f, 1.f);
		m_pVertex[3].vTexUV = D3DXVECTOR2(float(iTen) * 0.1f, 1.f);

		m_pVertex[0].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
		m_pVertex[1].vPos = D3DXVECTOR3(1.5f, 0.5f, 0.f);
		m_pVertex[2].vPos = D3DXVECTOR3(1.5f, -0.5f, 0.f);
		m_pVertex[3].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);

		m_pBuffer->SetVtxInfo(m_pVertex);
		m_pTexture->Render(0, 0);
		m_pBuffer->Render(&matWorld);


		//One
		//vPos = m_vFontPos[i] + D3DXVECTOR3(0.4f, 0.f, 0.4f);
		memcpy(&m_pInfo->m_matWorld.m[3][0], &vPos, sizeof(D3DXVECTOR3));

		matWorld = matBill * m_pInfo->m_matWorld;

		int iOne = (m_iNumber[i] / 10);

		m_pVertex[0].vTexUV = D3DXVECTOR2(float(iOne) * 0.1f, 0.f);
		m_pVertex[1].vTexUV = D3DXVECTOR2(float(iOne) * 0.1f + 0.1f, 0.f);
		m_pVertex[2].vTexUV = D3DXVECTOR2(float(iOne) * 0.1f + 0.1f, 1.f);
		m_pVertex[3].vTexUV = D3DXVECTOR2(float(iOne) * 0.1f, 1.f);

		m_pVertex[0].vPos = D3DXVECTOR3(1.5f, 0.5f, 0.f);
		m_pVertex[1].vPos = D3DXVECTOR3(2.5f, 0.5f, 0.f);
		m_pVertex[2].vPos = D3DXVECTOR3(2.5f, -0.5f, 0.f);
		m_pVertex[3].vPos = D3DXVECTOR3(1.5f, -0.5f, 0.f);

		m_pBuffer->SetVtxInfo(m_pVertex);
		m_pTexture->Render(0, 0);
		m_pBuffer->Render(&matWorld);


		((Engine::CRcTex*)m_pBuffer)->ReSetUV();
		((Engine::CRcTex*)m_pBuffer)->ReSetBuffer();
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CDamageFont::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DamageFont");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

void CDamageFont::Release(void)
{
}

CDamageFont* CDamageFont::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDamageFont*	pGameObject = new CDamageFont(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
