cmake_minimum_required(VERSION 3.12)
project(Common2 VERSION 0.0.1)

enable_language(CXX)

if("${CMAKE_CONFIGURATION_TYPES}" STREQUAL "Debug")
	add_compile_definitions(CONTAINER_BOUND_TEST=1)
	add_compile_definitions(CONTAINER_MEMORY_TEST=1)
	add_compile_definitions(SCENE_BOUND_TEST=1)
	add_compile_definitions(RENDER_BOUND_TEST=1)
	add_compile_definitions(COLLIDER_BOUND_TEST=1)
	add_compile_definitions(STANDARD_ALLOCATION_BOUND_TEST=1)
	add_compile_definitions(USE_OPTICK=0)
else()
	add_compile_definitions(CONTAINER_BOUND_TEST=0)
	add_compile_definitions(CONTAINER_MEMORY_TEST=0)
	add_compile_definitions(SCENE_BOUND_TEST=0)
	add_compile_definitions(RENDER_BOUND_TEST=0)
	add_compile_definitions(COLLIDER_BOUND_TEST=0)
	add_compile_definitions(STANDARD_ALLOCATION_BOUND_TEST=0)
	add_compile_definitions(USE_OPTICK=0)
endif()

add_library(Common2 INTERFACE)
target_include_directories(Common2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Common2Test "./test/test.cpp" )
target_include_directories(Common2Test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Common2Test PUBLIC Common2)