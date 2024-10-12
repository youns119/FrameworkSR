#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIFloorBase
	: public Engine::CUIUnit
{
public:
	enum class UI_FLOOR
	{
		FLOOR_BACKGROUND,
		FLOOR_WORD1,
		FLOOR_WORD2,
		FLOOR_WORD3,
		FLOOR_WORD4,
		FLOOR_WORD5,
		FLOOR_DIGIT1,
		FLOOR_DIGIT2,
		FLOOR_END,
	};

private:
	explicit CUIFloorBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUIFloorBase();

public:
	static CUIFloorBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

public:
	void Update_Color();

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_FLOOR::FLOOR_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_FLOOR::FLOOR_END];

	_int m_iFloor;
	_float m_fScaleRatio[5];
	_float m_fPosRatio[5];
	_vec3 m_vDir[5];
	_float m_fTimeDelay;
	_float m_fAlpha;
};