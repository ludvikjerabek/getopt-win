cmake_minimum_required(VERSION 3.12)

if(NOT WIN32)
  message(FATAL_ERROR [=[
Expected the target system to be Windows.

This software is a modification of the Free Software Foundation, Inc. getopt
library for parsing command line arguments and its purpose is to provide a
Microsoft Visual C friendly derivative.]=]
  )
endif()

project(getopt-win
  VERSION 1.1.0
  DESCRIPTION "Full getopt port for unicode and multibyte Windows applications"
  HOMEPAGE_URL "https://github.com/ludvikjerabek/getopt-win"
  LANGUAGES C
)

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build the shared library." ON)
option(BUILD_TESTING "Build the testing tree." ON)

# getopt library

add_library(getopt src/getopt.c)
target_include_directories(getopt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)
target_compile_definitions(getopt
  PUBLIC  $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:STATIC_GETOPT>
  PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:EXPORTS_GETOPT>
)
set_target_properties(getopt PROPERTIES
  PUBLIC_HEADER src/getopt.h
  VERSION ${PROJECT_VERSION}
)

add_library(${PROJECT_NAME}::getopt ALIAS getopt)

# example program

add_executable(example EXCLUDE_FROM_ALL src/main.c)
target_link_libraries(example PRIVATE getopt)

# testing
if(BUILD_TESTING)
  enable_testing()

  add_test(NAME example
    COMMAND example --add -b -c hello -d 42 --verbose nonopt1 nonopt2
  )
  set_tests_properties(example PROPERTIES
    PASS_REGULAR_EXPRESSION [=[
option -a
option -b
option -c with value `hello'
option -d with value `42'
verbose flag is set
non-option ARGV-elements: nonopt1 nonopt2]=]
  )
endif()

# installation

install(TARGETS getopt
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE COMPONENT Library
  RUNTIME COMPONENT Library
  PUBLIC_HEADER
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT Library
)

# CMake export/import files

install(EXPORT ${PROJECT_NAME}-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  COMPONENT Config
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${PROJECT_NAME}-config.cmake.in
  ${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY SameMajorVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  COMPONENT Config
)
