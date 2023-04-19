add_library(vamos-hyper STATIC hyper.cpp)

install(TARGETS vamos-hyper
    EXPORT vamos-hyper_Export
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)

