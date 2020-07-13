#include "stdafx.h"
#include "Arisha.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "DynamicMesh.h"
#include "Shader.h"
#include "ExplosionEffect.h"
#include "SmashEffect.h"
#include "RuinEffect.h"
#include "ManaRegionEffect.h"
#include "Stage.h"
#include "ResonanceSubEffect.h"
#include "ResonanceEffect.h"
#include "ResonanceBoomEffect.h"
#include "DamageScreen.h"
#include "SoundMgr.h"
#include "ManaHandEffect.h"
#include "ManaHandEffect2.h"
#include "ManaMagicEffect.h"
#include "ManaBoomEffect.h"
#include "ArishaHitBox.h"

CArisha::CArisha(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fSpeed(0.f)
, m_iAniCount(0)
, m_pmatWeaponRef(NULL)
, m_dwMeshIndex(0)
, m_fDamageSpeed(0.f)
, m_fMoveSpeed(0.f)
, m_fDelay(0.f)
, m_fDelayCount(0.f)
, m_fHitCheck(2.f)
, m_fEffectDelay(0.f)
, m_iHp(5000)
, m_iHpMax(5000)
, m_iDamage(1)
, m_pHandLMatrix(NULL)
{
	ZeroMemory(m_fEffectCount, sizeof(float) * 10);
	ZeroMemory(m_bEffectRender, sizeof(bool) * 10);
}

CArisha::~CArisha(void)
{
	Release();
}

HRESULT CArisha::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fSpeed = 100.f;

	m_pInfo->m_vPos = D3DXVECTOR3(-25.f, 1.f, 0.f);
	m_pInfo->m_vScale = D3DXVECTOR3(0.03f, 0.03f, 0.03f);

	m_dwMeshIndex = 0;
	m_pArishaInfo = &m_pInfo->m_matWorld;
	m_pmatWeaponRef = m_pMesh->FindFrame("ValveBiped_Anim_Attachment_RH");

	//m_pCollisionRef[0] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Hand");
	//m_pCollisionRef[1] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Hand");
	m_pCollisionRef[4] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Forearm");
	m_pCollisionRef[5] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Forearm");
	//m_pCollisionRef[2] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Shoulder01");
	//m_pCollisionRef[3] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Shoulder01");
	m_pCollisionRef[2] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Calf");
	m_pCollisionRef[3] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Calf");
	m_pCollisionRef[0] = m_pMesh->FindFrame("ValveBiped_Bip01_R_Foot");
	m_pCollisionRef[1] = m_pMesh->FindFrame("ValveBiped_Bip01_L_Foot");
	m_pCollisionRef[6] = m_pMesh->FindFrame("ValveBiped_Bip01_Spine");
	m_pCollisionRef[7] = m_pMesh->FindFrame("ValveBiped_Bip01_Head1");

	m_pHandLMatrix = m_pMesh->FindFrame("ValveBiped_Bip01_L_Hand");

	HANDLE hFile = CreateFile(L"../bin/Data/ArishaAnimation.bat",
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

	m_eMainPlayerState = MAIN_PLAYER_IDLE;
	m_ePlayerState = PLAYER_IDLE;
	m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);
	m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(90.f);

	m_pObserver = CUIObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pObserver);

	Engine::Get_InfoSubject()->AddData(PLAYER_HP, &m_iHp);
	Engine::Get_InfoSubject()->AddData(PLAYER_HPMAX, &m_iHpMax);
	Engine::Get_InfoSubject()->AddData(PLAYER_DAMAGE, &m_iDamage);

	return S_OK;
}

void CArisha::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);

	HitCheck();
	KeyCheck();
	if (!Engine::Get_Input()->GetDIKeyState(DIK_Z))
		MouseMove();
	StateMove();

	if (m_pMesh->GetPeriod() - m_vecAniInfo[m_ePlayerState].dEndAniCount <= m_pMesh->GetTrackPos())
	{
		StateChange();
		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);
		m_pMesh->SetTrackPos(0.0);
		g_iCameraOption = 0;
	}
	Engine::CGameObject::Update();
	Engine::CGameObject::Compute_ViewZ(&m_pInfo->m_vPos);

	Engine::Get_InfoSubject()->Notify(PLAYER_HP);
	Engine::Get_InfoSubject()->Notify(PLAYER_HPMAX);
	Engine::Get_InfoSubject()->Notify(PLAYER_DAMAGE);
}

void CArisha::Render(void)
{
	SetConstantTable();

	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());
	m_pMesh->RenderMesh_ForSDK(m_pShader->GetEffectHandle()
		, &m_pInfo->m_matWorld);
}

