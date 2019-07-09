#pragma once
#include "VMF.h"
#include <iostream>

int main(int argc, char* argv[]) {
	string filename;
#ifdef _DEBUG
	filename = "reflectiontest.vmf";
#endif

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];
		if (arg[0] != '-') {
			// Assume it's the filename.
			filename = arg;
		}
	}
#ifndef _DEBUG
	if (argc < 2 || filename.empty()) {
		std::cout << "Usage: vmfreflect [filename]\n";
	}
#endif

	VMF vmf(filename);

	vmf.ReflectBrushes();
	vmf.ReflectEntities();

	vmf.writeFile("modified_" + filename);
	return 0;
}