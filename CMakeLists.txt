cmake_minimum_required(VERSION 3.28)

project(simpledb)

include_directories(include)

set(SOURCE_EXE src/main.cpp src/table.cpp src/file_manager.cpp
        src/command_parser.cpp
        include/command_parser.h)

add_executable(simpledb ${SOURCE_EXE})
