cmake_minimum_required(VERSION 3.15)
project(Log_level)

set(CMAKE_CXX_STANDARD 11)

add_executable(Log_level main.cpp)

target_link_libraries(${PROJECT_NAME} log4cplus)