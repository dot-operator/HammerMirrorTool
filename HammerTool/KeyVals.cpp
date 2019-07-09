#include "KeyVals.h"
#include <iostream>


KeyVals* KeyVals::addChild()
{
	children.push_back(std::make_unique<KeyVals>());
	children.back()->parent = this;
	return children.back().get();
}

void KeyVals::addChildExisting(KeyVals * kv)
{
	if(kv)
		children.push_back(std::unique_ptr<KeyVals>(kv));
#ifdef _DEBUG
	else std::cout << "Skipping nullptr child entity, probably from no_mirror.\n";
#endif
}

KeyVals* KeyVals::addChildTerminal(const string & key, const string & val)
{
	children.push_back(std::make_unique<KeyVals>(key, val));
	children.back()->parent = this;
	return children.back().get();
}

void KeyVals::setValString(const string & v)
{
	/*if (!value.empty())
		std::cout << "Parse ERROR: Tried to replace existing string value.\n";*/
	if (children.empty())
		value = v;
	else 
		std::cout << "Parse ERROR: Tried to add string value to node with children. Skipping.\n";
}

KeyVals::KeyVals()
{
	it = children.begin();
}

KeyVals::KeyVals(string k, string v)
{
	key = k;
	value = v;
	it = children.begin();
}


KeyVals::~KeyVals()
{
}
