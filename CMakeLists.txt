cmake_minimum_required(VERSION 3.30)
project(Chess)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Настройка SFML
set(SFML_ROOT "C:/SFML-2.6.1" CACHE PATH "SFML root directory")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")

find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

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

target_include_directories(Chess PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(Chess PRIVATE 
    "${SFML_ROOT}/lib/sfml-graphics.lib"
    "${SFML_ROOT}/lib/sfml-window.lib"
    "${SFML_ROOT}/lib/sfml-system.lib"
)
