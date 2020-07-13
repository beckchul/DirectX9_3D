#include "LightMgr.h"
#include "Light.h"

IMPLEMENT_SINGLETON(Engine::CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Release();
}

HRESULT Engine::CLightMgr::AddLight(LPDIRECT3DDEVICE9 pDevice , const D3DLIGHT9* pLightInfo , const DWORD& dwLightIdx)
{
	CLight*		pLight = CLight::Create(pDevice, pLightInfo, dwLightIdx);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_Lightlist.push_back(pLight);
	return S_OK;
}

void Engine::CLightMgr::Release(void)
{
	for_each(m_Lightlist.begin(), m_Lightlist.end(), CDeleteObj());
	m_Lightlist.clear();
}

const D3DLIGHT9* Engine::CLightMgr::GetLightInfo(const UINT& iIndex /*= 0*/)
{
	LIGHTLIST::iterator		iter = m_Lightlist.begin();
	for(UINT i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->GetLightInfo();
}

