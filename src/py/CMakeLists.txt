project(timingWheelerPy)

add_subdirectory(pybind11)

FILE(GLOB py_src_cpps *.cpp)
pybind11_add_module(timingWheelerPy ${py_src_cpps})
