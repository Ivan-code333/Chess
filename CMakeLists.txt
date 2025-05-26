cmake_minimum_required(VERSION 3.10)
project(Chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Находим SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Настройка SFML
set(SFML_ROOT "C:/SFML-2.6.1" CACHE PATH "SFML root directory")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")

# Исходные файлы
set(SOURCES
    main.cpp
    Core/board.cpp
    Core/piece.cpp
    Core/move.cpp
    Core/game.cpp
    Core/notation.cpp
    Core/renderer.cpp
    Texture.cpp
)

# Заголовочные файлы
set(HEADERS
    Core/board.h
    Core/piece.h
    Core/move.h
    Core/game.h
    Core/notation.h
    Core/renderer.h
    Font.h
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    "${SFML_ROOT}/lib/sfml-graphics.lib"
    "${SFML_ROOT}/lib/sfml-window.lib"
    "${SFML_ROOT}/lib/sfml-system.lib"
)

# Копируем ресурсы в директорию сборки
#file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
