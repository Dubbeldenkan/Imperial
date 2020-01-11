#include "UtilityClasses.h"

TupleInt::TupleInt()
{}

TupleInt::TupleInt(int xIn, int yIn) :
	std::pair<int, int>(xIn, yIn)
{}

TupleInt::TupleInt(NodeParserNS::ListNode* listNode)
{
	NodeParserNS::ListNode* xPos = NULL;
	NodeParserNS::ListNode* yPos = NULL;

	listNode->GetChild(&xPos);
	listNode->GetNext(&listNode);
	listNode->GetChild(&yPos);
	first = stoi(xPos->GetData());
	second = stoi(yPos->GetData());
}

TupleInt::~TupleInt()
{}

int TupleInt::GetX() const
{
	return first;
}

int TupleInt::GetY() const 
{
	return second;
}

TupleInt TupleInt::operator+(TupleInt tupleIntInput) const
{
	TupleInt tupleIntRes;
	tupleIntRes.first = first + tupleIntInput.first;
	tupleIntRes.second = second + tupleIntInput.second;
	return tupleIntRes;
}

TupleInt TupleInt::operator-(TupleInt tupleIntInput) const
{
	TupleInt tupleIntRes;
	tupleIntRes.first = first - tupleIntInput.first;
	tupleIntRes.second = second - tupleIntInput.second;
	return tupleIntRes;
}

TupleInt TupleInt::operator*(double multiplier) const
{
	TupleInt tupleIntRes;
	tupleIntRes.first = static_cast<int>(first* multiplier);
	tupleIntRes.second = static_cast<int>(second* multiplier);
	return tupleIntRes;
}

TupleInt TupleInt::Division(double denominator) const
{
	TupleInt res;
	res.first = static_cast<int>(first / denominator);
	res.second = static_cast<int>(second / denominator);
	return res;
}

NodeParserNS::ListNode* TupleInt::ToListNode() const
{
	NodeParserNS::ListNode* listNodeXName = new NodeParserNS::ListNode("X");
	NodeParserNS::ListNode* listNodeXData = new NodeParserNS::ListNode(first);
	listNodeXName->SetChild(listNodeXData);

	NodeParserNS::ListNode* listNodeYName = new NodeParserNS::ListNode("Y");
	NodeParserNS::ListNode* listNodeYData = new NodeParserNS::ListNode(second);
	listNodeYName->SetChild(listNodeYData);

	listNodeXName->SetNext(listNodeYName);
	return listNodeXName;
}