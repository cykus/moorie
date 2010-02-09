add_executable (moorie main.cpp)

target_link_libraries (moorie moor  ${Libs} ${V8Libs})
install (TARGETS moorie DESTINATION bin)
