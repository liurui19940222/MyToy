#pragma once
#include<assert.h>

namespace my_collection
{
#define INIT_CAPACITY 4

	template<typename T>
	class ArrayList
	{
	public:
		ArrayList() : m_Array(NULL), m_Size(0), m_Capacity(0) {}

		ArrayList(size_t capacity) : m_Array(NULL), m_Size(0)
		{
			m_Array = Alloc(capacity);
			if (m_Array)
				m_Capacity = capacity;
		}

		~ArrayList()
		{
			if (m_Array)
			{
				delete[] m_Array;
				m_Array = NULL;
			}
		}

		void PushBack(T element)
		{
			if (m_Size >= m_Capacity)
				Realloc();
			m_Array[m_Size++] = element;
		}

		void Insert(int index, T element)
		{
			if (index >= m_Capacity)
				Realloc();
			if (index != m_Size)
				memcpy(m_Array + index + 1, m_Array + index, (m_Size - index) * sizeof(T));
			m_Array[index] = element;
			++m_Size;
		}

		void Erase(T element)
		{
			for (int i = 0; i < m_Size; ++i)
			{
				if (m_Array[i] == element)
				{
					if (i != m_Size)
						memcpy(m_Array + i, m_Array + i + 1, (m_Size - i) * sizeof(T));
					--m_Size;
					break;
				}
			}
		}

		void Clear()
		{
			m_Size = 0;
		}

		void Foreach(bool(*callback)(T& t))
		{
			for (int i = 0; i < m_Size; ++i)
			{
				if (callback(m_Array[i])) break;
			}
		}

		T& operator[](int index) const
		{
			return m_Array[index];
		}

		inline size_t size() const { return m_Size; }
		inline size_t capacity() const { return m_Capacity; }

	private:
		T* Alloc(int capacity)
		{
			try
			{
				T* arr = new T[capacity];
				return arr;
			}
			catch (const bad_alloc& e)
			{
				assert(0);
			}
		}

		void Realloc()
		{
			if (m_Array)
			{
				T* temp = Alloc(m_Capacity * 2);
				if (temp)
				{
					memcpy(temp, m_Array, sizeof(T) * m_Size);
					delete[] m_Array;
					m_Array = temp;
					m_Capacity *= 2;
				}
			}
			else
			{
				m_Array = Alloc(INIT_CAPACITY);
				if (m_Array)
					m_Capacity = INIT_CAPACITY;
			}
		}

		T*			m_Array;
		size_t		m_Size;
		size_t		m_Capacity;
	};

}