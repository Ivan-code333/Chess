cmake_minimum_required(VERSION 3.30)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chess main.cpp
        Core/piece.h
        Core/piece.cpp
        Core/board.cpp
        Core/board.h
        Core/move.cpp
        Core/move.h
        Core/game.cpp
        Core/game.h
        Core/notation.cpp
        Core/notation.h)
