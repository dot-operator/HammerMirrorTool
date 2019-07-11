#pragma once
#include "VMF.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
	string filename;
#ifdef _DEBUG
	filename = "homme.vmf";
#endif

#ifndef _DEBUG
	if (argc < 2) {
		std::cout << "Usage: vmfreflect [file.vmf]\n";
		return -1;
	}
	
	fs::path path = argv[1];
	filename = argv[1];
	if (fs::is_directory(path)) {
		std::cout << "vmfreflect requires a file, not a folder.\n";
		return -1;
	}
#endif

	VMF vmf(filename);

	vmf.ReflectBrushes();
	vmf.ReflectEntities();

#ifndef _DEBUG
	string name = path.parent_path().string() + "/modified_" + path.filename().string();
	// remove the slash if it's unnecessary.
	if (name[0] == '/')
		name = name.substr(1);
	vmf.writeFile(name);
#else
	vmf.writeFile("modified_" + filename);
#endif
	return 0;
}