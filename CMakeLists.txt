cmake_minimum_required(VERSION 3.0.0)
project(connection_handle VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")
add_executable(connection_handle main.cpp sqlite3.c)
