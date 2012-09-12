cmake_minimum_required(VERSION 2.8)

# Set up the environment.
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJ "cocosdenshion")

project(${PROJ})

# Clear variables for our current project.
set(${PROJ}_SOURCES)
set(${PROJ}_INCLUDE_DIRS)
set(${PROJ}_LINK_DIRS)
set(${PROJ}_LIBRARIES)

find_package(cocos2dx REQUIRED)

set(${PROJ}_INCLUDE_DIRS
	${${PROJ}_INCLUDE_DIRS}
	${SRC}
	${SRC}/include
	${cocos2dx_INCLUDE_DIRS})
link_directories(${PROJECT_BINARY_DIR}/cocos2dx)

# Determine the architecture.
set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
endif()

# Determine target platform.
# TODO: Add IOS and MacOSX
if(ANDROID)
	add_definitions(-DANDROID)
	set(${PROJ}_INCLUDE_DIRS
		${${PROJ}_INCLUDE_DIRS}
		${SRC}/android)

	set(${PROJ}_SOURCES
		${${PROJ}_SOURCES}
		${SRC}/android/SimpleAudioEngine.cpp
		${SRC}/android/jni/SimpleAudioEngineJni.cpp
		${SRC}/android/opensl/OpenSLEngine.cpp
		${SRC}/android/opensl/SimpleAudioEngineOpenSL.cpp)
elseif(UNIX)
	add_definitions(-DLINUX)
	set(${PROJ}_INCLUDE_DIRS
		${${PROJ}_INCLUDE_DIRS}
		${SRC}/linux)

	if (bitness EQUAL 64)
		# Include 64-bit FMOD
		set(${PROJ}_INCLUDE_DIRS
			${${PROJ}_INCLUDE_DIRS}
			${SRC}/third_party/fmod/lib64/api/inc)
		link_directories(${SRC}/third_party/fmod/lib64/api/lib)
	else()
		# Include 32-bit FMOD
		set(${PROJ}_INCLUDE_DIRS
			${${PROJ}_INCLUDE_DIRS}
			${SRC}/hird_party/fmod/lib/api/inc)
		link_directories(${SRC}/third_party/fmod/lib/api/lib)
	endif()
	set(${PROJ}_SOURCES
		${${PROJ}_SOURCES}
		${SRC}/linux/FmodAudioPlayer.cpp
		${SRC}/linux/SimpleAudioEngine.cpp
	)
endif()

if (bitness EQUAL 64)
	set(${PROJ}_LIBRARIES
		${${PROJ}_LIBRARIES}
		fmodex64)
else()
	set(${PROJ}_LIBRARIES
		${${PROJ}_LIBRARIES}
		fmodex)
endif()

# Create the actual target.
include_directories(${${PROJ}_INCLUDE_DIRS})
add_library(${PROJ} ${${PROJ}_SOURCES})
target_link_libraries(${PROJ} ${${PROJ}_LIBRARIES})

message(STATUS "PROJ: " ${PROJ})
message(STATUS "SRC: " ${SRC})

foreach(item ${${PROJ}_SOURCES})
    message(STATUS ${PROJ} "_SOURCES: " ${item})
endforeach(item)

foreach(item ${${PROJ}_INCLUDE_DIRS})
    message(STATUS ${PROJ} "_INCLUDE_DIRS: " ${item})
endforeach(item)

foreach(item ${${PROJ}_LIBRARIES})
    message(STATUS ${PROJ} "_LIBRARIES: " ${item})
endforeach(item)

# Create a module configuration file for this project.
configure_file(${CMAKE_SOURCE_DIR}/CMake/Modules/FindModule.cmake.in
	${CMAKE_BINARY_DIR}/CMake/Modules/Find${PROJ}.cmake)