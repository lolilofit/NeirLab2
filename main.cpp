#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include "Data.h"
#include "CSVParser.h"
#include "GainRatio.h"


//startline = 3
int findMaxMin(Data* data, float &min, float &max, int startLine, int columnNumber) {
    max = 0;
    int i = startLine;
    float well = data->data[startLine][0];
    while(data->data[i][0] == well && data->data[i][columnNumber] < 0) {
        i++;
    }
    min = data->data[i][columnNumber];
    int line, returnLine = data->data.size();

    for(line = startLine; line < data->data.size(); line++) {
        bool shouldCheck = false;
        if(data->data[line].size() != data->totalColNum) {
            shouldCheck = true;
        }
        else {
            if(data->data[line][data->data[line].size() - 3] == -1.0)
                shouldCheck = true;
        }

        if(!(data->data[line][data->data[line].size() - 1] == -1.0 && data->data[line][data->data[line].size() - 2] == -1.0 && shouldCheck)) {
            if (well == data->data[line][0]) {
                float val = data->data[line][columnNumber];
                if (min > val)
                    min = val;
                if (max < val)
                    max = val;
            } else {
                returnLine = line;
                line = data->data.size();
            }
        }
    }
    return returnLine;
}

int sterdFormula(int n) {
    return 1 + (int)(3.22 * log(n));
}

float interval(float min, float max, int n) { return (max-min)/((float)n);}

int findNonEmptyRes(Data* data, int startLine) {
    int notEmptyAnswer = -1;
    if(data->data[startLine].size() != data->totalColNum) {
        if(data->data[startLine][data->data[startLine].size() - 1] > 0)
            notEmptyAnswer = data->data[startLine].size() - 1;
        else {
            if(data->data[startLine][data->data[startLine].size() - 2] > 0)
                notEmptyAnswer = data->data[startLine].size() - 2;
        }
    } else {
        if(data->data[startLine][data->data[startLine].size() - 1] > 0)
            notEmptyAnswer = data->data[startLine].size() - 1;
        else {
            if(data->data[startLine][data->data[startLine].size() - 2] > 0)
                notEmptyAnswer = data->data[startLine].size() - 2;
            else {
                if(data->data[startLine][data->data[startLine].size() - 3] > 0)
                    notEmptyAnswer = data->data[startLine].size() - 3;
            }
        }
    }
    return notEmptyAnswer;
}

std::vector<float> createGist(Data* data, int columnNumber, int startLine, int lastLine, float min, float h, int n, std::vector<float> &resCol, std::vector<std::vector<float>> &diffRes, float &heigh) {
    std::vector<float> gist;
    gist.clear();
    resCol.clear();
    diffRes.clear();
    int elNum = 0;

    bool isOneValue = false;
    if(h == 0) {
        std::vector<float > oneCol;
        diffRes.push_back(oneCol);
        gist.push_back(0.0);
        isOneValue = true;
    }

    if(!isOneValue) {
        for (int i = 0; i < n; i++) {
            gist.push_back(0.0);
            std::vector<float> oneCol;
            diffRes.push_back(oneCol);
        }
    }


    int notEmptyAnswer = findNonEmptyRes(data, startLine);

    for(int i = startLine; i < lastLine; i++) {
        bool shouldCheck = false;
        if(data->data[i].size() != data->totalColNum) {
            shouldCheck = true;
        }
        else {
            if(data->data[i][data->data[i].size() - 3] == -1.0)
                shouldCheck = true;
        }

        if(!(data->data[i][data->data[i].size() - 1] == -1.0 && data->data[i][data->data[i].size() - 2] == -1.0 && shouldCheck)) {
            float value = data->data[i][columnNumber];
            if(!isOneValue) {
                for (int j = 0; j < n; j++) {
                    float left = (min + h * (float) j);
                    float right = (min + h * (float) (j + 1));
                    if (value >= left && value <= right) {
                        gist[j]++;
                        diffRes[j].push_back(data->data[i][notEmptyAnswer]);
                        resCol.push_back(data->data[i][notEmptyAnswer]);
                        j = n;
                        elNum++;
                    }
                }
            }
            else {
                //gist[0]++;
                elNum++;
                diffRes[0].push_back(data->data[i][notEmptyAnswer]);
                resCol.push_back(data->data[i][notEmptyAnswer]);
            }
        }
    }

    heigh = elNum;
    if(h == 0)
        gist[0] = elNum;
    //if(!isOneValue) {
    //    for (int i = 0; i < n; i++)
    //        gist[i] = gist[i] / ((float)(elNum));
   // }
    return gist;
}

