add_library(vamos-hyper STATIC hyper.cpp trace.cpp pipeline.cpp transformer.cpp)
add_library(vamos-hyper-lib SHARED hyper.cpp trace.cpp pipeline.cpp transformer.cpp)

target_include_directories(vamos-hyper PUBLIC ${vamos-buffers_INCLUDE_DIRS})
target_include_directories(vamos-hyper-lib PUBLIC ${vamos-buffers_INCLUDE_DIRS})

install(TARGETS vamos-hyper vamos-hyper-lib
    EXPORT vamos-hyper_Export
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)

