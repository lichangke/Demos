cmake_minimum_required(VERSION 3.15)
project(SimpleReactorWithC C)

set(CMAKE_C_STANDARD 11)

add_executable(reactor reactor.c)
add_executable(client client.c)