std::vector<float> createGist(std::vector<float> &values, float min, float h, int n) {
    std::vector<float> gist;
    gist.clear();
    int elNum = 0;
    for(int i = 0; i < n; i++)
        gist.push_back(0.0);

    //for(auto val : values) {
    for(auto value : values) {
        for (int j = 0; j < n; j++) {
            float rightValue = (float)(min + h * ((float)j + 1.0f));
            float mapValue =  (float)(value);
            float leftValue = (float)(min + h * (float)j);
            if (mapValue >= leftValue && mapValue <= rightValue) {
                gist[j] += 1;
                j = n;
                elNum++;
            }
        }
    }

    //for(int i = 0; i < n; i++)
    //    gist[i] = gist[i]/((float)(elNum));
    return gist;
}

void printGist(Data* data, int pos, int col, std::vector<float> gist, float heigh) {
    std::cout << "\n--------\n";
    std::cout << "Sk: "<< (data->data[pos][0]) << ", Column " << col << "\n";
    for(auto el : gist) {
        std::cout << el/heigh << ", ";
    }

}

void gistForEveryS(Data* data) {
    int pos = 0, newPos, n;
    int totalColNum = data->totalColNum - 3;
    float max = 0.0, min = 0.0, h = 0.0;
    std::map<float, float> maxGR;

    for(int col = 1; col < totalColNum; col++) {
        while (pos < data->data.size()) {
            newPos = findMaxMin(data, min, max, pos, col);
            n = sterdFormula(newPos - pos);
            h = interval(min, max, n);

                std::vector<float> resCol;
                std::vector<std::vector<float>> diffRes;

                float heigh;
                std::vector<float> gist = createGist(data, col, pos, newPos, min, h, n, resCol, diffRes, heigh);
                printGist(data, pos, col, gist, heigh);

                float minAns = *(std::min_element(resCol.begin(), resCol.end()));
                float maxAns = *(std::max_element(resCol.begin(), resCol.end()));
                float hAns = interval(minAns, maxAns, n);
                std::vector<float> resGist = createGist(resCol, minAns, hAns, n);

                std::map<float, std::vector<float>> diffGist;
                for(auto &vec : diffRes) {
                    if(vec.size() != 0) {
                        float minOneCol = *(std::min_element(vec.begin(), vec.end()));
                        float maxOneCol = *(std::max_element(vec.begin(), vec.end()));
                        float hOneCol = interval(minOneCol, maxOneCol, n);
                        std::vector<float> oneColGist = createGist(vec, minOneCol, hOneCol, n);
                        diffGist.insert(std::pair<float, std::vector<float>>(hOneCol, oneColGist));
                    }
                    else {
                        std::vector<float> v;
                        diffGist.insert(std::pair<float, std::vector<float>>(0.0, v)); }
                }

                float gainR = GainRatio::gainRatio(diffGist, resGist, gist, n, h, hAns);

                std::cout << "GR for " << col << " Sk " << data->data[pos][0] << " is: " << gainR;
                auto foundGR = maxGR.find(data->data[pos][0]);
                if(foundGR == maxGR.end()) {
                    std::pair<float, float> insPair = std::pair<float, float>(data->data[pos][0], (float)gainR);
                    maxGR.insert(insPair);
                }
                else {
                    if (foundGR->second < gainR)
                        foundGR->second = gainR;
                }
                pos = newPos;
        }
        pos = 0;
    }

    for(auto e : maxGR) {
        std::cout << "\nMax gain ratio for " << e.first << " = " << e.second;
    }
}

std::vector<float> createGist(std::map<float, int> values, float min, float h, int n) {
    std::vector<float> gist;
    gist.clear();
    int elNum = 0;
    for(int i = 0; i < n; i++)
        gist.push_back(0.0);

    //for(auto val : values) {
    for(auto val  = values.begin(); val != values.end(); val++) {

            for (int j = 0; j < n; j++) {
                float rightValue = (float)(min + h * ((float)j + 1.0f));
                float mapValue =  (float)(val->first);
                float leftValue = (float)(min + h * (float)j);
                if (mapValue >= leftValue && mapValue <= rightValue) {
                    gist[j] += val->second;
                    elNum++;
                    j = n;
                }
            }
        }

    for(int i = 0; i < n; i++)
        gist[i] = gist[i]/((float)(elNum));
    return gist;
}