HRESULT CArisha::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Arisha");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_DynamicMesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CArisha* CArisha::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CArisha*		pGameObject = new CArisha(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CArisha::KeyCheck(void)
{
	if (m_eMainPlayerState == MAIN_PLAYER_DAMAGE ||
		m_eMainPlayerState == MAIN_PLAYER_DAMAGE_MB)
		return;

	float		fTime = Engine::Get_TimeMgr()->GetTime();

	if (Engine::Get_Input()->GetDIKeyState(DIK_W))
	{
		if (m_eMainPlayerState != MAIN_PLAYER_ATTACK
			&& m_eMainPlayerState != MAIN_PLAYER_ATTACK_MB
			&& m_eMainPlayerState != MAIN_PLAYER_SKILL
			&& m_eMainPlayerState != MAIN_PLAYER_SKILL_MB)
		{
			m_bDesh = true;
			m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
				, &(m_pInfo->m_vDir * m_fSpeed * fTime * 2.f), m_dwMeshIndex);

			if (m_eMainPlayerState == MAIN_PLAYER_IDLE)
				m_eMainPlayerState = MAIN_PLAYER_MOVE;
			else if (m_eMainPlayerState == MAIN_PLAYER_IDLE_MB)
				m_eMainPlayerState = MAIN_PLAYER_MOVE_MB;

			if (m_ePlayerState == PLAYER_IDLE ||
				m_ePlayerState == PLAYER_IDLE_MB)
			{
				m_ePlayerState = PLAYER_RUN;
				m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);
			}
		}
	}
	else
	{
		if (m_ePlayerState == PLAYER_RUN)
		{
			m_fDelayCount = 3.f;
			m_ePlayerState = PLAYER_RUN_STOP;
			m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);
			m_fMoveSpeed = 100.f;
		}
		m_bDesh = false;
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_H))
	{
		m_ePlayerState = PLAYER_MANA_BLADE;
		m_eMainPlayerState = MAIN_PLAYER_SKILL;
		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		CSoundMgr::GetInstance()->PlaySound(L"002_sword_1.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
		CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_manablade_enable_02.wav", CHANNEL_PLAYER_EFFECT2, 0.5f);
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_LSHIFT)
		&& m_fDelayCount <= 0.f)
	{
		m_ePlayerState = PLAYER_SPEEDY_MOVE;

		if (m_eMainPlayerState <= MAIN_PLAYER_END)
			m_eMainPlayerState = MAIN_PLAYER_SKILL;
		else
			m_eMainPlayerState = MAIN_PLAYER_SKILL_MB;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		m_fDelayCount = 10.f;
		m_fMoveSpeed = 1800.f;
		
		CSoundMgr::GetInstance()->PlaySound(L"arisha_lift_long_01.wav", CHANNEL_PLAYER_SOUND, 0.5f);
		CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_speedy_move.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
	}



	if (Engine::Get_Input()->GetDIKeyState(DIK_G)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[5])
			return;

		m_bPush[5] = true;

		if (m_ePlayerState == PLAYER_ATTACK_DOOMSAYER)
			return;

		m_ePlayerState = PLAYER_ATTACK_DOOMSAYER;

		if (m_eMainPlayerState <= MAIN_PLAYER_END)
			m_eMainPlayerState = MAIN_PLAYER_ATTACK;
		else
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		if (Engine::Get_Input()->GetDIKeyState(DIK_W))
			m_fMoveSpeed = 800.f;

		m_fDelayCount = 12.f;

		const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"SmashEffect");
		((CSmashEffect*)pObjectlist.front())->SetRender(true);
		D3DXVECTOR3 vPos;
		memcpy(&vPos, &(*m_pmatWeaponRef).m[3][0], sizeof(D3DXVECTOR3));
		D3DXVec3TransformCoord(&vPos, &vPos, &m_pInfo->m_matWorld);
		((CSmashEffect*)pObjectlist.front())->SetEffectPos(vPos);

		m_iDamage = 350;

		CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_07.wav", CHANNEL_PLAYER_SOUND, 0.5f);
		CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_manablade_05.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
	}
	else
		m_bPush[5] = false;

	if (Engine::Get_Input()->GetDIKeyState(DIK_F)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[2])
			return;

		m_bPush[2] = true;

		m_ePlayerState = PLAYER_MANA_REGION;

		if (m_eMainPlayerState <= MAIN_PLAYER_END)
			m_eMainPlayerState = MAIN_PLAYER_SKILL;
		else
			m_eMainPlayerState = MAIN_PLAYER_SKILL_MB;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		m_fDelayCount = 10.f;

		const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ManaRegionEffect");
		((CManaRegionEffect*)pObjectlist.front())->SetRender(true);
		D3DXVECTOR3 vPos = (m_pInfo->m_vPos) - (m_pInfo->m_vDir * 20.f);
		((CManaRegionEffect*)pObjectlist.front())->SetEffectPos(vPos);

		CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_manastone_mana_region_01.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
	}
	else
		m_bPush[2] = false;

	if (Engine::Get_Input()->GetDIKeyState(DIK_V)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[3])
			return;

		m_bPush[3] = true;

		m_ePlayerState = PLAYER_ATTACK_RUIN_BLACE;

		if (m_eMainPlayerState <= MAIN_PLAYER_END)
			m_eMainPlayerState = MAIN_PLAYER_SKILL;
		else
			m_eMainPlayerState = MAIN_PLAYER_SKILL_MB;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		m_fDelayCount = 15.f;

		const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ExplosionEffect");
		((CExplosionEffect*)pObjectlist.front())->SetRender(true);
		D3DXVECTOR3 vPos = (m_pInfo->m_vPos) + (m_pInfo->m_vDir * 5.f);
		((CExplosionEffect*)pObjectlist.front())->SetEffectPos(vPos);

		m_fEffectCount[0] = 25.f;
		m_bEffectRender[0] = true;
		m_iDamage = 400;


		CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_ruin_blade_encahnt.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
	}
	else
		m_bPush[3] = false;

	if (Engine::Get_Input()->GetDIKeyState(DIK_B)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[4])
			return;

		m_bPush[4] = true;

		m_ePlayerState = PLAYER_ATTACK_RESONANCE;

		if (m_eMainPlayerState <= MAIN_PLAYER_END)
			m_eMainPlayerState = MAIN_PLAYER_SKILL;
		else
			m_eMainPlayerState = MAIN_PLAYER_SKILL_MB;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		m_fDelayCount = 30.f;

		const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ResonanceEffect");
		((CResonanceEffect*)pObjectlist.front())->SetRender(true);
		D3DXVECTOR3 vPos = (m_pInfo->m_vPos) + (m_pInfo->m_vDir * 400.f);
		((CResonanceEffect*)pObjectlist.front())->SetEffectPos(vPos);

		g_iCameraOption = 1;

		m_fEffectCount[0] = 30.f;
		m_bEffectRender[0] = true;

		m_fEffectCount[1] = 50.f;
		m_bEffectRender[1] = true;

		m_iDamage = 600;

		CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_resonance_hand_begin.wav", CHANNEL_PLAYER_EFFECT2, 0.5f);
	}
	else
		m_bPush[4] = false;
	

	if (Engine::Get_Input()->GetDIKeyState(DIK_N)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[5])
			return;

		m_bPush[5] = true;

		m_ePlayerState = PLAYER_ATTACK_LORD_OF_MANA;

		if (m_eMainPlayerState <= MAIN_PLAYER_END)
			m_eMainPlayerState = MAIN_PLAYER_SKILL;
		else
			m_eMainPlayerState = MAIN_PLAYER_SKILL_MB;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		m_fDelayCount = 30.f;
		g_bBlackScreen = true;
		const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ManaHandEffect");
		((CManaHandEffect*)pObjectlist.front())->SetRender(true);
		
		D3DXVECTOR3 vPos;

		D3DXMATRIX mat = (*m_pHandLMatrix) * m_pInfo->m_matWorld;
		memcpy(&vPos, &mat.m[3][0], sizeof(D3DXVECTOR3));
		((CManaHandEffect*)pObjectlist.front())->SetEffectPos(vPos);

		g_iCameraOption = 2;

		m_fEffectCount[0] = 29.f;
		m_bEffectRender[0] = true;

		m_fEffectCount[1] = 35.f;
		m_bEffectRender[1] = true;

		m_fEffectCount[4] = 50.f;
		m_bEffectRender[4] = true;

		m_fEffectCount[2] = 70.f;
		m_bEffectRender[2] = true;	

		m_fEffectCount[3] = 80.f;
		m_bEffectRender[3] = true;

		CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_lord_of_mana_begin.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
		//CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_manastone_mana_region_01.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
	}
	else
		m_bPush[5] = false;

	if (Engine::Get_Input()->GetDIKeyState(DIK_T)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[6])
			return;

		if (m_ePlayerState != PLAYER_IDLE_MB)
			return;

		m_bPush[6] = true;

		m_ePlayerState = PLAYER_IDLE;

		if (m_eMainPlayerState == MAIN_PLAYER_SKILL_MB)
			m_eMainPlayerState = MAIN_PLAYER_IDLE;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);
	}
	else
		m_bPush[6] = false;

	if (Engine::Get_Input()->GetDIKeyState(DIK_A))
	{
		if (/*m_eMainPlayerState != MAIN_PLAYER_ATTACK &&*/
			m_eMainPlayerState != MAIN_PLAYER_SKILL &&
			m_eMainPlayerState != MAIN_PLAYER_DAMAGE &&
			m_eMainPlayerState != MAIN_PLAYER_DAMAGE_MB/*&&
			m_eMainPlayerState != MAIN_PLAYER_ATTACK_MB &&
			m_eMainPlayerState != MAIN_PLAYER_SKILL_MB &&
			m_eMainPlayerState != MAIN_PLAYER_DAMAGE_MB*/)
		{
			m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(45.f) * fTime;
		}
	}

	if (Engine::Get_Input()->GetDIKeyState(DIK_D))
	{
		if (/*m_eMainPlayerState != MAIN_PLAYER_ATTACK &&*/
			m_eMainPlayerState != MAIN_PLAYER_SKILL &&
			m_eMainPlayerState != MAIN_PLAYER_DAMAGE &&
			m_eMainPlayerState != MAIN_PLAYER_DAMAGE_MB/*&&
			m_eMainPlayerState != MAIN_PLAYER_ATTACK_MB &&
			m_eMainPlayerState != MAIN_PLAYER_SKILL_MB &&
			m_eMainPlayerState != MAIN_PLAYER_DAMAGE_MB*/)
		{
			m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(45.f) * fTime;
		}
	}

	if (Engine::Get_Input()->GetDIMouseState(Engine::CInput::DIM_RBUTTON)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[0])
			return;

		m_bPush[0] = true;

		if (m_eMainPlayerState > MAIN_PLAYER_END 
			|| m_eMainPlayerState == MAIN_PLAYER_SKILL
			|| m_eMainPlayerState == MAIN_PLAYER_SKILL_MB)
			return;

		if (m_ePlayerState == PLAYER_ATTACK_STRONG_1 ||
			m_ePlayerState == PLAYER_ATTACK_STRONG_4 ||
			m_ePlayerState == PLAYER_ATTACK_DOOMSAYER)
			return;

		switch (m_ePlayerState)
		{
		case PLAYER_ATTACK_1:
			m_ePlayerState = PLAYER_ATTACK_STRONG_1;
			m_fDelayCount = 5.f;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_05.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_longblade_strong_01.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_2:
			return;
			break;
		case PLAYER_ATTACK_3:
			m_ePlayerState = PLAYER_ATTACK_STRONG_4;
			m_fDelayCount = 5.f;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_01.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_longblade_strong_03.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		}

		if (m_eMainPlayerState < MAIN_PLAYER_END)
			m_eMainPlayerState = MAIN_PLAYER_ATTACK;


		if (Engine::Get_Input()->GetDIKeyState(DIK_W))
			m_fMoveSpeed = 800.f;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

		const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"SmashEffect");
		((CSmashEffect*)pObjectlist.front())->SetRender(true);
		D3DXVECTOR3 vPos;
		memcpy(&vPos, &(*m_pmatWeaponRef).m[3][0], sizeof(D3DXVECTOR3));
		D3DXVec3TransformCoord(&vPos, &vPos, &m_pInfo->m_matWorld);
		((CSmashEffect*)pObjectlist.front())->SetEffectPos(vPos);

		m_iDamage = 300;
	}
	else
		m_bPush[0] = false;

	if (Engine::Get_Input()->GetDIMouseState(Engine::CInput::DIM_LBUTTON)
		&& m_fDelayCount <= 0.f)
	{
		if (m_bPush[1])
			return;

		m_bPush[1] = true;

		if (m_eMainPlayerState == MAIN_PLAYER_SKILL
			|| m_eMainPlayerState == MAIN_PLAYER_SKILL_MB)
			return;

		if (m_ePlayerState == PLAYER_ATTACK_STRONG_1 ||
			m_ePlayerState == PLAYER_ATTACK_STRONG_4 ||
			m_ePlayerState == PLAYER_ATTACK_DOOMSAYER)
			return;

		switch (m_ePlayerState)
		{
		case PLAYER_IDLE:
			m_ePlayerState = PLAYER_ATTACK_1;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK;
			m_fDelayCount = 3.f;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_01.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_longblade_01.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_IDLE_MB:
			m_ePlayerState = PLAYER_ATTACK_MB_STRONG_1;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			m_fDelayCount = 3.f;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_01.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_manablade_01.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_RUN:
			if (m_eMainPlayerState == MAIN_PLAYER_MOVE)
			{
				m_ePlayerState = PLAYER_ATTACK_1;
				m_eMainPlayerState = MAIN_PLAYER_ATTACK;
			}
			else if (m_eMainPlayerState == MAIN_PLAYER_MOVE_MB)
			{
				m_ePlayerState = PLAYER_ATTACK_MB_STRONG_1;
				m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			}
			m_fDelayCount = 3.f;
			break;
		case PLAYER_RUN_STOP:
			return;
		case PLAYER_ATTACK_1:
			m_ePlayerState = PLAYER_ATTACK_2;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK;
			m_fDelayCount = 3.f;
			m_iDamage = 200;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_02.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_longblade_02.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_2:
			m_ePlayerState = PLAYER_ATTACK_3;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK;
			m_fDelayCount = 3.f;
			m_iDamage = 200;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_03.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_longblade_03.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_3:
			m_eMainPlayerState = MAIN_PLAYER_ATTACK;
			m_iDamage = 200;
			return;
		case PLAYER_ATTACK_STRONG_1:
			break;
		case PLAYER_ATTACK_STRONG_4:
			break;
		case PLAYER_ATTACK_STRONG_5:
			break;
		case PLAYER_ATTACK_MB_STRONG_1:
			m_ePlayerState = PLAYER_ATTACK_MB_STRONG_2;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			m_fDelayCount = 3.f;
			m_iDamage = 250;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_02.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_manablade_02.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_MB_STRONG_2:
			m_ePlayerState = PLAYER_ATTACK_MB_STRONG_3;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			m_fDelayCount = 3.f;
			m_iDamage = 250;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_03.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_manablade_03.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_MB_STRONG_3:
			m_ePlayerState = PLAYER_ATTACK_MB_STRONG_4;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			m_fDelayCount = 3.f;
			m_iDamage = 250;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_04.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_manablade_04.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_MB_STRONG_4:
			m_ePlayerState = PLAYER_ATTACK_MB_STRONG_5;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			m_fDelayCount = 3.f;
			m_iDamage = 250;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_05.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_manablade_05.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_MB_STRONG_5:
			m_ePlayerState = PLAYER_ATTACK_MB_STRONG_6;
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			m_fDelayCount = 3.f;
			m_iDamage = 250;
			CSoundMgr::GetInstance()->PlaySound(L"arisha_attack_strong_06.wav", CHANNEL_PLAYER_SOUND, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_swing_longblade_strong_02.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			break;
		case PLAYER_ATTACK_MB_STRONG_6:
			m_eMainPlayerState = MAIN_PLAYER_ATTACK_MB;
			m_iDamage = 250;
			return;
		}

		if (Engine::Get_Input()->GetDIKeyState(DIK_W))
			m_fMoveSpeed = 800.f;

		m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);
	}
	else
		m_bPush[1] = false;
}


