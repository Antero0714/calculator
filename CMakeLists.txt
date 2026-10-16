cmake_minimum_required(VERSION 3.14)

project(calculator LANGUAGES CXX)

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)

if(NOT CLANG_FORMAT_EXECUTABLE)
    message(FATAL_ERROR "clang-format was not found")
endif()

find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)

if(NOT CLANG_TIDY_EXECUTABLE)
    message(FATAL_ERROR "clang-tidy was not found")
endif()

include(FetchContent)

FetchContent_Declare(
    calc_math
    GIT_REPOSITORY https://github.com/Antero0714/math-library.git
    GIT_TAG 33b3bddb988a49bc77c46635bdda381fa6b0884e
)

FetchContent_MakeAvailable(calc_math)

add_executable(calculator
    main.cpp
    application.cpp)

set_property(TARGET calculator PROPERTY
    CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}"
)

target_compile_options(calculator PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_compile_features(calculator PRIVATE cxx_std_17)

target_link_libraries(calculator PRIVATE calc_math)

install(TARGETS calculator
    RUNTIME DESTINATION bin
)

add_custom_target(format
    COMMAND "${CLANG_FORMAT_EXECUTABLE}" -i
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/application.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/application.h"
    VERBATIM
)

add_dependencies(calculator format)
