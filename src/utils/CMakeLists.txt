cmake_minimum_required (VERSION 3.10)

project(utils)

include_directories(${project_include_dirs})

aux_source_directory(. DIR_SOURCES)
add_library(${PROJECT_NAME} ${DIR_SOURCES})
