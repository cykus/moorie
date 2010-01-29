find_package(Boost COMPONENTS regex thread filesystem program_options REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable (moorie main.cpp)

target_link_libraries (moorie moor
    mcrypt
    mhash 
    ${CURL_LIBRARIES} 
    ${OPENSSL_LIBRARIES} 
    ${Boost_THREAD_LIBRARY} 
    ${Boost_REGEX_LIBRARY} 
    ${Boost_FILESYSTEM_LIBRARY} 
    ${Boost_PROGRAM_OPTIONS_LIBRARY})
install (TARGETS moorie DESTINATION bin)
