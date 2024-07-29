cmake_minimum_required(VERSION 3.10)
project(AceCompiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(AceCompiler src/main.cpp src/Lexer.cpp)
