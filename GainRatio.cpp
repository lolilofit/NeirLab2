#include <cmath>
#include <iostream>
#include "GainRatio.h"

float GainRatio::enthrophy(std::vector<float> &gist, float h, float partSumm) {
    float sum = 0.0;
    float gistSize = (float)gist.size();
    //one column case??
    //if(h == 0.0)
    //    return 0;

    for(int i = 0; i < gist.size(); i++) {
        if(gist[i] != 0.0) {
            float p = gist[i]/partSumm;
            float v1 = p*std::log(p);
            float v2 =  std::log(2);
            sum += (-1.0f)*(v1 / v2);
        }
    }
    return sum;
}

float GainRatio::ig(std::map<float, std::vector<float>> &diffResGist, std::vector<float> &resGist, std::vector<float> &colGist, int n, float h, float hRes) {

    float summ = 0.0, totalSumm = 0.0;
    for(auto el : colGist)
        totalSumm += el;

    for(auto line : diffResGist) {
        float partSumm = 0.0;
        for(auto el : line.second)
            partSumm += el;
        summ += (partSumm/totalSumm) * GainRatio::enthrophy(line.second, line.first, partSumm);
    }
    float result = GainRatio::enthrophy(resGist, hRes, totalSumm) - summ;
    return result;
}

float GainRatio::iv(std::map<float, std::vector<float>> &diffResGist, std::vector<float> &resGist, std::vector<float> &colGist, int n, float h, float hRes) {
    float result = 0.0;
    for(auto line : diffResGist) {
        float partSumm = 0.0, totalSumm = 0.0;
        for(auto el : line.second)
            partSumm += el;
        for(auto el : colGist)
            totalSumm += el;
        if(partSumm != 0.0) {
            float p = partSumm / totalSumm;
            result += (-1.0f) * (p) * (log(p) / log(2));
        }
    }
    return result;
}

float GainRatio::gainRatio(std::map<float, std::vector<float>> &diffResGist, std::vector<float> &resGist, std::vector<float> &colGist, int n, float h, float hRes) {
    float f = GainRatio::ig(diffResGist, resGist, colGist, n, h, hRes);
    if(f == 0)
        return  0.0;
    float result = f/GainRatio::iv(diffResGist, resGist, colGist, n, h, hRes);
    return result;
}

float GainRatio::enthrophy(std::map<std::vector<float>, int> col, int N) {
    float sum = 0.0;
    for(const auto& el : col) {
        if(el.second != 0) {
            float p = (float)el.second/(float)N;
            float v1 = p*std::log(p);
            float v2 =  std::log(2);
            sum += (-1.0f)*(v1 / v2);
        }
    }
    return sum;
}


float GainRatio::ig(std::map<float, Attr*> column, int N) {
    float summ = 0.0, totalSumm = 0.0;
    std::map<std::vector<float>, int> totalRes;

    for(auto sign : column) {
        summ += (sign.second->count/(float)N) * GainRatio::enthrophy(sign.second->resColumns, sign.second->count);
        for(const auto& resVal : sign.second->resColumns) {
            auto found = totalRes.find(resVal.first);
            if(found == totalRes.end())
                totalRes.insert(resVal);
            else
                found->second += resVal.second;
        }
    }
    float result = GainRatio::enthrophy(totalRes, N) - summ;
    return result;
}

float GainRatio::iv(const std::map<float, Attr*>& column, int N) {
    float result = 0.0;
    for(auto sign : column) {
        float p = sign.second->count / (float)N;
        result += (-1.0f) * (p) * (log(p) / log(2));
    }
    return result;
}

float GainRatio::gainRatio(const std::map<float, Attr*>& column, int N) {
    float f = GainRatio::ig(column, N);
    if(f == 0)
        return  0.0;
    float result = f/GainRatio::iv(column, N);
    return result;
}