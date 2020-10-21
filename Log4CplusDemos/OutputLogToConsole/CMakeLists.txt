cmake_minimum_required(VERSION 3.15)
project(OutputLogToConsole)

set(CMAKE_CXX_STANDARD 11)

set(libraries log4cplus)

add_executable(OutputLogToConsole main.cpp)

target_link_libraries(${PROJECT_NAME} ${libraries})