cmake_minimum_required(VERSION 2.8)

# Set up the environment.
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJ "cocos2dx")

project(${PROJ})

# Clear variables for our current project.
set(${PROJ}_SOURCES)
set(${PROJ}_INCLUDE_DIRS)
set(${PROJ}_LIBRARIES)

# Find required libraries.
find_package(EGL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(LibXml2 REQUIRED)

if(ANDROID)
	#TODO: Move these into /platform/android/CMakeLists.txt
	add_definitions(-DANDROID)

	# Platform-specific includes.
	set(${PROJ}_INCLUDE_DIRS
		${${PROJ}_INCLUDE_DIRS}
		${SRC}/platform/android)

	# Platform-specific sources.
	set(${PROJ}_SOURCES
		${${PROJ}_SOURCES}
		${SRC}/platform/android/CCEGLView.cpp
		${SRC}/platform/android/CCAccelerometer.cpp
		${SRC}/platform/android/CCApplication.cpp
		${SRC}/platform/android/CCCommon.cpp
		${SRC}/platform/android/CCFileUtils.cpp
		${SRC}/platform/android/CCImage.cpp
		${SRC}/platform/android/jni/EditBoxJni.cpp
		${SRC}/platform/android/jni/JniHelper.cpp
		${SRC}/platform/android/jni/IMEJni.cpp
		${SRC}/platform/android/jni/MessageJni.cpp
		${SRC}/platform/android/jni/SensorJni.cpp
		${SRC}/platform/android/jni/SystemInfoJni.cpp
		${SRC}/platform/android/jni/TouchesJni.cpp)
elseif(APPLE)
elseif(UNIX)
	#TODO: Move these into /platform/linux/CMakeLists.txt
	add_definitions(-DLINUX)
	find_package(Freetype REQUIRED)

	# Platform-specific includes.
	set(${PROJ}_INCLUDE_DIRS
		${${PROJ}_INCLUDE_DIRS}
		${SRC}/platform/linux
		${FREETYPE_INCLUDE_DIRS})

	# Platform-specific libraries.
	set(${PROJ}_LIBRARIES
		${${PROJ}_LIBRARIES}
		${FREETYPE_LIBRARIES})

	# Platform-specific sources.
	set(${PROJ}_SOURCES
		${${PROJ}_SOURCES}
		${SRC}/platform/linux/CCApplication.cpp
		${SRC}/platform/linux/CCCommon.cpp
		${SRC}/platform/linux/CCEGLView.cpp
		${SRC}/platform/linux/CCFileUtils.cpp
		${SRC}/platform/linux/CCImage.cpp
		${SRC}/platform/linux/CCStdC.cpp)
elseif(WIN32)
	add_definitions(-DWIN32)
endif()

set(${PROJ}_INCLUDE_DIRS
	${${PROJ}_INCLUDE_DIRS}
	${SRC}
	${SRC}/include
	${SRC}/kazmath/include)

set(${PROJ}_SOURCES
	${${PROJ}_SOURCES}
	${SRC}/CCConfiguration.cpp
	${SRC}/CCDrawingPrimitives.cpp
	${SRC}/CCScheduler.cpp
	${SRC}/CCCamera.cpp
	${SRC}/actions/CCAction.cpp
	${SRC}/actions/CCActionCamera.cpp
	${SRC}/actions/CCActionCatmullRom.cpp
	${SRC}/actions/CCActionEase.cpp
	${SRC}/actions/CCActionGrid.cpp
	${SRC}/actions/CCActionGrid3D.cpp
	${SRC}/actions/CCActionInstant.cpp
	${SRC}/actions/CCActionInterval.cpp
	${SRC}/actions/CCActionManager.cpp
	${SRC}/actions/CCActionPageTurn3D.cpp
	${SRC}/actions/CCActionProgressTimer.cpp
	${SRC}/actions/CCActionTiledGrid.cpp
	${SRC}/actions/CCActionTween.cpp
	${SRC}/base_nodes/CCAtlasNode.cpp
	${SRC}/base_nodes/CCNode.cpp
	${SRC}/cocoa/CCAffineTransform.cpp
	${SRC}/cocoa/CCGeometry.cpp
	${SRC}/cocoa/CCAutoreleasePool.cpp
	${SRC}/cocoa/CCDictionary.cpp
	${SRC}/cocoa/CCNS.cpp
	${SRC}/cocoa/CCObject.cpp
	${SRC}/cocoa/CCSet.cpp
	${SRC}/cocoa/CCString.cpp
	${SRC}/cocoa/CCZone.cpp
	${SRC}/cocoa/CCArray.cpp
	${SRC}/cocos2d.cpp
	${SRC}/CCDirector.cpp
	${SRC}/effects/CCGrabber.cpp
	${SRC}/effects/CCGrid.cpp
	${SRC}/kazmath/src/aabb.c
	${SRC}/kazmath/src/mat3.c
	${SRC}/kazmath/src/mat4.c
	${SRC}/kazmath/src/neon_matrix_impl.c
	${SRC}/kazmath/src/plane.c
	${SRC}/kazmath/src/quaternion.c
	${SRC}/kazmath/src/ray2.c
	${SRC}/kazmath/src/utility.c
	${SRC}/kazmath/src/vec2.c
	${SRC}/kazmath/src/vec3.c
	${SRC}/kazmath/src/vec4.c
	${SRC}/kazmath/src/GL/mat4stack.c
	${SRC}/kazmath/src/GL/matrix.c
	${SRC}/keypad_dispatcher/CCKeypadDelegate.cpp
	${SRC}/keypad_dispatcher/CCKeypadDispatcher.cpp
	${SRC}/label_nodes/CCLabelAtlas.cpp
	${SRC}/label_nodes/CCLabelBMFont.cpp
	${SRC}/label_nodes/CCLabelTTF.cpp
	${SRC}/layers_scenes_transitions_nodes/CCLayer.cpp
	${SRC}/layers_scenes_transitions_nodes/CCScene.cpp
	${SRC}/layers_scenes_transitions_nodes/CCTransitionPageTurn.cpp
	${SRC}/layers_scenes_transitions_nodes/CCTransition.cpp
	${SRC}/layers_scenes_transitions_nodes/CCTransitionProgress.cpp
	${SRC}/menu_nodes/CCMenu.cpp
	${SRC}/menu_nodes/CCMenuItem.cpp
	${SRC}/misc_nodes/CCMotionStreak.cpp
	${SRC}/misc_nodes/CCProgressTimer.cpp
	${SRC}/misc_nodes/CCRenderTexture.cpp
	${SRC}/particle_nodes/CCParticleExamples.cpp
	${SRC}/particle_nodes/CCParticleSystem.cpp
	${SRC}/particle_nodes/CCParticleBatchNode.cpp
	${SRC}/particle_nodes/CCParticleSystemQuad.cpp
	${SRC}/platform/CCSAXParser.cpp
	${SRC}/platform/CCThread.cpp
	${SRC}/platform/platform.cpp
	${SRC}/platform/CCEGLViewProtocol.cpp
	${SRC}/script_support/CCScriptSupport.cpp
	${SRC}/shaders/ccShaders.cpp
	${SRC}/shaders/CCGLProgram.cpp
	${SRC}/shaders/ccGLStateCache.cpp
	${SRC}/shaders/CCShaderCache.cpp
	${SRC}/sprite_nodes/CCAnimation.cpp
	${SRC}/sprite_nodes/CCAnimationCache.cpp
	${SRC}/sprite_nodes/CCSprite.cpp
	${SRC}/sprite_nodes/CCSpriteBatchNode.cpp
	${SRC}/sprite_nodes/CCSpriteFrame.cpp
	${SRC}/sprite_nodes/CCSpriteFrameCache.cpp
	${SRC}/support/CCNotificationCenter.cpp
	${SRC}/support/CCProfiling.cpp
	${SRC}/support/CCPointExtension.cpp
	${SRC}/support/TransformUtils.cpp
	${SRC}/support/CCUserDefault.cpp
	${SRC}/support/base64.cpp
	${SRC}/support/ccUtils.cpp
	${SRC}/support/CCVertex.cpp
	${SRC}/support/data_support/ccCArray.cpp
	${SRC}/support/image_support/TGAlib.cpp
	${SRC}/support/zip_support/ZipUtils.cpp
	${SRC}/support/zip_support/ioapi.cpp
	${SRC}/support/zip_support/unzip.cpp
	${SRC}/text_input_node/CCIMEDispatcher.cpp
	${SRC}/text_input_node/CCTextFieldTTF.cpp
	${SRC}/textures/CCTexture2D.cpp
	${SRC}/textures/CCTextureAtlas.cpp
	${SRC}/textures/CCTextureCache.cpp
	${SRC}/textures/CCTexturePVR.cpp
	${SRC}/tilemap_parallax_nodes/CCParallaxNode.cpp
	${SRC}/tilemap_parallax_nodes/CCTMXLayer.cpp
	${SRC}/tilemap_parallax_nodes/CCTMXObjectGroup.cpp
	${SRC}/tilemap_parallax_nodes/CCTMXTiledMap.cpp
	${SRC}/tilemap_parallax_nodes/CCTMXXMLParser.cpp
	${SRC}/tilemap_parallax_nodes/CCTileMapAtlas.cpp
	${SRC}/touch_dispatcher/CCTouchDispatcher.cpp
	${SRC}/touch_dispatcher/CCTouchHandler.cpp
	${SRC}/touch_dispatcher/CCTouch.cpp)

set(${PROJ}_INCLUDE_DIRS
	${${PROJ}_INCLUDE_DIRS}
	${ZLIB_INCLUDES}
	${LIBXML2_INCLUDE_DIR})

set(${PROJ}_LIBRARIES
	${${PROJ}_LIBRARIES}
	GLESv2 #FIXME
	${EGL_LIBRARIES}
	log #FIXME
	${ZLIB_LIBRARIES}
	${LIBXML2_LIBRARIES}
	${COCOS2DX_PLATFORM_LIBRARIES})

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