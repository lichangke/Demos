cmake_minimum_required(VERSION 3.15)
project(SocketMonitor)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Threads::Threads zmq)