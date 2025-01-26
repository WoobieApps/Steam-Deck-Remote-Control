set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/App.cpp
    ${SOURCE_DIR}/InputMgr.cpp
    ${SOURCE_DIR}/LogWrapper.cpp
    ${SOURCE_DIR}/StateMgr.cpp
    ${SOURCE_DIR}/VisualMgr.cpp
)

add_executable(SDRC ${SOURCES})

target_include_directories(SDRC PRIVATE ${INCLUDE_DIR})

target_link_libraries(SDRC PRIVATE SDL2::SDL2)
target_link_libraries(SDRC PRIVATE SDL2_image::SDL2_image)
target_link_libraries(SDRC PRIVATE SDL2_ttf::SDL2_ttf)