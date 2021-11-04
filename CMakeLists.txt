cmake_minimum_required(VERSION 3.0.0)
project(cli-breakpad VERSION 0.1.0)

add_executable(cli-breakpad main.cpp)

target_include_directories(cli-breakpad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/breakpad/include
)

if(WIN32)
    set_target_properties(cli-breakpad PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    target_link_directories(cli-breakpad PRIVATE
        $<$<CONFIG:Debug>:"${CMAKE_CURRENT_SOURCE_DIR}/breakpad/windows/${CMAKE_SYSTEM_PROCESSOR}/Debug">
        $<$<CONFIG:Release>:"${CMAKE_CURRENT_SOURCE_DIR}/breakpad/windows/${CMAKE_SYSTEM_PROCESSOR}/Release">)
    target_link_libraries(cli-breakpad PRIVATE exception_handler crash_generation_client common)
elseif(APPLE)
    find_library(BREAKPAD_CLIENT Breakpad "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/mac/")
    target_link_libraries(cli-breakpad PRIVATE ${BREAKPAD_CLIENT})
elseif(UNIX)
    execute_process(COMMAND arch COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    find_library(BREAKPAD_CLIENT breakpad_client "${CMAKE_CURRENT_SOURCE_DIR}/breakpad/linux/${ARCHITECTURE}")
    target_link_libraries(cli-breakpad PRIVATE ${BREAKPAD_CLIENT} pthread)
endif()
