#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_subdirectory(src)
add_subdirectory(tests)

set(MONITOR_MAIN src/main.cpp)


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

foreach (NUM_STREAMS RANGE 500 2000 500)
foreach (STREAM_LEN RANGE 500 10000 500)
add_executable(monitor-1t-${NUM_STREAMS}-${STREAM_LEN} ${MONITOR_MAIN} monitor_inputs-onetrace.cpp)
target_include_directories(monitor-1t-${NUM_STREAMS}-${STREAM_LEN} PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-1t-${NUM_STREAMS}-${STREAM_LEN} PRIVATE monitor-od)
target_compile_definitions(monitor-1t-${NUM_STREAMS}-${STREAM_LEN}
	                   #PRIVATE RAND_SEED=0xdeed
			   PRIVATE NUM_STREAMS=${NUM_STREAMS}
			   PRIVATE STREAM_LEN=${STREAM_LEN}
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   )
endforeach()
endforeach()

