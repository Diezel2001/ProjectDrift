project(pyMsp)


pybind11_add_module(${PROJECT_NAME} pyMsp.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/utilities/msp/msp.cpp)  # your actual code

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/utilities/msp
    ${CMAKE_SOURCE_DIR}/src/utilities/serial
)