const D3DXMATRIX * CArisha::GetWeaponRef(void)
{
	return m_pmatWeaponRef;
}

const D3DXMATRIX* CArisha::GetArishaInfo(void)
{
	return m_pArishaInfo;
}

const D3DXMATRIX* CArisha::GetCollisionRef(int iIndex)
{
	return m_pCollisionRef[iIndex];
}

const PLAYERSTATE* CArisha::GetPlayerState(void)
{
	return &m_ePlayerState;
}

const D3DXVECTOR3* CArisha::GetPlayerPos(void)
{
	return &(m_pInfo->m_vPos);
}

void CArisha::SetConstantTable(void)
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
}

void CArisha::StateMove(void)
{
	float		fTime = Engine::Get_TimeMgr()->GetTime();
	if (m_fDamageSpeed >= 0.f)
		m_fDamageSpeed -= 1200.f * fTime;

	if (m_fMoveSpeed < 0.f)
		m_fMoveSpeed = 0.f;

	if (m_fDamageSpeed < 0.f)
		m_fDamageSpeed = 0.f;

	m_fDelay += Engine::Get_TimeMgr()->GetTime() * 10.f;
	if (m_fDelay >= 1.f)
	{
		m_fDelay = 0.f;
		--m_fDelayCount;

		if (m_ePlayerState == PLAYER_ATTACK_LORD_OF_MANA)
			g_fWorldColor -= 0.03f;

		if (g_fWorldColor <= 0.3f)
			g_fWorldColor = 0.3f;
	}

	m_fEffectDelay += Engine::Get_TimeMgr()->GetTime() * 10.f;
	if (m_fEffectDelay >= 1.f)
	{
		m_fEffectDelay = 0.f;
		for(int i = 0; i < 10; ++i)
		--m_fEffectCount[i];

		if (m_bEffectRender[0] && m_fEffectCount[0] <= 0.f 
			&& m_ePlayerState == PLAYER_ATTACK_RUIN_BLACE)
		{
			m_bEffectRender[0] = false;
			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"RuinEffect");
			((CRuinEffect*)pObjectlist.front())->SetRender(true);
			D3DXVECTOR3 vPos = (m_pInfo->m_vPos) + (m_pInfo->m_vDir * 50.f);
			((CRuinEffect*)pObjectlist.front())->SetEffectPos(vPos, m_pInfo->m_vDir);

			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_ruin_blade_slash.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_ruin_blade_wave.wav", CHANNEL_PLAYER_EFFECT2, 0.7f);
		}
		if (m_bEffectRender[0] && m_fEffectCount[0] <= 0.f
			&& m_ePlayerState == PLAYER_ATTACK_RESONANCE)
		{
			m_bEffectRender[0] = false;
			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ResonanceSubEffect");
			((CResonanceSubEffect*)pObjectlist.front())->SetRender(true);
			D3DXVECTOR3 vPos = (m_pInfo->m_vPos) + (m_pInfo->m_vDir * 50.f);
			((CResonanceSubEffect*)pObjectlist.front())->SetEffectPos(vPos, m_pInfo->m_vDir);

			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_resonance_attack_during_1.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_resonance_hand_during.wav", CHANNEL_PLAYER_EFFECT2, 0.5f);
		}

		if (m_bEffectRender[1] && m_fEffectCount[1] <= 0.f
			&& m_ePlayerState == PLAYER_ATTACK_RESONANCE)
		{
			m_bEffectRender[1] = false;
			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ResonanceBoomEffect");
			((CResonanceBoomEffect*)pObjectlist.front())->SetRender(true);
			D3DXVECTOR3 vPos = (m_pInfo->m_vPos) + (m_pInfo->m_vDir * 500.f);
			((CResonanceBoomEffect*)pObjectlist.front())->SetEffectPos(vPos, m_pInfo->m_vDir);

			
		}

		if (m_bEffectRender[0] && m_fEffectCount[0] <= 0.f
			&& m_ePlayerState == PLAYER_ATTACK_LORD_OF_MANA)
		{
			m_bEffectRender[0] = false;
			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ManaMagicEffect");
			((CManaMagicEffect*)pObjectlist.front())->SetRender(true);
			((CManaMagicEffect*)pObjectlist.front())->SetEffectPos(m_pInfo->m_vPos);
			g_iCameraOption = 3;

			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_lord_of_mana_during.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
		}		

		if (m_bEffectRender[4] && m_fEffectCount[4] <= 0.f
			&& m_ePlayerState == PLAYER_ATTACK_LORD_OF_MANA)
		{
			m_bEffectRender[4] = false;
			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ManaHandEffect2");
			((CManaHandEffect2*)pObjectlist.front())->SetRender(true);

			D3DXVECTOR3 vPos, vDir;
			D3DXMATRIX mat = (*m_pHandLMatrix) * m_pInfo->m_matWorld;
			memcpy(&vPos, &mat.m[3][0], sizeof(D3DXVECTOR3));
			D3DXMATRIX matRotY, mat2;
			D3DXMatrixRotationY(&matRotY, D3DXToRadian(90.f));
			D3DXVec3TransformNormal(&vDir, &m_pInfo->m_vDir, &matRotY);
			
			vPos += vDir * 0.3f - (m_pInfo->m_vDir * 20.f);
			((CManaHandEffect2*)pObjectlist.front())->SetEffectPos(vPos);

			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_resonance_attack_during_2.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
		}

		if (m_bEffectRender[1] && m_fEffectCount[1] <= 0.f
			&& m_ePlayerState == PLAYER_ATTACK_LORD_OF_MANA)
		{
			m_bEffectRender[1] = false;
			g_iCameraOption = 4;
		}

		if (m_bEffectRender[2] && m_fEffectCount[2] <= 0.f
			&& m_ePlayerState == PLAYER_ATTACK_LORD_OF_MANA)
		{
			m_bEffectRender[2] = false;
			g_iCameraOption = 5;

			m_iDamage = 1400;
			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ArishaHitBox");
			((CArishaHitBox*)pObjectlist.front())->SetCollision(true);
			((CArishaHitBox*)pObjectlist.front())->SetColPos(m_pInfo->m_vPos);
			((CArishaHitBox*)pObjectlist.front())->SetColScale(D3DXVECTOR3(40.f, 40.f, 40.f));	


			const list<CGameObject*> pObjectlist2 = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ManaBoomEffect");
			((CManaBoomEffect*)pObjectlist2.front())->SetRender(true);
			D3DXVECTOR3 vPos = (m_pInfo->m_vPos) + (m_pInfo->m_vDir * 100.f);
			((CManaBoomEffect*)pObjectlist2.front())->SetEffectPos(vPos, m_pInfo->m_vDir);

			CSoundMgr::GetInstance()->PlaySound(L"arisha_skill_lord_of_mana_end.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
		}

		if (m_bEffectRender[3] && m_fEffectCount[3] <= 0.f
			&& m_ePlayerState == PLAYER_ATTACK_LORD_OF_MANA)
		{
			m_bEffectRender[3] = false;
			g_iCameraOption = 4;
			const list<CGameObject*> pObjectlist = Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_GAMELOGIC, L"ArishaHitBox");
			((CArishaHitBox*)pObjectlist.front())->SetCollision(false);

			g_bBlackScreen = false;
		}
	}

	switch (m_ePlayerState)
	{
	case PLAYER_RUN:
		break;
	case PLAYER_RUN_STOP:
		DirMove(100.f, fTime);
		break;
	case PLAYER_TELEPORT_MANA_STONE:
		break;
	case PLAYER_DAMAGE_LIGHT_FRONT:
		{
			m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &(m_pInfo->m_vDir * m_fDamageSpeed * fTime), m_dwMeshIndex);
		}
		break;
	case PLAYER_DAMAGE_LIGHT_RIGHT:
	{
		D3DXMATRIX matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(90.f));
		D3DXVECTOR3 vecTempDir;
		D3DXVec3TransformNormal(&vecTempDir, &m_pInfo->m_vDir, &matRotY);
		m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &(vecTempDir * m_fDamageSpeed * fTime), m_dwMeshIndex);
	}
		break;
	case PLAYER_DAMAGE_LIGHT_BACK:
	{
		m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &(m_pInfo->m_vDir * m_fDamageSpeed * fTime * -1.f), m_dwMeshIndex);
	}
		break;
	case PLAYER_DAMAGE_LIGHT_LEFT:
	{
		D3DXMATRIX matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(-90.f));
		D3DXVECTOR3 vecTempDir;
		D3DXVec3TransformNormal(&vecTempDir, &m_pInfo->m_vDir, &matRotY);
		m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &(vecTempDir * m_fDamageSpeed * fTime), m_dwMeshIndex);
	}
		break;
	case PLAYER_DAMAGE_MIDDLE_FRONT_BEGIN:
	{
		m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &(m_pInfo->m_vDir * m_fDamageSpeed * fTime), m_dwMeshIndex);
	}
		break;
	case PLAYER_DAMAGE_MIDDLE_BACK_BEGIN:
	{
		m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
			, &(m_pInfo->m_vDir * m_fDamageSpeed * fTime * -1.f), m_dwMeshIndex);
	}
		break;
	case PLAYER_ATTACK_1:
	case PLAYER_ATTACK_2:
	case PLAYER_ATTACK_3:
	case PLAYER_ATTACK_STRONG_1:
	case PLAYER_ATTACK_STRONG_4:
	case PLAYER_ATTACK_STRONG_5:
	case PLAYER_ATTACK_MB_STRONG_1:
	case PLAYER_ATTACK_MB_STRONG_2:
	case PLAYER_ATTACK_MB_STRONG_3:
	case PLAYER_ATTACK_MB_STRONG_4:
	case PLAYER_ATTACK_MB_STRONG_5:
	case PLAYER_ATTACK_MB_STRONG_6:
		DirMove(6000.f, fTime);
		break;
	case PLAYER_SPEEDY_MOVE:
		DirMove(8000.f, fTime);
		break;
	case PLAYER_ATTACK_DOOMSAYER:
		break;
	case PLAYER_ATTACK_RESONANCE:
		break;
	case PLAYER_ATTACK_RUIN_BLACE:
		break;
	case PLAYER_ATTACK_CASTLET_DIMENSION_HOLE:
		break;
	case PLAYER_ATTACK_LORD_OF_MANA:
		break;
	case PLAYER_DARK_KNIGHT_TANSFORMATION_BEGIN_1:
		break;
	case PLAYER_DARK_KNIGHT_TANSFORMATION_BEGIN_2:
		break;
	case PLAYER_DARK_KNIGHT_TANSFORMATION_END_1:
		break;
	}
}

