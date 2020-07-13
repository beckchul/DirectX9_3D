#include "stdafx.h"
#include "Cromcruach.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "PunchEffect.h"
#include "Stage.h"
#include "LightningEffect.h"
#include "Stone.h"
#include "CromcruachHitBox.h"
#include "DamageFont.h"
#include "SoundMgr.h"
#include "EndUI.h"

CCromcruach::CCromcruach(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fSpeed(0.f)
, m_fDelay(0.f)
, m_dwMeshIndex(0)
, m_fCullTime(0.f)
, m_vAIDir(0.f, 0.f, 0.f)
, m_bLeft(false)
, m_fLeftCount(0.f)
, m_fHitColor(1.f)
, m_iHp(30000)
, m_iHpMax(30000)
, m_iDamage(200)
, m_bDeadCheck(false)
, m_iDashCount(0)
{
	ZeroMemory(m_fMoveSpeed, sizeof(float) * 3);
	ZeroMemory(m_fDelayCount, sizeof(float) * 3);
	for (int i = 0; i < 3; ++i)
	{
		m_bDelayCheck[i] = false;
	}
}

CCromcruach::~CCromcruach(void)
{
	Release();
}

HRESULT CCromcruach::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fSpeed = 30.f;

	m_pInfo->m_vPos = D3DXVECTOR3(25.f, 0.4f, 0.f);
	m_pInfo->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);

	m_pCromcruachInfo = &m_pInfo->m_matWorld;
	m_pmatWeaponRef = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_LH");


	HANDLE hFile = CreateFile(L"../bin/Data/CromcruachAnimation.bat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwByte;

	while (true)
	{
		Engine::ANI_INFO AniData;

		ReadFile(hFile, &AniData, sizeof(Engine::ANI_INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			break;
		}

		m_vecAniInfo.push_back(AniData);
	}

	CloseHandle(hFile);
	
	m_pCollisionRef[0] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Calf");
	m_pCollisionRef[1] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Calf");
	m_pCollisionRef[2] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Foot");
	m_pCollisionRef[3] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Foot");
	m_pCollisionRef[4] = m_pMesh->FindFrame("ValveBiped_Bip01_Head1");
	m_pCollisionRef[5] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Shoulder01");
	m_pCollisionRef[6] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Shoulder01");
	m_pCollisionRef[7] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Forearm");
	m_pCollisionRef[8] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Forearm");
	m_pCollisionRef[9] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Hand");
	m_pCollisionRef[10] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Hand");
	m_pCollisionRef[11] = m_pMesh->FindFrame("ValveBiped_Bip01_Spine");
	m_pCollisionRef[12] = m_pMesh->FindFrame("ValveBiped_Bip01_Spine1");




	//m_JiggleOrignal[0] = m_pMesh->FindFrame2("jiggle_cromcruach_left_1");
	//m_JiggleOrignal[1] = m_pMesh->FindFrame2("jiggle_cromcruach_left_2");
	//m_JiggleOrignal[2] = m_pMesh->FindFrame2("jiggle_cromcruach_left_3");
	//m_JiggleOrignal[3] = m_pMesh->FindFrame2("jiggle_cromcruach_left_4");
	//m_JiggleOrignal[4] = m_pMesh->FindFrame2("jiggle_cromcruach_left_5");
	//m_JiggleOrignal[5] = m_pMesh->FindFrame2("jiggle_cromcruach_left_6");
	//m_JiggleOrignal[6] = m_pMesh->FindFrame2("jiggle_cromcruach_right_1");
	//m_JiggleOrignal[7] = m_pMesh->FindFrame2("jiggle_cromcruach_right_2");
	//m_JiggleOrignal[8] = m_pMesh->FindFrame2("jiggle_cromcruach_right_3");
	//m_JiggleOrignal[9] = m_pMesh->FindFrame2("jiggle_cromcruach_right_4");
	//m_JiggleOrignal[10] = m_pMesh->FindFrame2("jiggle_cromcruach_right_5");
	//m_JiggleOrignal[11] = m_pMesh->FindFrame2("jiggle_cromcruach_right_6");

	m_eMainMonsterState = MAIN_MONSTER_IDLE;
	m_eMonsterState = MONSTER_IDLE;
	m_pMesh->SetAnimationSet(m_vecAniInfo[m_eMonsterState]);

	m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f);

	m_pObserver = CUIObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pObserver);

	Engine::Get_InfoSubject()->AddData(MONSTER_HP, &m_iHp);
	Engine::Get_InfoSubject()->AddData(MONSTER_HPMAX, &m_iHpMax);
	Engine::Get_InfoSubject()->AddData(MONSTER_DAMAGE, &m_iDamage);

	return S_OK;
}

