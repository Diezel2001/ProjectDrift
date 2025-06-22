project(config)

add_library(${PROJECT_NAME} STATIC
    config.cpp
)


target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../external
)

