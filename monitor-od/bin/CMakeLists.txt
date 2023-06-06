#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

set(MONITOR_MAIN ../src/main.cpp)

foreach (NUM_STREAMS RANGE 500 2000 500)
foreach (STREAM_LEN RANGE 500 10000 500)
set(target monitor_rand-${NUM_STREAMS}-${STREAM_LEN})
add_executable(${target} ${MONITOR_MAIN} ../monitor_inputs.cpp)
target_include_directories(${target} PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(${target} PRIVATE monitor-od)
target_compile_definitions(${target}
	                   #PRIVATE RAND_SEED=0xdeed
			   PRIVATE NUM_STREAMS=${NUM_STREAMS}
			   PRIVATE STREAM_LEN=${STREAM_LEN}
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   )
endforeach()
endforeach()

foreach (DIST RANGE 10 200 30)
foreach (NUM_STREAMS RANGE 100 500 100)
foreach (STREAM_LEN 100 500 1000)
set(target monitor_1t_${DIST}-${NUM_STREAMS}-${STREAM_LEN})
add_executable(${target} ${MONITOR_MAIN} ../monitor_inputs-1t.cpp)
target_include_directories(${target} PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(${target} PRIVATE monitor-od)
target_compile_definitions(${target}
			   PRIVATE NUM_STREAMS=${NUM_STREAMS}
			   PRIVATE STREAM_LEN=${STREAM_LEN}
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   PRIVATE DIST=${DIST}
			   )
endforeach()
endforeach()
endforeach()

foreach (SPOS RANGE 33 100 33)
foreach (DIST RANGE 10 200 30)
foreach (NUM_STREAMS RANGE 100 500 100)
foreach (STREAM_LEN 100 500 1000)
set(target monitor_2t_${SPOS}_${DIST}-${NUM_STREAMS}-${STREAM_LEN})
add_executable(${target} ${MONITOR_MAIN} ../monitor_inputs-2t.cpp)
target_include_directories(${target} PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(${target} PRIVATE monitor-od)
target_compile_definitions(${target}
			   PRIVATE NUM_STREAMS=${NUM_STREAMS}
			   PRIVATE STREAM_LEN=${STREAM_LEN}
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   PRIVATE SPOS=${SPOS}
			   PRIVATE DIST=${DIST}
			   )
endforeach()
endforeach()
endforeach()
endforeach()
