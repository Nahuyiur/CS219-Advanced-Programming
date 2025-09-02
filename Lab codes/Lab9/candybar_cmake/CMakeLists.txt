cmake_minimum_required(VERSION 3.10)

project(CandyBar)

aux_source_directory(. SRC)
add_executable(candybar ${SRC})