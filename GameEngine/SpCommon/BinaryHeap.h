#pragma once

#include"defs.h"
#include"EngineDefine.h"
#include<functional>

using namespace std;

BEGIN_NAMESPACE_ENGINE

/*
* �����
* by liurui 2018-03-05
*/
template<typename T>
class BinaryHeap
{
	//�����һ��Ԫ�ش��ڵڶ���������1��С�ڷ���-1����ȷ���0
	typedef function<int(T, T)> comparator;

	//����Ԫ�صĻص�����
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
		//����ڵ㵽����λ��
		m_Array[m_Size] = element;
		int curIndex = m_Size;
		++m_Size;
		int parentIndex = GetParent(curIndex);
		int result = 0;
		//�븸�ڵ�Ƚϣ��ȸ��ڵ�С�򽻻�
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
		//��������Ԫ��
		t = m_Array[0];
		m_Array[0] = m_Array[m_Size-1];
		--m_Size;
		//��β��Ԫ�طŵ�ջ�����������±Ƚϣ����������򽻻�
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

	//��������Ƿ���Ч
	inline bool CheckAvailableIndex(int index) { return index >= 0 && index < m_Size; }

	//��������������Ӧ������
	inline void Switch(int a, int b)
	{
		T temp = m_Array[a];
		m_Array[a] = m_Array[b];
		m_Array[b] = temp;
	}

	//�Ƚ�
	inline int Compare(int indexA, int indexB)
	{
		return m_Comparator(m_Array[indexA], m_Array[indexB]);
	}

	T*			m_Array;		//����
	size_t		m_Size;			//Ԫ�ظ���
	size_t		m_Capacity;		//����
	comparator	m_Comparator;	//�Ƚ���
	bool		m_SmallHeap;	//С���ѻ��Ǵ����
};

END_NAMESPACE_ENGINE

