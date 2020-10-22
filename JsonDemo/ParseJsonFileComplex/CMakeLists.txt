cmake_minimum_required(VERSION 3.15)
project(ParseJsonFileComplex)

set(CMAKE_CXX_STANDARD 11)

add_executable(ParseJsonFileComplex main.cpp)

find_package(nlohmann_json 3.2.0 REQUIRED) # 找包

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json) # link 库