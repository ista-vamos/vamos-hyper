#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_library(monitor-od STATIC monitor.cpp private.cpp cfgs.cpp)
target_include_directories(monitor-od PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-od PUBLIC vamos-hyper vamos-buffers-event)

add_library(monitor-od-dbg STATIC monitor.cpp private.cpp cfgs.cpp)
target_include_directories(monitor-od-dbg PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-od-dbg PUBLIC vamos-hyper vamos-buffers-event)
target_compile_definitions(monitor-od-dbg
			   PRIVATE EXIT_ON_ERROR=1
			   PRIVATE OUTPUT=1)

