set( INC_INTERFACES include )

FOREACH(inc ${INC_INTERFACES})
    add_library(${inc} INTERFACE)
    target_include_directories(${inc} INTERFACE ${inc})
ENDFOREACH()

target_link_libraries(include INTERFACE spdlog)

FILE(GLOB src_cpps *.cpp)
add_library(timingWheelerSrc OBJECT ${src_cpps})
target_link_libraries(timingWheelerSrc PRIVATE ${INC_INTERFACES})

# To install a static lib with headers
add_library(${CMAKE_PROJECT_NAME} STATIC)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE timingWheelerSrc)

install(TARGETS ${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
install(FILES include/*.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME}
)

# Simple test
add_subdirectory(test)

# Python lib
if(BUILD_PY_LIB)
    add_subdirectory(py)
endif()
