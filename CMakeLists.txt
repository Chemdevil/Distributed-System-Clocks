cmake_minimum_required(VERSION 3.25)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c clockLibrary/Lamport/LamportClock.c clockLibrary/Lamport/LamportClock.h clockLibrary/Vector/VectorClock.c clockLibrary/Vector/VectorClock.h)
