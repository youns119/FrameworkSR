#pragma once

#include "Component.h"

BEGIN(Engine)

class CAnimation;
class CTexture;

class ENGINE_DLL CAnimator
	: public CComponent
{
private:
	explicit CAnimator();
	explicit CAnimator(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CAnimator(const CAnimator& _rhs);
	virtual ~CAnimator();

public:
	CAnimation* GetCurrAnim() const { return m_pCurrAnimation; }

public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT	Ready_Animator();
	virtual _int Update_Component(const _float& _fTimeDelta);
	void Render_Animator();

public:
	void CreateAnimation
	(
		const _tchar* _pAnimName,
		CTexture* _pTexture,
		float _fAnimSpeed
	);
	CAnimation* FindAnimation(const _tchar* _pAnimName);
	void PlayAnimation(const _tchar* _pAnimName, bool _bAnimRepeat);
	void Toggle_Pause() { m_bAnimPause = !m_bAnimPause; }

public:
	virtual CComponent* Clone();

private:
	virtual void Free();

private:
	map<const _tchar*, CAnimation*> m_mapAnimation;
	CAnimation* m_pCurrAnimation;
	bool m_bAnimRepeat;
	bool m_bAnimPause;


};

END