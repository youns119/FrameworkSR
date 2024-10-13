#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIFloorBoss
	: public Engine::CUIUnit
{
public:
	enum class UI_FLOORBOSS
	{
		FLOORBOSS_DANGER,
		FLOORBOSS_BAR,
		FLOORBOSS_NETWORK,
		FLOORBOSS_TIMELIMIT,
		FLOORBOSS_TIME,
		FLOORBOSS_BALLOON,
		FLOORBOSS_END,
	};

private:
	explicit CUIFloorBoss(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIFloorBoss();

public:
	static CUIFloorBoss* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_FLOORBOSS::FLOORBOSS_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_FLOORBOSS::FLOORBOSS_END];

	_bool m_bTick;
	_float m_fAlpha;
	_float m_fTickTime;
	_float m_fDuration;
	_float m_fWordTime;


	_tchar m_szComment1[128];
	_tchar m_szComment2[128];
	_tchar m_szComment3[128];
	_tchar m_szComment4[128];
	_tchar m_szComment5[128];
	_tchar m_szComment6[128];

	_int m_iWordCount[6];
	_int m_iWordIndex;
};