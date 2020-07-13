/*!
 * \file MainApp.h
 * \date 2015/04/01 15:08
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

#ifndef MainApp_h__
#define MainApp_h__

#include "Include.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CTimeMgr;
}

class CMainApp
{
private:
	CMainApp(void);

public:
	~CMainApp(void);

public:
	HRESULT InitApp(void);
	void Update(void);
	void Render(void);

public:
	static CMainApp* Create(void);

private:
	void Release(void);

private:
	Engine::CGraphicDev*	m_pGraphicDev;
	Engine::CManagement*	m_pManagement;
	Engine::CTimeMgr*		m_pTimeMgr;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
};

#endif // MainApp_h__