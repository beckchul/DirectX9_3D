#include "TerrainTex.h"

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
: CVIBuffer(rhs)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

Engine::CResources* Engine::CTerrainTex::CloneResource(void)
{
	return new CTerrainTex(*this);
}

HRESULT Engine::CTerrainTex::CreateBuffer(const WORD& wCntX
										  , const WORD& wCntZ
										  , const WORD& wItv)
{
	HANDLE	hFile;
	DWORD	dwByte;

	hFile = CreateFile(L"../bin/Resources/Texture/Terrain/Height.bmp", GENERIC_READ
		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	DWORD*		pdwPixel = new DWORD[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pdwPixel, sizeof(DWORD) * ih.biWidth * ih.biHeight, &dwByte, NULL);
	CloseHandle(hFile);

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = wCntX * wCntZ;
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	m_dwTriCnt = (wCntX - 1) * (wCntZ - 1) * 2;

	HRESULT		hr = CVIBuffer::CreateBuffer();
	FAILED_CHECK(hr);

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	int		iIndex = 0;
	for(int z = 0; z < wCntZ; ++z)
	{
		for(int x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;

			float	fy = (pdwPixel[iIndex] & 0x000000ff) / 10.f;
			pVtxTex[iIndex].vPos = D3DXVECTOR3(float(x) * wItv, fy, float(z) * wItv);
			pVtxTex[iIndex].vNormal = D3DXVECTOR3(0.f, 0.f, 0.f);
			pVtxTex[iIndex].vTexUV = D3DXVECTOR2(x / (wCntX - 1.f), z / (wCntZ - 1.f));
		}
	}

	INDEX32*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int		iTriCnt = 0;
	for(int z = 0; z < wCntZ - 1; ++z)
	{
		for(int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;
			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + wCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;

			D3DXVECTOR3		vDest, vSour, vNormal;
			vDest = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPos - pVtxTex[pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			vDest = pVtxTex[pIndex[iTriCnt]._2].vPos - pVtxTex[pIndex[iTriCnt]._1].vPos;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPos - pVtxTex[pIndex[iTriCnt]._2].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;
			++iTriCnt;
		}
	}

	for(DWORD i = 0; i < m_dwVtxCnt; ++i)
	{
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	Safe_Delete_Array(pdwPixel);
	return S_OK;
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pDevice
												 , const WORD& wCntX, const WORD& wCntZ, const WORD& wItv)
{
	CTerrainTex*	pBuffer = new CTerrainTex(pDevice);
	if(FAILED(pBuffer->CreateBuffer(wCntX, wCntZ, wItv)))
	{
		Safe_Delete(pBuffer);
	}
	return pBuffer;
}
