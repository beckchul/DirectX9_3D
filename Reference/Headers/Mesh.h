#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class CVIBuffer;

class ENGINE_DLL CMesh
	: public CResources
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh(const CMesh& rhs);

public:
	virtual ~CMesh(void);

public:
	virtual void GetComputeBoundingBox(D3DXVECTOR3* const pMin, D3DXVECTOR3* const pMax) PURE;

public:
	virtual CResources* CloneResource(void)PURE;

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)PURE;
	virtual void RenderMesh_ForSDK(LPD3DXEFFECT pEffect,
		const D3DXMATRIX* pWorldMatrix)PURE;
};

END

#endif // Mesh_h__