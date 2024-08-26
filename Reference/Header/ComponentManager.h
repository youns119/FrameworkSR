#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CComponentManager
	: public CBase
{
	DECLARE_SINGLETON(CComponentManager)

private :
	CComponentManager();
	virtual ~CComponentManager();

public :
	HRESULT Ready_Proto(const _tchar* _pComponentTag, CComponent* _pComponent);
	CComponent* Clone_Proto(const _tchar* _pComponentTag);

private :
	CComponent* Find_Proto(const _tchar* _pComponentTag);

private :
	virtual void Free();

private :
	map<const _tchar*, CComponent*> m_mapProtoType;
};

END