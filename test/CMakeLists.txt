# see /opt/local/share/cmake-3.16/Modules/FindGTest.cmake
find_package(GTest QUIET)

# NOTE: the upper case GTEST! CK
if(NOT GTEST_FOUND)
  # Download and unpack googletest at configure time
  # but only if needed! CK
  configure_file(${CMAKE_SOURCE_DIR}/cmake/GoogleTest-CMakeLists.txt.in
    ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
    ${CMAKE_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL)
endif()

# Now simply link against gtest as needed. Eg
add_executable(bitvec_test 
  src/main.cc
  src/utils.cc
)
add_executable(TestBitRef 
  src/TestBitRef.cc
  src/utils.cc
)
target_link_libraries(bitvec_test gtest_main bitvec gtest)
target_link_libraries(TestBitRef gtest_main bitvec gtest)

add_test(NAME bitvec_test COMMAND bitvec_test)
add_test(NAME bitref_test COMMAND TestBitRef)
