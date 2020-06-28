cmake_minimum_required(VERSION 3.15)
project(json)

set(CMAKE_CXX_STANDARD 17)

add_library(json lib/json.hpp lib/parser.cpp)

add_executable(example examples/example.cpp)
target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(example PRIVATE json)