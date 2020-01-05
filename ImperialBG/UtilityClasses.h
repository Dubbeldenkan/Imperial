#ifndef UTILITY_CLASSES_H
#define UTILITY_CLASSES_H

#include "NodeParser.h"

#include <utility>

class TupleInt : public std::pair<int, int>
{

public:
	TupleInt();
	TupleInt(int, int);
	TupleInt(NodeParserNS::ListNode*);

	~TupleInt();

	int GetX() const;
	int GetY() const;

	TupleInt operator+(TupleInt) const;
	TupleInt operator-(TupleInt) const;
	TupleInt operator*(double) const;
	// TODO använd operator/ istället
	TupleInt Division(double) const;

	NodeParserNS::ListNode* ToListNode() const;
};

#endif // !UTILITY_CLASSES_H