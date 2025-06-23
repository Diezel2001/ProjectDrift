project(msp)

# Source directories
set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BOARD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/firm_${TARGET_BOARD}")

message("COMMMON DIR: " ${COMMON_DIR})
message("BOSRD DIR: " ${BOARD_DIR})

# Collect sources
set(SOURCES
    ${COMMON_DIR}/msp.cpp
    ${COMMON_DIR}/msp_datatypes.cpp
)

# Conditionally add board-specific source
if(TARGET_BOARD STREQUAL "beta")
    message(STATUS "Configuring for Beta board")
    # list(APPEND SOURCES ${BOARD_DIR}/datatypes.cpp)

elseif(TARGET_BOARD STREQUAL "inav")
    message(STATUS "Configuring for iNav board")
    # list(APPEND SOURCES ${BOARD_DIR}/datatypes.cpp)
else()
    message(FATAL_ERROR "Unknown board: ${TARGET_BOARD}")
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${COMMON_DIR}
    ${BOARD_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    serial
)
