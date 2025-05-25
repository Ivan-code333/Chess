# FindSFML.cmake
set(SFML_ROOT "C:/SFML-2.6.1" CACHE PATH "SFML root directory")

find_path(SFML_INCLUDE_DIR SFML/Graphics.hpp
    PATHS
    ${SFML_ROOT}/include
)

find_library(SFML_GRAPHICS_LIBRARY
    NAMES sfml-graphics
    PATHS
    ${SFML_ROOT}/lib
)

find_library(SFML_WINDOW_LIBRARY
    NAMES sfml-window
    PATHS
    ${SFML_ROOT}/lib
)

find_library(SFML_SYSTEM_LIBRARY
    NAMES sfml-system
    PATHS
    ${SFML_ROOT}/lib
)

set(SFML_FOUND TRUE)
set(SFML_INCLUDE_DIRS ${SFML_INCLUDE_DIR})
set(SFML_LIBRARIES ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY}) 