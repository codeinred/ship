cmake_minimum_required(VERSION 3.0.0)
project(conduit CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_executable(ship ship.cpp)

add_subdirectory(conduit)
add_subdirectory(fmt)

target_include_directories(ship PUBLIC include)


target_link_libraries(ship conduit fmt)

