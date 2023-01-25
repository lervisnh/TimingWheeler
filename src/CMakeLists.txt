set( INC_INTERFACES include )

FOREACH(inc ${INC_INTERFACES})
    add_library(${inc} INTERFACE)
    target_include_directories(${inc} INTERFACE ${inc})
ENDFOREACH()

target_link_libraries(include INTERFACE spdlog)

FILE(GLOB src_cpps *.cpp)
add_library(TimingWheelerSrc OBJECT ${src_cpps})
target_link_libraries(TimingWheelerSrc PRIVATE ${INC_INTERFACES})

add_subdirectory(test)