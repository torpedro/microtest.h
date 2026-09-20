cmake_minimum_required(VERSION 3.23)
project(microtest VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
add_library(microtest INTERFACE)
add_library(microtest::microtest ALIAS microtest)
target_compile_features(microtest INTERFACE cxx_std_11)
target_sources(microtest INTERFACE FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
  FILES include/microtest/microtest.h)

option(MICROTEST_BUILD_TESTS "Build regression tests" ${PROJECT_IS_TOP_LEVEL})
option(MICROTEST_BUILD_EXAMPLES "Build examples" ${PROJECT_IS_TOP_LEVEL})
option(MICROTEST_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

# Development flags never propagate through the public library target.
function(microtest_development_target target)
  set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /WX)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
  endif()
  if(MICROTEST_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT MSVC)
      target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
      target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    else()
      message(FATAL_ERROR "MICROTEST_SANITIZERS requires GCC or Clang")
    endif()
  endif()
endfunction()

if(MICROTEST_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
if(MICROTEST_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

install(TARGETS microtest EXPORT microtestTargets FILE_SET HEADERS)
install(EXPORT microtestTargets NAMESPACE microtest::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/microtest")
configure_package_config_file(cmake/microtestConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/microtestConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/microtest")
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/microtestConfigVersion.cmake"
  VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/microtestConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/microtestConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/microtest")
