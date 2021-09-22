cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

set(SFML_INCLUDE_DIR "lib/SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "lib/SFML-2.5.1/lib")
set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)
include_directories(src)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

add_executable(server
        src/main.cpp
        src/game/game.cpp
        src/game/game.h
        src/game/state.cpp
        src/game/state.h
        src/game/objects.cpp
        src/communication/socketserver.cpp
        src/communication/socketserver.h)

target_link_libraries(server sfml-system sfml-window sfml-graphics sfml-audio sfml-network)