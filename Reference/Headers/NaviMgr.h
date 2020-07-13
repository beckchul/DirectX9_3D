/*!
 * \file NaviMgr.h
 * \date 2017/06/07 16:21
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

#ifndef NaviMgr_h__
#define NaviMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CNaviCell;

class ENGINE_DLL CNaviMgr
{
public:
	DECLARE_SINGLETON(CNaviMgr)

private:
	CNaviMgr(void);
	~CNaviMgr(void);

public:
	void ReserveCellSize(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwSize);
	HRESULT AddCell(const D3DXVECTOR3* pPointA
		, const D3DXVECTOR3* pPointB
		, const D3DXVECTOR3* pPointC);
	void LinkCell(void);
	DWORD MoveOnNaviMesh(D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir
		, const DWORD& dwCurrentIdx);
	void Render_NaviMesh(void);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	DWORD					m_dwReserveSize;
	DWORD					m_dwIdxCnt;

private:
	LPD3DXLINE				m_pLine;

private:
	typedef vector<CNaviCell*>		VECCELL;
	VECCELL			m_vecNaviMesh;
};

END

#endif // NaviMgr_h__
