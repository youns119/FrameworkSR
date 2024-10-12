#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define	WINCX		1280 
#define	WINCY		720  
					//1400
					//800

#define	VTXCNTX		129
#define VTXCNTZ		129
#define VTXITV		1

#define TRIGGERCOUNT	64 //Trigger


#define	VTXTILEX		50
#define	VTXTILEZ		300


#define VK_MAX		256 
#define MS_MAX		3

#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif

#define NULL_CHECK( _ptr)	\
		{if( _ptr == 0){_CrtDbgBreak();return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
		{if( _ptr == 0){_CrtDbgBreak();return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
		{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);_CrtDbgBreak();}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
		{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);_CrtDbgBreak();return _return;}}

#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
		{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); _CrtDbgBreak(); return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
		{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); _CrtDbgBreak(); return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
		{ MessageBoxW(NULL, _message, L"System Message",MB_OK); _CrtDbgBreak();return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
		{ MessageBoxW(NULL, _message, L"System Message",MB_OK); _CrtDbgBreak();return _return;}

#define NO_COPY(CLASSNAME)								\
			private:										\
			CLASSNAME(const CLASSNAME&) = delete;					\
			CLASSNAME& operator = (const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)					\
			NO_COPY(CLASSNAME)								\
			private:										\
			static CLASSNAME*	m_pInstance;				\
			public:											\
			static CLASSNAME*	GetInstance( void );		\
			static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
			CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
			CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
				if(NULL == m_pInstance) {					\
					m_pInstance = new CLASSNAME;			\
				}											\
				return m_pInstance;							\
			}												\
			void CLASSNAME::DestroyInstance( void ) {		\
				if(NULL != m_pInstance)	{					\
					delete m_pInstance;						\
					m_pInstance = NULL;						\
				}											\
			}
}

#endif // Engine_Macro_h__