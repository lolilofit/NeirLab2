//
// Created by User on 11.02.2020.
//
#pragma once

#include <string>
#include <cmath>
#include "Data.h"

class GainRatio {
public:
    float static gainRatio(std::map<float, std::vector<float>> &diffResGist, std::vector<float> &resGist, std::vector<float> &colGist, int n, float h, float hRes);
    float static enthrophy(std::vector<float> &gist, float h, float partSumm);
    float static ig(std::map<float, std::vector<float>> &diffResGist, std::vector<float> &resGist, std::vector<float> &colGist, int n, float h, float hRes);
    float static iv(std::map<float, std::vector<float>> &diffResGist, std::vector<float> &resGist, std::vector<float> &colGist, int n, float h, float hRes);

    float static enthrophy(std::map<std::vector<float>, int> col, int N);
    float static ig(std::map<float, Attr*> column, int N);
    float static iv(const std::map<float, Attr*>& column, int N);
    float static gainRatio(const std::map<float, Attr*>& column, int N);
};
