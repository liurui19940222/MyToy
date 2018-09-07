#pragma once

#include"defs.h"
#include"EngineDefine.h"
#include<functional>

using namespace std;

BEGIN_NAMESPACE_ENGINE

/*
* 二叉堆
* by liurui 2018-03-05
*/
template<typename T>
class BinaryHeap
{
	//如果第一个元素大于第二个，返回1，小于返回-1，相等返回0
	typedef function<int(T, T)> comparator;

	//遍历元素的回调函数
	typedef function<void(T)> foreach_call;
public:
	BinaryHeap(int capacity, comparator comp)
	{
		try
		{
			m_Array = new T[capacity];
			m_Capacity = capacity;
			m_Comparator = comp;
			m_Size = 0;
		}
		catch (const std::bad_alloc&)
		{
			fatalError("binary heap alloc is failed!");
		}
	}

	~BinaryHeap()
	{
		delete[] m_Array;
	}

	bool Push(T element)
	{
		if (m_Size >= m_Capacity)
			return false;
		//插入节点到最后的位置
		m_Array[m_Size] = element;
		int curIndex = m_Size;
		++m_Size;
		int parentIndex = GetParent(curIndex);
		int result = 0;
		//与父节点比较，比父节点小则交换
		while (parentIndex != -1)
		{
			if (Compare(curIndex, parentIndex) == -1)
			{
				Switch(parentIndex, curIndex);
				curIndex = parentIndex;
				parentIndex = GetParent(curIndex);
			}
			else break;
		}
		return true;
	}

	bool Pop(T& t)
	{
		if (m_Size == 0)
			return false;
		//弹出顶部元素
		t = m_Array[0];
		m_Array[0] = m_Array[m_Size-1];
		--m_Size;
		//将尾部元素放到栈顶，依次向下比较，满足条件则交换
		int curIndex = 0, leftChild, rightChild;
		while (curIndex != -1)
		{
			leftChild = GetLeft(curIndex);
			rightChild = GetRight(curIndex);
			if (leftChild != -1 && rightChild != -1)
			{
				int r = Compare(leftChild, rightChild) == 1 ? rightChild : leftChild;
				if (Compare(curIndex, r) == 1)
				{
					Switch(curIndex, r);
					curIndex = r;
				}
				else break;
			}
			else if (leftChild != -1 && rightChild == -1)
			{
				if (Compare(curIndex, leftChild) == 1)
				{
					Switch(curIndex, leftChild);
					curIndex = leftChild;
				}
				else break;
			}
			else break;
		};
		return true;
	}

	void Foreach(foreach_call call)
	{
		for (int i = 0; i < m_Size; ++i)
			call(m_Array[i]);
	}

	inline size_t capacity() const { return m_Capacity; }

	inline size_t size() const { return m_Size; }

private:
	int GetLeft(int parentIndex)
	{
		int childIndex = 2 * parentIndex + 1;
		if (!CheckAvailableIndex(childIndex))
			return -1;
		return childIndex;
	}

	int GetRight(int parentIndex)
	{
		int childIndex = 2 * parentIndex + 2;
		if (!CheckAvailableIndex(childIndex))
			return -1;
		return childIndex;
	}

	int GetParent(int childIndex)
	{
		if (childIndex == 0)
			return -1;
		if (!CheckAvailableIndex(childIndex))
			return -1;
		return childIndex % 2 ? ((childIndex - 1) >> 1) : ((childIndex - 2) >> 1);
	}

	//检查索引是否有效
	inline bool CheckAvailableIndex(int index) { return index >= 0 && index < m_Size; }

	//交换两个索引对应的数据
	inline void Switch(int a, int b)
	{
		T temp = m_Array[a];
		m_Array[a] = m_Array[b];
		m_Array[b] = temp;
	}

	//比较
	inline int Compare(int indexA, int indexB)
	{
		return m_Comparator(m_Array[indexA], m_Array[indexB]);
	}

	T*			m_Array;		//数组
	size_t		m_Size;			//元素个数
	size_t		m_Capacity;		//容量
	comparator	m_Comparator;	//比较器
	bool		m_SmallHeap;	//小根堆还是大根堆
};

END_NAMESPACE_ENGINE

