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
add_executable(monitor_od-test1 ${OD_MONITOR_TEST} monitor_inputs-true1.cpp)
target_include_directories(monitor_od-test1 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test1 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test1 COMMAND monitor_od-test1 true)

add_executable(monitor_od-test2 ${OD_MONITOR_TEST} monitor_inputs-true2.cpp)
target_include_directories(monitor_od-test2 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test2 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test2 COMMAND monitor_od-test2 true)

add_executable(monitor_od-test3 ${OD_MONITOR_TEST} monitor_inputs-false1.cpp)
target_include_directories(monitor_od-test3 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test3 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test3 COMMAND monitor_od-test3 false)

add_executable(monitor_od-test4 ${OD_MONITOR_TEST} monitor_inputs-true3.cpp)
target_include_directories(monitor_od-test4 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test4 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test4 COMMAND monitor_od-test4 true)

add_executable(monitor_od-test5 ${OD_MONITOR_TEST} monitor_inputs-false2.cpp)
target_include_directories(monitor_od-test5 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test5 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test5 COMMAND monitor_od-test5 false)

add_executable(monitor_od-test6 ${OD_MONITOR_TEST} monitor_inputs-true4.cpp)
target_include_directories(monitor_od-test6 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test6 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test6 COMMAND monitor_od-test6 true)

add_executable(monitor_od-test7 ${OD_MONITOR_TEST} monitor_inputs-false3.cpp)
target_include_directories(monitor_od-test7 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test7 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test7 COMMAND monitor_od-test7 false)

add_executable(monitor_od-test8 ${OD_MONITOR_TEST} monitor_inputs-false4.cpp)
target_include_directories(monitor_od-test8 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test8 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test8 COMMAND monitor_od-test8 false)

add_executable(monitor_od-test9 ${OD_MONITOR_TEST} monitor_inputs-true5.cpp)
target_include_directories(monitor_od-test9 PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_od-test9 PRIVATE vamos-hyper vamos-buffers-event)
add_test(NAME test-monitor_od-test9 COMMAND monitor_od-test9 true)



