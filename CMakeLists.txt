cmake_minimum_required(VERSION 3.15)
project(DataMass)

set(CMAKE_CXX_STANDARD 14)

add_executable(DataMass main.cpp CSVParser.cpp Gist.cpp GainRatio.cpp)