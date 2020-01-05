#include "ListNode.h"

namespace NodeParserNS
{
	ListNode::ListNode(std::string data)
		: _data(data)
	{}

	ListNode::ListNode(int data)
		: _data(std::to_string(data))
	{}

	bool ListNode::GetChild(ListNode** child)
	{
		bool childIsNull = (_child == NULL);
		*child = _child;
		return childIsNull;
	}

	bool ListNode::GetNext(ListNode** next)
	{
		bool nextIsNull = (_next == NULL);
		*next = _next;
		if (!nextIsNull)
		{
			delete this;
		}
		return nextIsNull;
	}

	std::string ListNode::GetData()
	{
		return _data;
	}

	void ListNode::SetNext(ListNode* next)
	{
		_next = next;
	}

	void ListNode::SetChild(ListNode* child)
	{
		_child = child;
	}
}