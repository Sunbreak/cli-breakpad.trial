cmake_minimum_required(VERSION 3.0.0)
project(cli-breakpad VERSION 0.1.0)

add_executable(cli-breakpad main.cpp)

target_include_directories(cli-breakpad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/breakpad/include
)

if(WIN32)
    find_library(EXCEPTION_HANDLER exception_handler "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/windows/${CMAKE_SYSTEM_PROCESSOR}/Debug")
    find_library(CRASH_GENERATION_CLIENT crash_generation_client "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/windows/${CMAKE_SYSTEM_PROCESSOR}/Debug")
    find_library(COMMON common "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/windows/${CMAKE_SYSTEM_PROCESSOR}/Debug")
    target_link_libraries(cli-breakpad PRIVATE ${EXCEPTION_HANDLER} ${CRASH_GENERATION_CLIENT} ${COMMON})
elseif(APPLE)
    find_library(BREAKPAD_CLIENT Breakpad "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/mac/")
    target_link_libraries(cli-breakpad PRIVATE ${BREAKPAD_CLIENT})
elseif(UNIX)
    execute_process(COMMAND arch COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    find_library(BREAKPAD_CLIENT breakpad_client "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/linux/${ARCHITECTURE}")
    target_link_libraries(cli-breakpad PRIVATE ${BREAKPAD_CLIENT} pthread)
endif()
