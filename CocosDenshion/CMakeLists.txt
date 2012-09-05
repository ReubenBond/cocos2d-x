cmake_minimum_required(VERSION 2.8)

project (cocosdenshion)

include_directories(include)

# Determine the architecture.
set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
endif()
message("Bitness is ${bitness}")


include_directories(${PROJECT_SOURCE_DIR}/include)

if(ANDROID)
	add_definitions(-DANDROID)
	include_directories(
		${PROJECT_SOURCE_DIR}/android
		${PROJECT_SOURCE_DIR}/../cocos2dx
		${PROJECT_SOURCE_DIR}/../cocos2dx/include
		${PROJECT_SOURCE_DIR}/../cocos2dx/platform/android)
	set(COCOSDENSION_PLATFORM_SOURCES
		${PROJECT_SOURCE_DIR}/android/SimpleAudioEngine.cpp
		${PROJECT_SOURCE_DIR}/android/jni/SimpleAudioEngineJni.cpp
		${PROJECT_SOURCE_DIR}/android/opensl/OpenSLEngine.cpp
		${PROJECT_SOURCE_DIR}/android/opensl/SimpleAudioEngineOpenSL.cpp
	)
elseif(UNIX)
	add_definitions(-DLINUX)
	include_directories(${PROJECT_SOURCE_DIR}/linux)
	if (bitness EQUAL 64)
		# Include 64-bit FMOD
		include_directories(${PROJECT_SOURCE_DIR}/third_party/fmod/lib64/api/inc)
		link_directories(${PROJECT_SOURCE_DIR}/third_party/fmod/lib64/api/lib)
	else()
		# Include 32-bit FMOD
		include_directories(${PROJECT_SOURCE_DIR}/third_party/fmod/lib/api/inc)
		link_directories(${PROJECT_SOURCE_DIR}/third_party/fmod/lib/api/lib)
	endif()
	set(COCOSDENSION_PLATFORM_SOURCES
		linux/FmodAudioPlayer.cpp
		linux/SimpleAudioEngine.cpp
	)
endif()

add_library(cocosdenshion ${COCOSDENSHION_SOURCES} ${COCOSDENSION_PLATFORM_SOURCES})

if (bitness EQUAL 64)
	target_link_libraries(cocosdenshion fmodex64)
else()
	target_link_libraries(cocosdenshion fmodex)
endif()