
# CMake project for LearnOpengl, 
# - include source and define
# - project specific logic here.

cmake_minimum_required (VERSION 3.20)


# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
        "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

# ---- ============== ----

find_package(fmt   CONFIG REQUIRED)

add_executable (lib_sit_demo "demo.cpp")
set_target_properties(lib_sit_demo PROPERTIES CXX_STANDARD 20)
# ---- Add dependencies ----
target_link_libraries(lib_sit_demo PRIVATE fmt::fmt)

add_executable (lib_sit_demo_uni "demo_uni.cpp")
set_target_properties(lib_sit_demo_uni PROPERTIES CXX_STANDARD 20)
# ---- Add dependencies ----
target_link_libraries(lib_sit_demo_uni PRIVATE fmt::fmt)

add_executable (lib_sit_demo_uni_3 "demo_uni3.cpp")
set_target_properties(lib_sit_demo_uni_3 PROPERTIES CXX_STANDARD 20)
# ---- Add dependencies ----
target_link_libraries(lib_sit_demo_uni_3 PRIVATE fmt::fmt)

add_executable (lib_sit_demo_uni_4 "demo_uni4.cpp")
set_target_properties(lib_sit_demo_uni_4 PROPERTIES CXX_STANDARD 20)
# ---- Add dependencies ----
target_link_libraries(lib_sit_demo_uni_4 PRIVATE fmt::fmt)

# 1> [CMake] mp-units provides CMake targets:
# 1> [CMake]     # this is heuristically generated, and may not be correct
# 1> [CMake]     find_package(mp-units CONFIG REQUIRED)
# 1> [CMake]     # note: 12 additional targets are not displayed.    target_link_libraries(main PRIVATE mp-units::si mp-units::isq mp-units::core mp-units::si-cgs)
# find_package(mp-units CONFIG REQUIRED)
# target_link_libraries(main PRIVATE mp-units::si mp-units::isq mp-units::core mp-units::si-cgs)
# # target_link_libraries(main PRIVATE mp-units::mp-units)


# ---- Custom commands ----
# - mainly copying stuff pust or pre -build


message(STATUS "=== variables in subdirectory")
message(STATUS "    PROJECT_BINARY_DIR        =" ${PROJECT_BINARY_DIR})
message(STATUS "    PROJECT_SOURCE_DIR        =" ${PROJECT_SOURCE_DIR})
message(STATUS "")
message(STATUS "    CMAKE_BINARY_DIR          =" ${CMAKE_BINARY_DIR})
message(STATUS "    CMAKE_CURRENT_BINARY_DIR  =" ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "")
message(STATUS "    CMAKE_SOURCE_DIR          =" ${CMAKE_SOURCE_DIR})
message(STATUS "    CMAKE_CURRENT_SOURCE_DIR  =" ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "")
message(STATUS "    CMAKE_LIST_DIR            =" ${CMAKE_LIST_DIR})
message(STATUS "    CMAKE_CURRENT_LIST_DIR    =" ${CMAKE_CURRENT_LIST_DIR})

# TODO: Add tests and install targets if needed.

