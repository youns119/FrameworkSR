#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE { MODE_FULL, MODE_WIN };

	enum class COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum class INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum class ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum class TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum class RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_ORTHOGONAL, RENDER_UI, RENDER_END };

	enum class MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum class MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class KEYSTATE { KEY_PRESS, KEY_HOLD, KEY_RELEASE, KEY_END };

	enum class UIID
	{
		UI_PLUS_LEFT,
		UI_PLUS_CENTER,
		UI_PLUS_HEAL,
		UI_PLUS_COMBO,
		UI_END,
	};
}

#endif // Engine_Enum_h__