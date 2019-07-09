#include "Plane.h"
#include <iostream>

void Plane::mirror()
{
#if 0
	// Cross product to determine winding
	Vector3 edge1, edge2, normalInit, normalEnd;
	edge1 = verts[1] - verts[0];
	edge2 = verts[2] - verts[0];

	normalInit.x = (edge1.y * edge2.z) - (edge1.z * edge2.y);
	normalInit.y = (edge1.z * edge2.x) - (edge1.x * edge2.z);
	normalInit.z = (edge1.x * edge2.y) - (edge1.y * edge2.x);
#endif

	for (unsigned i = 0; i < 3; ++i) {
		verts[i].x = -verts[i].x;
	}

#if 0
	// Cross product again after reflection
	edge1 = verts[1] - verts[0];
	edge2 = verts[2] - verts[0];

	normalEnd.x = (edge1.y * edge2.z) - (edge1.z * edge2.y);
	normalEnd.y = (edge1.z * edge2.x) - (edge1.x * edge2.z);
	normalEnd.z = (edge1.x * edge2.y) - (edge1.y * edge2.x);
#endif

	//if (normalEnd + normalInit == Vector3(0)) {
		 //Normal got flipped. Reverse winding.
		std::swap(verts[0], verts[2]);
	//}
	//else
		//std::cout << "Normal not flipped; winding retained.\n";

}

string Plane::toString()
{
	return "(" + verts[0].toString() + ") (" + verts[1].toString() + ") (" + verts[2].toString() + ")";
}

Plane::Plane(const string & fromString)
{
	// Expected: (123 -345 103) (456 89 -1) (0 -11 -12)
	if (fromString.size() < 4 || fromString[0] != '(') {
		std::cout << "ERROR: Couldn't parse plane string:\n\t" << fromString << "\n\n";
		return;
	}

	size_t pos = 0;
	for (unsigned i = 0; i < 3; ++i) {
		pos = fromString.find('(', pos) + 1;
		size_t endPos = fromString.find(')', pos);
		verts[i] = Vector3(fromString.substr(pos, endPos - pos));
	}
}

Plane::Plane()
{
}


Plane::~Plane()
{
}

Vector3::Vector3(float magnitude)
{
	x = magnitude;
	y = magnitude;
	z = magnitude;
}

Vector3::Vector3(float nx, float ny, float nz)
{
	x = nx;
	y = ny;
	z = nz;
}

Vector3::Vector3(const string & fromString)
{
	// Expected: 111 222 -333
	size_t pos = fromString.find(' ');
	
	string num = fromString.substr(0, pos);
	x = std::stof(num);

	size_t pos2 = fromString.find(' ', pos + 1);
	num = fromString.substr(pos + 1, pos2 - pos);
	y = std::stof(num);

	num = fromString.substr(pos2 + 1);
	z = std::stof(num);
}
