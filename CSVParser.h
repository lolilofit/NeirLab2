#pragma once

#include<string>
#include <fstream>
#include<strstream>
#include<vector>
#include<sstream>
#include"Data.h"


class CSVParser {
public:
	void static readAndStoreData(const std::string& fileName, Data* data);
};
