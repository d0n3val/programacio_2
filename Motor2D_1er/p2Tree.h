// ----------------------------------------------------
// Generic tree implementation, any number of childs --
// ----------------------------------------------------

#ifndef __P2TREE_H__
#define __P2TREE_H__

#include "p2List.h"

// Tree node -------------------------------------------------------
template<class tdata>
class p2TreeNode
{

public:

	tdata				data;
	p2TreeNode*			parent;
	p2List<p2TreeNode*> childs;

public:

	p2TreeNode(const tdata& _data)
	{
		data = _data;
		parent = NULL;
	}

	void AddChild(p2TreeNode* node)
	{
		ASSERT(node);

		childs.add(node);
		node->parent = this;
	}

	bool RemoveChild(p2TreeNode* node)
	{
		ASSERT(node);
		bool ret = false;

		p2List_item<p2TreeNode*>* item = childs.start;
		for(; item != NULL; item = item->next)
		{
			p2TreeNode* child = item->data;
				
			if(node == child)
			{
				childs.del(item);
				node->parent = NULL;
				ret = true;
				break;
			}
		}

		return ret;
	}

	p2TreeNode<tdata>* FindRecursive(const tdata& node)
	{
		if(node == data)
			return this;

		p2TreeNode<tdata>* result = NULL;
		p2List_item<p2TreeNode*>* item = childs.start;
		for(; item != NULL; item = item->next)
		{
			p2TreeNode* child = item->data;
			result = child->FindRecursive(node);
			
			if(result != NULL)
				break;
		}

		return result;
	}

	void GatherAll(p2List<p2TreeNode*>* list)
	{
		ASSERT(list);
		list->add(this);

		p2List_item<p2TreeNode*>* item = childs.start;

		for(; item != NULL; item = item->next)
		{
			p2TreeNode* child = item->data;
			child->GatherAll(list);
		}
	}

	void GatherAllData(p2List<tdata>* list)
	{
		ASSERT(list);
		list->add(data);

		p2List_item<p2TreeNode*>* item = childs.start;

		for(; item != NULL; item = item->next)
		{
			p2TreeNode* child = item->data;
			child->GatherAllData(list);
		}
	}

};

// Tree class -------------------------------------------------------
template<class tdata>
class p2Tree
{
public:

	// Constructor
	p2Tree(const tdata& _data) : trunk(_data)
	{}

	// Destructor
	virtual ~p2Tree()
	{}

	void Add(const tdata& data, const tdata& parent)
	{
		p2TreeNode<tdata>* p = trunk.FindRecursive(parent);
		p2TreeNode<tdata>* node = new p2TreeNode<tdata>(data);
		p->AddChild(node);
	}

	void Add(const tdata& data)
	{
		p2TreeNode<tdata>* node = new p2TreeNode<tdata>(data);
		trunk.AddChild(node);
	}

	bool Relocate(const tdata& data, const tdata& parent)
	{
		bool ret = false;

		p2TreeNode<tdata>* dad = trunk.FindRecursive(parent);
		p2TreeNode<tdata>* child = trunk.FindRecursive(data);

		if(dad && child && child->parent != dad)
		{
			child->parent->RemoveChild(child);
			dad->AddChild(child);
			ret = true;
		}

		return ret;
	}

	bool DelRecursive(const tdata& data)
	{
		bool ret = false;
		
		p2TreeNode<tdata>* node = trunk.FindRecursive(data);
		
		if(node != NULL)
		{
			p2List<p2TreeNode<tdata>*> results;
			node->GatherAll(&results);

			p2List_item<p2TreeNode<tdata>*>* item = results.start;

			for(; item != NULL; item = item->next)
			{
				p2TreeNode<tdata>* child = item->data;

				if(child->parent)
					child->parent->RemoveChild(child);
			}

			ret = true;
		}

		return ret;
	}

	void Clear()
	{
		p2List<p2TreeNode<tdata>*> results;
		trunk.GatherAll(&results);

		p2List_item<p2TreeNode<tdata>*>* item = results.start;
		for(; item != NULL; item = item->next)
		{
			p2TreeNode<tdata>* child = item->data;
			if(child->parent)
				child->parent->RemoveChild(child);
		}
	}

public:

	p2TreeNode<tdata>	trunk;

};

#endif // __P2TREE_H__