cmake_minimum_required(VERSION 3.12)
project(Tema1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread -lm -Wall")

include_directories(.)

add_executable(Tema1
        homework.c
        homework.h
        main.c
        Makefile)
