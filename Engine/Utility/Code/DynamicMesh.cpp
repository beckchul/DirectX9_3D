#include "DynamicMesh.h"
#include "HierachyLoader.h"
#include "AniCtrl.h"
#include "CubeColor.h"

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pDevice)
	: CMesh(pDevice)
	, m_pLoader(NULL)
	, m_pRootBone(NULL)
	, m_pAniCtrl(NULL)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CMesh(rhs)
	, m_pLoader(rhs.m_pLoader)
	, m_pRootBone(rhs.m_pRootBone)
{
	m_pAniCtrl = new CAniCtrl(*(rhs.m_pAniCtrl));
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

Engine::CResources* Engine::CDynamicMesh::CloneResource(void)
{
	return new CDynamicMesh(*this);
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	TCHAR		szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = new CHierachyLoader(m_pDevice, pPath);
	m_pAniCtrl = new CAniCtrl;

	HRESULT hr = D3DXLoadMeshHierarchyFromX(szFullPath
		, D3DXMESH_MANAGED
		, m_pDevice
		, m_pLoader
		, NULL
		, &m_pRootBone
		, m_pAniCtrl->GetAniCtrl());
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, szFullPath);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	UpdateFrameMatrix((DERIVED_FRAME*)m_pRootBone, &matIdentity);
	SetUpFrameMatrixPointer((DERIVED_FRAME*)m_pRootBone);

	return S_OK;
}

void Engine::CDynamicMesh::RenderMesh_ForSDK(LPD3DXEFFECT pEffect,
	const D3DXMATRIX* pWorldMatrix)
{
	pEffect->Begin(NULL, 0);
	Find_MeshContainer(pEffect, (DERIVED_FRAME*)m_pRootBone);
	pEffect->End();
}

DWORD Engine::CDynamicMesh::Release(void)
{
	if (m_dwRefCnt == 0)
	{
		m_pLoader->DestroyFrame(m_pRootBone);
		Safe_Delete(m_pLoader);
		Safe_Delete(m_pAniCtrl);
		return 0;
	}
	else
	{
		Safe_Delete(m_pAniCtrl);
		--m_dwRefCnt;
	}

	return m_dwRefCnt;
}

void Engine::CDynamicMesh::UpdateFrameMatrix(DERIVED_FRAME* pFrame
	, const D3DXMATRIX* pParentMatrix)
{
	if (pFrame != NULL)
		pFrame->CombinedMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (pFrame->pFrameFirstChild != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameFirstChild, &pFrame->CombinedMatrix);

	if (pFrame->pFrameSibling != NULL)
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameSibling, pParentMatrix);
}

