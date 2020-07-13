#ifndef AniSceneSelector_h__
#define AniSceneSelector_h__

#include "SceneTool.h"

class CAniSceneSelector
{
public:
	enum SCENE { SCENE_TOOL, SCENE_END };
public:
	explicit CAniSceneSelector(SCENE eSceneID) : m_eSceneID(eSceneID) {}
	~CAniSceneSelector(void) {}
public:
	HRESULT operator () (Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice)
	{
		switch (m_eSceneID)
		{
		case SCENE_TOOL:
			*ppScene = CSceneTool::Create(pDevice);
			break;

		}

		NULL_CHECK_RETURN(*ppScene, E_FAIL);

		return S_OK;
	}
private:
	SCENE			m_eSceneID;
};


#endif // SceneSelector_h__
