cmake_minimum_required(VERSION 3.5)
project (linked_list)

add_executable(linked_list 
    src/test.c 
    src/linked_list.c
)

target_include_directories(linked_list
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
