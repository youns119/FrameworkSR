#pragma once

#include "../../Base/Base.h"

class CMainApp : public CBase
{
private :
	explicit CMainApp();
	virtual ~CMainApp();

public :
	HRESULT	Ready_MainApp();
	int	Update_MainApp(const float& fTimeDelta);
	void LateUpdate_MainApp();
	void Render_MainApp();

protected :
private :

public :
	static CMainApp* Create();

private :
	virtual void Free();
};