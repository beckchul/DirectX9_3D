#ifndef HierachyLoader_h__
#define HierachyLoader_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CHierachyLoader
	: public ID3DXAllocateHierarchy
{
public:
	CHierachyLoader(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath);
	~CHierachyLoader(void);

public:
	virtual STDMETHODIMP CreateFrame(THIS_ LPCSTR Name
		, LPD3DXFRAME *ppNewFrame);
	virtual STDMETHODIMP CreateMeshContainer(THIS_ LPCSTR Name
		, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials
		, CONST D3DXEFFECTINSTANCE *pEffectInstances
		, DWORD NumMaterials, CONST DWORD *pAdjacency
		, LPD3DXSKININFO pSkinInfo
		, LPD3DXMESHCONTAINER *ppNewMeshContainer);
	virtual STDMETHODIMP DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree);
	virtual STDMETHODIMP DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

private:
	void AllocateName(const CHAR* const pName, CHAR** ppNewName);

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	const TCHAR*		m_pPath;
};

END

#endif // HierachyLoader_h__