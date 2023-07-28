add_executable(monitor-rvhyper ../src/main_argv.cpp ../monitor_inputs_rvhyper.cpp)
target_include_directories(monitor-rvhyper PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-rvhyper PRIVATE monitor-od)

