#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CPipeline
{
private :
	explicit CPipeline();
	~CPipeline();

public :
	static void	MakeTransformMatrix
	(
		_matrix* _pOut,
		const _vec3* _pRight,
		const _vec3* _pUp,
		const _vec3* _pLook,
		const _vec3* _pPos
	);
	static void	MakeViewSpaceMatrix
	(
		_matrix* _pOut,
		const _vec3* _pEye,
		const _vec3* _pAt,
		const _vec3* _pUp
	);
	static void	MakeProjMatrix
	(
		_matrix* _pOut,
		const _float& _fFov,
		const _float& _fAspect,
		const _float& _fNear,
		const _float& _fFar
	);
};

END