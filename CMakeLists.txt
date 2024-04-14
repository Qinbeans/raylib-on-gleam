cmake_minimum_required(VERSION 3.24)
project(ray-erl VERSION 1.0 DESCRIPTION "My Library Description")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define paths for the local vcpkg installation and installation of packages
set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/external/vcpkg")
set(VCPKG_INSTALLED_DIR "${CMAKE_BINARY_DIR}/external")
# get the triplet for the current platform via the vcpkg toolchain
set(TRIPLETS "x64-osx" "arm64-osx" "x64-linux" "x64-windows")
foreach(TRIPLET ${TRIPLETS})
  if(EXISTS "${VCPKG_ROOT}/installed/${TRIPLET}")
    set(VCPKG_INSTALLED_TRIPLET_DIR "${VCPKG_ROOT}/installed/${TRIPLET}")
    break()
  endif()
endforeach()

# Find Erlang
find_program(ERL_EXECUTABLE NAMES erl)
if(NOT ERL_EXECUTABLE)
  message(FATAL_ERROR "Erlang not found")
endif()

# Get Erlang include path
execute_process(
  COMMAND "${ERL_EXECUTABLE}" -noshell -eval "io:format(\"~s/erts-~s/include\", [code:root_dir(), erlang:system_info(version)]), halt()."
  OUTPUT_VARIABLE ERL_ERTS_INCLUDE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE ERL_ERTS_INCLUDE_DIR_RESULT
)

if(NOT ERL_ERTS_INCLUDE_DIR_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to get Erlang include path")
endif()

# Ensure vcpkg is bootstrapped
if(NOT EXISTS "${VCPKG_ROOT}/vcpkg")
  message(STATUS "Bootstrapping vcpkg...")
  execute_process(
    COMMAND "${VCPKG_ROOT}/bootstrap-vcpkg.sh" -disableMetrics  # For Unix
    # COMMAND "${VCPKG_ROOT}/bootstrap-vcpkg.bat" -disableMetrics  # Uncomment this for Windows
    WORKING_DIRECTORY ${VCPKG_ROOT}
    RESULT_VARIABLE BOOTSTRAP_RESULT
  )
  if(NOT BOOTSTRAP_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to bootstrap vcpkg")
  endif()
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

find_package(raylib CONFIG QUIET)

if(NOT raylib_FOUND)
  message(STATUS "Installing raylib locally via vcpkg...")
  execute_process(
    COMMAND "${VCPKG_ROOT}/vcpkg" install raylib
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE VCPKG_INSTALL_RESULT
  )
  if(NOT VCPKG_INSTALL_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to install raylib via vcpkg")
  endif()
  # Raylib
  set(raylib_DIR "${VCPKG_INSTALLED_TRIPLET_DIR}/share/raylib")
  set(raylib_INCLUDE_DIR "${VCPKG_INSTALLED_TRIPLET_DIR}/include")
  set(raylib_LIBRARY "${VCPKG_INSTALLED_TRIPLET_DIR}/lib/libraylib.a")
  set(raylib_LIBRARY "${VCPKG_INSTALLED_TRIPLET_DIR}/lib/libraylib.a")
  #glfw3
  set(glfw3_DIR "${VCPKG_INSTALLED_TRIPLET_DIR}/share/glfw3")
  set(glfw3_INCLUDE_DIR "${VCPKG_INSTALLED_TRIPLET_DIR}/include")
  set(glfw3_LIBRARY "${VCPKG_INSTALLED_TRIPLET_DIR}/lib/libglfw3.a")
endif()

find_package(raylib CONFIG REQUIRED) # Reattempt finding package to update configuration
find_package(glfw3 CONFIG REQUIRED)

add_library(ray-erl SHARED src/ray-erl.c)
target_link_libraries(ray-erl PRIVATE raylib)
target_link_libraries(ray-erl PRIVATE glfw)
include_directories(BEFORE SYSTEM ${ERL_ERTS_INCLUDE_DIR})
# add option undefined dynamic lookup
target_link_options(ray-erl PRIVATE -undefined dynamic_lookup)
set_target_properties(ray-erl PROPERTIES POSITION_INDEPENDENT_CODE ON)
# ends in .so
set_target_properties(ray-erl PROPERTIES PREFIX "")
set_target_properties(ray-erl PROPERTIES OUTPUT_NAME "ray_erl")
set_target_properties(ray-erl PROPERTIES SUFFIX ".so")