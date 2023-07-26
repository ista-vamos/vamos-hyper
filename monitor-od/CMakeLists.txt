#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_subdirectory(src)
add_subdirectory(src-gps)
add_subdirectory(tests)
add_subdirectory(bin)

