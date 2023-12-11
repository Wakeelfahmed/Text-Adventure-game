set(CMAKE_CXX_STANDARD 20)

cmake_minimum_required(VERSION 3.8)
project(textadv)

set(SOURCE_FILES main.cpp Room.h Room.cpp wordwrap.h wordwrap.cpp State.cpp State.h strings.h
        GameObject.h
        GameObject.cpp)
add_executable(textadv ${SOURCE_FILES})