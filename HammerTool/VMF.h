#pragma once
#include "KeyVals.h"
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

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
	unsigned numSkipped = 0, numIOReflected = 0;

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
	static KeyVals* copyRecursive(KeyVals* kv);

	unordered_map<string, string> reflectedNames; // old name, new name
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

