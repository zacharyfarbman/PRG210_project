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
    cart.cpp
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

#irene included for testing wasnt working with this
# set(BUILD_TESTING OFF CACHE BOOL "Disable tests temporarily" FORCE)
include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
    )

    if(MSVC)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()

    FetchContent_MakeAvailable(googletest)

    add_executable(course_management_tests
        tests/course_tests.cpp
        tests/department_tests.cpp
        tests/cart_tests.cpp
        cart.cpp
        course.cpp
        department.cpp
    )

    target_include_directories(course_management_tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    set_target_properties(course_management_tests PROPERTIES
        CXX_STANDARD ${PROJECT_CXX_STANDARD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    target_compile_options(course_management_tests
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
                -Wall -Wextra -Wpedantic>
    )

    target_link_libraries(course_management_tests
        PRIVATE
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(course_management_tests)
endif()
