cmake_minimum_required(VERSION 3.10)

# Project name
project(CppTimer)

string(TOLOWER ${PROJECT_NAME} LIB_NAME)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Request C++11 standard features
set(CMAKE_CXX_STANDARD 17)

# Tell the compiler to optimise heavily
add_compile_options(-O2)


### SOURCE CODE ###

# Get a list of source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Make library
add_library(${LIB_NAME} ${SRC_FILES})

# Include the `include` directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set the header to public
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER CppTimer.h)

# Set the library's output directory to bin/
set_target_properties(${LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Make PIC
set_target_properties(${LIB_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)


### UNIT TESTING ###

# Enable testing
enable_testing()

# Use the testing framework
include_directories(${PROJECT_SOURCE_DIR}/test/framework)

# Get test source files
file(GLOB TEST_SRCS ${PROJECT_SOURCE_DIR}/test/*.cpp)

# Run through each test file
foreach(testSrc ${TEST_SRCS})
    # Get extension-less file name
    get_filename_component(testName ${testSrc} NAME_WE)
    # Add target
    add_executable(${testName} ${testSrc})
    # Link to relevant libraries
    target_link_libraries(${testName} ${LIB_NAME})
    # Put test executables in their own directory
    set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)

    # Add test to execution
    add_test(
        NAME ${testName}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin
        COMMAND ${testName}
    )
endforeach(testSrc)
