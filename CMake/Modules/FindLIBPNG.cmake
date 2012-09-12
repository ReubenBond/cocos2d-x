#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find LIBPNG
# Once done, this will define
#
#  LIBPNG_FOUND - system has FreeImage
#  LIBPNG_INCLUDE_DIRS - the FreeImage include directories 
#  LIBPNG_LIBRARIES - link these to use FreeImage

set(LIBPNG_LIBRARY_NAMES libpng png)
set(LIBPNG_LIBRARY_NAMES_DBG libpng png)

find_path(LIBPNG_INCLUDE_DIR NAMES png.h)
find_library(LIBPNG_LIBRARY_REL NAMES ${LIBPNG_LIBRARY_NAMES} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(LIBPNG_LIBRARY_DBG NAMES ${LIBPNG_LIBRARY_NAMES_DBG} PATH_SUFFIXES "" debug)

# Couple a set of release AND debug libraries (or frameworks)
if (LIBPNG_LIBRARY_REL AND LIBPNG_LIBRARY_DBG)
	set(LIBPNG_LIBRARY optimized ${LIBPNG_LIBRARY_REL} debug ${LIBPNG_LIBRARY_DBG})
elseif (LIBPNG_LIBRARY_REL)
	set(LIBPNG_LIBRARY ${LIBPNG_LIBRARY_REL})
elseif (LIBPNG_LIBRARY_DBG)
	set(LIBPNG_LIBRARY ${LIBPNG_LIBRARY_DBG})
endif ()

if (NOT LIBPNG_FOUND)
	if (LIBPNG_INCLUDE_DIR AND LIBPNG_LIBRARY)
		set(LIBPNG_FOUND TRUE)
		set(LIBPNG_INCLUDE_DIRS ${LIBPNG_INCLUDE_DIR})
		set(LIBPNG_LIBRARIES ${LIBPNG_LIBRARY})
		if (NOT LIBPNG_FIND_QUIETLY)
			message(STATUS "Found LIBPNG: ${LIBPNG_LIBRARIES}")
		endif ()
		else ()
			if (NOT LIBPNG_FIND_QUIETLY)
			message(STATUS "Could not locate LIBPNG")
		endif ()
		if (LIBPNG_FIND_REQUIRED)
			message(FATAL_ERROR "Required library LIBPNG not found! Install the library (including dev packages) and try again. If the library is already installed, set the missing variables manually in cmake.")
		endif ()
	endif ()
	mark_as_advanced(LIBPNG_INCLUDE_DIR LIBPNG_LIBRARY LIBPNG_LIBRARY_REL LIBPNG_LIBRARY_DBG)
endif ()

