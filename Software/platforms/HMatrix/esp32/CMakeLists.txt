# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)


#set(EXTRA_COMPONENT_DIRS "../../../")
list(APPEND EXTRA_COMPONENT_DIRS "../../../")
#list(APPEND EXTRA_COMPONENT_DIRS "./impl/")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

#target_compile_options(${COMPONENT_TARGET} PUBLIC -DYOUR_LOVELY_DEFINITIONS)
#target_compile_definitions(${COMPONENT_TARGET} INTERFACE BAR)
add_compile_definitions(FOO)

#set(EXTRA_COMPONENT_DIRS "../../../os/")

project(LaLeMaTest)
