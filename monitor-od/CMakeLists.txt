#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_subdirectory(src)
add_subdirectory(tests)

#add_executable(monitor-msr-gps monitor_msr_gps_main.cpp monitor_inputs_msr_gps.cpp)
#target_include_directories(monitor-msr-gps PUBLIC ${vamos-buffers_INCLUDE_DIRS})
#target_link_libraries(monitor-msr-gps PRIVATE monitor-od)
#target_compile_definitions(monitor-msr-gps
#                           PRIVATE REDUCT_SYMMETRY=1
#                           PRIVATE REDUCT_REFLEXIVITY=1
#                           )

set(MONITOR_MAIN src/main.cpp)

add_executable(monitor ${MONITOR_MAIN} monitor_inputs-onetrace.cpp)
target_include_directories(monitor PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor PRIVATE monitor-od)
target_compile_definitions(monitor
	                   #PRIVATE RAND_SEED=0xdeed
			   PRIVATE NUM_STREAMS=2
			   PRIVATE STREAM_LEN=10
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   )


foreach (NUM_STREAMS RANGE 500 2000 500)
foreach (STREAM_LEN RANGE 500 10000 500)
add_executable(monitor-${NUM_STREAMS}-${STREAM_LEN} ${MONITOR_MAIN} monitor_inputs.cpp)
target_include_directories(monitor-${NUM_STREAMS}-${STREAM_LEN} PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-${NUM_STREAMS}-${STREAM_LEN} PRIVATE monitor-od)
target_compile_definitions(monitor-${NUM_STREAMS}-${STREAM_LEN}
	                   #PRIVATE RAND_SEED=0xdeed
			   PRIVATE NUM_STREAMS=${NUM_STREAMS}
			   PRIVATE STREAM_LEN=${STREAM_LEN}
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   )
endforeach()
endforeach()

foreach (NUM_STREAMS RANGE 100 500 100)
foreach (STREAM_LEN RANGE 100 1000 100)
add_executable(monitor_1t-${NUM_STREAMS}-${STREAM_LEN} ${MONITOR_MAIN} monitor_inputs-onetrace.cpp)
target_include_directories(monitor_1t-${NUM_STREAMS}-${STREAM_LEN} PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor_1t-${NUM_STREAMS}-${STREAM_LEN} PRIVATE monitor-od)
target_compile_definitions(monitor_1t-${NUM_STREAMS}-${STREAM_LEN}
			   PRIVATE NUM_STREAMS=${NUM_STREAMS}
			   PRIVATE STREAM_LEN=${STREAM_LEN}
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   )
endforeach()
endforeach()

