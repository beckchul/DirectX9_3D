/*!
 * \file NaviCell.h
 * \date 2017/06/07 15:53
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef NaviCell_h__
#define NaviCell_h__

#include "Engine_Include.h"

BEGIN(Engine)

enum POINTID {POINT_A, POINT_B, POINT_C, POINT_END};
enum LINE {LINE_AB, LINE_BC, LINE_CA, LINE_END};
enum NEIGHBORID {NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};

class CLine2D;

class ENGINE_DLL CNaviCell
{
private:
	CNaviCell(const D3DXVECTOR3* pPointA
		, const D3DXVECTOR3* pPointB
		, const D3DXVECTOR3* pPointC);

public:
	~CNaviCell(void);

public:
	const D3DXVECTOR3* GetPoint(POINTID ePointID) {return &m_vPoint[ePointID];}
	CNaviCell* GetNeighbor(NEIGHBORID eNeighborID) {return m_pNeighbor[eNeighborID];}
	DWORD GetIndex(void) {return m_dwIndex;}

public:
	void SetNeighbor(NEIGHBORID eNeighborID, CNaviCell* pNeighbor)
	{m_pNeighbor[eNeighborID] = pNeighbor;}
	D3DXVECTOR3 GetPoint(int iIndex);
public:
	HRESULT InitCell(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwIdx);
	void Render(LPD3DXLINE pLine);

public:
	bool CheckPass(const D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir, NEIGHBORID* pNeighborID);

public:
	bool ComparePoint(const D3DXVECTOR3* pFristPoint, const D3DXVECTOR3* pSecondPoint
		, CNaviCell* pNeighbor);

private:
	void Release(void);

public:
	static CNaviCell* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pPointA
		, const D3DXVECTOR3* pPointB
		, const D3DXVECTOR3* pPointC
		, const DWORD& dwIdx);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CLine2D*				m_pLine2D[LINE_END];
	CNaviCell*				m_pNeighbor[NEIGHBOR_END];

private:
	D3DXVECTOR3				m_vPoint[POINT_END];
	DWORD					m_dwIndex;
};

END

#endif // NaviCell_h__