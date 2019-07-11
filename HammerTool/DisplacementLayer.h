#pragma once
#include "DisplacementRow.h"

class KeyVals;

// Didn't make a template because 
// there's just these two specializations,
// and they both work differently.

// This represents what's basically a table
// like what can be found in the "dispinfo" VMF node.
// To reflect it on an axis, we'll need to
// flip the table on its side (weird, I know)
// ---hence the rotate function---
class ScalarDisplacementLayer
{
private:
	std::vector<ScalarDisplacementRow> columns;
public:
	ScalarDisplacementLayer rotate();
	KeyVals* getNode();

	ScalarDisplacementLayer(KeyVals* source);
	ScalarDisplacementLayer(size_t rows, size_t numcolumns);
	~ScalarDisplacementLayer() {};
};


class VectorDisplacementLayer {
private:
	std::vector<VectorDisplacementRow> columns;
public:
	VectorDisplacementLayer rotate();
	KeyVals* getNode();

	VectorDisplacementLayer(size_t rows, size_t numcolumns);
	VectorDisplacementLayer(KeyVals* source);
	~VectorDisplacementLayer() {};
};
