#include "RcTex.h"

Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CRcTex::~CRcTex(void)
{

}

Engine::CResources* Engine::CRcTex::CloneResource(void)
{
	/*CResources* pResource = new CRcTex(*this);

	pResource->AddRef();

	return pResource;*/

	return new CRcTex(*this);
}

HRESULT Engine::CRcTex::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 2;
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXTEX*			pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVtxTex[1].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVtxTex[2].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVtxTex[3].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRcTex*		pRcTex = new CRcTex(pDevice);

	if(FAILED(pRcTex->CreateBuffer()))	
		Engine::Safe_Delete(pRcTex);

	return pRcTex;
}

void Engine::CRcTex::SetEffectBuffer(const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax)
{
	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(pMin->x, pMin->y, pMin->z);
	pVertex[1].vPos = D3DXVECTOR3(pMin->x, pMax->y, pMin->z);
	pVertex[2].vPos = D3DXVECTOR3(pMax->x, pMax->y, pMax->z);
	pVertex[3].vPos = D3DXVECTOR3(pMax->x, pMin->y, pMax->z);

	m_pVB->Unlock();
}

void Engine::CRcTex::SetEffectUV(float fSizeX, float fSizeY, float fPosX, float fPosY)
{
	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vTexUV = D3DXVECTOR2(fPosX / fSizeX, fPosY / fSizeY);
	pVertex[1].vTexUV = D3DXVECTOR2((fPosX + 1.f) / fSizeX, fPosY / fSizeY);
	pVertex[2].vTexUV = D3DXVECTOR2((fPosX + 1.f) / fSizeX, (fPosY + 1.f) / fSizeY);
	pVertex[3].vTexUV = D3DXVECTOR2(fPosX / fSizeX, (fPosY + 1.f) / fSizeY);
	
	m_pVB->Unlock();
}

void Engine::CRcTex::ReSetUV(void)
{
	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);
	pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();
}

void Engine::CRcTex::ReSetBuffer(void)
{
	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVertex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVertex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);

	m_pVB->Unlock();
}


