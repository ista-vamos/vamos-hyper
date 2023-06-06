#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_subdirectory(src)
add_subdirectory(tests)

#set(MONITOR_MAIN src/main.cpp)
#add_executable(monitor ${MONITOR_MAIN} monitor_inputs-onetrace.cpp)
#target_include_directories(monitor PUBLIC ${vamos-buffers_INCLUDE_DIRS})
#target_link_libraries(monitor PRIVATE monitor-od)
#target_compile_definitions(monitor
#                           #PRIVATE RAND_SEED=0xdeed
#                           PRIVATE NUM_STREAMS=2
#                           PRIVATE STREAM_LEN=10
#                           PRIVATE REDUCT_SYMMETRY=1
#                           PRIVATE REDUCT_REFLEXIVITY=1
#                           )

add_subdirectory(bin)

