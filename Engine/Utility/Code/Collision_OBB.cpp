#include "Collision_OBB.h"

#include "CubeColor.h"
#include "Mesh.h"

Engine::CCollision_OBB::CCollision_OBB(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pmatWorld(NULL)
	, m_pBoundingBox(NULL)
	, m_vMin(0.f, 0.f, 0.f)
	, m_vMax(0.f, 0.f, 0.f)
	, m_bCollision(true)
{
	ZeroMemory(m_vPoint, sizeof(D3DXVECTOR3) * 8);
	ZeroMemory(&m_tOBB, sizeof(OBB));
}

Engine::CCollision_OBB::CCollision_OBB(const CCollision_OBB& rhs)
	: CCollision(rhs)
	, m_pDevice(rhs.m_pDevice)
	, m_pmatWorld(rhs.m_pmatWorld)
	, m_pBoundingBox(rhs.m_pBoundingBox)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
	, m_tOBB(rhs.m_tOBB)
{
	memcpy(m_vPoint, rhs.m_vPoint, sizeof(D3DXVECTOR3) * 8);
}

Engine::CCollision_OBB::~CCollision_OBB(void)
{

}

Engine::CCollision* Engine::CCollision_OBB::CloneColObject(void)
{
	return new CCollision_OBB(*this);
}

void Engine::CCollision_OBB::SetColInfo(const D3DXMATRIX* pWorld
	, CMesh* pMesh
	, const D3DXVECTOR3* pMin /*= NULL */
	, const D3DXVECTOR3* pMax /*= NULL*/)
{
	m_pmatWorld = pWorld;

	if (pMesh != NULL)
		pMesh->GetComputeBoundingBox(&m_vMin, &m_vMax);
	else
	{
		m_vMin = *pMin;
		m_vMax = *pMax;
	}
}

void Engine::CCollision_OBB::Update(void)
{
	ZeroMemory(&m_tOBB, sizeof(OBB));
	ComputePoint();

	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], m_pmatWorld);

	D3DXVec3TransformCoord(&m_tOBB.vCenter, &m_tOBB.vCenter, m_pmatWorld);

	ComputeAxis();
}

void Engine::CCollision_OBB::Render(const DWORD& dwColor, CMesh* pMesh /*= NULL*/)
{
#ifdef _DEBUG
	if (pMesh != NULL)
		pMesh->GetComputeBoundingBox(&m_vMin, &m_vMax);

	m_pBoundingBox->SetBoundingBox(&m_vMin, &m_vMax, dwColor);
	m_pBoundingBox->Render(m_pmatWorld);
#endif
}

DWORD Engine::CCollision_OBB::Release(void)
{
	if (m_dwRefCnt == 0)
	{
		DWORD	dwCnt = m_pBoundingBox->Release();
		if (dwCnt != 0)
			MSG_BOX(L"지우는데 실패했어");

		Safe_Delete(m_pBoundingBox);
		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

bool Engine::CCollision_OBB::CheckCollision(const CCollision_OBB* pTarget)
{
	const OBB*	pOBB[2] = { &m_tOBB, pTarget->GetObbInfo() };
	if (!pTarget->GetCollisionState())
		return false;

	float		fDistance[3];
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < AXIS_END; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&pOBB[0]->vProj[AXIS_X], &pOBB[i]->vParallel[j]))
				+ fabs(D3DXVec3Dot(&pOBB[0]->vProj[AXIS_Y], &pOBB[i]->vParallel[j]))
				+ fabs(D3DXVec3Dot(&pOBB[0]->vProj[AXIS_Z], &pOBB[i]->vParallel[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&pOBB[1]->vProj[AXIS_X], &pOBB[i]->vParallel[j]))
				+ fabs(D3DXVec3Dot(&pOBB[1]->vProj[AXIS_Y], &pOBB[i]->vParallel[j]))
				+ fabs(D3DXVec3Dot(&pOBB[1]->vProj[AXIS_Z], &pOBB[i]->vParallel[j]));

			D3DXVECTOR3		vTemp = pOBB[1]->vCenter - pOBB[0]->vCenter;
			fDistance[2] = fabs(D3DXVec3Dot(&vTemp, &pOBB[i]->vParallel[j]));

			if (fDistance[2] > fDistance[0] + fDistance[1])
				return false;
		}
	}

	((CCollision_OBB*)pTarget)->CompleteCheck();
	return true;
}

HRESULT Engine::CCollision_OBB::InitCollision(void)
{
	m_pBoundingBox = CCubeColor::Create(m_pDevice, m_vMin, m_vMax, D3DCOLOR_ARGB(255, 0, 0, 0));
	NULL_CHECK_RETURN(m_pBoundingBox, E_FAIL);
	return S_OK;
}

void Engine::CCollision_OBB::ComputePoint(void)
{
	m_vPoint[0] = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	m_vPoint[1] = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	m_vPoint[2] = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	m_vPoint[3] = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);

	m_vPoint[4] = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	m_vPoint[5] = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	m_vPoint[6] = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	m_vPoint[7] = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);

	for (int i = 0; i < 8; ++i)
		m_tOBB.vCenter += m_vPoint[i];

	m_tOBB.vCenter *= 0.125f;
}

void Engine::CCollision_OBB::ComputeAxis(void)
{
	m_tOBB.vProj[AXIS_X] = (m_vPoint[1] + m_vPoint[2] + m_vPoint[5] + m_vPoint[6]) * 0.25f
		- m_tOBB.vCenter;
	m_tOBB.vParallel[AXIS_X] = m_vPoint[1] - m_vPoint[0];

	m_tOBB.vProj[AXIS_Y] = (m_vPoint[0] + m_vPoint[1] + m_vPoint[4] + m_vPoint[5]) * 0.25f
		- m_tOBB.vCenter;
	m_tOBB.vParallel[AXIS_Y] = m_vPoint[0] - m_vPoint[3];

	m_tOBB.vProj[AXIS_Z] = (m_vPoint[4] + m_vPoint[5] + m_vPoint[6] + m_vPoint[7]) * 0.25f
		- m_tOBB.vCenter;
	m_tOBB.vParallel[AXIS_Z] = m_vPoint[7] - m_vPoint[3];

	for (int i = 0; i < AXIS_END; ++i)
		D3DXVec3Normalize(&m_tOBB.vParallel[i], &m_tOBB.vParallel[i]);
}

Engine::CCollision_OBB* Engine::CCollision_OBB::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollision_OBB*	pCollision = new CCollision_OBB(pDevice);
	if (FAILED(pCollision->InitCollision()))
	{
		pCollision->Release();
		Safe_Delete(pCollision);
	}
	return pCollision;
}
