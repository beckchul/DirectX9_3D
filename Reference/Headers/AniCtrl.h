/*!
 * \file AniCtrl.h
 * \date 2017/06/09 15:47
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

#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl
{
public:
	CAniCtrl(void);
	CAniCtrl(const CAniCtrl& rhs);
	~CAniCtrl();

public:
	LPD3DXANIMATIONCONTROLLER* GetAniCtrl(void);
	DOUBLE GetPeriod(void);
	DOUBLE GetTrackPos(void);

public:
	void SetAnimationSet(ANI_INFO tAniInfo);
	void SetTrackPos(DOUBLE dPosition);
	void FrameMove(const float& fTimePerSec);

private:
	void Release(void);

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;

private:
	UINT		m_iMaxAniSet;
	UINT		m_iCurrentTrack;
	UINT		m_iNewTrack;
	DOUBLE		m_dAccTime;
	DOUBLE		m_dPeriod;
};

END

#endif // AniCtrl_h__