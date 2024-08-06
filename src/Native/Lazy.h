#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <optional>

#include "LazyThreadSafetyMode.h"

namespace Native
{
	template<typename T>
	class Lazy
	{

	public:
		constexpr Lazy() noexcept = default;

		constexpr Lazy(const std::function<T()> valueFactory) noexcept
			: Lazy(valueFactory, Threading::LazyThreadSafetyMode::ExecutionAndPublication)
		{
		}

		constexpr Lazy(const std::function<T()> valueFactory, bool is_thread_safe) noexcept
			: Lazy(valueFactory, is_thread_safe ? Threading::LazyThreadSafetyMode::ExecutionAndPublication : Threading::LazyThreadSafetyMode::None)
		{
		}

		constexpr Lazy(const std::function<T()> valueFactory, const Threading::LazyThreadSafetyMode mode) noexcept
			: _mode(mode),
			_valueFactory(valueFactory)
		{
		}

		virtual ~Lazy() noexcept = default;

		constexpr const T& value() const
		{
			this->init_value();

			return *this->_value;
		}

		constexpr const T* operator->() const
		{
			return &this->value();
		}

		constexpr const T& operator*() const
		{
			return this->value();
		}

	private:

		const Threading::LazyThreadSafetyMode _mode;

		const std::function<T()> _valueFactory;

		mutable std::mutex _mutex;

		mutable std::optional<T> _value;

		constexpr void init_value() const
		{
			switch (this->_mode)
			{
				case Threading::LazyThreadSafetyMode::PublicationOnly:

					this->init_value_publication_thread_safe();
					break;

				case Threading::LazyThreadSafetyMode::ExecutionAndPublication:

					this->init_value_creation_and_publication_thread_safe();
					break;

				default:

					this->init_value_not_thread_safe();
					break;
			}
		}

		constexpr void init_value_not_thread_safe() const
		{
			if (!this->_value.has_value())
				this->_value = this->_valueFactory();
		}

		constexpr void init_value_publication_thread_safe() const
		{
			if (!this->_value.has_value())
			{
				T value = this->_valueFactory();

				{
					std::lock_guard<std::mutex> lock(this->_mutex);

					if (!this->_value.has_value())
						this->_value = std::move(value);
				}
			}
		}

		constexpr void init_value_creation_and_publication_thread_safe() const
		{
			std::lock_guard<std::mutex> lock(this->_mutex);

			if (!this->_value.has_value())
				this->_value = this->_valueFactory();
		}
	};
}