void Engine::CDynamicMesh::SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame)
{
	if (pFrame == NULL)
		return;

	if (pFrame->pMeshContainer != NULL)
	{
		if (pFrame->pMeshContainer->pSkinInfo == NULL)
			return;

		DWORD		dwNumFrame = pFrame->pMeshContainer->pSkinInfo->GetNumBones();
		((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameCombinedMatrix = new D3DXMATRIX*[dwNumFrame];
		ZeroMemory(((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameCombinedMatrix, sizeof(D3DXMATRIX*) * dwNumFrame);

		for (DWORD i = 0; i < dwNumFrame; ++i)
		{
			((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameCombinedMatrix[i] =
				&((DERIVED_FRAME*)D3DXFrameFind(m_pRootBone
					, pFrame->pMeshContainer->pSkinInfo->GetBoneName(i)))->CombinedMatrix;
		}
	}

	if (pFrame->pFrameFirstChild != NULL)
		SetUpFrameMatrixPointer((DERIVED_FRAME*)pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling != NULL)
		SetUpFrameMatrixPointer((DERIVED_FRAME*)pFrame->pFrameSibling);
}

void Engine::CDynamicMesh::Find_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_FRAME* pFrame)
{
	if (pFrame == NULL)
		return;

	if (pFrame->pMeshContainer != NULL)
		Render_MeshContainer(pEffect, (DERIVED_MESHCONTAINER*)pFrame->pMeshContainer);

	if (pFrame->pFrameFirstChild != NULL)
		Find_MeshContainer(pEffect, (DERIVED_FRAME*)pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling != NULL)
		Find_MeshContainer(pEffect, (DERIVED_FRAME*)pFrame->pFrameSibling);
}

void Engine::CDynamicMesh::Render_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_MESHCONTAINER* pMeshContainer)
{
	if (pMeshContainer->pSkinInfo != NULL)
	{
		DWORD		dwNumBone = pMeshContainer->pSkinInfo->GetNumBones();
		if (pMeshContainer->pResultMatrix == NULL)
			pMeshContainer->pResultMatrix = new D3DXMATRIX[dwNumBone];

		for (DWORD i = 0; i < dwNumBone; ++i)
		{
			pMeshContainer->pResultMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		BYTE*		pSour;
		BYTE*		pDest;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, (void**)&pSour);//변형전
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (void**)&pDest);//변형후

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pResultMatrix
			, NULL, pSour, pDest);

		pMeshContainer->pOriMesh->UnlockVertexBuffer();//변형전
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();//변형후

		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			if(pMeshContainer->ppTexture[i] == NULL)
				continue;

			bool bCheckMetal = CheckCount(pMeshContainer->vecTagName[i], L"M");
			bool bCheckSkin = CheckCount(pMeshContainer->vecTagName[i], L"S");
			bool bCheckAlph = CheckCount(pMeshContainer->vecTagName[i], L"A");
			bool bCheckArmor = CheckCount(pMeshContainer->vecTagName[i], L"W");
			bool bCheckX = CheckCount(pMeshContainer->vecTagName[i], L"X");
			bool bCheckHair = CheckCount(pMeshContainer->vecTagName[i], L"H");
			bool bCheckMonster = CheckCount(pMeshContainer->vecTagName[i], L"Y");

			int iPassIndex = 2;

			if (bCheckMetal)
				iPassIndex = 2;
			else if (bCheckSkin)
				iPassIndex = 4;
			else if (bCheckAlph)
				iPassIndex = 1;
			else if (bCheckArmor || bCheckX)
				iPassIndex = 0;
			else if (bCheckHair)
				iPassIndex = 3;
			else if (bCheckMonster)
				iPassIndex = 5;

			pEffect->BeginPass(iPassIndex);


			/*pEffect->SetVector("g_vMtrlDiffuse", (D3DXVECTOR4*)(&pMeshContainer->pMaterials[i].MatD3D.Diffuse));
			pEffect->SetVector("g_vMtrlAmbient", &D3DXVECTOR4(0.9f, 0.9f, 0.9f, 0.9f));
			pEffect->SetVector("g_vMtrlSpecular", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
			pEffect->SetFloat("g_fPower", pMeshContainer->pMaterials[i].MatD3D.Power);

			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);*/
			pEffect->SetTexture("g_DiffuseMap", pMeshContainer->ppTexture[i]);
			pEffect->CommitChanges();
			pMeshContainer->MeshData.pMesh->DrawSubset(i);

			pEffect->EndPass();
		}
	}
}

bool Engine::CDynamicMesh::CheckCount(WCHAR* pFindTag, WCHAR* pFindTagName)
{

	if (*pFindTagName == pFindTag[0])
		return true;

	return false;
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName)
{
	CDynamicMesh*	pMesh = new CDynamicMesh(pDevice);
	if (FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
		Safe_Delete(pMesh);

	return pMesh;
}

void Engine::CDynamicMesh::SetAnimationSet(Engine::ANI_INFO tAniInfo)
{
	m_pAniCtrl->SetAnimationSet(tAniInfo);
}

void Engine::CDynamicMesh::SetTrackPos(DOUBLE dTrackPos)
{
	m_pAniCtrl->SetTrackPos(dTrackPos);
}

void Engine::CDynamicMesh::FrameMove(const float& fTimePerSec)
{
	m_pAniCtrl->FrameMove(fTimePerSec);

	D3DXMATRIX		matIdeneiey;
	D3DXMatrixIdentity(&matIdeneiey);

	UpdateFrameMatrix((DERIVED_FRAME*)m_pRootBone, &matIdeneiey);
}

DOUBLE Engine::CDynamicMesh::GetPeriod(void)
{
	return m_pAniCtrl->GetPeriod();
}

DOUBLE Engine::CDynamicMesh::GetTrackPos(void)
{
	return m_pAniCtrl->GetTrackPos();
}

void Engine::CDynamicMesh::ComputeMeshMinMax(D3DXFRAME* pFrame
	, D3DXVECTOR3* const pMin
	, D3DXVECTOR3* const pMax)
{
	if (pFrame->pMeshContainer != NULL)
	{
		LPD3DXMESH		pMesh = pFrame->pMeshContainer->MeshData.pMesh;
		if (pMesh != NULL)
		{
			DWORD	dwFVF = pMesh->GetFVF();

			if (dwFVF & D3DFVF_XYZ)
			{
				D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];
				ZeroMemory(VtxFVF, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);
				pMesh->GetDeclaration(VtxFVF);

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
				pMesh->LockVertexBuffer(0, &pVtxInfo);

				D3DXVECTOR3		vTempMin = D3DXVECTOR3(0.f, 0.f, 0.f);
				D3DXVECTOR3		vTempMax = D3DXVECTOR3(0.f, 0.f, 0.f);
				D3DXComputeBoundingBox((D3DXVECTOR3*)((BYTE*)pVtxInfo + dwOffset)
					, pMesh->GetNumVertices()
					, D3DXGetFVFVertexSize(pMesh->GetFVF())
					, &vTempMin, &vTempMax);

				pMesh->UnlockVertexBuffer();

				pMax->x = max(vTempMax.x, pMax->x);
				pMax->y = max(vTempMax.y, pMax->y);
				pMax->z = max(vTempMax.z, pMax->z);

				pMin->x = min(vTempMin.x, pMin->x);
				pMin->y = min(vTempMin.y, pMin->y);
				pMin->z = min(vTempMin.z, pMin->z);
			}
		}
	}

	if (pFrame->pFrameFirstChild != NULL)
		ComputeMeshMinMax(pFrame->pFrameFirstChild, pMin, pMax);

	if (pFrame->pFrameSibling != NULL)
		ComputeMeshMinMax(pFrame->pFrameSibling, pMin, pMax);
}

const D3DXMATRIX* Engine::CDynamicMesh::FindFrame(const char* pFrameName)
{
	DERIVED_FRAME*		pFrame = (DERIVED_FRAME*)D3DXFrameFind(m_pRootBone, pFrameName);

	return &pFrame->CombinedMatrix;
}

void Engine::CDynamicMesh::GetComputeBoundingBox(D3DXVECTOR3* const pMin, D3DXVECTOR3* const pMax)
{
	*pMin = D3DXVECTOR3(0.f, 0.f, 0.f);
	*pMax = D3DXVECTOR3(0.f, 0.f, 0.f);
	ComputeMeshMinMax(m_pRootBone, pMin, pMax);
}

D3DXMATRIX* Engine::CDynamicMesh::FindFrame2(const char* pFrameName)
{
	DERIVED_FRAME*		pFrame = (DERIVED_FRAME*)D3DXFrameFind(m_pRootBone, pFrameName);

	return &pFrame->CombinedMatrix;
}

Engine::DERIVED_FRAME* Engine::CDynamicMesh::GetFrame(const char* pFrameName)
{
	DERIVED_FRAME*		pFrame = (DERIVED_FRAME*)D3DXFrameFind(m_pRootBone, pFrameName);

	return pFrame;
}

