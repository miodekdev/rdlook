cmake_minimum_required(VERSION 3.27)
project(rdlook)

set(CMAKE_CXX_STANDARD 23)

add_executable(rdlook main.cpp)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")