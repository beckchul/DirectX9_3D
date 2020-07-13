#ifndef UIObserver_h__
#define UIObserver_h__

#include "Observer.h"

class CUIObserver
	:public Engine::CObserver
{
private:
	CUIObserver();
public:
	virtual ~CUIObserver();

	virtual void Update(int message);

	static CUIObserver* Create();
public:
	//Get ÇÔ¼ö
	const int*	 GetPlayerHp() { return &m_iPlayerHp; } // 1p m_iPlayerHp
	const int*	 GetPlayerHpMax() { return &m_iPlayerHpMax; } // 2p m_iPlayerHpMax
	const int*   GetPlayerDamage() { return &m_iPlayerDamage; } // 1p m_iPlayerDamage
	const int*   GetMonsterHp() { return &m_iMonsterHp; } // 2p m_iMonsterHp
	const int*   GetMonsterHpMax() { return &m_iMonsterHpMax; } // 1p m_iMonsterHpMax
	const int*   GetMonsterDamage() { return &m_iMonsterDamage; } // 2p m_iMonsterDamage


private:
	int m_iPlayerHp, m_iPlayerHpMax, m_iPlayerDamage;
	int m_iMonsterHp, m_iMonsterHpMax, m_iMonsterDamage;
};

#endif // UIObserver_h__
