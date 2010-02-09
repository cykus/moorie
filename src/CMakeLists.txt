set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules/")

find_package(Boost COMPONENTS regex program_options thread filesystem REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(mcrypt REQUIRED)
find_package(mhash REQUIRED)
find_package(V8)
SET(Libs
    ${MCRYPT_LIBRARIES}
    ${MHASH_LIBRARIES}
    ${CURL_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${Boost_THREAD_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY})

SET(V8_INCLUDE_DIR)
SET(V8Libs)

IF(V8_FOUND)

    find_package(scons REQUIRED)
    find_package(Subversion REQUIRED)
    IF(Subversion_SVN_EXECUTABLE)
        MESSAGE(STATUS "Looking for subversion binary found")
        EXECUTE_PROCESS(COMMAND ${Subversion_SVN_EXECUTABLE} checkout http://v8.googlecode.com/svn/trunk/ ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/v8)
        FILE(COPY ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/SConstruct DESTINATION ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/v8)
        EXECUTE_PROCESS(COMMAND ${SCONS_BIN_PATH}/scons library=shared snapshot=on mode=release verbose=on WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/v8)
    ENDIF(Subversion_SVN_EXECUTABLE)
    SET(V8_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/v8/include)
    SET(V8Libs ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/v8/libv8.so)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/v8/libv8.so DESTINATION lib${LIB_SUFFIX})
ENDIF()


add_subdirectory (libmoor)
add_subdirectory (moor_bin)
if(ENABLE_GUI)
	add_subdirectory (moor_gui)
endif(ENABLE_GUI)


