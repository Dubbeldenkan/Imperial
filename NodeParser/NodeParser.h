#ifndef NODE_PARSER_H
#define NODE_PARSER_H

#include "ListNode.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <Windows.h>

namespace NodeParserNS
{
	class NodeParser
	{
	private:
		static const std::string fileType;
	public:
		NodeParser();
		~NodeParser() {};

		static ListNode* ReadDataFile(std::string);
		static ListNode* ReadSaveFile(std::string);

		static void WriteToFile(ListNode*, std::string);

	private:
		static std::string GetLocalPath();
		static ListNode* ReadFile(std::string);

		static void AddNewItem(ListNode*, std::ifstream*);
		static std::string CreateSaveString(ListNode*);
		static std::string AddStringPart(ListNode*, int);
		static std::string AddIndentation(int);
	};
}

#endif // !NODE_PARSER_H