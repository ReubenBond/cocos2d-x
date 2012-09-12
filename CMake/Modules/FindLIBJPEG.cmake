#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find LIBJPEG
# Once done, this will define
#
#  LIBJPEG_FOUND - system has FreeImage
#  LIBJPEG_INCLUDE_DIRS - the FreeImage include directories 
#  LIBJPEG_LIBRARIES - link these to use FreeImage

set(LIBJPEG_LIBRARY_NAMES libjpeg)
set(LIBJPEG_LIBRARY_NAMES_DBG libjpeg)

find_path(LIBJPEG_INCLUDE_DIR NAMES jpeglib.h)
find_library(LIBJPEG_LIBRARY_REL NAMES ${LIBJPEG_LIBRARY_NAMES} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(LIBJPEG_LIBRARY_DBG NAMES ${LIBJPEG_LIBRARY_NAMES_DBG} PATH_SUFFIXES "" debug)

# Couple a set of release AND debug libraries (or frameworks)
if (LIBJPEG_LIBRARY_REL AND LIBJPEG_LIBRARY_DBG)
	set(LIBJPEG_LIBRARY optimized ${LIBJPEG_LIBRARY_REL} debug ${LIBJPEG_LIBRARY_DBG})
elseif (LIBJPEG_LIBRARY_REL)
	set(LIBJPEG_LIBRARY ${LIBJPEG_LIBRARY_REL})
elseif (LIBJPEG_LIBRARY_DBG)
	set(LIBJPEG_LIBRARY ${LIBJPEG_LIBRARY_DBG})
endif ()

if (NOT LIBJPEG_FOUND)
	if (LIBJPEG_INCLUDE_DIR AND LIBJPEG_LIBRARY)
		set(LIBJPEG_FOUND TRUE)
		set(LIBJPEG_INCLUDE_DIRS ${LIBJPEG_INCLUDE_DIR})
		set(LIBJPEG_LIBRARIES ${LIBJPEG_LIBRARY})
		if (NOT LIBJPEG_FIND_QUIETLY)
			message(STATUS "Found LIBJPEG: ${LIBJPEG_LIBRARIES}")
		endif ()
		else ()
			if (NOT LIBJPEG_FIND_QUIETLY)
			message(STATUS "Could not locate LIBJPEG")
		endif ()
		if (LIBJPEG_FIND_REQUIRED)
			message(FATAL_ERROR "Required library LIBJPEG not found! Install the library (including dev packages) and try again. If the library is already installed, set the missing variables manually in cmake.")
		endif ()
	endif ()
	mark_as_advanced(LIBJPEG_INCLUDE_DIR LIBJPEG_LIBRARY LIBJPEG_LIBRARY_REL LIBJPEG_LIBRARY_DBG)
endif ()

