include_directories(../src)
add_executable(parsival_remote parsival_remote.cpp Serial.cpp Serial.h)
target_link_libraries(parsival_remote wiiuse)