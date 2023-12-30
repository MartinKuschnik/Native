#pragma once

#include <type_traits>

#include <comdef.h>

#include <HResultException.h>

namespace Native
{
	namespace Windows
	{
		namespace COM
		{
			template<typename T>
				requires std::is_base_of<IUnknown, T>::value
			class ComObject
			{
			private:

				T* _ptr;

			public:

				ComObject() noexcept
					: _ptr(nullptr)
				{
				}

				ComObject(const T* ptr) noexcept
					: _ptr(ptr)
				{
				}

				ComObject(const ComObject&) = delete;

				ComObject(ComObject&& other)
					: _ptr(other._ptr)
				{
					other._ptr = nullptr;
				}

				~ComObject() noexcept
				{
					if (this->_ptr != nullptr)
						this->_ptr->Release();
				}

				constexpr T* operator->() noexcept
				{
					return this->_ptr;
				}

				constexpr const T* operator->() const noexcept
				{
					return this->_ptr;
				}

				constexpr operator T* () noexcept
				{
					return this->_ptr;
				}

				constexpr operator T** () noexcept
				{
					return &this->_ptr;
				}

				constexpr operator void**() noexcept
				{
					return reinterpret_cast<void**>(&this->_ptr);
				}

				void SetProxyBlanket(DWORD dwAuthnSvc, DWORD dwAuthzSvc, OLECHAR* pServerPrincName, DWORD dwAuthnLevel, DWORD dwImpLevel, RPC_AUTH_IDENTITY_HANDLE pAuthInfo, DWORD dwCapabilities)
				{
					const HRESULT result = ::CoSetProxyBlanket(this->_ptr, dwAuthnSvc, dwAuthzSvc, pServerPrincName, dwAuthnLevel, dwImpLevel, pAuthInfo, dwCapabilities);

					if (FAILED(result))
						throw HResultException(result, nameof(CoSetProxyBlanket));
				}
			};
		}
	}
}

