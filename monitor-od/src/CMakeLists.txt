#target_compile_options(test2 PRIVATE -fsanitize=address,undefined)

add_library(monitor-od STATIC monitor.cpp private.cpp cfgs.cpp)
target_include_directories(monitor-od PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-od PUBLIC vamos-hyper vamos-buffers-event)
target_compile_definitions(monitor-od
			   #PRIVATE EXIT_ON_ERROR=1
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   )


add_library(monitor-od-dbg STATIC monitor.cpp private.cpp cfgs.cpp)
target_include_directories(monitor-od-dbg PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_link_libraries(monitor-od-dbg PUBLIC vamos-hyper vamos-buffers-event)
target_compile_definitions(monitor-od-dbg
		           #PRIVATE EXIT_ON_ERROR=1
			   PRIVATE OUTPUT=1
			   PRIVATE DEBUG=1
			   PRIVATE DEBUG_EVENTS=1
			   PRIVATE DEBUG_CFGS=1
			   PRIVATE DEBUG_WORKBAG=1
			   PRIVATE REDUCT_SYMMETRY=1
			   PRIVATE REDUCT_REFLEXIVITY=1
			   )

