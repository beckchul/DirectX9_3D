#include "stdafx.h"
#include "SkillIcon.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"

CSkillIcon::CSkillIcon(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_fX(0.f)
, m_fY(0.f)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CSkillIcon::~CSkillIcon(void)
{
	Safe_Delete(m_pUIObserver);
}

HRESULT CSkillIcon::Initialize(int _iIndex)
{
	FAILED_CHECK(AddComponent());

	switch (_iIndex)
	{
	case 0:
		m_fX = 100.f;
		m_fY = float(WINCY - 200);
		break;
	case 1:
		m_fX = 150.f;
		m_fY = float(WINCY - 200);
		break;
	case 2:
		m_fX = 200.f;
		m_fY = float(WINCY - 200);
		break;
	case 3:
		m_fX = 100.f;
		m_fY = float(WINCY - 150);
		break;
	case 4:
		m_fX = 150.f;
		m_fY = float(WINCY - 150);
		break;

	case 5:
		m_fX = 200.f;
		m_fY = float(WINCY - 150);
		break;
	}

	m_fSizeX = 40;
	m_fSizeY = 40;

	m_iIndex = _iIndex;

	m_pUIObserver = CUIObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pUIObserver);


	return S_OK;
}

void CSkillIcon::Update(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_matView._11 = m_fSizeX;
	m_matView._22 = m_fSizeY;
	m_matView._33 = 1.f;

	m_matView._41 = m_fX - (WINCX >> 1);
	m_matView._42 = -m_fY + (WINCY >> 1);

	RECT	rcSkillIcon = { long(m_fX - m_fSizeX * 0.5f)
		, long(m_fY - m_fSizeY * 0.5f)
		, long(m_fX + m_fSizeX * 0.5f)
		, long(m_fY + m_fSizeY * 0.5f) };

	Engine::CGameObject::Update();
}

void CSkillIcon::Render(void)
{
	if (Engine::Get_Input()->GetDIKeyState(DIK_P))
		return;

	if ((int)*m_pUIObserver->GetMonsterHp() <= 0)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX		matOldView, matOldProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTexture->Render(0, m_iIndex);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CSkillIcon::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"Skillicon");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CSkillIcon* CSkillIcon::Create(LPDIRECT3DDEVICE9 pDevice, int _iIndex)
{
	CSkillIcon*		pGameObject = new CSkillIcon(pDevice);
	if (FAILED(pGameObject->Initialize(_iIndex)))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}