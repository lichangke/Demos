cmake_minimum_required(VERSION 3.15)
project(_BIOAndProgressDemo C)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c)
add_executable(client client.c)

find_package(Threads)

target_link_libraries(server Threads::Threads)
