cmake_minimum_required(VERSION 3.15)
project(SimpleReactorWithC__)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(srcs reactor_srcs)

add_executable(reactor  ${reactor_srcs} main.cpp)
add_executable(client client.c)