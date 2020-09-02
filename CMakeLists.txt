cmake_minimum_required(VERSION 3.17)
project(c_hash_tables C)

set(CMAKE_C_STANDARD 99)

add_executable(c_hash_tables hash_table.c hash_table.h main.c)

target_link_libraries(c_hash_tables m)