cmake_minimum_required(VERSION 3.15)
project(ThreadPool01)

set(CMAKE_CXX_STANDARD 11)
find_package(Threads)

add_executable(${PROJECT_NAME} main.cpp ThreadPool.hpp)

target_link_libraries(${PROJECT_NAME} Threads::Threads)