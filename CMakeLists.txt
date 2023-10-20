cmake_minimum_required(VERSION 3.21)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(resultpp LANGUAGES C CXX)

if (${CMAKE_SOURCE_DIR} STREQUAL {CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source build is not allowed, please create a separate build folder.")
endif ()

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(FATAL_ERROR "Cannot compile this project on 32-bit architecture")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

option(resultpp_COMPILE_COMMANDS "cmake export compile commands" ON)
option(resultpp_WARN_UNUSED "Enable warning for unused functions" ON)
option(resultpp_ENABLE_EXTRA_DEBUG "Enable extra flags for debugging" OFF)
option(resultpp_BUILD_EXAMPLES "Build example project for this library" OFF)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	message(STATUS "Enabling extra debug info ...")
	set(ENABLE_EXTRA_DEBUG ON)
	set(ENABLE_DEBUG_MODE 1)
endif ()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Debug' as none was specified.")
	set(CMAKE_BUILD_TYPE Debug
		CACHE STRING "Choose the type of build" FORCE)

	set_property(CACHE CMAKE_BUILD_TYPE
		PROPERTY STRINGS
		"Debug"
		"Release"
		"RelWithDebInfo")
endif ()

############################################################
# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

############################################################
# ccache
find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
	message(STATUS "Using ccache")
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif ()

############################################################
## print info
message(STATUS "build info:")
message(STATUS "  Host System: ${CMAKE_HOST_SYSTEM}")
message(STATUS "  Host System Name: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "  Host System Processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

include(CheckCXXCompilerFlag)

############################################################
# Set compiler flags
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

if (${WARN_UNUSED})
	set(CXX_EXTRA "${CXX_EXTRA} -Wunused")
endif ()

if (CMAKE_LTO)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
endif ()

if (${ENABLE_EXTRA_DEBUG})
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -DDEBUG -Wall -Wnarrowing -Wconversion -pedantic ${CXX_EXTRA}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops -fPIC ${CXX_EXTRA}")

check_cxx_compiler_flag(-fno-builtin HAVE_FNO_BUILTIN)
check_cxx_compiler_flag(-fstack-protector-strong HAS_STACK_PROTECTOR_STRONG)
check_cxx_compiler_flag(-march=native SUPPORTS_MARCH_NATIVE)
check_cxx_compiler_flag(-mtune=native SUPPORTS_MTUNE_NATIVE)

if (HAVE_FNO_BUILTIN)
	add_compile_options(-fno-builtin)
endif ()

if (HAS_STACK_PROTECTOR_STRONG)
	add_compile_options(-fstack-protector-strong)
	link_libraries(-fstack-protector-strong)
endif ()

if (SUPPORTS_MARCH_NATIVE)
	message(STATUS "Compiler supports compiling for native architecture")
	add_compile_options(-march=native)
endif ()

if (SUPPORTS_MTUNE_NATIVE)
	message(STATUS "Compiler supports tuning for native architecture")
	add_compile_options(-mtune=native)
endif ()

add_compile_options(-Wstrict-overflow=2)

# Configure output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

############################################################
# Common dependency
set(DEP_LIBRARIES)

############################################################
# resultpp :: library sources
set(
	resultpp_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

set(resultpp_SOURCES
	lib/resultpp.hxx
	lib/ResultImpl.hxx)

include_directories(${resultpp_INCLUDE_DIRS})
add_library(resultpp INTERFACE)
target_include_directories(resultpp INTERFACE ${resultpp_INCLUDE_DIRS})
target_link_libraries(resultpp INTERFACE ${DEP_LIBRARIES})

if (${resultpp_BUILD_EXAMPLES})
	add_subdirectory(examples)
endif ()

############################################################
# Unit testing
if (${resultpp_ENABLE_TESTING})
	message(STATUS "Enabling unit testing for this project")
	enable_testing()
	add_subdirectory(test)
endif ()