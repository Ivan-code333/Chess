cmake_minimum_required(VERSION 3.30)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chess main.cpp
        piece.h
        piece.cpp
        board.cpp
        board.h
        move.cpp
        move.h)
