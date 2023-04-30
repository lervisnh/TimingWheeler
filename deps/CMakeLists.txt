# SUBDIRLIST(DEPS_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(DEPS_SUBDIRS "spdlog;googletest")

set(INSTALL_GTEST OFF CACHE BOOL "Disable to install GoogleTest")

FOREACH(subdir ${DEPS_SUBDIRS})
    message(STATUS "Dependency : ${subdir}")
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()