/*!
 * \file Stage.h
 * \date 2015/04/03 0:13
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 스테이지
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

class CStage
	: public Engine::CScene
{
public:
	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI};

private:
	CStage(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStage(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT	Add_Light(void);
	HRESULT	Add_NaviMesh(void);
	void Release(void);
};

#endif // Stage_h__