void CCromcruach::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f), &m_pInfo->m_matWorld);

	m_fHitColor += Engine::Get_TimeMgr()->GetTime() * 5.f;

	if (!m_bDeadCheck)
	{
		Patton();
		StateMove();
		HitCheck();

		if (m_iHp <= 0)
		{
			m_eMonsterState = MONSTER_ATTACK_FINAL_BEGIN;
			m_pMesh->SetAnimationSet(m_vecAniInfo[m_eMonsterState]);
			m_bDeadCheck = true;

			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_breath_01.wav", CHANNEL_MONSTER_SOUND, 0.5f);
			return;
		}
	}

	if (m_pMesh->GetPeriod() - m_vecAniInfo[m_eMonsterState].dEndAniCount <= m_pMesh->GetTrackPos())
	{
		if (m_eMonsterState == MONSTER_ATTACK_FINAL_END &&
			m_bDeadCheck)
		{
			const list<CGameObject* > pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_UI, L"EndUI");
			((CEndUI*)pObjectlist.front())->RenderEnd();
		}

		StateChange();
		m_pMesh->SetAnimationSet(m_vecAniInfo[m_eMonsterState]);
		m_pMesh->SetTrackPos(0.0);
	}

	Engine::CGameObject::Update();
	Engine::CGameObject::Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Get_InfoSubject()->Notify(MONSTER_HP);
	Engine::Get_InfoSubject()->Notify(MONSTER_HPMAX);
	Engine::Get_InfoSubject()->Notify(MONSTER_DAMAGE);
}

void CCromcruach::JiggleMove(void)
{
	D3DXMATRIX pTempMatrix;
	for (int i = 0; i < 12; ++i)
	{
		pTempMatrix = (*m_JiggleOrignal)[i];
		pTempMatrix._41 += (float(rand() % 200) - 100.f) / 50;
		pTempMatrix._42 += (float(rand() % 200) - 100.f) / 50;
		pTempMatrix._43 += (float(rand() % 200) - 100.f) / 50;
	}
}

void CCromcruach::HitCheck(void)
{
	for (int i = 0; i < 8; ++i)
	{
		if (*(m_pCheckCollision[i]) && m_fHitColor >= 1.f)
		{
			int iDamage = (int)*m_pObserver->GetPlayerDamage();
			m_fHitColor = 0.f;
			int iPower = rand() % iDamage + iDamage / 2;
			m_iHp -= iPower;

			const list<CGameObject* > pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_UI, L"DamageFont");

			D3DXVECTOR3 vPos = m_pInfo->m_vPos + D3DXVECTOR3(0.f, 3.f, 0.f);
			((CDamageFont*)pObjectlist.front())->RenderFont(iPower, i, vPos);

			CSoundMgr::GetInstance()->PlaySound(L"sword_hit1.wav", CHANNEL_NONE_EFFECT, 0.5f);
		}
	}
}

void CCromcruach::Release(void)
{
	Safe_Delete(m_pObserver);
}

void CCromcruach::Render(void)
{
	SetConstantTable();
	//JiggleMove();

	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());
	m_pMesh->RenderMesh_ForSDK(m_pShader->GetEffectHandle()
		, &m_pInfo->m_matWorld);
}

HRESULT CCromcruach::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Cromcruach");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_DynamicMesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CCromcruach* CCromcruach::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCromcruach*		pGameObject = new CCromcruach(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}


