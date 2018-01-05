#ifndef PRIORITY_QUEUE_H 
#define PRIORITY_QUEUE_H

#include<iostream>
#include<functional>
#include"ApiDefine.h"

using namespace std::tr1;

namespace container {
	template<typename T>
	struct ENGINE_API_TEMPLATE CTwoWayNode
	{
		T value;
		CTwoWayNode* prev;
		CTwoWayNode* next;

		CTwoWayNode(T v) : value(v), prev(NULL), next(NULL) { }
	};

	template<typename T>
	class ENGINE_API_TEMPLATE CPriorityQueue
	{
	public:
		typedef function<int(T, T)> Comparator;
		typedef function<bool(T)> ForeachCallResult;
		typedef function<void(T)> ForeachCall;

		CPriorityQueue() : CPriorityQueue([](T a, T b) {
			if (a > b) return 1;
			else if (a < b) return -1;
			return 0;
		}) {}

		CPriorityQueue(Comparator comparator) : m_comparator(comparator), m_count(0), m_head(NULL), m_trailer(NULL) {}

		void SetComparator(Comparator comparator) { m_comparator = comparator; }

		//�������
		T Enqueue(T value)
		{
			m_count++;
			CTwoWayNode<T>* v_node = new CTwoWayNode<T>(value);
			if (!m_head)
				return (m_head = m_trailer = v_node)->value;

			CTwoWayNode<T>* p = m_head;
			while (p)
			{
				if (m_comparator(v_node->value, p->value) >= 0)
				{
					return Before(p, v_node)->value;
				}
				p = p->next;
			}

			return After(m_head, v_node)->value;
		}

		//���׳���
		T Dequeue()
		{
			if (m_count <= 0)
				return NULL;
			--m_count;
			CTwoWayNode<T>* p = m_head->next;
			delete(m_head);
			return (m_head = p)->value;
		}

		//ɾ��������ָ���ڵ�
		T Remove(T value)
		{
			if (m_count <= 0)
				return value;
			--m_count;
			CTwoWayNode<T>* p = m_head;
			CTwoWayNode<T>* next = NULL;
			CTwoWayNode<T>* prev = NULL;
			while (p)
			{
				if (p->value == value)
				{
					next = p->next;
					prev = p->prev;
					if (p == m_head) m_head = prev ? prev : next;
					if (p == m_trailer) m_trailer = next ? next : prev;
					if (next) next->prev = prev;
					if (prev) prev->next = next;
					delete(p);
					break;
				}
				p = p->next;
			}
			return value;
		}

		//���ض��׵�Ԫ��
		T Head()
		{
			if (m_count <= 0)
				return NULL;
			return m_head->value;
		}

		//���ض�β��Ԫ��
		T Trailer()
		{
			if (m_count <= 0)
				return NULL;
			return m_trailer->value;
		}

		//���е�����
		inline size_t Count()
		{
			return m_count;
		}

		//��ն���
		void Clear()
		{
			if (m_count <= 0)
				return;
			m_count = 0;
			CTwoWayNode<T>* p = m_head;
			CTwoWayNode<T>* next = NULL;
			while (p)
			{
				next = p->next;
				delete(p);
				p = next;
			}
			m_head = NULL;
			m_trailer = NULL;
		}

		//�����������
		void Foreach(ForeachCall call)
		{
			if (m_count <= 0)
				return;
			CTwoWayNode<T>* p = m_head;
			while (p)
			{
				call(p->value);
				p = p->next;
			}
		}

		//�����������
		void ForeachR(ForeachCallResult call)
		{
			if (m_count <= 0)
				return;
			CTwoWayNode<T>* p = m_head;
			while (p)
			{
				if (!call(p->value)) break;
				p = p->next;
			}
		}

		//�����������
		void ForeachInverse(ForeachCall call)
		{
			if (m_count <= 0)
				return;
			CTwoWayNode<T>* p = m_trailer;
			while (p)
			{
				call(p->value);
				p = p->prev;
			}
		}

		//�����������
		void ForeachInverseR(ForeachCallResult call)
		{
			if (m_count <= 0)
				return;
			CTwoWayNode<T>* p = m_trailer;
			while (p)
			{
				if (!call(p->value)) break;
				p = p->prev;
			}
		}

	private:
		//���е�����
		size_t m_count;

		//���׵Ľڵ�
		CTwoWayNode<T>* m_head;

		//��β�Ľڵ�
		CTwoWayNode<T>* m_trailer;

		//�Ƚϴ�С�ĺ���ָ��
		Comparator m_comparator;

		//��b�ŵ�aǰ�棬����b
		CTwoWayNode<T>* Before(CTwoWayNode<T>* a, CTwoWayNode<T>* b)
		{
			b->prev = a->prev;
			if (b->prev) b->prev->next = b;
			b->next = a;
			a->prev = b;
			if (a == m_head) m_head = b;
			return b;
		}

		//��b�ŵ�a���棬����b
		CTwoWayNode<T>* After(CTwoWayNode<T>* a, CTwoWayNode<T>* b)
		{
			b->next = a->next;
			if (b->next) b->next->prev = b;
			b->prev = a;
			a->next = b;
			if (a == m_trailer) m_trailer = b;
			return b;
		}
	};
}
#endif