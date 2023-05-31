add_executable(test2 test2.cpp)
target_include_directories(test2 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(test2 PRIVATE vamos-hyper vamos-buffers-event)

#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_executable(test3 test3.cpp)
target_include_directories(test3 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(test3 PRIVATE vamos-hyper vamos-buffers-event)

set(OD_MONITOR monitor_main.cpp monitor_od.cpp monitor_impl.cpp monitor_cfgs.cpp)
set(OD_MONITOR_TEST monitor_test.cpp monitor_od.cpp monitor_impl.cpp monitor_cfgs.cpp)

add_executable(monitor_od ${OD_MONITOR} monitor_inputs.cpp)
target_include_directories(monitor_od PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od PRIVATE vamos-hyper vamos-buffers-event)

####
# OD TESTS
add_executable(monitor_od-true1 ${OD_MONITOR_TEST} monitor_inputs-true1.cpp)
target_include_directories(monitor_od-true1 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-true1 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-true1 COMMAND monitor_od-true1 true)

add_executable(monitor_od-true2 ${OD_MONITOR_TEST} monitor_inputs-true2.cpp)
target_include_directories(monitor_od-true2 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-true2 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-true2 COMMAND monitor_od-true2 true)

add_executable(monitor_od-false1 ${OD_MONITOR_TEST} monitor_inputs-false1.cpp)
target_include_directories(monitor_od-false1 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-false1 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-false1 COMMAND monitor_od-false1 false)

add_executable(monitor_od-true3 ${OD_MONITOR_TEST} monitor_inputs-true3.cpp)
target_include_directories(monitor_od-true3 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-true3 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-true3 COMMAND monitor_od-true3 true)

add_executable(monitor_od-false2 ${OD_MONITOR_TEST} monitor_inputs-false2.cpp)
target_include_directories(monitor_od-false2 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-false2 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-false2 COMMAND monitor_od-false2 false)

add_executable(monitor_od-true4 ${OD_MONITOR_TEST} monitor_inputs-true4.cpp)
target_include_directories(monitor_od-true4 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-true4 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-true4 COMMAND monitor_od-true4 true)

add_executable(monitor_od-false3 ${OD_MONITOR_TEST} monitor_inputs-false3.cpp)
target_include_directories(monitor_od-false3 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-false3 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-false3 COMMAND monitor_od-false3 false)

add_executable(monitor_od-false4 ${OD_MONITOR_TEST} monitor_inputs-false4.cpp)
target_include_directories(monitor_od-false4 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-false4 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-false4 COMMAND monitor_od-false4 false)

add_executable(monitor_od-true5 ${OD_MONITOR_TEST} monitor_inputs-true5.cpp)
target_include_directories(monitor_od-true5 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-true5 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-true5 COMMAND monitor_od-true5 true)


