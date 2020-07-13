/*!
* \file StaticMesh.h
* \date 2017/06/02 16:55
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

#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CStaticMesh(const CStaticMesh& rhs);

public:
	virtual ~CStaticMesh(void);

public:
	virtual CResources* CloneResource(void);
	virtual void GetComputeBoundingBox(D3DXVECTOR3* const pMin
		, D3DXVECTOR3* const pMax);

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void RenderMesh_ForSDK(LPD3DXEFFECT pEffect, const D3DXMATRIX* pWorldMatrix);
	virtual DWORD Release(void);

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice
		, const TCHAR* pPath, const TCHAR* pFileName);

public:
	LPD3DXMESH GetMesh(void)
	{
		return m_pMesh;
	}
private:
	bool CheckAlpha(int iSubsetIdx, const char* pFindTag);

private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	D3DMATERIAL9*			m_pMtrl;

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pSubsetBuffer;
	LPD3DXMATERIAL			m_pSubset;
	DWORD					m_dwSubsetCnt;
};

END

#endif // StaticMesh_h__