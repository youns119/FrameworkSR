#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& _Pointer)
	{
		if (_Pointer != nullptr)
		{
			delete _Pointer;
			_Pointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& _Pointer)
	{
		if (_Pointer != nullptr)
		{
			delete[] _Pointer;
			_Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& _pInstance)
	{
		unsigned long dwRefCnt = 0;

		if (_pInstance != nullptr)
		{
			dwRefCnt = _pInstance->Release();

			if (dwRefCnt == 0)
				_pInstance = NULL;
		}

		return dwRefCnt;
	}

	class CTag_Finder
	{
	public :
		explicit CTag_Finder(const _tchar* _pTag) : m_pTargetTag(_pTag) {}
		~CTag_Finder(void) {}

	public :
		template<typename T> 
		_bool operator () (const T& _pair)
		{
			if (lstrcmpW(m_pTargetTag, _pair.first) == 0)
				return true;
			
			return false;
		}

	private :
		const _tchar* m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public :
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}

	public :
		template <typename T>
		void operator () (T& _pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = _pInstance->Release();

			if (dwRefCnt == 0)
				_pInstance = nullptr;
		}
	};

	class CDeleteMap
	{
	public :
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}

	public :	
		template <typename T>
		void operator () (T& _Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = _Pair.second->Release();

			if (dwRefCnt == 0)
				_Pair.second = NULL;
		}
	};
}

#endif // Engine_Function_h__