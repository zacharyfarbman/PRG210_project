cmake_minimum_required(VERSION 3.20)

project(course_management_system
    VERSION 0.1.0
    LANGUAGES CXX
)

# Default to C++23, but allow 17, 20, or 23.
set(PROJECT_CXX_STANDARD 23 CACHE STRING "C++ standard to use: 17, 20, or 23")
set_property(CACHE PROJECT_CXX_STANDARD PROPERTY STRINGS 17 20 23)

if(NOT PROJECT_CXX_STANDARD STREQUAL "17"
   AND NOT PROJECT_CXX_STANDARD STREQUAL "20"
   AND NOT PROJECT_CXX_STANDARD STREQUAL "23")
    message(FATAL_ERROR "PROJECT_CXX_STANDARD must be one of: 17, 20, 23")
endif()

add_executable(course_management_system
    main.cpp
    # cart.cpp
    course.cpp
    department.cpp
    # interface.cpp
)

target_include_directories(course_management_system
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(course_management_system PROPERTIES
    CXX_STANDARD ${PROJECT_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(course_management_system
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
)
