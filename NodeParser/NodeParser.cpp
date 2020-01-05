#include "NodeParser.h"

namespace NodeParserNS
{
	const std::string NodeParser::fileType = ".dmd";

	NodeParser::NodeParser()
	{};

	std::string NodeParser::GetLocalPath()
	{
		char tempPath[MAX_PATH];
		GetModuleFileName(NULL, tempPath, MAX_PATH);
		std::string str = std::string(tempPath);
		str.erase(str.rfind('\\'));
		str.erase(str.rfind('\\'));
		return str;
	}

	ListNode* NodeParser::ReadDataFile(std::string fileName)
	{
		std::string folderPath = GetLocalPath() +"\\Data\\";

		std::string filePath = folderPath + fileName + fileType;
		return ReadFile(filePath);
	}

	ListNode* NodeParser::ReadSaveFile(std::string fileName)
	{
		std::string folderPath = GetLocalPath() + "\\SavedGames\\";

		std::string filePath = folderPath + fileName + fileType;
		return ReadFile(filePath);
	}

	ListNode* NodeParser::ReadFile(std::string filePath)
	{

		std::ifstream file;
		file.open(filePath);

		ListNode* mainNode = new ListNode("");
		ListNode* latestNode = mainNode;

		if (file.is_open())
		{
			AddNewItem(latestNode, &file);
			mainNode->GetNext(&mainNode);
			file.close();
		}
		else
		{
			OutputDebugString(std::string("unable to open file " + filePath).c_str());
		}
		return mainNode;
	}

	void NodeParser::AddNewItem(ListNode* currentNode, std::ifstream* file)
	{
		std::string line;
		while (std::getline(*file, line))
		{
			line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
			line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
			if (line.find("//") == 0 || line.empty())
			{
				//This is a comment or an empty line
			}
			else if (line.find("{") != -1)
			{
				if (line.find("}") == -1)
				{
					ListNode* childNode = new ListNode("");
					AddNewItem(childNode, file);
					childNode->GetNext(&childNode);
					currentNode->SetChild(childNode);
				}
			}
			else if (line.find("}") != -1)
			{
				break;
			}
			else
			{
				//line.erase(std::remove(line.begin(), line.end(), ' '), line.end()); //TODO gör så att man tar bort alla mellanslag före och efter texten men inte mellan 
				ListNode* tempNode = new ListNode(line);
				currentNode->SetNext(tempNode);
				currentNode = tempNode;
				OutputDebugString((line + "\n").c_str());
			}
		}
	}

	void NodeParser::WriteToFile(ListNode* listNode, std::string fileName)
	{
		char tempPath[MAX_PATH];
		GetModuleFileName(NULL, tempPath, MAX_PATH);
		std::string str = std::string(tempPath);
		str.erase(str.rfind('\\'));
		str.erase(str.rfind('\\'));
		std::string folderPath = str + "\\SavedGames\\";
		std::string filePath = folderPath + fileName + fileType;

		std::fstream file;
		file.open(filePath, std::fstream::in | std::fstream::app);

		if (file.is_open())
		{
			std::string saveString = CreateSaveString(listNode);
			file << saveString;
			file.close();
		}
		else
		{
			OutputDebugString(std::string("unable to open file " + filePath).c_str());
		}
	}

	std::string NodeParser::CreateSaveString(ListNode* listNode)
	{
		std::string saveString = AddStringPart(listNode, 0);

		return saveString;
	}

	std::string NodeParser::AddStringPart(ListNode* listNode, int indentCount) //TODO kan man lösa indentCount på bättre sätt?
	{
		std::string saveString = AddIndentation(indentCount);
		saveString += listNode->GetData();
		saveString += "\n";
		ListNode* childNode = NULL;
		if (!listNode->GetChild(&childNode))
		{
			saveString += AddIndentation(indentCount);
			saveString += "{\n";
			saveString += AddStringPart(childNode, indentCount + 1);
			saveString += AddIndentation(indentCount);
			saveString += "}\n";
		}
		ListNode* nextNode = NULL;
		if (!listNode->GetNext(&nextNode))
		{
			saveString += AddStringPart(nextNode, indentCount);
		}

		return saveString;
	}

	std::string NodeParser::AddIndentation(int indentCount)
	{
		std::string result = "";
		for (int counter = 0; counter < indentCount; counter++)
		{
			result += "\t";
		}
		return result;
	}
}