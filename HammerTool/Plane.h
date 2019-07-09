#pragma once
#include <string>

using std::string;


struct Vector3 {
	float x = 0, y = 0, z = 0;
	inline string toString() {
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
	}

	inline Vector3 operator+(const Vector3& other) {
		Vector3 result;
		result.x = other.x + x;
		result.y = other.y + y;
		result.z = other.z + z;
		return result;
	};

	inline Vector3 operator-(const Vector3& other) {
		Vector3 result;
		result.x = other.x - x;
		result.y = other.y - y;
		result.z = other.z - z;
		return result;
	};

	inline bool operator==(const Vector3& other) {
		return ((other.x == x) && (other.y == y) && (other.z == z));
	};

	inline bool operator!=(const Vector3& other) {
		return !((other.x == x) && (other.y == y) && (other.z == z));
	}

	Vector3(float magnitude);
	Vector3(float nx, float ny, float nz);
	Vector3(const string& fromString);
	Vector3() {};
	~Vector3() {};
};

class Plane
{
public:
	Vector3 verts[3];

	void mirror();

	string toString();

	Plane(const string& fromString);
	Plane();
	~Plane();
};

