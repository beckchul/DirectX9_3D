/*!
* \file DynamicMesh.h
* \date 2017/06/08 15:27
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

#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class CHierachyLoader;
class CAniCtrl;

class ENGINE_DLL CDynamicMesh
	: public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CDynamicMesh(const CDynamicMesh& rhs);

public:
	virtual ~CDynamicMesh(void);

public:
	virtual CResources* CloneResource(void);
	DOUBLE GetPeriod(void);
	DOUBLE GetTrackPos(void);
	virtual void GetComputeBoundingBox(D3DXVECTOR3* const pMin
		, D3DXVECTOR3* const pMax);

public:
	const D3DXMATRIX* FindFrame(const char* pFrameName);
	D3DXMATRIX* FindFrame2(const char* pFrameName);
	DERIVED_FRAME* GetFrame(const char* pFrameName);
public:
	void SetAnimationSet(Engine::ANI_INFO tAniInfo);
	void SetTrackPos(DOUBLE dTrackPos);
	void FrameMove(const float& fTimePerSec);

public:
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void RenderMesh_ForSDK(LPD3DXEFFECT pEffect,
		const D3DXMATRIX* pWorldMatrix);
	virtual DWORD Release(void);

private:
	void UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix);
	void SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame);
	void Find_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_FRAME* pFrame);
	void Render_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_MESHCONTAINER* pMeshContainer);
	bool CheckCount(WCHAR* pFindTag, WCHAR* pFindTagName);
private:
	void ComputeMeshMinMax(D3DXFRAME* pFrame
		, D3DXVECTOR3* const pMin
		, D3DXVECTOR3* const pMax);

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice
		, const TCHAR* pPath, const TCHAR* pFileName);

public:
	CHierachyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;
	LPD3DXFRAME				m_pRootBone;
};

END

#endif // DynamicMesh_h__