const D3DXMATRIX* CCromcruach::GetCromcruachInfo(void)
{
	return m_pCromcruachInfo;
}

const D3DXMATRIX* CCromcruach::GetWeaponRef(void)
{
	return m_pmatWeaponRef;
}

const D3DXMATRIX* CCromcruach::GetCollisionRef(int iIndex)
{
	return m_pCollisionRef[iIndex];
}

const MONSTERSTATE* CCromcruach::GetMonsterState(void)
{
	return &m_eMonsterState;
}

void CCromcruach::SetConstantTable(void)
{
	LPD3DXEFFECT	pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK(pEffect);

	pEffect->SetVector("gLightColor", &D3DXVECTOR4(1, 1, 1, 1));
	pEffect->SetVector("gWorldLightPosition", &D3DXVECTOR4(500, 500, 0, 1));
	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("gWorldCameraPosition", (D3DXVECTOR4*)&matView.m[3][0]);
	pEffect->SetFloat("g_fDetail", m_fHitColor);

	if (g_bBlackScreen)
	{
		pEffect->SetFloat("g_fBlack", 0.f);
		pEffect->SetFloat("g_fColor", g_fWorldColor);
	}
	else
		pEffect->SetFloat("g_fBlack", 1.f);
}

void CCromcruach::Patton(void)
{
	if (m_fCullTime <= 0.f && m_eMonsterState == MONSTER_IDLE)
	{
		if (g_bBlackScreen)
		{
			m_eMonsterState = MONSTER_IDLE;
			m_pMesh->SetAnimationSet(m_vecAniInfo[m_eMonsterState]);
			return;
		}

		const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"PunchEffect");
		((CPunchEffect*)pObjectlist.front())->SetRender(true);

		int iRandom = rand() % 3500;
		if ((iRandom % 7) == 0)
		{
			m_eMonsterState = MONSTER_ATTACK_JUMP_SMASH1;
			m_iDamage = 400;
			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_threat_01.wav", CHANNEL_MONSTER_SOUND, 0.5f);
		}
		else if ((iRandom % 7) == 1)
		{
			m_eMonsterState = MONSTER_ATTACK_NORMAL;
			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_shout_short_01.wav", CHANNEL_MONSTER_SOUND, 0.5f);
		}
		else if ((iRandom % 7) == 2)
		{
			m_eMonsterState = MONSTER_ATTACK_LOOK;
			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_shout_long_01.wav", CHANNEL_MONSTER_SOUND, 0.5f);
		}
		else if ((iRandom % 7) == 3)
		{
			m_eMonsterState = MONSTER_ATTACK_2COMBO;
			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_shout_long_03.wav", CHANNEL_MONSTER_SOUND, 0.5f);
		}
		else if ((iRandom % 7) == 4)
		{
			m_eMonsterState = MONSTER_ATTACK_3COMBO;
			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_shout_long_03.wav", CHANNEL_MONSTER_SOUND, 0.5f);
		}
		else if ((iRandom % 7) == 5)
			//m_eMonsterState = MONSTER_ATTACK_4COMBO;
		{
			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_threat_02.wav", CHANNEL_MONSTER_SOUND, 0.5f);
			m_eMonsterState = MONSTER_ATTACK_MAGIC_SQUARE;
		}
		else if ((iRandom % 7) == 6)
		{
			CSoundMgr::GetInstance()->PlaySound(L"cromcruach_shout_short_01.wav", CHANNEL_MONSTER_SOUND, 0.5f);
			m_eMonsterState = MONSTER_DASH_BEGIN;
		}
			
		/*else if ((iRandom % 8) == 7)
			m_eMonsterState = MONSTER_ATTACK_MAGIC_SQUARE;
		*/
		m_pMesh->SetAnimationSet(m_vecAniInfo[m_eMonsterState]);
		m_fDelay = 0.f;
		switch (m_eMonsterState)
		{
		case MONSTER_ATTACK_NORMAL:
			m_bDelayCheck[0] = false;
			m_fDelayCount[0] = 9.f;
			m_fMoveSpeed[0] = 2000.f;
			m_fCullTime = 40.f;
			m_iDamage = 200;
			break;
		case MONSTER_ATTACK_LOOK:
			m_fDelayCount[0] = 20.f;
			m_bDelayCheck[0] = false;
			m_fMoveSpeed[0] = 4500.f;
			m_fCullTime = 40.f;
			m_iDamage = 200;
			break;
		case MONSTER_ATTACK_2COMBO:
			m_fDelayCount[0] = 10.f;
			m_fDelayCount[1] = 20.f;
			m_bDelayCheck[0] = false;
			m_bDelayCheck[1] = false;
			m_fMoveSpeed[0] = 2000.f;
			m_fMoveSpeed[1] = 3000.f;
			m_fCullTime = 50.f;
			m_iDamage = 230;
			break;
		case MONSTER_ATTACK_3COMBO:
			m_fDelayCount[0] = 5.f;
			m_fDelayCount[1] = 18.f;
			m_fDelayCount[2] = 28.f;
			m_bDelayCheck[0] = false;
			m_bDelayCheck[1] = false;
			m_bDelayCheck[2] = false;
			m_fMoveSpeed[0] = 800.f;
			m_fMoveSpeed[1] = 500.f;
			m_fMoveSpeed[2] = 1000.f;
			m_fCullTime = 60.f;
			m_iDamage = 250;
			break;
		case MONSTER_ATTACK_4COMBO:
			m_fDelayCount[0] = 20.f;
			m_fDelayCount[1] = 26.f;
			m_fDelayCount[2] = 32.f;
			m_bDelayCheck[0] = false;
			m_bDelayCheck[1] = false;
			m_bDelayCheck[2] = false;
			m_fMoveSpeed[0] = 700.f;
			m_fMoveSpeed[1] = 700.f;
			m_fMoveSpeed[2] = 500.f;
			m_fCullTime = 60.f;
			break;
		case MONSTER_DASH_BEGIN:
			m_fDelayCount[0] = 3.f;
			m_bDelayCheck[0] = false;
			m_fMoveSpeed[0] = 6000.f;
			m_fCullTime = 60.f;
			m_iDashCount = 2;
			break;
		case MONSTER_DASH_BEGIN_SHORT:
			break;
		case MONSTER_DASH_END_SHORT:
			m_bDelayCheck[0] = true;
			m_fMoveSpeed[0] = 600.f;
			break;
		case MONSTER_DASH_END:
			m_bDelayCheck[0] = true;
			m_fMoveSpeed[0] = 600.f;
			break;
		case MONSTER_ATTACK_MAGIC_SQUARE:
			m_fCullTime = 40.f;
			m_iDamage = 400;
			break;
		}
	}
}

