#include <iostream>
#include "CSVParser.h"

void CSVParser::readAndStoreData(const std::string& fileName, Data* data, std::vector<std::string> &names) {

    std::ifstream in;
    in.open("..\\resources\\" + fileName);
    std::string line, token;
    int rowNum = 0;

    while(getline(in, line)) {
        if(rowNum == 1) {
            int cnt = 0;
            std::stringstream s(line);
            while (getline(s, token, ';')) {
                cnt++;
                names.push_back(token);
            }
            data->totalColNum = cnt - 1;
        }
        if (rowNum < 3) {
            rowNum++;
        } else {
            std::vector<float> row;
            std::stringstream s(line);
            int colNum = 0;
            while (getline(s, token, ';')) {
                if (colNum != 1) {
                    if (token.empty() || token == "-")
                        row.push_back(-1.0);
                    else
                        try {
                            size_t found = token.find(',');
                            if(found != std::string::npos)
                                token[found]='.';
                            row.push_back(std::stof(token));
                        }
                        catch (std::invalid_argument ex) {
                            std::cout << "  ";
                        }
                }
                colNum++;
            }
            data->data.push_back(row);
        }
    }
    in.close();
}


