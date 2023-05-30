add_executable(test2 test2.cpp)
target_include_directories(test2 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(test2 PRIVATE vamos-hyper vamos-buffers-event)

#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_executable(test3 test3.cpp)
target_include_directories(test3 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(test3 PRIVATE vamos-hyper vamos-buffers-event)

add_executable(monitor_od monitor_od.cpp monitor_inputs.cpp monitor_impl.cpp monitor_cfgs.cpp)
target_include_directories(monitor_od PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od PRIVATE vamos-hyper vamos-buffers-event)

