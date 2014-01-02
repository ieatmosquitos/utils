cmake_minimum_required(VERSION 2.8)

project(clgui)
set(clgui_VERSION_MAJOR 0)
set(clgui_VERSION_MINOR 1)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 1.6 COMPONENTS system window graphics network audio REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

add_executable(clgui src/clustering_gui.cpp)

target_link_libraries(clgui ${SFML_LIBRARIES})

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)