cmake_minimum_required(VERSION 3.15)
project(SocketMonitor)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

add_executable(server Server.cpp)
add_executable(client Client.cpp)

target_link_libraries(server Threads::Threads zmq)
target_link_libraries(client Threads::Threads zmq)