cmake_minimum_required(VERSION 3.20)
project(client)

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

add_executable(client
        src/main.cpp
        src/game/handler.cpp
        src/game/handler.h
        src/communication/socketclient.cpp
        src/communication/socketclient.h)

target_link_libraries(client sfml-system sfml-window sfml-graphics sfml-audio sfml-network)