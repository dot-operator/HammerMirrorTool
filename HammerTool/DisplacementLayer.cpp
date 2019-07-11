#include "DisplacementLayer.h"
#include "KeyVals.h"

ScalarDisplacementLayer ScalarDisplacementLayer::rotate()
{
	ScalarDisplacementLayer rotatedLayer(columns[0].size(), columns.size());

	size_t numData = columns[0].size() * columns.size();
	for (size_t i = 0; i < numData; ++i) {
		size_t newX, newY, oldX, oldY;

		newX = i % columns[0].size();
		newY = i / columns[0].size();

		oldY = i % columns.size();
		oldX = i / columns.size();

		rotatedLayer.columns[newY][newX] = columns[oldY][oldX];
	}

	return rotatedLayer;
}

KeyVals * ScalarDisplacementLayer::getNode()
{
	KeyVals* kv = new KeyVals();

	for (int i = 0; i < columns.size(); ++i) {
		auto child = kv->addChild();
		child->setKey("row" + std::to_string(i));
		child->setValString(columns[i].toString());
	}

	return kv;
}

ScalarDisplacementLayer::ScalarDisplacementLayer(KeyVals * source)
{
	for (auto row = source->getFirst(); row; row = source->getNext()) {
		columns.push_back(ScalarDisplacementRow(row->getValString()));
	}
}

ScalarDisplacementLayer::ScalarDisplacementLayer(size_t rows, size_t numcolumns)
{
	for (size_t i = 0; i < numcolumns; ++i) {
		columns.push_back(ScalarDisplacementRow(rows));
	}
}

VectorDisplacementLayer VectorDisplacementLayer::rotate()
{
	VectorDisplacementLayer rotatedLayer(columns[0].size(), columns.size());

	size_t numData = columns[0].size() * columns.size();
	for (size_t i = 0; i < numData; ++i) {
		size_t newX, newY, oldX, oldY;

		newX = i % columns[0].size();
		newY = i / columns[0].size();

		oldY = i % columns.size();
		oldX = i / columns.size();

		rotatedLayer.columns[newY][newX] = columns[oldY][oldX];
	}

	return rotatedLayer;
}

KeyVals * VectorDisplacementLayer::getNode()
{
	KeyVals* kv = new KeyVals();

	for (int i = 0; i < columns.size(); ++i) {
		auto child = kv->addChild();
		child->setKey("row" + std::to_string(i));
		child->setValString(columns[i].toString());
	}

	return kv;
}

VectorDisplacementLayer::VectorDisplacementLayer(size_t rows, size_t numcolumns)
{
	for (size_t i = 0; i < numcolumns; ++i) {
		columns.push_back(VectorDisplacementRow(rows));
	}
}

VectorDisplacementLayer::VectorDisplacementLayer(KeyVals * source)
{
	for (auto row = source->getFirst(); row; row = source->getNext()) {
		columns.push_back(VectorDisplacementRow(row->getValString()));
	}
}
