#include "Collision_AABB.h"

#include "CubeColor.h"
#include "Mesh.h"

Engine::CCollision_AABB::CCollision_AABB(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pmatWorld(NULL)
	, m_pBoundingBox(NULL)
	, m_vMin(0.f, 0.f, 0.f)
	, m_vMax(0.f, 0.f, 0.f)
{
	D3DXMatrixIdentity(&m_matColliderMatrix);
}

Engine::CCollision_AABB::CCollision_AABB(const CCollision_AABB& rhs)
	: CCollision(rhs)
	, m_pDevice(rhs.m_pDevice)
	, m_pmatWorld(rhs.m_pmatWorld)
	, m_pBoundingBox(rhs.m_pBoundingBox)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)
	, m_matColliderMatrix(rhs.m_matColliderMatrix)
{

}

Engine::CCollision_AABB::~CCollision_AABB(void)
{

}

Engine::CCollision* Engine::CCollision_AABB::CloneColObject(void)
{
	return new CCollision_AABB(*this);
}

void Engine::CCollision_AABB::SetColInfo(const D3DXMATRIX* pWorld
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

void Engine::CCollision_AABB::Update(void)
{
	D3DXVECTOR3		vScale, vPos;
	vScale = D3DXVECTOR3(D3DXVec3Length((D3DXVECTOR3*)&m_pmatWorld->m[0][0])
		, D3DXVec3Length((D3DXVECTOR3*)&m_pmatWorld->m[1][0])
		, D3DXVec3Length((D3DXVECTOR3*)&m_pmatWorld->m[2][0]));
	memcpy(&vPos, &m_pmatWorld->m[3][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matScale, matTrans;
	m_matColliderMatrix = *D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z)
		* *D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	D3DXVec3TransformCoord(&m_vMin, &m_vMin, &m_matColliderMatrix);
	D3DXVec3TransformCoord(&m_vMax, &m_vMax, &m_matColliderMatrix);
}

void Engine::CCollision_AABB::Render(const DWORD& dwColor, CMesh* pMesh /*= NULL*/)
{
#ifdef _DEBUG
	if (pMesh != NULL)
		pMesh->GetComputeBoundingBox(&m_vMin, &m_vMax);

	m_pBoundingBox->SetBoundingBox(&m_vMin, &m_vMax, dwColor);
	m_pBoundingBox->Render(&m_matColliderMatrix);
#endif
}

DWORD Engine::CCollision_AABB::Release(void)
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

bool Engine::CCollision_AABB::CheckCollision(const CCollision_AABB* pTarget)
{
	D3DXVECTOR3		vDestMin, vDestMax;

	D3DXMATRIX		matTargetMatrix = *pTarget->GetMatrix();
	D3DXVECTOR3		vTargetMin = *pTarget->GetMin();
	D3DXVECTOR3		vTargetMax = *pTarget->GetMax();

	D3DXMATRIX		matAABB;
	D3DXMatrixIdentity(&matAABB);
	memcpy(&matAABB.m[3][0], &matTargetMatrix.m[3][0], sizeof(D3DXVECTOR3));
	D3DXVec3TransformCoord(&vDestMin, &vTargetMin, &matAABB);
	D3DXVec3TransformCoord(&vDestMax, &vTargetMax, &matAABB);

	float		fMin = 0.f;
	float		fMax = 0.f;

	fMin = max(m_vMin.x, vDestMin.x);
	fMax = min(m_vMax.x, vDestMax.x);
	if (fMin > fMax)
		return false;

	fMin = max(m_vMin.y, vDestMin.y);
	fMax = min(m_vMax.y, vDestMax.y);
	if (fMin > fMax)
		return false;

	fMin = max(m_vMin.z, vDestMin.z);
	fMax = min(m_vMax.z, vDestMax.z);
	if (fMin > fMax)
		return false;

	return true;
}

HRESULT Engine::CCollision_AABB::InitCollision(void)
{
	m_pBoundingBox = CCubeColor::Create(m_pDevice, m_vMin, m_vMax, D3DCOLOR_ARGB(255, 0, 0, 0));
	NULL_CHECK_RETURN(m_pBoundingBox, E_FAIL);
	return S_OK;
}

Engine::CCollision_AABB* Engine::CCollision_AABB::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollision_AABB*	pCollision = new CCollision_AABB(pDevice);
	if (FAILED(pCollision->InitCollision()))
	{
		pCollision->Release();
		Safe_Delete(pCollision);
	}
	return pCollision;
}
