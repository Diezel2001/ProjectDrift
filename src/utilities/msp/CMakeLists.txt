project(msp)

add_library(${PROJECT_NAME} STATIC
    msp.cpp
)


target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    serial
)
