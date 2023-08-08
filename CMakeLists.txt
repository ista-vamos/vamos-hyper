cmake_minimum_required(VERSION 3.9)
project(vamos-hyper LANGUAGES CXX VERSION 0.1)

find_package(vamos-buffers REQUIRED)

enable_testing()

set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
        if (vamos_buffers_BUILD_TYPE)
	        message(STATUS "Build type not set, using the same as vamos-buffers.")
	        set(CMAKE_BUILD_TYPE ${vamos_buffers_BUILD_TYPE} CACHE STRING "" FORCE)
        else()
	        message(STATUS "Build type not set. Setting default RelWithDebInfo.")
	        set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
        endif()
endif()

if (NOT ${vamos_buffers_BUILD_TYPE} STREQUAL ${CMAKE_BUILD_TYPE})
        message(WARNING "Build type (${CMAKE_BUILD_TYPE}) is different from "
                        "vamos-buffers' build type (${vamos_buffers_BUILD_TYPE}). "
                        "That can cause troubles with linking if IPO is enabled at "
                        "vamos-buffers and not in this project "
                        "(vamos_buffers_IPO=${vamos_buffers_OPTION_ENABLE_IPO})")
endif()

OPTION(ENABLE_IPO "Enable interprocedural optimizations" ${vamos_buffers_IPO})

if (ENABLE_IPO)
if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	cmake_policy(SET CMP0069 NEW)
	include(CheckIPOSupported)
	check_ipo_supported()
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
endif(ENABLE_IPO)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_compile_options(-Wall -Wextra -pedantic -fno-rtti -fno-exceptions)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(tests)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT vamos-hyper_Export
    FILE vamos-hyper.cmake
    #NAMESPACE shmn::
    DESTINATION lib/cmake/vamos-hyper)
# also export the .cmake file into current build directory
# so that we can use also non-installed builds
export(EXPORT vamos-hyper_Export FILE ${CMAKE_CURRENT_BINARY_DIR}/cmake/vamos-hyper/vamos-hyper.cmake)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vamos-hyper/vamos-hyper-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/vamos-hyper/vamos-hyper-config.cmake"
  INSTALL_DESTINATION "lib/cmake/vamos-hyper"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

write_basic_package_version_file(
    "vamos-hyper-version.cmake"
    VERSION ${vamos-hyper_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/vamos-hyper/vamos-hyper-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/vamos-hyper/vamos-hyper-version.cmake"
  DESTINATION lib/cmake/vamos-hyper)

