#include "stdafx.h"
#include "UIObserver.h"
//Client
#include "Include.h"
//Engine
#include "Export_Function.h"


CUIObserver::CUIObserver()
{

}

CUIObserver::~CUIObserver()
{

}

void CUIObserver::Update(int message)
{
	const list<void*>* pList = Engine::Get_InfoSubject()->GetDatalist(message);
	NULL_CHECK(pList);

	switch (message)
	{
	case PLAYER_HP: //enum
		m_iPlayerHp = *((int*)pList->front());
		break;
	case PLAYER_HPMAX:
		m_iPlayerHpMax = *((int*)pList->front());
		break;
	case PLAYER_DAMAGE:
		m_iPlayerDamage = *((int*)pList->front());
		break;
	case MONSTER_HP:
		m_iMonsterHp = *((int*)pList->front());
		break;
	case MONSTER_HPMAX:
		m_iMonsterHpMax = *((int*)pList->front());
		break;
	case MONSTER_DAMAGE:
		m_iMonsterDamage = *((int*)pList->front());
		break;
	}
}

CUIObserver* CUIObserver::Create()
{
	return new CUIObserver;
}
