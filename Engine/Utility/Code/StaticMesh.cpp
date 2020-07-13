#include "StaticMesh.h"
#include "CubeColor.h"

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
	: CMesh(pDevice)
	, m_ppTexture(NULL)
	, m_pMtrl(NULL)
	, m_pMesh(NULL)
	, m_pSubsetBuffer(NULL)
	, m_dwSubsetCnt(0)
	, m_pSubset(NULL)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_ppTexture(rhs.m_ppTexture)
	, m_pMtrl(rhs.m_pMtrl)
	, m_pMesh(rhs.m_pMesh)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pSubset(rhs.m_pSubset)
{

}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

Engine::CResources* Engine::CStaticMesh::CloneResource(void)
{
	return new CStaticMesh(*this);
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT		hr = NULL;

	TCHAR		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	hr = D3DXLoadMeshFromX(szFullPath
		, D3DXMESH_MANAGED	// 메쉬를 생성할 메모리 설정(열거형으로)
		, m_pDevice			// Direct3D의 device 설정
		, NULL				// 인접 정점의 정보를 리턴
		, &m_pSubsetBuffer	// material의 정보를 리턴
		, NULL				// effect의 초기값 리턴
		, &m_dwSubsetCnt	// material의 개수 리턴
		, &m_pMesh);
	FAILED_CHECK(hr);

	m_pSubset = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();

	m_pMtrl = new D3DMATERIAL9[m_dwSubsetCnt];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		m_pMtrl[i] = m_pSubset[i].MatD3D;

		lstrcpy(szFullPath, pPath);
		MultiByteToWideChar(CP_ACP
			, 0, m_pSubset[i].pTextureFilename
			, strlen(m_pSubset[i].pTextureFilename)
			, szFileName, 128);
		lstrcat(szFullPath, szFileName);

		hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i]);
		FAILED_CHECK(hr);
	}
	return S_OK;
}

void Engine::CStaticMesh::RenderMesh_ForSDK(LPD3DXEFFECT pEffect
	, const D3DXMATRIX* pWorldMatrix)
{
	pEffect->Begin(NULL, 0);
	for (DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		bool		isAlpha = CheckAlpha(i, "A");

		int		iPassIndex = 0;
		if (isAlpha == true)
			iPassIndex = 1;
		else
			iPassIndex = 0;

		pEffect->BeginPass(iPassIndex);

		//m_pDevice->SetTexture(0, m_ppTexture[i]);
		//m_pDevice->SetMaterial(&m_pMtrl[i]);

		pEffect->SetVector("g_vMtrlDiffuse", (D3DXVECTOR4*)(&m_pMtrl[i].Diffuse));
		pEffect->SetVector("g_vMtrlAmbient", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
		pEffect->SetVector("g_vMtrlSpecular", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
		pEffect->SetFloat("g_fPower", m_pMtrl[i].Power);

		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);

		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}
	pEffect->End();
}

DWORD Engine::CStaticMesh::Release(void)
{
	if (m_dwRefCnt == 0)
	{
		Safe_Delete_Array(m_pMtrl);
		for (size_t i = 0; i < m_dwSubsetCnt; ++i)
		{
			Safe_Release(m_ppTexture[i]);
		}
		Safe_Delete_Array(m_ppTexture);
		Safe_Release(m_pSubsetBuffer);
		Safe_Release(m_pMesh);
		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName)
{
	CStaticMesh*	pMesh = new CStaticMesh(pDevice);
	if (FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
		Safe_Delete(pMesh);

	return pMesh;
}

bool Engine::CStaticMesh::CheckAlpha(int iSubsetIdx, const char* pFindTag)
{
	int		iLength = strlen(m_pSubset[iSubsetIdx].pTextureFilename);

	for (int i = 0; i < iLength; ++i)
	{
		if ('.' == m_pSubset[iSubsetIdx].pTextureFilename[i])
		{
			if (*pFindTag == m_pSubset[iSubsetIdx].pTextureFilename[i - 1])
				return true;
		}
	}
	return false;
}

void Engine::CStaticMesh::GetComputeBoundingBox(D3DXVECTOR3* const pMin, D3DXVECTOR3* const pMax)
{
	D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];
	ZeroMemory(VtxFVF, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);
	m_pMesh->GetDeclaration(VtxFVF);

	DWORD	dwOffset = -1;
	for (int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (VtxFVF[i].Usage == D3DDECLUSAGE_POSITION)
		{
			dwOffset = VtxFVF[i].Offset;
			break;
		}
	}

	if (dwOffset < 0)
		return;

	void*	pVtxInfo = NULL;
	m_pMesh->LockVertexBuffer(0, &pVtxInfo);

	D3DXComputeBoundingBox((D3DXVECTOR3*)((BYTE*)pVtxInfo + dwOffset)
		, m_pMesh->GetNumVertices()
		, D3DXGetFVFVertexSize(m_pMesh->GetFVF())
		, pMin, pMax);

	m_pMesh->UnlockVertexBuffer();
}
