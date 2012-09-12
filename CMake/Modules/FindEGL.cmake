# - Try to find ImageEGL
# Once done, this will define
#
#  EGL_FOUND - system has EGL
#  EGL_INCLUDE_DIRS - the EGL include directories
#  EGL_LIBRARIES - link these to use EGL

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(EGL_PKGCONF egl)

# Include dir
find_path(EGL_INCLUDE_DIR
  NAMES egl.h
  PATHS ${EGL_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(EGL_LIBRARY
  NAMES EGL
  PATHS ${EGL_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(EGL_PROCESS_INCLUDES EGL_INCLUDE_DIR EGL_INCLUDE_DIRS)
set(EGL_PROCESS_LIBS EGL_LIBRARY EGL_LIBRARIES)
libfind_process(egl)