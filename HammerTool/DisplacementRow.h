#pragma once
#include <sstream>
#include <vector>
#include <iostream>
#include "Plane.h"

// Row structure for DisplacementLayer
// Effectively a vector wrapper with a special constructor
// Not sure if it's good to do it like this anymore.

template<typename T>
class DisplacementRow
{
protected:
	std::vector<T> row;
public:
	T& operator[](size_t pos) {
		if (pos < row.size()) {
			return row[pos];
		}
		else {
			std::cout << "ERROR: Out of bounds access in DisplacementRow::operator[].\n";
			exit(-1);
			return row[0];
		}
	};
	size_t size() const {
		return row.size();
	};

	virtual string toString() = 0;

	DisplacementRow() {};
	~DisplacementRow() {};
};


class VectorDisplacementRow : public DisplacementRow<Vector3>
{
public:
	string toString() override {
		string result;

		for (auto d : row) {
			result += d.toString() + " ";
		}

		result = result.substr(0, result.size() - 1);
		return result;
	};

	VectorDisplacementRow(const string& source) {
		std::stringstream stream(source);
		string curNum, curVec;
		unsigned numSpaces = 0;
		while (std::getline(stream, curNum, ' ')) {
			if (++numSpaces <= 3) {
				curVec += curNum + " ";
			}

			if (numSpaces >= 3) {
				curVec = curVec.substr(0, curVec.size() - 1);
				row.push_back(Vector3(curVec));

				curVec.clear();
				numSpaces = 0;
			}
		}

		if (numSpaces % 3 != 0) {
			std::cout << "ERROR: Displacement data isn't the right size.\n";
			exit(-1);
		}
	};

	VectorDisplacementRow(size_t length) {
		row.reserve(length);

		for(size_t i = 0; i < length; ++i)
			row.push_back(Vector3());
	};
};



class ScalarDisplacementRow : public DisplacementRow<float>
{
public:
	string toString() override {
		string result;

		for (auto d : row) {
			result += std::to_string(d) + " ";
		}

		result = result.substr(0, result.size() - 1);
		return result;
	};

	ScalarDisplacementRow(const string& source) {
		std::stringstream stream(source);
		float f;
		while (stream >> f) {
			row.push_back(f);
			if (stream.peek() == ' ') {
				stream.ignore();
			}
		}
	};

	ScalarDisplacementRow(size_t length) {
		row.reserve(length);

		for (size_t i = 0; i < length; ++i)
			row.push_back(0.f);
	};
};
