#ifndef _GUI_TREE_H_
#define _GUI_TREE_H_

#include"GUIManager.h"
#include"GUIGroup.h"
#include<vector>
#include<functional>

using namespace std;

template<typename T> class CGUITree;

template<typename T> class CGUITreeNode
{
private:
	vector<CGUITreeNode<T>*> m_nodes;
	CGUITree<T>* m_tree;
	CGUITreeNode<T>* m_parent;
	CGUIManager* m_manager;
	CGUIGroup* m_group;
	T m_value;

	void RemoveInParent()
	{
		if (m_parent != NULL)
		{
			for (int i = 0; i < m_parent->m_nodes.size(); ++i)
			{
				if (m_parent->m_nodes[i] == this)
				{
					m_parent->m_nodes.erase(m_parent->m_nodes.begin() + i);
					return;
				}
			}
		}
	}

public:
	CGUITreeNode<T>(CGUITree<T>* tree, CGUIManager* manager, CGUITreeNode<T>* parent, const wstring& name, T value)
	{
		m_tree = tree;
		m_parent = parent;
		m_value = value;
		m_manager = manager;
		m_group = m_manager->Create<CGUIGroup>();
		m_group->SetText(name);
		m_group->AddOnMouseDownListener([this](Vector2 mousePos) {
			m_tree->SelectNode(this);
		});

		if (parent == NULL)
		{
			manager->PutIntoGrid(-1, 0, m_group, false);
		}
		else
		{
			parent->m_group->AddMember(m_group);
		}
	}

	~CGUITreeNode<T>() 
	{
		for (int i = m_nodes.size() - 1; i >= 0; --i)
		{
			delete(m_nodes[i]);
		}
		RemoveInParent();
	}

	void ReleaseGroup()
	{
		m_group->RemoveInParent();
		m_manager->Destroy(m_group);
	}

	CGUITreeNode<T>* AddNode(const wstring& name, T value)
	{
		CGUITreeNode<T>* node = new CGUITreeNode<T>(m_tree, m_manager, this, name, value);
		m_nodes.push_back(node);
		return node;
	}

	CGUITreeNode<T>& operator[](int index)
	{
		return *(m_nodes[index]);
	}

	const T& value()
	{
		return m_value;
	}

	CGUIGroup* group()
	{
		return m_group;
	}

	vector<CGUITreeNode<T>*>& nodes()
	{
		return m_nodes;
	}
};

template<typename T> class CGUITree
{
	typedef function<bool(CGUITreeNode<T>*)> ForeachCallback;
private:
	CGUIManager* m_manager;
	vector<CGUITreeNode<T>*> m_nodes;
	CGUITreeNode<T>* m_lastClickNode;

	bool Foreach(CGUITreeNode<T>* root, ForeachCallback callback)
	{
		if (callback(root)) return true;
		for (CGUITreeNode<T>* node : root->nodes())
		{
			if (Foreach(node, callback)) return true;
		}
	}

public:
	CGUITree<T>() : m_manager(NULL), m_lastClickNode(NULL) {}
	CGUITree<T>(CGUIManager* manager) : m_manager(manager), m_lastClickNode(NULL)
	{
	}

	CGUITreeNode<T>* AddNode(const wstring& name, T value)
	{
		CGUITreeNode<T>* node = new CGUITreeNode<T>(this, m_manager, NULL, name, value);
		m_nodes.push_back(node);
		return node;
	}

	CGUITreeNode<T>* FindNode(T t)
	{
		CGUITreeNode<T>* result = NULL;
		Foreach([&result, &t](CGUITreeNode<T>* node) {
			if (node->value() == t)
			{
				result = node;
				return true;
			}
			return false;
		});
		return result;
	}

	bool DeleteNode(T t)
	{
		return DeleteNode(FindNode(t));
	}

	bool DeleteNode(CGUITreeNode<T>* node)
	{
		if (node == NULL) return false;
		if (m_lastClickNode == node)
			m_lastClickNode = NULL;
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			if (node == m_nodes[i])
			{
				m_nodes.erase(m_nodes.begin() + i);
				break;
			}
		}
		node->ReleaseGroup();
		delete(node);
		return true;
	}

	void Clear()
	{
		for (int i = m_nodes.size() - 1; i >= 0; --i)
		{
			DeleteNode(m_nodes[i]);
		}
		this->m_nodes.clear();
	}

	CGUITreeNode<T>& operator[](int index)
	{
		return *(m_nodes[index]);
	}

	void Foreach(ForeachCallback callback)
	{
		for (CGUITreeNode<T>* node : m_nodes)
		{
			if (Foreach(node, callback)) return;
		}
	}

	void SelectNode(CGUITreeNode<T>* node)
	{
		if (node != m_lastClickNode && m_lastClickNode != NULL)
		{
			m_lastClickNode->group()->SetSelected(false);
		}
		m_lastClickNode = node;
	}

	void SetManager(CGUIManager* manager)
	{
		m_manager = manager;
	}

	CGUITreeNode<T>* GetSelectedNode()
	{
		return m_lastClickNode;
	}
};

#endif