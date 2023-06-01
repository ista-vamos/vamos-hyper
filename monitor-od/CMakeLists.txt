#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_subdirectory(src)
add_subdirectory(tests)

set(MONITOR_MAIN src/main.cpp)

add_executable(monitor ${MONITOR_MAIN} monitor_inputs.cpp)
target_include_directories(monitor PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor PRIVATE monitor-od)

