cmake_minimum_required(VERSION 3.0.0)
project(ship CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(conduit)
add_subdirectory(fmt)

add_executable(ship main.cpp)

include_directories(include)
target_link_libraries(ship conduit fmt)

file(GLOB source_files "src/*.cpp")

foreach(file_name ${source_files})
    get_filename_component(source_name ${file_name} NAME_WE)
    add_library("${source_name}" "${file_name}")
    target_link_libraries("${source_name}" conduit fmt)
    target_link_libraries(ship "${source_name}")
endforeach()

