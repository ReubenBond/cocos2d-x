#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find LIBCURL
# Once done, this will define
#
#  LIBCURL_FOUND - system has FreeImage
#  LIBCURL_INCLUDE_DIRS - the FreeImage include directories 
#  LIBCURL_LIBRARIES - link these to use FreeImage

set(LIBCURL_LIBRARY_NAMES libcurl_imp)
set(LIBCURL_LIBRARY_NAMES_DBG libcurl_imp)

find_path(LIBCURL_INCLUDE_DIR NAMES curl/curl.h)
find_library(LIBCURL_LIBRARY_REL NAMES ${LIBCURL_LIBRARY_NAMES} PATH_SUFFIXES "" release relwithdebinfo minsizerel)
find_library(LIBCURL_LIBRARY_DBG NAMES ${LIBCURL_LIBRARY_NAMES_DBG} PATH_SUFFIXES "" debug)

# Couple a set of release AND debug libraries (or frameworks)
if (LIBCURL_LIBRARY_REL AND LIBCURL_LIBRARY_DBG)
	set(LIBCURL_LIBRARY optimized ${LIBCURL_LIBRARY_REL} debug ${LIBCURL_LIBRARY_DBG})
elseif (LIBCURL_LIBRARY_REL)
	set(LIBCURL_LIBRARY ${LIBCURL_LIBRARY_REL})
elseif (LIBCURL_LIBRARY_DBG)
	set(LIBCURL_LIBRARY ${LIBCURL_LIBRARY_DBG})
endif ()

if (NOT LIBCURL_FOUND)
	if (LIBCURL_INCLUDE_DIR AND LIBCURL_LIBRARY)
		set(LIBCURL_FOUND TRUE)
		set(LIBCURL_INCLUDE_DIRS ${LIBCURL_INCLUDE_DIR})
		set(LIBCURL_LIBRARIES ${LIBCURL_LIBRARY})
		if (NOT LIBCURL_FIND_QUIETLY)
			message(STATUS "Found LIBCURL: ${LIBCURL_LIBRARIES}")
		endif ()
		else ()
			if (NOT LIBCURL_FIND_QUIETLY)
			message(STATUS "Could not locate LIBCURL")
		endif ()
		if (LIBCURL_FIND_REQUIRED)
			message(FATAL_ERROR "Required library LIBCURL not found! Install the library (including dev packages) and try again. If the library is already installed, set the missing variables manually in cmake.")
		endif ()
	endif ()
	mark_as_advanced(LIBCURL_INCLUDE_DIR LIBCURL_LIBRARY LIBCURL_LIBRARY_REL LIBCURL_LIBRARY_DBG)
endif ()

