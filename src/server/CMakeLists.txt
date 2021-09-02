cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(SFML_INCLUDE_DIR "lib/SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "lib/SFML-2.5.1/lib")
set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

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
        src/game/states/state.cpp
        src/game/states/state.h
        src/game/states/initialstate.cpp
        src/game/states/initialstate.h
        src/game/states/playstate.cpp
        src/game/states/playstate.h
        src/game/objects/candy.cpp
        src/game/objects/bonus.cpp
        src/game/objects/ball.cpp)

target_link_libraries(server sfml-system sfml-window sfml-graphics sfml-audio sfml-network)