void CCromcruach::StateMove(void)
{
	float		fTime = Engine::Get_TimeMgr()->GetTime();
	m_fDelay += fTime * 10.f;

	for (int i = 0; i < 3; ++i)
	{
		if (m_fMoveSpeed[i] <= 0.f)
			m_fMoveSpeed[i] = 0.f;
	}

	if (m_fDelay >= 1.f)
	{
		m_fDelay = 0.f;
		--m_fCullTime;

		for (int i = 0; i < 3; ++i)
		{
			--m_fDelayCount[i];
			if (m_fDelayCount[i] <= 0.f)
			{
				m_bDelayCheck[i] = true;		

				if (m_bDelayCheck[0]
					&& (m_eMonsterState == MONSTER_ATTACK_JUMP_SMASH2 || m_eMonsterState == MONSTER_ATTACK_MAGIC_SQUARE_END)
					&& m_bEffectRender[0])
				{
					m_bEffectRender[0] = false;
					list<CGameObject*> pObjectlist = Engine::Get_Management()->GetNotConstGameObjectlist(CStage::LAYER_GAMELOGIC, L"LightningEffect");
					
					list<CGameObject*>::iterator iter = pObjectlist.begin();
					list<CGameObject*>::iterator iter_end = pObjectlist.end();

					for (; iter != iter_end; ++iter)
					{
						((CLightningEffect*)(*iter))->SetRender(true);
						((CLightningEffect*)(*iter))->SetEffectPos(m_pInfo->m_vPos);
					}

					const list<CGameObject*> pObjectlist2 = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"Stone");
					((CStone*)pObjectlist2.front())->SetEffexct(m_pInfo->m_vPos, 10.f);



					const list<CGameObject*> pObjectlist3 = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"CromcruachHitBox");
					((CCromcruachHitBox*)pObjectlist3.front())->SetColPos(m_pInfo->m_vPos);
					((CCromcruachHitBox*)pObjectlist3.front())->SetCollision(true);
					((CCromcruachHitBox*)pObjectlist3.front())->SetColScale(D3DXVECTOR3(2.5f, 2.5f, 2.5f));


					CSoundMgr::GetInstance()->PlaySound(L"effect_electric_spark_b.wav", CHANNEL_MONSTER_EFFECT, 0.5f);
					CSoundMgr::GetInstance()->PlaySound(L"Elahan_Rock_impact_Huge.wav", CHANNEL_MONSTER_EFFECT2, 0.5f);
				}	
			}
		}
	}

	const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"PunchEffect");
	D3DXVECTOR3 vPos;
	memcpy(&vPos, &(*m_pCollisionRef[10]).m[3][0], sizeof(D3DXVECTOR3));
	D3DXVec3TransformCoord(&vPos, &vPos, &m_pInfo->m_matWorld);
	((CPunchEffect*)pObjectlist.front())->SetEffectPos(vPos);

	switch (m_eMonsterState)
	{
	case MONSTER_ATTACK_NORMAL:
		DirMove(0, 10000.f, fTime);
		break;
	case MONSTER_ATTACK_LOOK:
		DirMove(0, 13500.f, fTime);
		break;
	case MONSTER_ATTACK_2COMBO:
		DirMove(0, 10000.f, fTime);
		DirMove(1, 12000.f, fTime);
		break;
	case MONSTER_ATTACK_3COMBO:
		DirMove(0, 700.f, fTime);
		DirMove(1, 800.f, fTime);
		DirMove(2, 700.f, fTime);
		break;
	case MONSTER_ATTACK_4COMBO:
		DirMove(0, 1200.f, fTime);
		DirMove(1, 1200.f, fTime);
		DirMove(2, 3000.f, fTime);
		break;
	case MONSTER_DASH_DURING:
		DirMove(0, 15000.f, fTime);
		break;
	case MONSTER_DASH_END_SHORT:
		DirMove(0, 3600.f, fTime);
		break;
	case MONSTER_DASH_END:
		DirMove(0, 3600.f, fTime);
		break;
	}
}

