cmake_minimum_required(VERSION 3.15)
project(Logging_macros)

set(CMAKE_CXX_STANDARD 11)

add_executable(Logging_macros main.cpp)

target_link_libraries(${PROJECT_NAME} log4cplus)