void CArisha::DirMove(float _fSpeedWeight, float _fTime)
{
	m_dwMeshIndex = Engine::Get_NaviMgr()->MoveOnNaviMesh(&m_pInfo->m_vPos
		, &(m_pInfo->m_vDir * m_fMoveSpeed * _fTime), m_dwMeshIndex);
	if (m_fMoveSpeed >= 0.f)
		m_fMoveSpeed -= _fTime * _fSpeedWeight;
}

void CArisha::HitCheck(void)
{
	if (m_eMainPlayerState == MAIN_PLAYER_DAMAGE ||
		m_eMainPlayerState == MAIN_PLAYER_DAMAGE_MB ||
		m_eMainPlayerState == MAIN_PLAYER_SKILL ||
		m_eMainPlayerState == MAIN_PLAYER_SKILL_MB ||
		m_ePlayerState == PLAYER_SPEEDY_MOVE)
		return;

	m_fHitCheck += Engine::Get_TimeMgr()->GetTime() * 2.f;

	if (m_fHitCheck < 1.f)
		return;

	for (int i = 0; i < 8; ++i)
	{
		if (*(m_pCheckCollsion[i]))
		{

			if (m_eMainPlayerState <= MAIN_PLAYER_END)
				m_eMainPlayerState = MAIN_PLAYER_DAMAGE;
			else
				m_eMainPlayerState = MAIN_PLAYER_DAMAGE_MB;

			switch (i)
			{
			case 0:
				m_ePlayerState = PLAYER_DAMAGE_MIDDLE_FRONT_BEGIN;
				m_iHp -= (int)*m_pObserver->GetMonsterDamage() * 2;

				CSoundMgr::GetInstance()->PlaySound(L"arisha_hurt_medium_01.wav", CHANNEL_PLAYER_SOUND, 0.5f);
				CSoundMgr::GetInstance()->PlaySound(L"Hart.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
				break;
			case 1:
				m_ePlayerState = PLAYER_DAMAGE_MIDDLE_BACK_BEGIN;
				m_iHp -= (int)*m_pObserver->GetMonsterDamage() * 2;
				CSoundMgr::GetInstance()->PlaySound(L"arisha_hurt_medium_02.wav", CHANNEL_PLAYER_SOUND, 0.5f);
				CSoundMgr::GetInstance()->PlaySound(L"Hart.wav", CHANNEL_PLAYER_EFFECT, 0.5f); 
				break;
			case 4:
			case 2:
				m_ePlayerState = PLAYER_DAMAGE_LIGHT_RIGHT;
				m_iHp -= (int)*m_pObserver->GetMonsterDamage();
				CSoundMgr::GetInstance()->PlaySound(L"arisha_hurt_weak_01.wav", CHANNEL_PLAYER_SOUND, 0.5f);
				CSoundMgr::GetInstance()->PlaySound(L"HartS.wav", CHANNEL_PLAYER_EFFECT, 0.5f);
				break;
			case 5:
			case 3:
				m_ePlayerState = PLAYER_DAMAGE_LIGHT_LEFT;
				m_iHp -= (int)*m_pObserver->GetMonsterDamage();
				CSoundMgr::GetInstance()->PlaySound(L"arisha_hurt_weak_02.wav", CHANNEL_PLAYER_SOUND, 0.5f); 
				CSoundMgr::GetInstance()->PlaySound(L"HartS.wav", CHANNEL_PLAYER_EFFECT, 0.5f); 
				break;
			case 6:
				m_ePlayerState = PLAYER_DAMAGE_LIGHT_BACK;
				m_iHp -= (int)*m_pObserver->GetMonsterDamage();
				CSoundMgr::GetInstance()->PlaySound(L"arisha_hurt_weak_03.wav", CHANNEL_PLAYER_SOUND, 0.5f);
				CSoundMgr::GetInstance()->PlaySound(L"HartS.wav", CHANNEL_PLAYER_EFFECT, 0.5f); 
				break;
			case 7:
				m_ePlayerState = PLAYER_DAMAGE_LIGHT_FRONT;
				m_iHp -= (int)*m_pObserver->GetMonsterDamage();
				CSoundMgr::GetInstance()->PlaySound(L"arisha_hurt_weak_03.wav", CHANNEL_PLAYER_SOUND, 0.5f);
				CSoundMgr::GetInstance()->PlaySound(L"HartS.wav", CHANNEL_PLAYER_EFFECT, 0.5f); 
				break;
			}
			m_fHitCheck = 0.f;
			m_fDamageSpeed = 400.f;
			m_pMesh->SetAnimationSet(m_vecAniInfo[m_ePlayerState]);

			((CDamageScreen*)(Engine::Get_Management()->GetGameObjectlist(CStage::LAYER_UI, L"DamageScreen").front()))->RenderScreen();

			return;
		}
	}
}

void CArisha::MouseMove(void)
{
	if (m_ePlayerState == PLAYER_ATTACK_RESONANCE)
		return;

	int		iDistance = 0;
	if (iDistance = Engine::Get_Input()->GetDIMouseMove(Engine::CInput::DIM_X))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(iDistance) * Engine::Get_TimeMgr()->GetTime() * 2.f;
	}
	POINT	ptMouse = { WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CArisha::Release(void)
{
	Safe_Delete(m_pObserver);
}

void CArisha::StateChange(void)
{
	switch (m_ePlayerState)
	{
	case PLAYER_IDLE:
		m_ePlayerState = PLAYER_IDLE;
		m_eMainPlayerState = MAIN_PLAYER_IDLE;
		break;
	case PLAYER_IDLE_MB:
		m_ePlayerState = PLAYER_IDLE_MB;
		m_eMainPlayerState = MAIN_PLAYER_IDLE_MB;
		break;
	case PLAYER_RUN:
		if (!m_bDesh)
			m_ePlayerState = PLAYER_RUN_STOP;
		break;
	case PLAYER_RUN_STOP:
	case PLAYER_SPEEDY_MOVE:
	case PLAYER_TELEPORT_MANA_STONE:
	case PLAYER_DAMAGE_LIGHT_FRONT:
	case PLAYER_DAMAGE_LIGHT_RIGHT:
	case PLAYER_DAMAGE_LIGHT_BACK:
	case PLAYER_DAMAGE_LIGHT_LEFT:	
	case PLAYER_DAMAGE_MIDDLE_FRONT_END:
	case PLAYER_DAMAGE_MIDDLE_BACK_END:
	case PLAYER_ATTACK_DOOMSAYER:
	case PLAYER_ATTACK_RESONANCE:
	case PLAYER_ATTACK_RUIN_BLACE:
	case PLAYER_ATTACK_CASTLET_DIMENSION_HOLE:
	case PLAYER_MANA_REGION:
		if (m_eMainPlayerState <= MAIN_PLAYER_END)
		{
			m_ePlayerState = PLAYER_IDLE;
			m_eMainPlayerState = MAIN_PLAYER_IDLE;
		}
		else
		{
			m_ePlayerState = PLAYER_IDLE_MB;
			m_eMainPlayerState = MAIN_PLAYER_IDLE_MB;
		}
		break;
	case PLAYER_ATTACK_LORD_OF_MANA:
		g_bBlackScreen = false;
		g_fWorldColor = 1.f;
		if (m_eMainPlayerState <= MAIN_PLAYER_END)
		{
			m_ePlayerState = PLAYER_IDLE;
			m_eMainPlayerState = MAIN_PLAYER_IDLE;
		}
		else
		{
			m_ePlayerState = PLAYER_IDLE_MB;
			m_eMainPlayerState = MAIN_PLAYER_IDLE_MB;
		}
		break;
	case PLAYER_DAMAGE_MIDDLE_FRONT_BEGIN:
		m_ePlayerState = PLAYER_DAMAGE_MIDDLE_FRONT_END;
		break;
	case PLAYER_DAMAGE_MIDDLE_BACK_BEGIN:
		m_ePlayerState = PLAYER_DAMAGE_MIDDLE_BACK_END;
		break;
	case PLAYER_TAKE_SNATCH:
		m_ePlayerState = PLAYER_IDLE;
		g_iCameraOption = 0;
		break;
	case PLAYER_ATTACK_1:
	case PLAYER_ATTACK_2:
	case PLAYER_ATTACK_3:
	case PLAYER_ATTACK_STRONG_1:
	case PLAYER_ATTACK_STRONG_4:
	case PLAYER_ATTACK_STRONG_5:
		m_ePlayerState = PLAYER_IDLE;
		m_eMainPlayerState = MAIN_PLAYER_IDLE;
		break;
	case PLAYER_MANA_BLADE:
	case PLAYER_ATTACK_MB_STRONG_1:
	case PLAYER_ATTACK_MB_STRONG_2:
	case PLAYER_ATTACK_MB_STRONG_3:
	case PLAYER_ATTACK_MB_STRONG_4:
	case PLAYER_ATTACK_MB_STRONG_5:
	case PLAYER_ATTACK_MB_STRONG_6:
		m_ePlayerState = PLAYER_IDLE_MB;
		m_eMainPlayerState = MAIN_PLAYER_IDLE_MB;
		break;
	case PLAYER_DARK_KNIGHT_TANSFORMATION_BEGIN_1:
		m_ePlayerState = PLAYER_IDLE;
		break;
	case PLAYER_DARK_KNIGHT_TANSFORMATION_BEGIN_2:
		m_ePlayerState = PLAYER_IDLE;
		break;
	case PLAYER_DARK_KNIGHT_TANSFORMATION_END_1:
		m_ePlayerState = PLAYER_IDLE;
		break;
	}
}


