cmake_minimum_required(VERSION 3.15)
project(OutputLogToRollFileAndConsle)

set(CMAKE_CXX_STANDARD 11)

set(libraries log4cplus)

# 将代码中config文件夹中数据拷贝到 执行目录
file(GLOB configFiles config/*)
file (COPY ${configFiles} DESTINATION ${CMAKE_BINARY_DIR}/config)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${libraries})