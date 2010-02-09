add_executable (moorie main.cpp)

include_directories(
    ${V8_INCLUDE_DIR}
)

target_link_libraries (moorie moor  ${Libs} ${V8Libs})
install (TARGETS moorie DESTINATION bin)
