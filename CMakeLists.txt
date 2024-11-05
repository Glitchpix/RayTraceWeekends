cmake_minimum_required(VERSION 3.20)
project(RayTrace VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(RayTrace src/main.cpp)
target_include_directories(RayTrace PUBLIC "${PROJECT_SOURCE_DIR}")
target_compile_options(RayTrace PRIVATE
     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
     -Werror -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion>
     $<$<CXX_COMPILER_ID:MSVC>:
          /W4>)