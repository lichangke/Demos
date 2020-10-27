cmake_minimum_required(VERSION 3.15)
project(HelloWorld C)

set(CMAKE_C_STANDARD 11)

find_package(ZeroMQ)

set(libraries zmq)

add_executable(hwserver hwserver.c)
add_executable(hwclient hwclient.c)

set(hwservers hwserver)
set(hwclients hwclient)


target_link_libraries(${hwservers} ${libraries})
target_link_libraries(${hwclients} ${libraries})
