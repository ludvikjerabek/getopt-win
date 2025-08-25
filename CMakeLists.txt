cmake_minimum_required(VERSION 3.12)

project(getopt VERSION 2.42.0
        DESCRIPTION "Getopt library for parsing command-line arguments, supporting ANSI and Unicode"
        HOMEPAGE_URL "https://github.com/ludvikjerabek/getopt-win"
        LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Build options
option(BUILD_SHARED_LIBS "Build shared library (produces getopt.dll and getopt.lib)" ON)
option(BUILD_STATIC_LIBS "Build static library (produces getopt_static.lib)" ON)
option(BUILD_TESTING "Enable tests for both shared and static libraries" ON)

set(GETOPT_SRC src/getopt.c)
set(GETOPT_HDR src/getopt.h)

# Shared library (produces getopt.dll and getopt.lib on Windows)
if(BUILD_SHARED_LIBS)
    add_library(getopt_shared SHARED ${GETOPT_SRC})
    target_compile_definitions(getopt_shared PRIVATE EXPORTS_GETOPT)
    target_include_directories(getopt_shared PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    set_target_properties(getopt_shared PROPERTIES
            OUTPUT_NAME getopt
            PUBLIC_HEADER ${GETOPT_HDR}
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
    )
    if(WIN32)
        set_target_properties(getopt_shared PROPERTIES
                WINDOWS_EXPORT_ALL_SYMBOLS TRUE
                # Ensure DLL is named getopt.dll and import library is getopt.lib
                RUNTIME_OUTPUT_NAME getopt
                ARCHIVE_OUTPUT_NAME getopt
        )
    endif()
    add_library(getopt::getopt_shared ALIAS getopt_shared)
endif()

# Static library (produces getopt_static.lib on Windows)
if(BUILD_STATIC_LIBS)
    add_library(getopt_static STATIC ${GETOPT_SRC})
    target_compile_definitions(getopt_static PRIVATE STATIC_GETOPT)
    target_include_directories(getopt_static PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    set_target_properties(getopt_static PROPERTIES
            OUTPUT_NAME getopt_static
            PUBLIC_HEADER ${GETOPT_HDR}
            VERSION ${PROJECT_VERSION}
    )
    add_library(getopt::getopt_static ALIAS getopt_static)
endif()

# Testing
if(BUILD_TESTING AND (BUILD_SHARED_LIBS OR BUILD_STATIC_LIBS))
    include(CTest)
    enable_testing()
    if(BUILD_SHARED_LIBS)
        add_executable(getopt_test_shared tests/main.c)
        target_include_directories(getopt_test_shared PRIVATE src)
        target_link_libraries(getopt_test_shared PRIVATE getopt_shared)
        add_test(NAME test_shared
                COMMAND getopt_test_shared --add -b -c hello -d 42 --verbose nonopt1 nonopt2
        )
        set_tests_properties(test_shared PROPERTIES
                PASS_REGULAR_EXPRESSION "option -a\noption -b\noption -c with value `hello'\noption -d with value `42'\nverbose flag is set\nnon-option ARGV-elements: nonopt1 nonopt2"
                TIMEOUT 30
                LABELS "shared"
        )
    endif()
    if(BUILD_STATIC_LIBS)
        add_executable(getopt_test_static tests/main.c)
        target_include_directories(getopt_test_static PRIVATE src)
        target_link_libraries(getopt_test_static PRIVATE getopt_static)
        target_compile_definitions(getopt_test_static PRIVATE STATIC_GETOPT)
        add_test(NAME test_static
                COMMAND getopt_test_static --add -b -c hello -d 42 --verbose nonopt1 nonopt2
        )
        set_tests_properties(test_static PROPERTIES
                PASS_REGULAR_EXPRESSION "option -a\noption -b\noption -c with value `hello'\noption -d with value `42'\nverbose flag is set\nnon-option ARGV-elements: nonopt1 nonopt2"
                TIMEOUT 30
                LABELS "static"
        )
    endif()
endif()

# Install rules
if(BUILD_SHARED_LIBS)
    install(TARGETS getopt_shared
            EXPORT getoptTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()
if(BUILD_STATIC_LIBS)
    install(TARGETS getopt_static
            EXPORT getoptTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

# Package config
install(EXPORT getoptTargets
        FILE getoptTargets.cmake
        NAMESPACE getopt::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/getopt
)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/getoptConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/getoptConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/getopt
)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/getoptConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/getoptConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/getoptConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/getopt
)