void CCromcruach::DirMove(int _iIndex, float _fSpeedWeight, float _fTime)
{
	if (m_bDelayCheck[_iIndex])
	{
		m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &(m_pInfo->m_vDir * m_fMoveSpeed[_iIndex] * _fTime), m_dwMeshIndex);

		if (m_fMoveSpeed[_iIndex] >= 0.f)
			m_fMoveSpeed[_iIndex] -= _fTime * _fSpeedWeight;
	}
	else
	{
		D3DXVECTOR3 vDir;
		D3DXVec3Normalize(&vDir, &((*m_pPlayerPos) - m_pInfo->m_vPos));
		
		float fDot, fRadian;
		fDot = D3DXVec3Dot(&vDir, &m_pInfo->m_vDir);
		fRadian = acosf(fDot);
		D3DXVECTOR3 vRightDir;
		D3DXVec3Cross(&vRightDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		if (D3DXVec3Dot(&vRightDir, &vDir) > 0)
		{
			if (!m_bLeft)
			{
				m_fLeftCount += Engine::Get_TimeMgr()->GetTime();
				if (m_fLeftCount < 1.f)
					return;
			}
			// аб
			m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(90.f) * Engine::Get_TimeMgr()->GetTime();
			m_bLeft = true;
			m_fLeftCount = 0.f;
		}
		else
		{
			// ©Л
			if (m_bLeft)
			{
				m_fLeftCount += Engine::Get_TimeMgr()->GetTime();
				if (m_fLeftCount < 1.f)
					return;
			}
			m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f) * Engine::Get_TimeMgr()->GetTime();
			m_bLeft = false;
			m_fLeftCount = 0.f;
		}
	}
}

