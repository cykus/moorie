add_executable (moorie main.cpp)

target_link_libraries (moorie moor
    ${MCRYPT_LIBRARIES}
    ${MHASH_LIBRARIES}
    ${CURL_LIBRARIES} 
    ${OPENSSL_LIBRARIES} 
    ${Boost_THREAD_LIBRARY} 
    ${Boost_REGEX_LIBRARY} 
    ${Boost_FILESYSTEM_LIBRARY} 
    ${Boost_PROGRAM_OPTIONS_LIBRARY})
install (TARGETS moorie DESTINATION bin)
