#include "..\..\Header\SoundManager.h"

IMPLEMENT_SINGLETON(CSoundManager)

CSoundManager::CSoundManager()
{
	ZeroMemory(&m_pChannelArr, sizeof(FMOD::Channel*) * (UINT)CHANNELID::MAXCHANNEL);
	m_pSystem = nullptr;
}

CSoundManager::~CSoundManager()
{
	Free();
}

void CSoundManager::Ready_SoundManager()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(64, FMOD_INIT_NORMAL, nullptr);
	LoadSoundFile();
}

void CSoundManager::Play_Sound(const _tchar* _pSoundKey, CHANNELID _eID, float _fVolume)
{
	map<const _tchar*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[(UINT)_eID]);
	m_pChannelArr[(UINT)_eID]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSoundManager::Play_BGM(const _tchar* _pSoundKey, float _fVolume)
{
	map<const _tchar*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[(UINT)CHANNELID::SOUND_BGM]);
	m_pChannelArr[(UINT)CHANNELID::SOUND_BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[(UINT)CHANNELID::SOUND_BGM]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSoundManager::Play_BGMOnce(const _tchar* _pSoundKey, CHANNELID _eID, float _fVolume)
{
	map<const _tchar*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(_pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	bool isPlaying = false;
	if (m_pChannelArr[(UINT)_eID])
	{
		m_pChannelArr[(UINT)_eID]->isPlaying(&isPlaying);
		if (isPlaying)
		{
			m_pChannelArr[(UINT)_eID]->stop();
		}
	}

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[(UINT)_eID]);
	m_pChannelArr[(UINT)_eID]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSoundManager::Stop_Sound(CHANNELID _eID)
{
	if (m_pChannelArr[(UINT)_eID])
		m_pChannelArr[(UINT)_eID]->stop();
}

void CSoundManager::Stop_All()
{
	for (int i = 0; i < (UINT)CHANNELID::MAXCHANNEL; ++i)
		if (m_pChannelArr[i])
			m_pChannelArr[i]->stop();
}

void CSoundManager::Set_ChannelVolume(CHANNELID _eID, float _fVolume)
{
	if (m_pChannelArr[(UINT)_eID])
		m_pChannelArr[(UINT)_eID]->setVolume(_fVolume);

	m_pSystem->update();
}

void CSoundManager::LoadSoundFile()
{
	_finddata_t fd;

	intptr_t handle = _findfirst("../Bin/Resource/Sound/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Bin/Resource/Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		FMOD::Sound* pSound = nullptr;

		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1;

			_tchar* pSoundKey = new _tchar[iLength];
			ZeroMemory(pSoundKey, sizeof(_tchar) * iLength);

			MultiByteToWideChar(CP_ACP, 0, fd.name, -1, pSoundKey, static_cast<int>(iLength));

			m_mapSound.emplace(pSoundKey, pSound);
		}

		iResult = _findnext(handle, &fd);
	}

	m_pSystem->update();

	_findclose(handle);
}

void CSoundManager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}
	m_mapSound.clear();

	m_pSystem->close();
	m_pSystem->release();
}