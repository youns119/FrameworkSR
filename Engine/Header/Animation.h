#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CAnimator;
class CTexture;

class ENGINE_DLL CAnimation
	: public CBase
{
private:
	explicit CAnimation(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CAnimation();

public:
	CTexture* GetTexture() const { return m_pTexture; }
	_float GetCurrFrame() const { return m_fCurrFrame; }
	_float GetAnimSpeed() const { return m_fAnimSpeed; }
	_bool GetFinish() const { return m_bAnimFinish; }

	void SetAnimSpeed(const _float _fAnimSpeed) { m_fAnimSpeed = _fAnimSpeed; }
	void SetFinish(const _bool _bAnimFinish) { m_bAnimFinish = _bAnimFinish; }

public:
	static CAnimation* Create(LPDIRECT3DDEVICE9 _pGraphicDev, CTexture* _pTexture, _float _fAnimSpeed);

public:
	HRESULT Ready_Animation(CTexture* _pTexture, _float _fAnimSpeed);
	void Update_Animation(const _float& _fTimeDelta);

public:
	void ResetAnimFrame(const _float _fCurrFrame)
	{
		m_bAnimFinish = false;
		m_fCurrFrame = _fCurrFrame;
	}

private:
	virtual void Free();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	CTexture* m_pTexture;

	_float m_fCurrFrame;
	_float m_fAnimSpeed;
	_bool m_bAnimFinish;
};

END