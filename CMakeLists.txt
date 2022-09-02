cmake_minimum_required(VERSION 3.22)
project(CPP_Dithering_2)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 17)

add_executable(CPP_Dithering_2 main.cpp)

target_link_libraries(CPP_Dithering_2 ${OpenCV_LIBS})