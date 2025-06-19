project(projectowl)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    msp
    config
    serial
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/project_config.json
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
     
add_compile_definitions(CONFIG_FILE_PATH="${CMAKE_CURRENT_BINARY_DIR}/project_config.json")

add_subdirectory(utilities)