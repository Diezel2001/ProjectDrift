project(serial)

add_library(${PROJECT_NAME} STATIC
    serial.cpp
)


target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
