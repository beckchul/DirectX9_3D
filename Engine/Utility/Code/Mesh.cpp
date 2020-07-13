#include "Mesh.h"

#include "VIBuffer.h"

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pDevice)
	: CResources(pDevice)
{

}

Engine::CMesh::CMesh(const CMesh& rhs)
	: CResources(rhs)
{

}

Engine::CMesh::~CMesh(void)
{

}