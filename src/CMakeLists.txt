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

SET(V8Libs)

IF(V8_FOUND)
    SET(V8Libs ${V8_LIBRARY})
ELSE()
    SET(V8Libs ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/libv8.so)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/src/libmoor/Libraries/libv8.so DESTINATION lib${LIB_SUFFIX})
ENDIF()

add_subdirectory (libmoor)
add_subdirectory (moor_bin)
if(ENABLE_GUI)
	add_subdirectory (moor_gui)
endif(ENABLE_GUI)


