cmake_minimum_required(VERSION 2.8)

# Set up the environment.
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJ "HelloCpp")

project(${PROJ})

# Clear variables for our current project.
set(${PROJ}_SOURCES)
set(${PROJ}_INCLUDE_DIRS)
set(${PROJ}_LIBRARIES)
set(${PROJ}_RESOURCES)
set(${PROJ}_RESOURCE_OUTPUT)

# Resources such as media.
set(${PROJ}_RESOURCES
	${${PROJ}_RESOURCES}
	${SRC}/Resources/iphonehd/CloseNormal.png
	${SRC}/Resources/iphonehd/CloseSelected.png
	${SRC}/Resources/iphonehd/HelloWorld.png)

set(${PROJ}_RESOURCE_OUTPUT
	${PROJECT_BINARY_DIR})

find_package(cocos2dx REQUIRED)

set(${PROJ}_INCLUDE_DIRS
	${${PROJ}_INCLUDE_DIRS}
	${SRC}/Classes
	${cocos2dx_INCLUDE_DIRS})
link_directories(${PROJECT_BINARY_DIR}/cocos2dx)

set(${PROJ}_LIBRARIES
	${${PROJ}_LIBRARIES}
	${cocos2dx_LIBRARIES})

# Determine the architecture.
set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
endif()

# Determine target platform.
# TODO: Add IOS and MacOSX
if(ANDROID)
	add_definitions(-DANDROID)
elseif(UNIX)
	add_definitions(-DLINUX)
	set(${PROJ}_SOURCES
		${${PROJ}_SOURCES}
		${SRC}/proj.linux/main.cpp)
endif()

set(${PROJ}_SOURCES
	${${PROJ}_SOURCES}
	${SRC}/Classes/AppDelegate.cpp
	${SRC}/Classes/HelloWorldScene.cpp)

# Create the actual target.
include_directories(${${PROJ}_INCLUDE_DIRS})
add_executable(${PROJ} ${${PROJ}_SOURCES})
target_link_libraries(${PROJ} ${${PROJ}_LIBRARIES})

# Copy all resources.
#TODO: This should probably be done via INSTALL
foreach(resource ${${PROJ}_RESOURCES})
	get_filename_component(filename ${resource} NAME)
	configure_file(${resource}
		${${PROJ}_RESOURCE_OUTPUT}/${filename}
		COPYONLY)
endforeach(resource)

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