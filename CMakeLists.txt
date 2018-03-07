cmake_minimum_required(VERSION 3.5)
project(HashTable)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fsanitize=address")

set(SOURCE_FILES hash_map.cpp hash_map.h)
add_executable(HashTable ${SOURCE_FILES} hash_map.h test_hashmap.cpp)