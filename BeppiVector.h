#pragma once
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <memory>
#include <compare>
// Allocator Def
/*The std::allocator class template is the default Allocator used
by all standard library containers if no
user-specified allocator is provided. 
The default allocator is stateless, that is, 
all instances of the given allocator are interchangeable,
compare equal and can deallocate memory 
allocated by any other instance of the same allocator type.
*/


// Foward Dec Beppi Iterator
template <class V> class BeppiIterator;

// Uses Allocator
template <class T/*, class Alloc = std::allocator<T>*/>
class BeppiVector
{
public: // Typedefs
	typedef BeppiIterator<T> iterator;
private:
	//std::allocator<T> Alloc;
	// Pointer to first element in Vector
	T* m_vec = 0;
	// Number of elements in vector
	std::size_t m_size = 0; 
	// Space taken by vector
	std::size_t m_space = 0;
public: // Class Constructors and destructors (there are a lot) 
	// Default
	BeppiVector() = default;
	// Vector given size
	explicit BeppiVector(int size) : m_size(size), m_space(size)
	{
		m_vec(new T[m_size]);
		
	}
	// Copy Constructor
	BeppiVector(const BeppiVector& bvec) : m_size(bvec.m_size), m_space(bvec.m_space)
	{
		// Allocate new new elements to the size of the previous vector
		m_vec(new T[bvec.m_size]);
		// copy the elements over
		for (std::size_t ind = 0; ind < bvec.m_size; ind++)
		{
			m_vec[ind] = bvec.m_vec[ind];
		}
	}
	BeppiVector(std::initializer_list<T> list) : BeppiVector()
	{
		for (T element : list)
		{
			push_back(element);
		}
	}
	// Destructor
	~BeppiVector()
	{
		delete[] m_vec;
	}
public: // operator overloads
	BeppiVector& operator=(const BeppiVector<T>& rhs)
	{
		// To Optimize just incase someone wanted to put to things of the same together
		if (this == &rhs)
			return *this;
		// If Enough space
		if (rhs.m_size <= m_space)
		{
			for (std::size_t ind = 0; ind < rhs.m_size; ind++)
			{
				m_vec[ind] = rhs.m_vec[ind];

			}
			m_size = rhs.m_size;
			return *this;
		}
		// Same as the reserve function
		T* tmp_vec = new T[rhs.m_size];
		// Move the data over
		for (std::size_t ind = 0; ind < m_size; ind++)
		{
			tmp_vec[ind] = m_vec[ind];
		}
		// Deallocate existing data
		delete[] m_vec;
		m_vec = tmp_vec;
		m_space = rhs.m_size;
	}
	T& operator[](int index)
	{
		return m_vec[index];
	}

public: // Functions
	// Increase the size of the vector the new capcity given
	void reserve(int new_cap)
	{
		if (new_cap <= m_size)
			return;
		// Create tmp vector to hold data while transfer data 
		T* tmp_vec = new T[new_cap];
		// Move the data over
		for (std::size_t ind = 0; ind < m_size; ind++)
		{
			tmp_vec[ind] = m_vec[ind];
		}
		// Deallocate existing data
		delete[] m_vec;
		m_vec = tmp_vec;
		m_space = new_cap;
	}
	void push_back(const T& elm)
	{
		// Give more space
		if (m_space == 0)
		{
			reserve(16);
		}
		else if (m_size == m_space)
		{
			reserve(m_space + 16);
		}
		m_vec[m_size] = elm;
		// M_size is now the next
		++m_size;
		// Just in case new size is at the limit
		if (m_size == m_space)
		{
			reserve(m_space + 16);
		}
	}
	std::size_t capacity()
	{
		return m_space;
	}
	std::size_t size()
	{
		return m_size;
	}
	void clear()
	{
		// Delete Current Vector
		delete[] m_vec;
		// so the capacity stays the same
		T* tmp_vec = new T[m_space];
		m_size = 0;
		m_vec = tmp_vec;
		//delete[] tmp_vec;
	}
	void pop_back()
	{

		//m_vec[m_size] = nullptr;
		m_size--;
		T* tmp_vec = new T[m_space];
		for (std::size_t ind = 0; ind < m_size; ind++)
		{
			tmp_vec[ind] = m_vec[ind];
		}
		delete[] m_vec;
		m_vec = tmp_vec;
	}
	void resize(int count, T value)
	{
		// make space just in case
		if (count >= m_space)
		{
			reserve((count + m_space) + 16);
		}
		if (count == m_size)
			return;
		if (count < m_size)
		{
			while (m_size != count)
			{
				pop_back();
				m_size--;
			}
			return;

		}
		
		//int fillin = count - m_size;
		// Push back every value 
		while (m_size < count)
		{
			push_back(value);
		}
	}
	iterator begin()
	{
		return iterator(*this, 0);
	}
	iterator end()
	{
		return iterator(*this, m_size);
	}
public: // Friends
	friend class BeppiIterator<T>;

};

template <class V>
class BeppiIterator
{
private:
	// pointer to element in vector
	V* m_ptr;
	int m_position;
	BeppiVector<V>& m_ref;
public: // Class Constructors
	// Default
	explicit BeppiIterator() : m_ptr(nullptr) {}
	// Constructor with element given
	//explicit BeppiIterator(V* ptr, int position) : m_ptr(ptr), m_position(position) {}
	explicit BeppiIterator(BeppiVector<V>& ptr, int position) : m_ref(ptr), m_position(position) { }
public: // Operators
	// ++ Operator
	BeppiIterator& operator++()
	{
		m_position++;
		return *this;
	}
	// Increment ++iterator 
	BeppiIterator operator++(int)
	{
		BeppiIterator tmp = *this;
		// Increment Iterator
		++* this;
		return tmp;
	}
	// -- Operator
	BeppiIterator& operator--()
	{
		m_position--;
		return *this;
	}
	// Increment ++iterator 
	BeppiIterator operator--(int)
	{
		BeppiIterator tmp = *this;
		// Increment Iterator
		--*this;
		return tmp;
	}
	// Derefrence pointer
	V& operator*()
	{
		return m_ref[m_position];
	}
	// check if equal
	bool operator==(const BeppiIterator& rhs)
	{
		return m_position == rhs.m_position;
	}
	bool operator!=(const BeppiIterator& rhs)
	{
		return m_position != rhs.m_position;
	}

};