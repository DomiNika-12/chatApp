cmake_minimum_required(VERSION 3.23)
project(chatApp)

set(CMAKE_CXX_STANDARD 14)

add_executable(chatAppServer main.cpp server.h server.cpp client.h client.cpp)
add_executable(chatAppClient clientMain.cpp client.h client.cpp server.h server.cpp)
add_executable(chatAppClient1 clientMain.cpp client.h client.cpp server.h server.cpp)