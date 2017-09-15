cmake_minimum_required(VERSION 3.7)

project(sml CXX)

if(CMAKE_BUILD_TYPE STREQUAL Release OR NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    set(DEBUG FALSE)
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(DEBUG TRUE)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(SML_DIR ${CMAKE_SOURCE_DIR})
set(CPPUTEST_DIR ${CMAKE_SOURCE_DIR}/cpputest)

set(SML_INCLUDE_DIR ${SML_DIR}/src)
set(CPPUTEST_INCLUDE_DIR ${CPPUTEST_DIR}/include)

if(DEBUG)
    set(CPPUTEST_LIB_DIR ${CPPUTEST_DIR}/lib/debug)
else()
    set(CPPUTEST_LIB_DIR ${CPPUTEST_DIR}/lib/release)
endif()

add_subdirectory(${SML_DIR}/src sml)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)