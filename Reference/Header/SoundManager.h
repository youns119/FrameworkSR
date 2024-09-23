#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CSoundManager
	: public CBase
{
	DECLARE_SINGLETON(CSoundManager)

private:
	explicit CSoundManager();
	virtual ~CSoundManager();

public:
	void Ready_SoundManager();

public:
	void Play_Sound(const _tchar* _pSoundKey, CHANNELID _eID, float _fVolume);
	void Play_BGM(const _tchar* _pSoundKey, float _fVolume);
	void Play_BGMOnce(const _tchar* _pSoundKey, CHANNELID _eID, float _fVolume);
	void Stop_Sound(CHANNELID _eID);
	void Stop_All();
	void Set_ChannelVolume(CHANNELID _eID, float _fVolume);

private:
	void LoadSoundFile();

private:
	virtual void Free();

private:
	map<const _tchar*, FMOD::Sound*> m_mapSound;
	FMOD::Channel* m_pChannelArr[(UINT)CHANNELID::MAXCHANNEL];
	FMOD::System* m_pSystem;
};

END