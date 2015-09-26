#pragma once
#include "stdafx.h"

#ifndef OBI_UTILS_H
#define OBI_UTILS_H

namespace OBIUtils {

	// util for reading shaders
	static bool ReadFile(const char* pFileName, string& outFile)
	{
		ifstream f(pFileName);
		if (f.is_open()) {
			string line;
			while (getline(f, line)) {
				outFile.append(line);
				outFile.append("\n");
			}
			f.close();
			return true;
		}
		else {
			return false;
		}
	}




};

#endif