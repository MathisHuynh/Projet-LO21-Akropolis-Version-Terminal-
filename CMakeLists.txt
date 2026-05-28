cmake_minimum_required(VERSION 3.10)

project(Akropolis)


set(CMAKE_CXX_STANDARD 17)

include_directories(inc)
file(GLOB SOURCES "src/*.cpp" "main.cpp")

add_executable(Akropolis ${SOURCES})