#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <string>

namespace NodeParserNS
{
	class ListNode
	{
	private:
		const std::string _data;
		ListNode* _child = NULL;
		ListNode* _next = NULL;

	public:
		ListNode(std::string);
		ListNode(int);
		~ListNode() {};

		std::string GetData();
		bool GetChild(ListNode**);
		bool GetNext(ListNode**);

		void SetChild(ListNode*);
		void SetNext(ListNode*);
	};
}

#endif // !LIST_NODE_H