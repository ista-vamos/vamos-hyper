#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_library(monitor-od STATIC monitor.cpp private.cpp cfgs.cpp)
target_include_directories(monitor-od PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-od PUBLIC vamos-hyper vamos-buffers-event)

