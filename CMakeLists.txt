cmake_minimum_required(VERSION 3.13...3.19 FATAL_ERROR)
project(libbitvec VERSION 1.0.0 LANGUAGES CXX)

#Make sure that custom modules like FindRapidJSON are found
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

##############################################
# Find system dependencies
find_package(fmt)

##############################################
# Create target and set properties

add_library(bitvec SHARED
    src/BitVec.cc
    src/debug.cc
    src/changable.cc
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(BitVec::bitvec ALIAS bitvec)

#Set target properties
target_include_directories(bitvec
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_options(bitvec PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

target_link_libraries(bitvec
    PUBLIC
        fmt::fmt
)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/BitVec)

install(TARGETS bitvec
    EXPORT bitvec-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name JSONUtils and not jsonutils
# set_target_properties(bitvec PROPERTIES EXPORT_NAME BitVec)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT bitvec-targets
    FILE
        BitVecTargets.cmake
    NAMESPACE
        BitVec::
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/BitVecConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/BitVecConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BitVecConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/BitVecConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/BitVecConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
export(EXPORT bitvec-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/BitVecTargets.cmake
    NAMESPACE BitVec::)

#Register package in the User Package Registry
export(PACKAGE BitVec)

add_executable(bitvec_example src/example.cc)
target_link_libraries(bitvec_example bitvec)

##############################################
# Add test
enable_testing()
add_subdirectory(test)
