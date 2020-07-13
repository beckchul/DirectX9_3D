#include "Resources.h"

Engine::CResources::CResources(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CResources::CResources(const CResources& rhs)
: CComponent(rhs)
, m_pDevice(rhs.m_pDevice)
{

}

Engine::CResources::~CResources(void)
{

}