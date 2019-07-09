#pragma once
#include "KeyVals.h"
#include <string>

using std::string;

enum ExpectState {
	Key,
	Value
};

class VMF
{
private:
	const static string writeChild(KeyVals* kv, unsigned depth = 0);

	unsigned sideID = 0, brushID = 0, entID = 0;
	unsigned nomirror_visID = 0;
	unsigned numSkipped = 0;

	KeyVals root;

	string source;
	string::iterator sourcePos;

	string curToken;
	bool getNextToken();

	void makeQuoteToken();
	void makeToken();

	ExpectState state = Key;

	KeyVals* reflectSolid(KeyVals* kv);
	KeyVals* reflectEntity(KeyVals* kv);
public:
	void Parse(string filepath);
	void writeFile(string filepath);

	void ReflectBrushes();
	void ReflectEntities();

	VMF(string file) {
		Parse(file);
	}
	VMF() {};
	~VMF();
};

