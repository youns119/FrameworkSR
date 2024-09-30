#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CEffect;

END

class CEffectMinigunShell : public CGameObject
{
private:
	enum Obj
	{
		CAPSULE,
		BULLET,

		OBJ_END
	};

private:
	explicit CEffectMinigunShell(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectMinigunShell();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectMinigunShell* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();

public:
	static void OnOperate(void* _pParam);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom[OBJ_END];
	Engine::CTexture* m_pTextureCom[OBJ_END];
	Engine::CEffect* m_pEffectCom;

	_uint m_iTotalFrame[OBJ_END];
	_uint m_iCurFrame[OBJ_END];

	_vec3 m_vVelocity[OBJ_END];
	_vec3 m_vVelocityNoise[OBJ_END];

	_uint m_iRandomSpriteOffset;

	// 지금 이 클래스는 "Com_Transform" 이라는 컴포넌트 태그를 사용하지 않는다.
	// 그래서 생기는 문제로 obj pool 을 사용하게 되면 transform 컴포넌트를 찾지 못한다.
	// 다음부터는 부모 - 자식 간의 transform 컴포넌트를 이용하듯이 현재 클래스에 Com_Transform 태그를 가지는 컴포넌트 생성 후
	// 여기에 오프셋 처럼 가지고 있어야 겠다? 기준점 같은 역할을 해줄것이다.
	// Render 함수에서 (오프셋 월드 행렬 * 기준 월드 행렬) 로 오프셋의 월드 위치 얻어올 수 있을 것이다.
	// 이 클래스에서는 시작 위치가 기준 transform 컴포넌트가 될 것이다.
};

