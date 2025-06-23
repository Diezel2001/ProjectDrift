project(pyMsp)


pybind11_add_module(${PROJECT_NAME} pyMsp.cpp)

target_sources(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/utilities/msp/msp.cpp
    ${CMAKE_SOURCE_DIR}/src/utilities/msp/msp_datatypes.cpp
    ${CMAKE_SOURCE_DIR}/src/utilities/serial/serial.cpp
)  # your actual code

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/utilities/msp/firm_${TARGET_BOARD}
    ${CMAKE_SOURCE_DIR}/src/utilities/msp
    ${CMAKE_SOURCE_DIR}/src/utilities/serial
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/pythonlibs/pyMsp
)
