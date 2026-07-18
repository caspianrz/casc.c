cmake_minimum_required(VERSION 3.16)

project(casc.c LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include("cmake/casc.c-version.cmake")

set(CAS_BUILD_TESTS OFF)
add_subdirectory(vendor/cashash.c)

set(casc.c_lib_name "casc.c")
set(casc.c_lib_cflags "")
set(casc.c_lib_libraries "")
set(casc.c_lib_defines "")
set(casc.c_lib_includes "")
set(casc.c_lib_build_targets "")
set(casc.c_lib_main "")

set(casc.c_install_objdir "${CMAKE_INSTALL_LIBDIR}")
set(casc.c_install_incdir "${CMAKE_INSTALL_INCLUDEDIR}")

# --- Options ---
option(CASC_C_LIB_BUILD_SHARED "Build shared library" ON)
option(CASC_C_LIB_BUILD_STATIC "Build static library" ON)
option(CASC_C_LIB_BUILD_OBJECT
	"Build object library"
	OFF
) # BUG: can't build object library fix this.
option(CASC_C_BUILD_EXE "Build executable" ON)

if(CASC_C_LIB_BUILD_SHARED)
	list(APPEND casc.c_lib_build_targets "shared")
endif()

if(CASC_C_LIB_BUILD_STATIC)
	list(APPEND casc.c_lib_build_targets "static")
endif()

if(CASC_C_LIB_BUILD_OBJECT)
	list(APPEND casc.c_lib_build_targets "object")
endif()

message(STATUS "")
message(STATUS "Library Name        : ${casc.c_lib_name}")
message(STATUS "Compiler            : ${CMAKE_C_COMPILER}")
message(STATUS "Link Libraries      : ${casc.c_lib_libraries}")
message(STATUS "Include Directories : ${casc.c_lib_includes}")
message(STATUS "Defines             : ${casc.c_lib_defines}")
message(STATUS "")

set(casc.c_lib_sources)
file(GLOB_RECURSE casc.c_lib_sources "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c")
message(STATUS "Library sources     : ${casc.c_lib_sources}")

# --- Library Builds ---
if(CASC_C_LIB_BUILD_SHARED)
	add_library(casc.c-shared SHARED ${casc.c_lib_sources})
	set_target_properties(casc.c-shared
		PROPERTIES
			VERSION ${casc.c_version}
			SOVERSION ${casc.c_version_major}
			OUTPUT_NAME ${casc.c_lib_name}
	)
	target_compile_definitions(casc.c-shared
		PRIVATE
			${casc.c_lib_defines}
			CASC_C_SHARED_LIB
	)
	target_compile_options(casc.c-shared
		PRIVATE
			${casc.c_lib_cflags}
			${casc.c_lib_cflags_shared}
	)
	target_link_libraries(casc.c-shared PRIVATE ${casc.c_lib_libraries})
	target_include_directories(casc.c-shared
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${casc.c_install_incdir}>
	)
	install(
		TARGETS casc.c-shared
		EXPORT casc.c
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)

	if("${casc.c_lib_main}" STREQUAL "")
		set(casc.c_lib_main "casc.c-shared")
	endif()
endif()

if(CASC_C_LIB_BUILD_STATIC)
	add_library(casc.c-static STATIC ${casc.c_lib_sources})
	set_property(TARGET casc.c-static PROPERTY OUTPUT_NAME ${casc.c_lib_name})
	set_property(TARGET casc.c-static PROPERTY POSITION_INDEPENDENT_CODE ON)
	target_compile_definitions(casc.c-static
		PRIVATE
			${casc.c_lib_defines}
			CASC_C_STATIC_LIB
	)
	target_compile_options(casc.c-static
		PRIVATE
			${casc.c_lib_cflags}
			${casc.c_lib_cflags_static}
	)
	target_link_libraries(casc.c-static PRIVATE ${casc.c_lib_libraries})
	target_include_directories(casc.c-static
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${casc.c_install_incdir}>
	)
	install(
		TARGETS casc.c-static
		EXPORT casc.c
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)

	if("${casc.c_lib_main}" STREQUAL "")
		set(casc.c_lib_main "casc.c-static")
	endif()
endif()

if(CASC_C_LIB_BUILD_OBJECT)
	add_library(casc.c-object OBJECT "static.c")
	set_property(TARGET casc.c-object PROPERTY POSITION_INDEPENDENT_CODE ON)
	target_compile_definitions(casc.c-object PRIVATE ${casc.c_lib_defines})
	target_compile_options(casc.c-object
		PRIVATE
			${casc.c_lib_cflags}
			${casc.c_lib_cflags_static}
	)
	target_link_libraries(casc.c-object PRIVATE ${casc.c_lib_libraries})
	target_include_directories(casc.c-object
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${casc.c_install_incdir}>
	)
	if(CMAKE_GENERATOR MATCHES "^Visual Studio.*$")
		set(casc.c-object-static
			"${CMAKE_CURRENT_BINARY_DIR}/casc.c-object.dir/$<CONFIG>/static${CMAKE_CXX_OUTPUT_EXTENSION}"
		)
	else()
		set(casc.c-object-static
			"${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/casc.c-object.dir/src/static.c${CMAKE_CXX_OUTPUT_EXTENSION}"
		)
	endif()
	set(casc.c-object-out
		"${CMAKE_CURRENT_BINARY_DIR}/${casc.c_lib_name}${CMAKE_C_OUTPUT_EXTENSION}"
	)
	add_custom_command(
		OUTPUT ${casc.c-object-out}
		DEPENDS casc.c-object
		COMMAND "${CMAKE_COMMAND}" -E copy "${casc.c-object-static}"
			"${casc.c-object-out}"
	)
	add_custom_target(casc.c-object-target ALL DEPENDS ${casc.c-object-out})

	install(
		FILES ${casc.c-object-static}
		DESTINATION ${casc.c_install_objdir}
		RENAME ${zolacasc.c_lib_namel_libname}${CMAKE_C_OUTPUT_EXTENSION}
	)

	if("${casc.c_lib_main}" STREQUAL "")
		set(casc.c_lib_main "casc.c-object")
	endif()
endif()

add_library(casc.c::casc.c ALIAS ${casc.c_lib_main})
target_link_libraries(${casc.c_lib_main} PRIVATE cashash::cashash)

# --- Compiler Executable ---
set(casc.c_exe_sources)
file(GLOB_RECURSE casc.c_exe_sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(casc.c ${casc.c_exe_sources})
target_link_libraries(casc.c PRIVATE casc.c::casc.c)
target_include_directories(casc.c
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${casc.c_install_incdir}>
)