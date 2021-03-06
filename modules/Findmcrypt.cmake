# - Find mash
# Find the native MCRYPT includes and library
#
#  MCRYPT_INCLUDE_DIR - where to find MCRYPT.h, etc.
#  MCRYPT_LIBRARIES   - List of libraries when using MCRYPT.
#  MCRYPT_FOUND       - True if mcrypt found.


IF (MCRYPT_INCLUDE_DIR)
  # Already in cache, be silent
  SET(MCRYPT_FIND_QUIETLY TRUE)
ENDIF (MCRYPT_INCLUDE_DIR)

FIND_PATH(MCRYPT_INCLUDE_DIR mcrypt.h)

SET(MCRYPT_NAMES mcrypt)
FIND_LIBRARY(MCRYPT_LIBRARY NAMES ${MCRYPT_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set MCRYPT_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MCRYPT DEFAULT_MSG MCRYPT_LIBRARY MCRYPT_INCLUDE_DIR)

IF(MCRYPT_FOUND)
  SET( MCRYPT_LIBRARIES ${MCRYPT_LIBRARY} )
ELSE(MCRYPT_FOUND)
  SET( MCRYPT_LIBRARIES )
ENDIF(MCRYPT_FOUND)
MARK_AS_ADVANCED( MCRYPT_LIBRARY MCRYPT_INCLUDE_DIR )
