cmake_minimum_required(VERSION 3.15)
project(PublishSubscribeDemo)

set(CMAKE_C_STANDARD 11)

find_package(ZeroMQ)

set(libraries zmq)

add_executable(PUBServer  PUBServer.c)
add_executable(SUBClient  SUBClient.c)

#set(hwservers hwserver)
#set(hwclients hwclient)


target_link_libraries(PUBServer ${libraries})
target_link_libraries(SUBClient ${libraries})