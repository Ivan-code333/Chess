cmake_minimum_required(VERSION 3.20)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

# Настройка SFML
set(SFML_ROOT "C:\\SFML-2.6.1")
set(SFML_DIR "${SFML_ROOT}\\lib\\cmake\\SFML")
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

target_link_libraries(Chess sfml-graphics sfml-window sfml-system)

# Копирование DLL (для windows)
if(WIN32)
    add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_ROOT}/bin" $<TARGET_FILE_DIR:Chess>)
        
    # Копирование шрифта
    configure_file(${CMAKE_SOURCE_DIR}/PragmaticaBoldReg.otf ${CMAKE_BINARY_DIR}/Debug/PragmaticaBoldReg.otf COPYONLY)
endif()
