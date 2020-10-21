cmake_minimum_required(VERSION 3.15)
project(Hello_World)

set(CMAKE_CXX_STANDARD 11)

add_executable(Hello_World main.cpp)

target_link_libraries(${PROJECT_NAME} log4cplus)