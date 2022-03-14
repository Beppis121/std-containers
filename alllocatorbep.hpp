#pragma once
#include <memory>
#include <utility>

// full namespace
namespace bep
{

	// Custom Allocator Class in c++20
	template <class T>
	class Allocator
	{
	public: // Type names
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = ptrdiff_t;
		using propagate_on_container_move_assignment = std::true_type;
	public: // constructors
		constexpr Allocator() noexcept {}
		constexpr Allocator(const Allocator&) noexcept = default;
		// constructs an allocator of a different type
		template<class U> constexpr Allocator(const Allocator<U>&) {}
		constexpr ~Allocator() = default;
		// assignment
		constexpr Allocator& operator=(const Allocator&) = default;
	public: // Member Functions
		// allocates based on the count and size of the type
		[[nodiscard]] constexpr T* allocate(size_type count)
		{
			return static_cast<T*>(std::_Allocate<std::_New_alignof<T>>(std::_Get_size_of_n<sizeof(T)>(count)));
		}
		// ptr is the storage ref must be a pointer obtained by an earlier call to allocate()
		constexpr void deallocate(T* ptr, size_type count)
		{
			std::_Deallocate<std::_New_alignof<T>>(ptr, sizeof(T) * count);
		}

	};

}
