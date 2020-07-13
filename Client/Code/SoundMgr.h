#pragma once

#include "Include.h"
#include "Engine_Include.h"
#include "Functor.h"

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	FMOD_SYSTEM*				m_pSystem;
	FMOD_CHANNEL*				m_pChannel[CHANNEL_END];

	map<TCHAR*, FMOD_SOUND*>	m_MapSound;

private:
	CSoundMgr(void);
	~CSoundMgr(void);

public:
	void Initialize(void);
	void LoadSoundFile(void);
	void PlaySound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel);
	void PlaySound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, CHANNEL_TYPE eChannel, float fVolume);
	void UpdateSound(void);
	void StopSound(CHANNEL_TYPE eChannel);
	void StopSoundAll(void);
};
