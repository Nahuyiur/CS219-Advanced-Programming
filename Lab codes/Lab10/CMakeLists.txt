cmake_minimum_required(VERSION 3.10)
project(Lab10)

include_directories(include)

file(GLOB LIBA liba/*.cpp) 
file(GLOB LIBS libs/*.cpp)  

add_library(liba STATIC ${LIBA})
add_library(libs SHARED ${LIBS})

add_executable(main main.cpp)

target_link_libraries(main liba libs)