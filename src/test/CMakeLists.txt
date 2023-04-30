FILE(GLOB tests *.cpp)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/test)

FOREACH(test_source ${tests})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable( ${test_name} ${test_source} )
    target_link_libraries(${test_name} PRIVATE ${INC_INTERFACES})
    target_link_libraries(${test_name} PRIVATE timingWheelerSrc)
ENDFOREACH()

if(NOT TARGET spdlog)
    message(SEND_ERROR "No spdlog")
endif()
