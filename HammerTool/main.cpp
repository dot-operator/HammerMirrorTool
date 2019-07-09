#pragma once
#include "VMF.h"
#include <iostream>

int main(int argc, char* argv[]) {
	string filename;
#ifdef _DEBUG
	filename = "koth_databank_b3.vmf";
#endif

#ifndef _DEBUG
	if (argc != 2 ) {
		std::cout << "Usage: vmfreflect [filename]\n";
		return 1;
	}
	else filename = argv[1];
#endif

	VMF vmf(filename);

	vmf.ReflectBrushes();
	vmf.ReflectEntities();

	vmf.writeFile("modified_" + filename);
	return 0;
}