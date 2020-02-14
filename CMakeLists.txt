cmake_minimum_required(VERSION 3.10)
project(gobang)

aux_source_directory(src SRC_LIST)
include_directories(
    include
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(Gobang ${SRC_LIST})
target_link_libraries(Gobang ncurses)
target_compile_options(Gobang INTERFACE -O2)