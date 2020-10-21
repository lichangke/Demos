cmake_minimum_required(VERSION 3.15)
project(RollingFileAppenderDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(RollingFileAppenderDemo main.cpp)
target_link_libraries(${PROJECT_NAME} log4cplus)