void CCromcruach::StateChange(void)
{
	switch (m_eMonsterState)
	{
	case MONSTER_IDLE:
		break;
	case MONSTER_ATTACK_NORMAL:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_ATTACK_LOOK:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_ATTACK_2COMBO:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_ATTACK_3COMBO:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_ATTACK_4COMBO:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_DASH_BEGIN:
		m_fDelayCount[1] = 4.f;
		m_bDelayCheck[1] = false;
		m_eMonsterState = MONSTER_DASH_DURING;
		CSoundMgr::GetInstance()->PlaySound(L"hagie_skill_speeed_move_attack.wav", CHANNEL_MONSTER_EFFECT, 0.5f);
		break;
	case MONSTER_DASH_BEGIN_SHORT:
		m_eMonsterState = MONSTER_DASH_END_SHORT;
		break;
	case MONSTER_DASH_DURING:
		if(m_bDelayCheck[1])
			m_eMonsterState = MONSTER_DASH_END;
		break;
	case MONSTER_DASH_END_SHORT:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_DASH_END:
		if (m_iDashCount > 0)
		{
			--m_iDashCount;
			m_eMonsterState = MONSTER_DASH_BEGIN;
			m_pInfo->m_fAngle[Engine::ANGLE_Y] =  D3DXToRadian((float)(rand() % 180 + 90));
			m_fDelayCount[0] = 3.f;
			m_bDelayCheck[0] = false;
			m_fMoveSpeed[0] = 6000.f;
		}
		else
			m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_ATTACK_MAGIC_SQUARE:
		m_eMonsterState = MONSTER_ATTACK_MAGIC_SQUARE_DURING1;
		break;
	case MONSTER_ATTACK_MAGIC_SQUARE_DURING1:
		m_eMonsterState = MONSTER_ATTACK_MAGIC_SQUARE_DURING2;
		m_fDelayCount[0] = 13.f;
		m_bDelayCheck[0] = false;
		m_bEffectRender[0] = true;
		break;
	case MONSTER_ATTACK_MAGIC_SQUARE_DURING2:
		m_eMonsterState = MONSTER_ATTACK_MAGIC_SQUARE_END;
		break;
	case MONSTER_ATTACK_MAGIC_SQUARE_END:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_ATTACK_JUMP_SMASH1:
		m_eMonsterState = MONSTER_ATTACK_JUMP_SMASH2;
		m_fDelayCount[0] = 17.f;
		m_bDelayCheck[0] = false;
		m_bEffectRender[0] = true;
		break;
	case MONSTER_ATTACK_JUMP_SMASH2:
		m_eMonsterState = MONSTER_IDLE;
		break;
	case MONSTER_ATTACK_FINAL_BEGIN:
		m_eMonsterState = MONSTER_ATTACK_FINAL_DURING;
		break;
	case MONSTER_ATTACK_FINAL_DURING:
		m_eMonsterState = MONSTER_ATTACK_FINAL_DURING_THROW;
		CSoundMgr::GetInstance()->PlaySound(L"cromcruach_breath_02.wav", CHANNEL_MONSTER_SOUND, 0.5f);
		break;
	case MONSTER_ATTACK_FINAL_DURING_THROW:
		CSoundMgr::GetInstance()->PlaySound(L"cromcruach_breath_03.wav", CHANNEL_MONSTER_SOUND, 0.5f);
		m_eMonsterState = MONSTER_ATTACK_FINAL_END;
		break;
	case MONSTER_ATTACK_FINAL_END:
		break;
	}
}

