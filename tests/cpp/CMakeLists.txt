add_executable(test2 test2.cpp)
target_include_directories(test2 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(test2 PRIVATE vamos-hyper)

#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

