#pragma once

#include <array>
#include <concepts>
#include <type_traits>
#include <vector>

#include "WaitHandle.h"

namespace Native
{
	namespace Threading
	{
		class MultiWaitHandle : public WaitHandle
		{
		public:
			
			MultiWaitHandle(const std::shared_ptr<const WaitHandle> waitHandle1, const std::shared_ptr<const WaitHandle> waitHandle2);

			MultiWaitHandle(const std::shared_ptr<const WaitHandle> waitHandle1, const std::shared_ptr<const WaitHandle> waitHandle2, const std::shared_ptr<const WaitHandle> waitHandle3);

			MultiWaitHandle(const std::shared_ptr<const WaitHandle> waitHandle1, const std::shared_ptr<const WaitHandle> waitHandle2, const std::shared_ptr<const WaitHandle> waitHandle3, const std::shared_ptr<const WaitHandle> waitHandle4);

			MultiWaitHandle(const std::shared_ptr<const WaitHandle> waitHandle1, const std::shared_ptr<const WaitHandle> waitHandle2, const std::shared_ptr<const WaitHandle> waitHandle3, const std::shared_ptr<const WaitHandle> waitHandle4, const std::shared_ptr<const WaitHandle> waitHandle5);

			template<typename T = std::iterator_traits<std::shared_ptr<const WaitHandle>>>
			MultiWaitHandle(T count_handles)
				: _handles(BuildHandleVector(count_handles))
			{
			}

		protected:

			virtual uint16_t count_handles() const override;
			virtual uint16_t copy_handles(HandleArray& dest, const uint16_t index) const override;

		private:

			const std::vector<std::shared_ptr<const WaitHandle>> _handles;

			template <typename... Args>
			static std::vector<std::shared_ptr<const WaitHandle>> BuildHandleVector(std::shared_ptr<const WaitHandle> waitHandle, std::convertible_to<std::shared_ptr<const WaitHandle>> auto ... args)
			{
				std::vector<std::shared_ptr<const WaitHandle>> count_handles { waitHandle };

				for (const auto& item : std::array<std::shared_ptr<const WaitHandle>, sizeof...(args)>{std::forward<decltype(args)>(args)...})
					count_handles.push_back(item);

				return count_handles;
			}

			template<typename T = std::iterator_traits<std::shared_ptr<const WaitHandle>>>
			static std::vector<std::shared_ptr<const WaitHandle>> BuildHandleVector(T count_handles)
			{
				std::vector<std::shared_ptr<const WaitHandle>> vec;

				for (const auto& handle : count_handles)
					vec.push_back(handle);

				return vec;
			}
		};
	}
}
