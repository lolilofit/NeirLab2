#pragma once

#include<map> 
#include<string>
#include<list>
#include<map>
#include <vector>


class Data {
public:
    int totalColNum = 0;
    std::vector<std::vector<float>> data;
};


class Attr {
public:
    int count = 0;
    std::map<std::vector<float>, int> resColumns;
};