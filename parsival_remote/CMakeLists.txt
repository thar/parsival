include(${CMAKE_SOURCE_DIR}/cmake/DownloadProject.cmake)

download_project(PROJ                wiiuse
        GIT_REPOSITORY      https://github.com/rpavlik/wiiuse
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
        )
list(APPEND CMAKE_MODULE_PATH "${wiiuse_SOURCE_DIR}/cmake")

add_subdirectory(${wiiuse_SOURCE_DIR} ${wiiuse_BINARY_DIR})

include_directories(${wiiuse_SOURCE_DIR}/src)

add_executable(parsival_remote parsival_remote.cpp Serial.cpp Serial.h)
target_link_libraries(parsival_remote wiiuse)