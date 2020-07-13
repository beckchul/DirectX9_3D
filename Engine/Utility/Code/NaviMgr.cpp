#include "NaviMgr.h"
#include "NaviCell.h"

IMPLEMENT_SINGLETON(Engine::CNaviMgr)

Engine::CNaviMgr::CNaviMgr(void)
: m_pDevice(NULL)
, m_dwReserveSize(0)
, m_dwIdxCnt(0)
, m_pLine(NULL)
{

}

Engine::CNaviMgr::~CNaviMgr(void)
{
	Release();
}

void Engine::CNaviMgr::ReserveCellSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize)
{
	m_pDevice = pDevice;
	m_dwReserveSize = dwSize;
	m_vecNaviMesh.reserve(dwSize);

	HRESULT hr = D3DXCreateLine(m_pDevice, &m_pLine);
	FAILED_CHECK_RETURN(hr, ;);
}

HRESULT Engine::CNaviMgr::AddCell(const D3DXVECTOR3* pPointA 
								  , const D3DXVECTOR3* pPointB 
								  , const D3DXVECTOR3* pPointC)
{
	CNaviCell*		pCell = CNaviCell::Create(m_pDevice
		, pPointA, pPointB, pPointC, m_dwIdxCnt);
	NULL_CHECK_RETURN(pCell, E_FAIL);

	++m_dwIdxCnt;

	m_vecNaviMesh.push_back(pCell);
	return S_OK;
}

void Engine::CNaviMgr::LinkCell(void)
{
	VECCELL::iterator	iter = m_vecNaviMesh.begin();
	if(iter == m_vecNaviMesh.end())
		return;

	for( ; iter != m_vecNaviMesh.end(); ++iter)
	{
		VECCELL::iterator	iter_Target = m_vecNaviMesh.begin();
		while(iter_Target != m_vecNaviMesh.end())
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if((*iter_Target)->ComparePoint((*iter)->GetPoint(POINT_A)
				, (*iter)->GetPoint(POINT_B)
				, (*iter)))
			{
				(*iter)->SetNeighbor(NEIGHBOR_AB, (*iter_Target));
			}
			else if((*iter_Target)->ComparePoint((*iter)->GetPoint(POINT_B)
				, (*iter)->GetPoint(POINT_C)
				, (*iter)))
			{
				(*iter)->SetNeighbor(NEIGHBOR_BC, (*iter_Target));
			}
			else if((*iter_Target)->ComparePoint((*iter)->GetPoint(POINT_C)
				, (*iter)->GetPoint(POINT_A)
				, (*iter)))
			{
				(*iter)->SetNeighbor(NEIGHBOR_CA, (*iter_Target));
			}

			++iter_Target;
		}
	}
}

void Engine::CNaviMgr::Render_NaviMesh(void)
{
	UINT		iSize = m_vecNaviMesh.size();
	for(UINT i = 0; i < iSize; ++i)
	{
		m_vecNaviMesh[i]->Render(m_pLine);
	}
}

void Engine::CNaviMgr::Release(void)
{
	for_each(m_vecNaviMesh.begin(), m_vecNaviMesh.end(), CDeleteObj());
	m_vecNaviMesh.clear();
}

DWORD Engine::CNaviMgr::MoveOnNaviMesh(D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir , const DWORD& dwCurrentIdx)
{
	NEIGHBORID		eNeighborID;
	DWORD			dwNextIndex = dwCurrentIdx;

	if(m_vecNaviMesh[dwCurrentIdx]->CheckPass(pPos, pDir, &eNeighborID))
	{
		CNaviCell*	pNeighbor = m_vecNaviMesh[dwCurrentIdx]->GetNeighbor(eNeighborID);
		if(pNeighbor == NULL)//존재하지 않는 이웃
		{
			//슬라이딩 벡터를 알아서 잘 만들어
		}
		else
		{
			dwNextIndex = pNeighbor->GetIndex();
			*pPos += *pDir;
		}
	}
	else
	{
		*pPos += *pDir;
	}

	D3DXPLANE		Plane;

	D3DXPlaneFromPoints(&Plane, &((CNaviCell*)m_vecNaviMesh[dwNextIndex])->GetPoint(0)
		, &((CNaviCell*)m_vecNaviMesh[dwNextIndex])->GetPoint(1)
		, &((CNaviCell*)m_vecNaviMesh[dwNextIndex])->GetPoint(2));

	(*pPos).y = ((-Plane.a * (*pPos).x - Plane.c * (*pPos).z - Plane.d) / Plane.b);


	return dwNextIndex;
}
