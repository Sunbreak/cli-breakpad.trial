cmake_minimum_required(VERSION 3.0.0)
project(cli-breakpad VERSION 0.1.0)

add_executable(cli-breakpad main.cpp)

target_include_directories(cli-breakpad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/breakpad/include
)

if (UNIX)
    execute_process(COMMAND arch COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    find_library(BREAKPAD_CLIENT breakpad_client "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/linux/${ARCHITECTURE}")
    target_link_libraries(cli-breakpad PRIVATE ${BREAKPAD_CLIENT} pthread)
endif()
