cmake_minimum_required(VERSION 2.8)

project(cocos2d)

SET(CMAKE_INSTALL_PREFIX "${cocos2d_SOURCE_DIR}/dist" CACHE PATH "install prefix" FORCE)

set(CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/CMake/Modules"
	"${CMAKE_BINARY_DIR}/CMake/Modules/"
	${CMAKE_MODULE_PATH}
)

# Determine the architecture.
set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
endif()
message("Bitness is ${bitness}")
set(cocos2dx_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cocos2dx)
add_subdirectory(cocos2dx)
add_subdirectory(extensions)
add_subdirectory(CocosDenshion)
add_subdirectory(samples/HelloCpp)
