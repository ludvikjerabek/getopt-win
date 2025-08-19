cmake_minimum_required(VERSION 3.12)
project(getopt
VERSION 1.1.0
DESCRIPTION "Getopt library for parsing command-line arguments, supporting ANSI and Unicode"
HOMEPAGE_URL "https://github.com/ludvikjerabek/getopt-win"
LANGUAGES C
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Build options
option(BUILD_SHARED_LIB "Build the shared library" ON)
option(BUILD_STATIC_LIB "Build the static library" ON)
option(BUILD_TESTING "Build and run tests" ON)

# Shared library target
if(BUILD_SHARED_LIB)
add_library(getopt_shared SHARED src/getopt.c src/getopt.h)
target_compile_definitions(getopt_shared PRIVATE EXPORTS_GETOPT)
target_include_directories(getopt_shared PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(getopt_shared PROPERTIES
OUTPUT_NAME getopt
PUBLIC_HEADER src/getopt.h
VERSION ${PROJECT_VERSION}
SOVERSION ${PROJECT_VERSION_MAJOR}
)
# Ensure DLL exports on Windows
if(WIN32)
set_target_properties(getopt_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()
add_library(${PROJECT_NAME}::getopt_shared ALIAS getopt_shared)
endif()

# Static library target
if(BUILD_STATIC_LIB)
add_library(getopt_static STATIC src/getopt.c src/getopt.h)
target_compile_definitions(getopt_static PRIVATE STATIC_GETOPT)
target_include_directories(getopt_static PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(getopt_static PROPERTIES
OUTPUT_NAME getopt
PUBLIC_HEADER src/getopt.h
VERSION ${PROJECT_VERSION}
)
add_library(${PROJECT_NAME}::getopt_static ALIAS getopt_static)
endif()

# Testing setup
if(BUILD_TESTING AND (BUILD_SHARED_LIB OR BUILD_STATIC_LIB))
enable_testing()
# Test program
add_executable(example_shared tests/main.c)
add_executable(example_static tests/main.c)
if(BUILD_SHARED_LIB)
target_link_libraries(example_shared PRIVATE getopt_shared)
target_include_directories(example_shared PRIVATE src)
add_test(NAME test_shared
COMMAND example_shared --add -b -c hello -d 42 --verbose nonopt1 nonopt2
)
set_tests_properties(test_shared PROPERTIES
PASS_REGULAR_EXPRESSION "option -a\noption -b\noption -c with value `hello'\noption -d with value `42'\nverbose flag is set\nnon-option ARGV-elements: nonopt1 nonopt2"
)
endif()
if(BUILD_STATIC_LIB)
target_link_libraries(example_static PRIVATE getopt_static)
target_include_directories(example_static PRIVATE src)
target_compile_definitions(example_static PRIVATE STATIC_GETOPT)
add_test(NAME test_static
COMMAND example_static --add -b -c hello -d 42 --verbose nonopt1 nonopt2
)
set_tests_properties(test_static PROPERTIES
PASS_REGULAR_EXPRESSION "option -a\noption -b\noption -c with value `hello'\noption -d with value `42'\nverbose flag is set\nnon-option ARGV-elements: nonopt1 nonopt2"
)
endif()
endif()

# Installation rules
if(BUILD_SHARED_LIB)
install(TARGETS getopt_shared
EXPORT ${PROJECT_NAME}Targets
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
endif()
if(BUILD_STATIC_LIB)
install(TARGETS getopt_static
EXPORT ${PROJECT_NAME}Targets
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
endif()

# CMake package configuration for third-party projects
install(EXPORT ${PROJECT_NAME}Targets
FILE ${PROJECT_NAME}Targets.cmake
NAMESPACE ${PROJECT_NAME}::
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

configure_package_config_file(
${CMAKE_CURRENT_SOURCE_DIR}/getoptConfig.cmake.in
${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
VERSION ${PROJECT_VERSION}
COMPATIBILITY SameMajorVersion
)

install(FILES
${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)