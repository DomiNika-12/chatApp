cmake_minimum_required(VERSION 3.23)
project(chatApp)

set(CMAKE_CXX_STANDARD 14)

add_executable(chatApp main.cpp server.h server.cpp)
