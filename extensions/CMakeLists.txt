cmake_minimum_required(VERSION 2.8)

# Set up the environment.
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJ "extension")

project(${PROJ})

# Clear variables for our current project.
set(${PROJ}_SOURCES)
set(${PROJ}_INCLUDE_DIRS)
set(${PROJ}_LIBRARIES)

find_package(cocos2dx REQUIRED)

set(${PROJ}_INCLUDE_DIRS
	${${PROJ}_INCLUDE_DIRS}
	${SRC}
	${cocos2dx_INCLUDE_DIRS})
link_directories(${PROJECT_BINARY_DIR}/cocos2dx)

set(${PROJ}_SOURCES
	${${PROJ}_SOURCES}
	${SRC}/CCBReader/CCBFileLoader.cpp
	${SRC}/CCBReader/CCBReader.cpp
	${SRC}/CCBReader/CCControlButtonLoader.cpp
	${SRC}/CCBReader/CCControlLoader.cpp
	${SRC}/CCBReader/CCLabelBMFontLoader.cpp
	${SRC}/CCBReader/CCLabelTTFLoader.cpp
	${SRC}/CCBReader/CCLayerColorLoader.cpp
	${SRC}/CCBReader/CCLayerGradientLoader.cpp
	${SRC}/CCBReader/CCLayerLoader.cpp
	${SRC}/CCBReader/CCMenuItemImageLoader.cpp
	${SRC}/CCBReader/CCMenuItemLoader.cpp
	${SRC}/CCBReader/CCNodeLoader.cpp
	${SRC}/CCBReader/CCNodeLoaderLibrary.cpp
	${SRC}/CCBReader/CCParticleSystemQuadLoader.cpp
	${SRC}/CCBReader/CCScale9SpriteLoader.cpp
	${SRC}/CCBReader/CCScrollViewLoader.cpp
	${SRC}/CCBReader/CCSpriteLoader.cpp

	${SRC}/GUI/CCControlExtension/CCControl.cpp
	${SRC}/GUI/CCControlExtension/CCControlButton.cpp
	${SRC}/GUI/CCControlExtension/CCControlColourPicker.cpp
	${SRC}/GUI/CCControlExtension/CCControlHuePicker.cpp
	${SRC}/GUI/CCControlExtension/CCControlSaturationBrightnessPicker.cpp
	${SRC}/GUI/CCControlExtension/CCControlSlider.cpp
	${SRC}/GUI/CCControlExtension/CCControlSwitch.cpp
	${SRC}/GUI/CCControlExtension/CCControlUtils.cpp
	${SRC}/GUI/CCControlExtension/CCInvocation.cpp
	${SRC}/GUI/CCControlExtension/CCMenuPassive.cpp
	${SRC}/GUI/CCControlExtension/CCScale9Sprite.cpp
	${SRC}/GUI/CCControlExtension/CCSpacer.cpp

	${SRC}/GUI/CCEditBox/CCEditBox.cpp

	${SRC}/GUI/CCScrollView/CCScrollView.cpp

	${SRC}/network/HttpClient.cpp)

# Determine target platform.
# TODO: Add IOS and MacOSX
if(ANDROID)
	add_definitions(-DANDROID)
	set(${PROJ}_SOURCES
		${${PROJ}_SOURCES}
		${SRC}/GUI/CCEditBox/CCEditBoxImplAndroid.cpp)
elseif(APPLE)
elseif(UNIX)
	add_definitions(-DLINUX)
elseif(WIN32)
	add_definitions(-DWIN32)
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