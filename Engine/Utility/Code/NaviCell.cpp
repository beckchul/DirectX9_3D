#include "NaviCell.h"
#include "Line2D.h"

Engine::CNaviCell::CNaviCell(const D3DXVECTOR3* pPointA
							 , const D3DXVECTOR3* pPointB
							 , const D3DXVECTOR3* pPointC)
{
	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	ZeroMemory(m_pNeighbor, 12);
}

Engine::CNaviCell::~CNaviCell(void)
{
	Release();
}

D3DXVECTOR3 Engine::CNaviCell::GetPoint(int iIndex)
{
	return m_vPoint[iIndex];
}

HRESULT Engine::CNaviCell::InitCell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx)
{
	m_pDevice = pDevice;
	m_dwIndex = dwIdx;

	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);
	return S_OK;
}

void Engine::CNaviCell::Render(LPD3DXLINE pLine)
{
	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3		vPoint[4];
	vPoint[0] = m_vPoint[0];
	vPoint[1] = m_vPoint[1];
	vPoint[2] = m_vPoint[2];
	vPoint[3] = m_vPoint[0];

	for(int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if(vPoint[i].z < 0.f)
			vPoint[i].z = 0.f;
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	pLine->SetWidth(2.f);
	pLine->Begin();
	pLine->DrawTransform(vPoint, POINT_END + 1
		, &matIdentity, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	pLine->End();
}

Engine::CNaviCell* Engine::CNaviCell::Create(LPDIRECT3DDEVICE9 pDevice , const D3DXVECTOR3* pPointA , const D3DXVECTOR3* pPointB , const D3DXVECTOR3* pPointC , const DWORD& dwIdx)
{
	CNaviCell*	pCell = new CNaviCell(pPointA, pPointB, pPointC);
	if(FAILED(pCell->InitCell(pDevice, dwIdx)))
		Safe_Delete(pCell);

	return pCell;
}

void Engine::CNaviCell::Release(void)
{
	for(int i = 0; i < LINE_END; ++i)
	{
		Safe_Delete(m_pLine2D[i]);
	}
}

bool Engine::CNaviCell::ComparePoint(const D3DXVECTOR3* pFristPoint, const D3DXVECTOR3* pSecondPoint , CNaviCell* pNeighbor)
{
	if(*pFristPoint == m_vPoint[POINT_A])
	{
		if(*pSecondPoint == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pSecondPoint == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
	}

	if(*pFristPoint == m_vPoint[POINT_B])
	{
		if(*pSecondPoint == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pSecondPoint == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}

	if(*pFristPoint == m_vPoint[POINT_C])
	{
		if(*pSecondPoint == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
		else if(*pSecondPoint == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}
	return false;
}

bool Engine::CNaviCell::CheckPass(const D3DXVECTOR3* pPos
								  , const D3DXVECTOR3* pDir
								  , NEIGHBORID* pNeighborID)
{
	for(int i = 0; i < 3; ++i)
	{
		if(m_pLine2D[i]->CheckLinePass(&D3DXVECTOR2(pPos->x + pDir->x
			, pPos->z + pDir->z)))
		{
			*pNeighborID = NEIGHBORID(i);
			return true;
		}
	}
	return false;
}

