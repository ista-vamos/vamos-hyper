#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_subdirectory(src)
add_subdirectory(src-gps)
add_subdirectory(tests)

add_executable(monitor-gps monitor_msr_gps_main.cpp monitor_inputs_msr_gps.cpp)
target_include_directories(monitor-gps PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-gps PRIVATE monitor-gps-od)
target_compile_definitions(monitor-gps
                           #PRIVATE RAND_SEED=0xdeed
                           PRIVATE NUM_STREAMS=2
                           PRIVATE STREAM_LEN=10
                           PRIVATE REDUCT_SYMMETRY=1
                           PRIVATE REDUCT_REFLEXIVITY=1
                           )

add_subdirectory(bin)

