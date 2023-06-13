cmake_minimum_required(VERSION 3.20)

project(hi_expat)

aux_source_directory(${CMAKE_SOURCE_DIR} SRC_FILES)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

find_package(expat REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} expat::expat)

