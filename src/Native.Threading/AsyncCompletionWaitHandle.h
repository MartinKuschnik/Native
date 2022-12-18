#pragma once

#include "ManualResetEvent.h"

namespace Native
{
	namespace Threading
	{
		template<typename T>
		requires std::is_default_constructible<T>::value
		class AsyncCompletionWaitHandle : public WaitHandle
		{
		private:

			ManualResetEvent _manualResetEvent;

			T _result;

		public:

			AsyncCompletionWaitHandle()
				: _manualResetEvent(false),
				_result()
			{
					
			}

			void set(const T& result)
			{
				this->_result = result;

				this->_manualResetEvent.set();
			}

			void set(T&& result)
			{
				this->_result = std::move(result);

				this->_manualResetEvent.set();
			}

			const T* result() const
			{
				return &this->_result;
			}

		protected:

			virtual uint16_t count_handles() const override
			{
				return WaitHandle::CountHandles(this->_manualResetEvent);
			}

			virtual uint16_t copy_handles(HandleArray& dest, const uint16_t index) const override
			{
				return WaitHandle::CopyHandles(this->_manualResetEvent, dest, index);
			}

		};
	}
}

