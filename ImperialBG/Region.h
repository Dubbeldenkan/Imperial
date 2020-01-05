#ifndef REGION_H
#define REGION_H

#include "NodeParser.h"

#include <string>

class Region
{
	enum FactoryType { Land, Sea };

private:
	std::string _name;
	FactoryType _factoryType;
	bool _factoryBuilt;

public:
	Region(NodeParserNS::ListNode*);
};

#endif // !REGION_H
