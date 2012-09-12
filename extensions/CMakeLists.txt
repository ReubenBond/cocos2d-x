cmake_minimum_required(VERSION 2.8)

# Set up the environment
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJ "extension")

project(${PROJ})

# Clear variables for our current project.
set(${PROJ}_SOURCES)
set(${PROJ}_INCLUDE_DIRS)
set(${PROJ}_LIBRARIES)

find_package(cocos2dx REQUIRED)

# Include Cocos2d-x includes.
set(${PROJ}_INCLUDE_DIRS
	${${PROJ}_INCLUDE_DIRS}
	${cocos2dx_INCLUDE_DIRS})
link_directories(${PROJECT_BINARY_DIR}/cocos2dx)

set(${PROJ}_SOURCES
	${${PROJ}_SOURCES}
	CCBReader/CCBFileLoader.cpp
	CCBReader/CCBReader.cpp
	CCBReader/CCControlButtonLoader.cpp
	CCBReader/CCControlLoader.cpp
	CCBReader/CCLabelBMFontLoader.cpp
	CCBReader/CCLabelTTFLoader.cpp
	CCBReader/CCLayerColorLoader.cpp
	CCBReader/CCLayerGradientLoader.cpp
	CCBReader/CCLayerLoader.cpp
	CCBReader/CCMenuItemImageLoader.cpp
	CCBReader/CCMenuItemLoader.cpp
	CCBReader/CCNodeLoader.cpp
	CCBReader/CCNodeLoaderLibrary.cpp
	CCBReader/CCParticleSystemQuadLoader.cpp
	CCBReader/CCScale9SpriteLoader.cpp
	CCBReader/CCScrollViewLoader.cpp
	CCBReader/CCSpriteLoader.cpp

	GUI/CCControlExtension/CCControl.cpp
	GUI/CCControlExtension/CCControlButton.cpp
	GUI/CCControlExtension/CCControlColourPicker.cpp
	GUI/CCControlExtension/CCControlHuePicker.cpp
	GUI/CCControlExtension/CCControlSaturationBrightnessPicker.cpp
	GUI/CCControlExtension/CCControlSlider.cpp
	GUI/CCControlExtension/CCControlSwitch.cpp
	GUI/CCControlExtension/CCControlUtils.cpp
	GUI/CCControlExtension/CCInvocation.cpp
	GUI/CCControlExtension/CCMenuPassive.cpp
	GUI/CCControlExtension/CCScale9Sprite.cpp
	GUI/CCControlExtension/CCSpacer.cpp
	GUI/CCEditBox/CCEditBox.cpp
	GUI/CCEditBox/CCEditBoxImplAndroid.cpp
	GUI/CCScrollView/CCScrollView.cpp

	network/HttpClient.cpp)

# Create the actual target.
include_directories(${PROJ}_INCLUDE_DIRS)
add_library(${PROJ} ${${PROJ}_SOURCES})
target_link_libraries(${PROJ} ${PROJ}_LIBRARIES)

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

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Create a module configuration file for this project.
configure_file(${CMAKE_SOURCE_DIR}/CMake/Modules/FindModule.cmake.in
	${CMAKE_BINARY_DIR}/CMake/Modules/Find${PROJ}.cmake)