//last line not included
std::vector<float> calcEmpty(Data *data, int startLine, int lastLine) {
    std::vector<float> emptyVec;
    emptyVec.clear();
    for(int i = 0; i < data->totalColNum - 4; i++)
        emptyVec.push_back(0.0);

    for(int line = startLine; line < lastLine; line++) {
        int lastColumnsNum = 3;
        bool shouldCheck = false;
        if(data->data[line].size() != data->totalColNum) {
            shouldCheck = true;
            lastColumnsNum = 2;
        }
        else {
            if(data->data[line][data->data[line].size() - 3] == -1.0)
                shouldCheck = true;
        }

        if(!(data->data[line][data->data[line].size() - 1] == -1.0 && data->data[line][data->data[line].size() - 2] == -1.0 && shouldCheck)) {
            for (int sign = 1; sign < data->data[line].size() - lastColumnsNum; sign++) {
                if(data->data[line][sign] < 0)
                    emptyVec[sign - 1] ++;
            }
        }
    }
    int n = lastLine - startLine;
    for(int i = 0; i < data->totalColNum - 4; i++)
        emptyVec[i] = emptyVec[i]/(float)n;

    return emptyVec;
}

void calkUnique(Data* data) {
    //сдвинуты колонки
    std::vector<std::map<float, int>> stat;
    stat.clear();
    int columnsNum = data->totalColNum - 3;
    int totalLinesNumber = data->data.size();
    for(int i = 0; i < columnsNum - 1; i++) {
        std::map<float, int> oneColumn;
        stat.push_back(oneColumn);
    }

    for(int line = 0; line < totalLinesNumber; line++) {
        for(int col = 1; col < columnsNum; col++) {
            if(data->data[line][col]> 0) {
                auto foundEl = stat[col - 1].find(data->data[line][col]);
                if (foundEl != stat[col - 1].end()) {
                    (foundEl->second)++;
                } else {
                    stat[col - 1].insert(std::pair<float, int>(data->data[line][col], 1));
                }
            }
        }
    }

    for(int l = 0; l < stat.size(); l++) {
        auto col = stat[l];
        int n = sterdFormula(totalLinesNumber);
        float max = col.rbegin()->first;
        float min = col.begin()->first;
        float h = interval(min, max, n);
        std::vector<float> gist = createGist(col, min, h, n);
        std::cout << "\n------------\n" << "Column number: " << l << "\n";
        for(auto el : gist) {
            std::cout << el;
            std::cout << " ,";
        }
    }
}

//float gainRatio(std::vector<float> gist, float h, ) {}

float M(Data* data, int columnNuber) {
    float val = 0.0;
    int linesNum = 0;

    for(int i = 0; i < data->data.size(); i++) {
        if(data->data[i][columnNuber] > 0.0) {
            val += data->data[i][columnNuber];
            linesNum++;
        }
    }

    return val/(float)linesNum;
}

float calkCorr(Data* data, int firstColumnNumber, int secondColumnNumber) {
    float xm = M(data, firstColumnNumber);
    float ym = M(data, secondColumnNumber);
    float up = 0.0;
    float downx = 0.0;
    float  downy = 0.0;

    for(int i = 0; i < data->data.size(); i++) {
        if(data->data[i][firstColumnNumber] > 0 && data->data[i][secondColumnNumber] > 0) {
            up += (data->data[i][firstColumnNumber] - xm) * (data->data[i][secondColumnNumber] - ym);
            downx +=(data->data[i][firstColumnNumber] - xm)*(data->data[i][firstColumnNumber] - xm);
            downy += (data->data[i][secondColumnNumber] - ym)*(data->data[i][secondColumnNumber] - ym);
        }
    }
    return up/std::sqrt(downx*downy);
}

void createCorrTable(Data* data) {
    int totalColNum = data->totalColNum - 4;
    float corrTable[totalColNum][totalColNum];
    for(int i = 0; i < totalColNum; i++) {
        for(int j = 0; j < totalColNum; j++) {
            if(i != j) {
                corrTable[i][j] = calkCorr(data, i + 1, j + 1);
                std::cout << corrTable[i][j] << "   Colimn numbers: " << i + 1 << ", " << j + 1 << " \n";
            }
        }
    }


}

int main() {
    Data data;
    CSVParser::readAndStoreData("file.csv", &data);

    std::vector<float> emptyVec;
    emptyVec = calcEmpty(&data, 0, data.data.size());

    std::cout << "\nEmpty gist:\n";
    for(int i = 0; i < emptyVec.size(); i++) {
        auto el = emptyVec[i];
        std::cout <<"Column " << i << ", " << el;
        std::cout << "\n";
    }
    std::cout << "\n-------------\n" << "Correlation table:\n";
    createCorrTable(&data);
    std::cout << "\n-------------\n" << "Unique gist:\n";
    calkUnique(&data);
    gistForEveryS(&data);
    return 0;
}
