#ifndef SceneTool_h_
#define SceneTool_h_

#include "Scene.h"
#include "Include.h"

class CSceneTool
	: public Engine::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI };

private:
	CSceneTool(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSceneTool(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CSceneTool* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT	Add_Light(void);
	HRESULT	Add_NaviMesh(void);
	void Release(void);
};

#endif // !SceneTool_h_
